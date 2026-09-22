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

	void testTextureStagingAndReuse(platformtest::Fixture& fixture)
	{
		for (auto format : {MyGUI::PixelFormat::R8G8B8, MyGUI::PixelFormat::R8G8B8A8})
		{
			auto* texture = static_cast<MyGUI::VulkanTexture*>(fixture.texture());
			texture->createManual(3, 2, MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write, format);
			const std::vector<unsigned char> expected(6 * texture->getNumElemBytes(), 41);
			platformtest::upload(texture, expected);
			const auto image = texture->getImage();

			auto* bytes = static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read));
			require(bytes != nullptr, "Read-only staging lock must succeed");
			bytes[0] = 0; // A read-only lock must never upload its temporary bytes.
			texture->unlock();
			require(platformtest::read(texture) == expected, "Read-only unlock must leave the image unchanged");
			require(texture->getImage() == image, "Read-only access must reuse its image");

			// The shared RGB/RGBA cases check partial-edit bytes and lock state.
			// Here both write access modes must avoid allocating an unneeded image.
			bytes = static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
			require(bytes != nullptr, "Read/write staging lock must succeed");
			bytes[1] = 93;
			texture->unlock();
			require(texture->getImage() == image, "Unrecorded read/write updates must reuse their image");
			platformtest::upload(texture, expected);
			require(texture->getImage() == image, "Unrecorded write-only updates must reuse their image");
		}
	}

	void testTextureUpdateVersions(platformtest::Fixture& fixture)
	{
		auto* output = target(fixture);
		auto* rtt = output->getRenderTarget();
		auto* source = static_cast<MyGUI::VulkanTexture*>(platformtest::solid(fixture, {255, 0, 0, 255}));
		const auto image = source->getImage();
		const auto descriptor = source->getDescriptorSet();
		const auto pointDescriptor = source->getPointDescriptorSet();
		std::weak_ptr<void> old = source->retainStorage();
		MyGUI::VulkanVertexBuffer buffer;
		fixture.fill(&buffer, 6, rtt->getInfo(), {0, 0, 64, 128}, {255, 255, 255, 255});
		rtt->begin();
		rtt->doRender(&buffer, source, 6);
		platformtest::upload(source, {255, 0, 0, 255});
		require(source->getImage() != image, "An update must preserve the image used by an unsubmitted draw");
		require(
			source->getDescriptorSet() != descriptor && source->getPointDescriptorSet() != pointDescriptor,
			"Both sampler descriptors must be versioned with the image");
		require(!old.expired(), "The old image must remain retained by the recording");
		fixture.fill(&buffer, 6, rtt->getInfo(), {64, 0, 64, 128}, {255, 255, 255, 255});
		rtt->doRender(&buffer, source, 6);
		rtt->end();
		fixture.scene([&](MyGUI::IRenderTarget* screen) { fixture.quad(screen, output); });
		fixture.capture();
		fixture.expect(32, 64, {255, 0, 0, 255});
		fixture.expect(96, 64, {0, 0, 255, 255});
		rtt->begin();
		const bool retired = old.expired();
		rtt->end();
		require(retired, "Completed texture versions must retire with their recording");
	}

	void testRenderTargetUpdate(platformtest::Fixture& fixture)
	{
		auto* source = static_cast<MyGUI::VulkanTexture*>(target(fixture));
		auto* cachedTarget = source->getRenderTarget();
		MyGUI::VulkanVertexBuffer buffer;
		fixture.fill(&buffer, 6, cachedTarget->getInfo(), {0, 0, 128, 128}, {0, 255, 0, 255});
		cachedTarget->begin();
		cachedTarget->doRender(&buffer, nullptr, 6);
		cachedTarget->end();

		auto* output = target(fixture);
		auto* consumer = output->getRenderTarget();
		fixture.fill(&buffer, 6, consumer->getInfo(), {0, 0, 64, 128}, {255, 255, 255, 255});
		consumer->begin();
		consumer->doRender(&buffer, source, 6);
		const auto oldImage = source->getImage();
		auto* bytes = static_cast<unsigned char*>(source->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
		bool green = true;
		for (size_t i = 0; i < 128 * 128 * 4; i += 4)
		{
			green = green && bytes[i] == 0 && bytes[i + 1] == 255 && bytes[i + 2] == 0 && bytes[i + 3] == 255;
			bytes[i] = 255; // Green becomes cyan, preserving the other channels.
		}
		source->unlock();
		require(green, "Read/write must read GPU-produced RTT pixels before editing them");
		require(source->getImage() != oldImage, "Updating a sampled RTT must preserve its recorded version");
		require(source->getRenderTarget() == cachedTarget, "Image replacement must preserve cached target pointers");
		fixture.fill(&buffer, 6, consumer->getInfo(), {64, 0, 64, 128}, {255, 255, 255, 255});
		consumer->doRender(&buffer, source, 6);
		consumer->end();
		fixture.scene([&](MyGUI::IRenderTarget* screen) { fixture.quad(screen, output); });
		fixture.capture();
		fixture.expect(32, 64, {0, 255, 0, 255});
		fixture.expect(96, 64, {0, 255, 255, 255});

		fixture.fill(&buffer, 6, cachedTarget->getInfo(), {0, 0, 128, 128}, {255, 0, 0, 255});
		cachedTarget->begin();
		cachedTarget->doRender(&buffer, nullptr, 6);
		cachedTarget->end();
		fixture.scene([&](MyGUI::IRenderTarget* screen) { fixture.quad(screen, source); });
		fixture.capture();
		fixture.expectCorners({255, 0, 0, 255});
	}

}

// SDL's Windows entry-point wrapper requires the argc/argv signature, even when unused.
int main(int, char**)
{
	return platformtest::runNativeTests(
		{{"", "retained-draws", testRetainedDraws},
		 {"", "allocation-reuse", testAllocationReuse},
		 {"", "pending-destruction", testPendingDestruction},
		 {"", "sampled-texture-lifetime", testSampledTextureLifetime},
		 {"", "automatic-frame-retirement", testAutomaticFrameRetirement},
		 {"", "texture-staging-reuse", testTextureStagingAndReuse},
		 {"", "texture-update-versions", testTextureUpdateVersions},
		 {"", "render-target-update", testRenderTargetUpdate}});
}
