#include "PlatformFixture.h"
#include "MyGUI_VulkanVertexBuffer.h"
#include "MyGUI_VulkanTexture.h"
#include <iostream>
#include <memory>

namespace
{

	using unittest::require;

	MyGUI::ITexture* target(platformtest::Fixture& fixture)
	{
		auto* texture = fixture.texture();
		texture->createManual(128, 128, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		return texture;
	}

	void testRetainedDraws(platformtest::Fixture& fixture)
	{
		auto* texture = target(fixture);
		auto* rtt = texture->getRenderTarget();
		auto buffer = std::make_unique<MyGUI::VulkanVertexBuffer>();
		fixture.fill(buffer.get(), 6, rtt->getInfo(), {0, 0, 128, 128}, {255, 0, 0, 255});
		const auto first = buffer->getBuffer();
		std::weak_ptr<void> red = buffer->retainStorage();

		rtt->begin();
		rtt->doRender(buffer.get(), nullptr, 6);
		fixture.fill(buffer.get(), 64, rtt->getInfo(), {64, 0, 64, 128}, {0, 255, 0, 255});
		require(buffer->getBuffer() != first, "A recorded draw must keep its old vertex allocation");
		std::weak_ptr<void> green = buffer->retainStorage();
		rtt->doRender(buffer.get(), nullptr, 6);
		buffer.reset();
		require(!red.expired() && !green.expired(), "Recorded storage must outlive its vertex-buffer wrapper");
		rtt->end();

		// This submits actual Vulkan work and checks both recorded versions on the GPU.
		fixture.scene([&](MyGUI::IRenderTarget* output) { fixture.quad(output, texture); });
		fixture.capture();
		fixture.expect(32, 64, {255, 0, 0, 255});
		fixture.expect(96, 64, {0, 255, 0, 255});

		// Reuse waits for the submitted work and resets its command buffer before
		// releasing the last references. No fake driver or allocator is involved.
		rtt->begin();
		const bool retired = red.expired() && green.expired();
		rtt->end();
		require(retired, "Reusing a completed RTT must retire its recorded vertex allocations");
	}

	void testAllocationReuse(platformtest::Fixture& fixture)
	{
		auto* texture = target(fixture);
		auto* rtt = texture->getRenderTarget();
		MyGUI::VulkanVertexBuffer buffer;
		auto fill = [&](platformtest::Pixel colour)
		{
			fixture.fill(&buffer, 6, rtt->getInfo(), {0, 0, 128, 128}, colour);
		};
		fill({255, 0, 0, 255});
		const auto first = buffer.getBuffer();
		fill({255, 0, 0, 255});
		require(buffer.getBuffer() == first, "Unrecorded updates within capacity must reuse their allocation");
		rtt->begin();
		rtt->doRender(&buffer, nullptr, 6);
		rtt->end();
		fill({0, 255, 0, 255});
		const auto second = buffer.getBuffer();
		require(second != first, "Submitted but unretired storage must not be overwritten");

		rtt->begin(); // Waits and retires the first allocation.
		rtt->doRender(&buffer, nullptr, 6);
		fill({0, 0, 255, 255});
		require(buffer.getBuffer() == first, "An explicitly retired allocation should be recycled");
		rtt->end();
		fixture.scene([&](MyGUI::IRenderTarget* output) { fixture.quad(output, texture); });
		fixture.capture();
		fixture.expectCorners({0, 255, 0, 255});
	}

	void testPendingDestruction(platformtest::Fixture& fixture)
	{
		auto* texture = target(fixture);
		auto* rtt = texture->getRenderTarget();
		auto buffer = std::make_unique<MyGUI::VulkanVertexBuffer>();
		fixture.fill(buffer.get(), 6, rtt->getInfo(), {0, 0, 128, 128}, {255, 0, 0, 255});
		std::weak_ptr<void> storage = buffer->retainStorage();
		rtt->begin();
		rtt->doRender(buffer.get(), nullptr, 6);
		rtt->end();
		buffer.reset();
		require(!storage.expired(), "Pending target work must retain its vertex storage");
		fixture.removeTexture(texture); // Production destructor waits and retires the recording.
		require(storage.expired(), "Destroyed target must release completed vertex storage");
	}

	void testSampledTextureLifetime(platformtest::Fixture& fixture)
	{
		auto* texture = target(fixture);
		auto* rtt = texture->getRenderTarget();
		auto* source = static_cast<MyGUI::VulkanTexture*>(platformtest::solid(fixture, {255, 0, 0, 255}));
		std::weak_ptr<void> red = source->retainStorage();
		MyGUI::VulkanVertexBuffer buffer;
		fixture.fill(&buffer, 6, rtt->getInfo(), {0, 0, 64, 128}, {255, 255, 255, 255});
		rtt->begin();
		rtt->doRender(&buffer, source, 6);
		source->destroy();
		require(!red.expired(), "An unsubmitted draw must retain its sampled image and descriptors");

		// Recreate the same wrapper: the first draw must still sample its old image.
		source->createManual(
			1,
			1,
			MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
			MyGUI::PixelFormat::R8G8B8A8);
		platformtest::upload(source, {0, 255, 0, 255});
		std::weak_ptr<void> green = source->retainStorage();
		fixture.fill(&buffer, 6, rtt->getInfo(), {64, 0, 64, 128}, {255, 255, 255, 255});
		rtt->doRender(&buffer, source, 6);
		rtt->end();
		fixture.removeTexture(source);
		require(!green.expired(), "A submitted draw must retain its destroyed sampled texture");

		fixture.scene([&](MyGUI::IRenderTarget* output) { fixture.quad(output, texture); });
		fixture.capture();
		fixture.expect(32, 64, {255, 0, 0, 255});
		fixture.expect(96, 64, {0, 255, 0, 255});
		rtt->begin();
		const bool retired = red.expired() && green.expired();
		rtt->end();
		require(retired, "Reusing a completed RTT must retire both versions of its sampled texture");
	}

	void testAutomaticFrameRetirement(platformtest::Fixture& fixture)
	{
		std::vector<std::weak_ptr<void>> recorded;
		fixture.scene(
			[&](MyGUI::IRenderTarget* output)
			{
				auto* source = static_cast<MyGUI::VulkanTexture*>(platformtest::solid(fixture, {255, 0, 0, 255}));
				MyGUI::VulkanVertexBuffer buffer;
				fixture.fill(&buffer, 6, output->getInfo(), {0, 0, 128, 128}, {255, 255, 255, 255});
				recorded.push_back(source->retainStorage());
				recorded.push_back(buffer.retainStorage());
				output->doRender(&buffer, source, 6);
				fixture.removeTexture(source);
			});
		for (int frame = 0; frame < 16; ++frame)
		{
			fixture.capture();
			fixture.expectCorners({255, 0, 0, 255});
			require(!recorded.back().expired(), "The current frame must retain destroyed draw resources");
			if (frame >= 3)
				require(recorded.front().expired(), "Earlier frames must retire while rendering continues");
		}
		fixture.scene({});
		// The first empty frame fences the last recording. Capture waits for the queue;
		// the second frame observes completion without any host retirement notification.
		fixture.capture();
		fixture.capture();
		for (const auto& storage : recorded)
			require(storage.expired(), "Completed frames must release resources automatically");
	}

}

int main()
{
	try
	{
		platformtest::Fixture fixture(false);
		fixture.open();
		testRetainedDraws(fixture);
		fixture.resetCase();
		std::cout << "PASS recorded vertices survive updates and wrapper destruction\n";
		testAllocationReuse(fixture);
		fixture.resetCase();
		std::cout << "PASS capacity reuse and retired allocation recycling\n";
		testPendingDestruction(fixture);
		fixture.resetCase();
		std::cout << "PASS pending RTT destruction retires vertex storage\n";
		testSampledTextureLifetime(fixture);
		fixture.resetCase();
		std::cout << "PASS sampled textures survive destruction and recreation until RTT completion\n";
		testAutomaticFrameRetirement(fixture);
		fixture.resetCase();
		std::cout << "PASS host frames retire textures and vertices without cleanup notifications\n";
		fixture.close();
		return 0;
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << '\n';
		return 1;
	}
}
