#include "OsgTestContext.h"
#include <osg/observer_ptr>
#include <osgDB/Registry>
#include <algorithm>
#include <cstring>
#include <thread>
#include <atomic>

namespace
{

	using unittest::require;
	using unittest::requireThrows;
	using unittest::OsgTestContext;
	using MyGUI::PixelFormat;
	using MyGUI::TextureUsage;

	void testTextureSnapshots()
	{
		OsgTestContext context;
		auto* texture = context.texture("pixels");
		texture->createManual(3, 2, TextureUsage::Dynamic, PixelFormat::R8G8B8);
		auto* pixels = static_cast<unsigned char*>(texture->lock(TextureUsage::Write));
		for (unsigned char i = 0; i < 18; ++i)
			pixels[i] = i;
		requireThrows([&] { texture->lock(TextureUsage::Read); }, "Double lock must fail");
		texture->unlock();
		osg::ref_ptr<osg::Texture2D> previous = texture->getTexture();
		texture->lock(TextureUsage::Read);
		texture->unlock();
		require(texture->getTexture() == previous, "Read-only unlock must not replace the GPU texture");
		pixels = static_cast<unsigned char*>(texture->lock(TextureUsage::Read | TextureUsage::Write));
		pixels[4] = 123;
		texture->unlock();
		require(previous->getImage()->data()[4] == 4, "Pending draw texture must retain original pixels");
		require(texture->getTexture()->getImage()->data()[4] == 123, "New texture must contain the edit");
		requireThrows([&] { texture->unlock(); }, "Unlock without lock must fail");
		texture->destroy();
		require(texture->getWidth() == 0 && !texture->isLocked(), "Destroy must reset texture state");
	}

	void testSharedImageLoader()
	{
		OsgTestContext context;
		osg::ref_ptr<osg::Image> image = new osg::Image;
		image->allocateImage(3, 2, 1, GL_RGBA, GL_UNSIGNED_BYTE);
		std::fill_n(image->data(), image->getTotalSizeInBytes(), 255);
		image->data()[0] = 10;
		image->data()[2] = 30;
		context.render().setImageLoader([image](const std::string&) { return image; });
		auto* texture = context.texture("loaded");
		texture->loadFromFile("fixture");
		require(image->data()[0] == 10, "Loading must not alter shared cached image data");
		image->allocateImage(1, 1, 1, GL_RGBA, GL_FLOAT);
		requireThrows([&] { texture->loadFromFile("float"); }, "Unsupported pixel types must be rejected");
	}

	void testPngPluginDispatch()
	{
		OsgTestContext context;
		// Simulate another PNG-capable plugin (such as ImageIO) registered first.
		class OtherPngReader : public osgDB::ReaderWriter
		{
		public:
			OtherPngReader()
			{
				supportsExtension("png", "Test PNG reader");
			}
			ReadResult readImage(const std::string&, const Options*) const override
			{
				called = true;
				return ReadResult::FILE_NOT_HANDLED;
			}
			mutable bool called = false;
		};
		struct Registration
		{
			Registration()
			{
				osgDB::Registry::instance()->getReaderWriterList().insert(
					osgDB::Registry::instance()->getReaderWriterList().begin(),
					reader);
			}
			~Registration()
			{
				osgDB::Registry::instance()->removeReaderWriter(reader);
			}
			osg::ref_ptr<OtherPngReader> reader = new OtherPngReader;
		} registration;
		auto* texture = context.texture("png");
		texture->loadFromFile("TransparentRgb.png");
		require(registration.reader->called, "Image loading must use normal osgDB plugin dispatch");
	}

	void testUnsupportedRenderTargetAccess()
	{
		OsgTestContext context;
		for (auto access :
			 {TextureUsage(TextureUsage::Read),
			  TextureUsage(TextureUsage::Write),
			  TextureUsage::Read | TextureUsage::Write})
			require(
				!context.render().isFormatSupported(PixelFormat::R8G8B8A8, TextureUsage::RenderTarget | access),
				"CPU access to render targets must not be advertised");
	}

	void testPendingVertices()
	{
		OsgTestContext context;
		MyGUI::OsgVertexBuffer buffer;
		require(buffer.lock() == nullptr, "Empty vertex buffers must not expose invalid memory");
		buffer.unlock();
		context.render().begin();
		context.render().doRender(&buffer, nullptr, 0);
		context.render().end();
		requireThrows(
			[&] { context.render().createBatch(&buffer, nullptr, 3, nullptr); },
			"Unallocated buffer must be rejected");
		buffer.setVertexCount(3);
		buffer.lock()[0].x = 12.0f;
		buffer.unlock();
		auto batch = context.render().createBatch(&buffer, nullptr, 3, nullptr);
		std::atomic<bool> preserved{true};
		std::thread draw(
			[&]
			{
				for (int i = 0; i < 10000; ++i)
				{
					MyGUI::Vertex vertex;
					std::memcpy(&vertex, batch.mArray->getDataPointer(), sizeof(vertex));
					if (vertex.x != 12.0f)
						preserved = false;
				}
			});
		for (int i = 0; i < 20; ++i)
		{
			buffer.lock()[0].x = static_cast<float>(i);
			buffer.unlock();
			context.render().createBatch(&buffer, nullptr, 3, nullptr);
		}
		draw.join();
		require(preserved, "Concurrent updates must preserve the drawing thread's vertex data");
		MyGUI::Vertex vertex;
		std::memcpy(&vertex, batch.mArray->getDataPointer(), sizeof(vertex));
		require(vertex.x == 12.0f, "Retained batches must survive more than four updates");
	}

	void testBufferPoolReuse()
	{
		OsgTestContext context;
		MyGUI::OsgVertexBuffer buffer;
		buffer.setVertexCount(3);
		buffer.lock()[0].x = 1;
		buffer.unlock();
		auto first = context.render().createBatch(&buffer, nullptr, 3, nullptr);
		osg::observer_ptr<osg::Array> firstArray = first.mArray;
		osg::observer_ptr<osg::VertexBufferObject> firstVbo = first.mVertexBuffer;
		buffer.lock()[0].x = 2;
		buffer.unlock();
		auto second = context.render().createBatch(&buffer, nullptr, 3, nullptr);
		first = {};
		buffer.lock()[0].x = 3;
		buffer.unlock();
		require(buffer.getVertexArray() == firstArray.get(), "Released vertex storage must be reused");
		require(buffer.getVertexBuffer() == firstVbo.get(), "Released VBO must be reused along with its array");
		MyGUI::Vertex previous;
		std::memcpy(&previous, second.mArray->getDataPointer(), sizeof(previous));
		require(previous.x == 2, "Reusing a free buffer must preserve the retained buffer");

		std::vector<MyGUI::Batch> pending;
		std::vector<osg::observer_ptr<osg::Array>> arrays;
		for (int i = 0; i < 16; ++i)
		{
			buffer.lock()[0].x = static_cast<float>(i);
			buffer.unlock();
			pending.push_back(context.render().createBatch(&buffer, nullptr, 3, nullptr));
			arrays.emplace_back(pending.back().mArray);
		}
		for (size_t i = 0; i < pending.size(); ++i)
		{
			std::memcpy(&previous, pending[i].mArray->getDataPointer(), sizeof(previous));
			require(previous.x == static_cast<float>(i), "A backlog beyond four updates must remain intact");
		}
		pending.clear();
		second = {};
		buffer.lock();
		buffer.unlock();
		require(
			std::count_if(arrays.begin(), arrays.end(), [](const auto& array) { return array.valid(); }) <= 3,
			"Released backlog must shrink to the active buffer and at most two spares");
	}

	void testBufferPoolBudget()
	{
		OsgTestContext context;
		MyGUI::OsgVertexBuffer buffer;
		buffer.setVertexCount(100000);
		std::vector<MyGUI::Batch> pending;
		std::vector<osg::observer_ptr<osg::Array>> arrays;
		for (int i = 0; i < 4; ++i)
		{
			buffer.lock()[0].x = static_cast<float>(i);
			buffer.unlock();
			pending.push_back(context.render().createBatch(&buffer, nullptr, 3, nullptr));
			arrays.emplace_back(pending.back().mArray);
		}
		pending.clear();
		buffer.setVertexCount(3);
		buffer.lock();
		buffer.unlock();
		require(
			std::count_if(arrays.begin(), arrays.end(), [](const auto& array) { return array.valid(); }) == 1,
			"Oversized spare buffers must be released after a backlog, even when spare count is small");
	}

	void testDrawablePoolReuse()
	{
		OsgTestContext context;
		auto& manager = context.render();
		manager.begin();
		manager.end();
		osg::observer_ptr<osg::Node> first = manager.getGuiRoot()->getChild(0);
		manager.begin();
		manager.end();
		manager.begin();
		manager.end();
		require(manager.getGuiRoot()->getChild(0) == first.get(), "Free GUI drawables must be reused");

		std::vector<osg::ref_ptr<osg::Node>> pending;
		std::vector<osg::observer_ptr<osg::Node>> nodes;
		for (int i = 0; i < 16; ++i)
		{
			manager.begin();
			manager.end();
			auto* node = manager.getGuiRoot()->getChild(0);
			require(
				std::none_of(pending.begin(), pending.end(), [node](const auto& old) { return old == node; }),
				"Referenced drawables must never be reused");
			pending.emplace_back(node);
			nodes.emplace_back(node);
		}
		pending.clear();
		for (int i = 0; i < 4; ++i)
		{
			manager.begin();
			manager.end();
		}
		require(
			std::count_if(nodes.begin(), nodes.end(), [](const auto& node) { return node.valid(); }) <= 4,
			"Idle drawable storage must shrink after a backlog");
	}

	void testRttDrawablePool()
	{
		OsgTestContext context;
		auto* texture = context.texture("pooled-rtt");
		texture->createManual(8, 8, TextureUsage::RenderTarget, PixelFormat::R8G8B8A8);
		auto* target = texture->getRenderTarget();
		context.render().flushRTTCameras();
		auto* camera = dynamic_cast<osg::Camera*>(context.render().getGuiRoot()->getChild(0));
		require(camera != nullptr, "RTT camera must be attached");
		target->begin();
		target->end();
		osg::observer_ptr<osg::Node> first = camera->getChild(0);
		target->begin();
		target->end();
		target->begin();
		target->end();
		require(camera->getChild(0) == first.get(), "Free RTT drawables must be reused");
		osg::ref_ptr<osg::Node> retained = first.get();
		for (int i = 0; i < 12; ++i)
		{
			target->begin();
			target->end();
			require(camera->getChild(0) != retained, "Referenced RTT drawables must not be reused");
		}
	}

	void testDummyTexture()
	{
		auto dummy = MyGUI::createDummyTexture();
		require(
			dummy->getImage() && dummy->getImage()->data()[0] == 255,
			"Untextured batches need initialized white pixels");
	}

	void testRenderTargetLifetime()
	{
		OsgTestContext context;
		auto* texture = context.texture("rtt");
		texture->createManual(8, 8, TextureUsage::RenderTarget, PixelFormat::R8G8B8A8);
		texture->getRenderTarget();
		texture->destroy();
		context.render().flushRTTCameras();
		require(
			context.render().getGuiRoot()->getNumChildren() == 1,
			"Destroy before attachment must cancel pending camera");
		texture->createManual(8, 8, TextureUsage::RenderTarget, PixelFormat::R8G8B8A8);
		texture->getRenderTarget();
		context.render().flushRTTCameras();
		require(context.render().getGuiRoot()->getNumChildren() == 2, "RTT camera must attach");
		requireThrows([&] { texture->lock(TextureUsage::Read); }, "RTT read must fail explicitly without readback");
		// External detachment must not leave a dangling queued-removal pointer.
		context.render().getGuiRoot()->removeChild(0, 1);
		texture->destroy();
		context.render().flushRTTCameras();
	}

}

int main(int argc, char** argv)
{
#ifdef MYGUI_OSG_RENDER_TESTS
	if (argc == 2 && std::string_view(argv[1]) == "--render")
		return runOsgRenderTests();
	if (argc == 2 && std::string_view(argv[1]) == "--benchmark")
		return runOsgBenchmark();
#else
	(void)argc;
	(void)argv;
#endif
	return unittest::runTests({
		{"Texture snapshots and lock validation", testTextureSnapshots},
		{"Shared OSG image ownership and type validation", testSharedImageLoader},
		{"osgDB PNG plugin dispatch", testPngPluginDispatch},
		{"Unsupported RTT CPU access", testUnsupportedRenderTargetAccess},
		{"Pending vertex snapshots", testPendingVertices},
		{"Buffer reuse and delayed draws", testBufferPoolReuse},
		{"Spare buffer memory budget", testBufferPoolBudget},
		{"Drawable reuse and delayed draws", testDrawablePoolReuse},
		{"RTT drawable reuse and delayed draws", testRttDrawablePool},
		{"Untextured batch dummy texture", testDummyTexture},
		{"Deferred render-target lifetime", testRenderTargetLifetime},
	});
}
