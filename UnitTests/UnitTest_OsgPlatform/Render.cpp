#include "OsgTestContext.h"
#include "TestRunner.h"
#include "MyGUI_OsgRTTexture.h"
#include "Base/Osg/GraphicsWindowSDL2.h"
#include <SDL.h>
#include <osg/Drawable>
#include <osg/State>
#include <osg/ColorMask>
#include <osg/GLExtensions>
#include <osg/ContextData>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <array>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>

namespace
{

	using unittest::require;

	class RenderContext
	{
	public:
		RenderContext(osg::Node* root)
		{
			resourceRoot = root;
			require(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0, SDL_GetError());
			base::GraphicsWindowSDL2::setContextAttributes();
			window = SDL_CreateWindow("OSG regression", 0, 0, 64, 64, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
			require(window != nullptr, SDL_GetError());
			graphics = new base::GraphicsWindowSDL2(window);
			require(graphics->valid() && graphics->realize(), SDL_GetError());
			require((SDL_GetWindowFlags(window) & SDL_WINDOW_HIDDEN) != 0, "Realize must keep test windows hidden");
			// Keep drawing errors pending for our readback assertion instead of
			// letting OSG consume them and merely print a warning.
			graphics->getState()->setCheckForGLErrors(osg::State::NEVER_CHECK_GL_ERRORS);
			graphics->setBeforeSwapCallback(
				[this]
				{
					glReadBuffer(GL_BACK);
					glReadPixels(32, 32, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel.data());
					glError = glGetError();
					++captures;
				});
			viewer = new osgViewer::Viewer;
			viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
			viewer->getCamera()->setGraphicsContext(graphics);
			viewer->getCamera()->setViewport(0, 0, 64, 64);
			viewer->getCamera()->setClearColor(osg::Vec4(0, 0, 0, 0));
			viewer->setSceneData(root);
		}
		~RenderContext()
		{
			closeRenderer();
			SDL_DestroyWindow(window);
			SDL_QuitSubSystem(SDL_INIT_VIDEO);
		}
		void closeRenderer()
		{
			if (!viewer)
				return;
			viewer->stopThreading();
			graphics->makeCurrent();
			auto* gc = viewer->getCamera()->getGraphicsContext();
			// Some tests replace the viewer scene while retaining the original GUI.
			// Release resources belonging to both graphs before closing this context.
			resourceRoot->releaseGLObjects(gc->getState());
			viewer->getSceneData()->releaseGLObjects(gc->getState());
			viewer->setSceneData(nullptr);
			viewer->getCamera()->setRenderer(nullptr);
			gc->removeAllOperations();
			resourceRoot = nullptr;
			viewer = nullptr;
			graphics->close(true);
			graphics = nullptr;
		}
		void expect(unsigned char red, unsigned char green, unsigned char blue)
		{
			const int previousCaptures = captures;
			viewer->frame();
			require(captures == previousCaptures + 1, "Each frame must invoke the callback before swapping");
			require(
				glError == GL_NO_ERROR,
				"Rendering generated OpenGL error " + std::to_string(glError) + "; capture " +
					std::to_string(captures));
			if (pixel[0] != red || pixel[1] != green || pixel[2] != blue)
				throw std::runtime_error(
					"Unexpected framebuffer pixel: " + std::to_string(pixel[0]) + "," + std::to_string(pixel[1]) + "," +
					std::to_string(pixel[2]) + "; expected " + std::to_string(red) + "," + std::to_string(green) + "," +
					std::to_string(blue) + "; capture " + std::to_string(captures));
		}
		SDL_Window* window = nullptr;
		osg::ref_ptr<base::GraphicsWindowSDL2> graphics;
		std::array<unsigned char, 4> pixel{};
		GLenum glError = GL_NO_ERROR;
		int captures = 0;
		osg::ref_ptr<osgViewer::Viewer> viewer;
		osg::ref_ptr<osg::Node> resourceRoot;
	};

	void fillQuad(MyGUI::OsgVertexBuffer& buffer, MyGUI::uint32 colour, size_t count = 6)
	{
		buffer.setVertexCount(count);
		MyGUI::Vertex* vertices = buffer.lock();
		const std::array<std::array<float, 2>, 6> positions{{{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}}};
		for (size_t i = 0; i < count; ++i)
		{
			const auto& position = positions[i % positions.size()];
			vertices[i].x = position[0];
			vertices[i].y = position[1];
			vertices[i].z = 0;
			vertices[i].colour = colour;
			vertices[i].u = (position[0] + 1) / 2;
			vertices[i].v = (1 - position[1]) / 2;
		}
		buffer.unlock();
	}

	void submit(MyGUI::OsgRenderManager& manager, MyGUI::OsgVertexBuffer& buffer, MyGUI::ITexture* texture = nullptr)
	{
		manager.begin();
		manager.doRender(&buffer, texture, 6);
		manager.end();
	}

	void testFirstFrameWithExternalState()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		// Reproduce a host using OSG's GL2 defaults without changing the OSG build.
		graphics.graphics->getState()->setUseModelViewAndProjectionUniforms(false);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xff0000ff);
		submit(manager, buffer);
		graphics.expect(255, 0, 0);
	}

	void testGlErrorDetection()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xff0000ff);
		submit(manager, buffer);
		graphics.expect(255, 0, 0);

		class InvalidOperation : public osg::Camera::DrawCallback
		{
		public:
			void operator()(osg::RenderInfo&) const override
			{
				// Invalid on both profiles, without changing any valid GL state.
				glEnable(static_cast<GLenum>(~0u));
			}
		};
		manager.getGuiRoot()->setPostDrawCallback(new InvalidOperation);
		bool rejected = false;
		try
		{
			graphics.expect(255, 0, 0);
		}
		catch (const std::runtime_error&)
		{
			rejected = true;
		}
		require(graphics.glError == GL_INVALID_ENUM, "The frame assertion must see the injected GL error");
		require(rejected, "Correct framebuffer pixels must not hide an OpenGL error");
		manager.getGuiRoot()->setPostDrawCallback(nullptr);
		graphics.expect(255, 0, 0);
	}

	void testRepeatedAndDelayedDraws()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext first(gui.root);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xff0000ff);
		submit(manager, buffer);
		osg::ref_ptr<osg::Node> retained = manager.getGuiRoot()->getChild(0);
		for (int i = 0; i < 6; ++i)
			first.expect(255, 0, 0);
		{
			require(first.graphics->makeCurrent(), SDL_GetError());
			SDL_GLContext previousContext = SDL_GL_GetCurrentContext();
			RenderContext second(gui.root);
			require(SDL_GL_GetCurrentContext() == previousContext, "Creating a context must restore the previous one");
			second.graphics->setSyncToVBlank(false);
			require(SDL_GL_GetCurrentContext() == previousContext, "Setting vsync must restore the previous context");
			second.expect(255, 0, 0);
			first.expect(255, 0, 0);
			second.expect(255, 0, 0);
		}
		for (int i = 0; i < 12; ++i)
		{
			fillQuad(buffer, 0xffff0000);
			submit(manager, buffer);
			first.expect(0, 0, 255);
		}
		manager.getGuiRoot()->setChild(0, retained);
		first.expect(255, 0, 0);

		// Cull/draw an independent camera containing the retained packet while the
		// update thread prepares newer packets. No SDL window operations run there.
		osg::ref_ptr<osg::Group> drawRoot = new osg::Group;
		drawRoot->addChild(new osg::Camera(*manager.getGuiRoot()));
		first.viewer->setSceneData(drawRoot);
		require(first.graphics->releaseContext(), SDL_GetError());
		std::atomic<bool> drawing{false};
		std::exception_ptr failure;
		std::thread draw(
			[&]
			{
				try
				{
					drawing = true;
					for (int i = 0; i < 20; ++i)
						first.expect(255, 0, 0);
				}
				catch (...)
				{
					failure = std::current_exception();
				}
				first.graphics->releaseContext();
			});
		while (!drawing)
			std::this_thread::yield();
		for (int i = 0; i < 100; ++i)
		{
			fillQuad(buffer, 0xffff0000);
			submit(manager, buffer);
		}
		draw.join();
		if (failure)
			std::rethrow_exception(failure);
		first.expect(255, 0, 0);

		osg::ref_ptr<osg::DisplaySettings> stereo = new osg::DisplaySettings;
		stereo->setStereo(true);
		stereo->setStereoMode(osg::DisplaySettings::HORIZONTAL_SPLIT);
		first.viewer->getCamera()->setDisplaySettings(stereo);
		first.expect(255, 0, 0);
		first.expect(255, 0, 0);
	}

	void testWindowResizeAndLifetime()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xff00ff00);
		submit(manager, buffer);
		graphics.expect(0, 255, 0);
		SDL_SetWindowSize(graphics.window, 96, 80);
		graphics.graphics->updateDrawableSize();
		int width = 0;
		int height = 0;
		SDL_GL_GetDrawableSize(graphics.window, &width, &height);
		require(width >= 96 && height >= 80, "SDL window must resize");
		const auto* traits = graphics.graphics->getTraits();
		require(traits->width == width && traits->height == height, "OSG must use drawable pixel dimensions");
		graphics.viewer->getCamera()->setViewport(0, 0, width, height);
		manager.getGuiRoot()->setViewport(0, 0, width, height);
		graphics.expect(0, 255, 0);

		const Uint32 windowId = SDL_GetWindowID(graphics.window);
		osg::ref_ptr<base::GraphicsWindowSDL2> retained = graphics.graphics;
		graphics.closeRenderer();
		require(!retained->valid() && !retained->isRealized(), "Closing the viewer must close its SDL context");
		require(SDL_GetWindowID(graphics.window) == windowId, "Closing the adapter must preserve the borrowed window");
		retained->close(true);
		require(SDL_GL_GetCurrentContext() == nullptr, "Closing the context must release it from the current thread");
	}

	void testRetainedGuiState()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xffffffff);
		submit(manager, buffer);
		osg::ref_ptr<osg::Node> retained = manager.getGuiRoot()->getChild(0);
		graphics.expect(255, 255, 255);
		osg::ref_ptr<osg::StateSet> state = new osg::StateSet;
		state->setAttribute(new osg::ColorMask(false, true, true, true), osg::StateAttribute::PROTECTED);
		manager.setGuiStateSet(state);
		for (int i = 0; i < 8; ++i)
		{
			submit(manager, buffer);
			graphics.expect(0, 255, 255);
		}
		manager.getGuiRoot()->setChild(0, retained);
		graphics.expect(255, 255, 255);
	}

	void testRetainedRenderTarget()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer content;
		MyGUI::OsgVertexBuffer quad;
		fillQuad(content, 0xff00ff00);
		fillQuad(quad, 0xffffffff);
		auto* texture = gui.texture("rtt");
		texture->createManual(64, 64, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		auto* target = texture->getRenderTarget();
		target->begin();
		target->doRender(&content, nullptr, 6);
		target->end();
		manager.flushRTTCameras();
		class CountPasses : public osg::Camera::DrawCallback
		{
		public:
			void operator()(osg::RenderInfo&) const override
			{
				++count;
			}
			mutable unsigned int count{};
		};
		osg::ref_ptr<CountPasses> passes = new CountPasses;
		auto* camera = dynamic_cast<osg::Camera*>(manager.getGuiRoot()->getChild(0));
		require(camera != nullptr, "RTT camera must be attached");
		camera->setPreDrawCallback(passes);
		osg::ref_ptr<osg::Node> retainedGreen = camera->getChild(0);
		submit(manager, quad, texture);
		for (int i = 0; i < 12; ++i)
		{
			fillQuad(content, 0xff0000ff);
			manager.createBatch(&content, nullptr, 6, nullptr);
			graphics.expect(0, 255, 0);
		}
		require(passes->count == 1, "An unchanged RTT must render only once");
		{
			RenderContext second(gui.root);
			second.expect(0, 255, 0);
			second.expect(0, 255, 0);
			graphics.expect(0, 255, 0);
			require(passes->count == 2, "Each independent context needs exactly one RTT draw");
		}
		target->begin();
		target->doRender(&content, nullptr, 6);
		target->end();
		graphics.expect(255, 0, 0);
		graphics.expect(255, 0, 0);
		require(passes->count == 3, "A replacement RTT submission must redraw once");
		osg::ref_ptr<osg::Node> currentRed = camera->getChild(0);
		camera->setChild(0, retainedGreen);
		graphics.expect(0, 255, 0);
		camera->setChild(0, currentRed);
		graphics.expect(255, 0, 0);
		require(passes->count == 5, "A delayed older packet must not mark newer RTT contents as current");
		graphics.graphics->makeCurrent();
		gui.root->releaseGLObjects(graphics.graphics->getState());
		graphics.expect(255, 0, 0);
		graphics.expect(255, 0, 0);
		require(passes->count == 6, "Releasing context resources must invalidate the RTT cache");
		// Empty submissions still clear the target once and cache the clear.
		target->begin();
		target->end();
		graphics.expect(0, 0, 0);
		graphics.expect(0, 0, 0);
		require(passes->count == 7, "An empty RTT update must clear the old pixels once");
	}

	void testRenderTargetDependencies()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer quad;
		fillQuad(quad, 0xffffffff);
		auto* source = gui.texture("source");
		source->createManual(1, 1, MyGUI::TextureUsage::Static, MyGUI::PixelFormat::R8G8B8A8);
		auto* bytes = static_cast<unsigned char*>(source->lock(MyGUI::TextureUsage::Write));
		bytes[0] = 0;
		bytes[1] = 255;
		bytes[2] = 0;
		bytes[3] = 255;
		source->unlock();
		auto* texture = gui.texture("rtt");
		texture->createManual(64, 64, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		auto* target = texture->getRenderTarget();
		target->begin();
		target->doRender(&quad, source, 6);
		target->end();
		manager.flushRTTCameras();
		submit(manager, quad, texture);
		graphics.expect(0, 255, 0);
		graphics.expect(0, 255, 0);
		// An external image can be dirtied without a new MyGUI batch submission.
		auto* image = source->getTexture()->getImage();
		image->data()[0] = 255;
		image->data()[1] = 0;
		image->dirty();
		graphics.expect(0, 0, 255);
		graphics.expect(0, 0, 255);
		// Updating an already-uploaded image elsewhere must not hide its revision.
		image->data()[0] = 0;
		image->data()[2] = 255;
		image->dirty();
		graphics.graphics->makeCurrent();
		source->getTexture()->apply(*graphics.graphics->getState());
		graphics.expect(255, 0, 0);
		// A dynamic texture's GPU contents can change without dirtying its image.
		source->getTexture()->setDataVariance(osg::Object::DYNAMIC);
		graphics.graphics->makeCurrent();
		source->getTexture()->apply(*graphics.graphics->getState());
		const unsigned char green[] = {0, 255, 0, 255};
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, green);
		graphics.expect(0, 255, 0);
		image->dirty();

		// Custom shader state can change independently of a batch submission.
		source->setShader("Default");
		target->begin();
		target->doRender(&quad, source, 6);
		target->end();
		graphics.expect(255, 0, 0);
		source->getShaderStateSet()->setAttribute(
			new osg::ColorMask(false, true, true, true),
			osg::StateAttribute::PROTECTED);
		graphics.expect(0, 0, 0);
	}

	void testNestedRenderTargets()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer quad;
		MyGUI::OsgVertexBuffer content;
		fillQuad(quad, 0xffffffff);
		fillQuad(content, 0xff00ff00);
		auto* producer = gui.texture("producer");
		producer->createManual(64, 64, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		auto* producerTarget = producer->getRenderTarget();
		auto* consumer = gui.texture("consumer");
		consumer->createManual(64, 64, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		auto* consumerTarget = consumer->getRenderTarget();
		producerTarget->begin();
		producerTarget->doRender(&content, nullptr, 6);
		producerTarget->end();
		consumerTarget->begin();
		consumerTarget->doRender(&quad, producer, 6);
		consumerTarget->end();
		manager.flushRTTCameras();
		// This test bypasses update traversal to retain submissions across frames.
		MyGUI::orderOsgRTTCameras(manager.getGuiRoot());
		submit(manager, quad, consumer);
		graphics.expect(0, 255, 0);
		graphics.expect(0, 255, 0);
		fillQuad(content, 0xff0000ff);
		producerTarget->begin();
		producerTarget->doRender(&content, nullptr, 6);
		producerTarget->end();
		graphics.expect(255, 0, 0);
		graphics.expect(255, 0, 0);
	}

	unsigned int failedMaps = 0;
	unsigned int failedUnmaps = 0;
	GLvoid* GL_APIENTRY failMap(GLenum, GLenum)
	{
		++failedMaps;
		return nullptr;
	}

	decltype(osg::GLExtensions::glUnmapBuffer) realUnmap = nullptr;
	GLboolean GL_APIENTRY failUnmap(GLenum target)
	{
		realUnmap(target);
		++failedUnmaps;
		return GL_FALSE;
	}

	void testStreamUploadFallbacks()
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext graphics(gui.root);
		MyGUI::OsgVertexBuffer buffer;
		fillQuad(buffer, 0xff0000ff);
		submit(manager, buffer);
		graphics.expect(255, 0, 0);
		auto* extensions = graphics.graphics->getState()->get<osg::GLExtensions>();
		struct Restore
		{
			osg::GLExtensions* extensions;
			decltype(osg::GLExtensions::glMapBuffer) map;
			decltype(osg::GLExtensions::glUnmapBuffer) unmap;
			~Restore()
			{
				extensions->glMapBuffer = map;
				extensions->glUnmapBuffer = unmap;
			}
		} restore{extensions, extensions->glMapBuffer, extensions->glUnmapBuffer};
		for (int mode = 0; mode < 3; ++mode)
		{
			extensions->glMapBuffer = mode == 0 ? nullptr : mode == 1 ? failMap : restore.map;
			realUnmap = restore.unmap;
			extensions->glUnmapBuffer = mode == 2 ? failUnmap : restore.unmap;
			for (int frame = 0; frame < 6; ++frame)
			{
				fillQuad(buffer, frame % 2 ? 0xff0000ff : 0xff00ff00);
				submit(manager, buffer);
				graphics.expect(frame % 2 ? 255 : 0, frame % 2 ? 0 : 255, 0);
			}
		}
		require(failedMaps > 0 && failedUnmaps > 0, "The streaming path must exercise both failure fallbacks");
		extensions->glMapBuffer = restore.map;
		extensions->glUnmapBuffer = restore.unmap;
		graphics.graphics->makeCurrent();
		gui.root->releaseGLObjects(graphics.graphics->getState());
		require(
			buffer.getVertexBuffer()->getGLBufferObject(graphics.graphics->getState()->getContextID()) == nullptr,
			"Drawables must release their retained VBOs with the context");
		graphics.expect(255, 0, 0);
		{
			RenderContext second(gui.root);
			for (int frame = 0; frame < 8; ++frame)
			{
				fillQuad(buffer, frame % 2 ? 0xff0000ff : 0xff00ff00);
				submit(manager, buffer);
				second.expect(frame % 2 ? 255 : 0, frame % 2 ? 0 : 255, 0);
				graphics.expect(frame % 2 ? 255 : 0, frame % 2 ? 0 : 255, 0);
			}
		}
		graphics.graphics->makeCurrent();
		const auto context = graphics.graphics->getState()->getContextID();
		osg::get<osg::GLBufferObjectManager>(context)->deleteAllGLObjects();
		require(
			buffer.getVertexBuffer()->getGLBufferObject(context) == nullptr,
			"OSG's manager must retain ownership of streaming buffer lifetime");
		graphics.expect(255, 0, 0);
	}

}

int runOsgRenderTests()
{
	return unittest::runTests({
		{"First frame with external OSG state", testFirstFrameWithExternalState},
		{"OpenGL error detection and recovery", testGlErrorDetection},
		{"Repeated draws, independent contexts and delayed snapshots", testRepeatedAndDelayedDraws},
		{"SDL drawable resize and borrowed window lifetime", testWindowResizeAndLifetime},
		{"GUI state changes preserve retained drawables", testRetainedGuiState},
		{"Retained RTT content and redraw", testRetainedRenderTarget},
		{"RTT image and shader dependencies", testRenderTargetDependencies},
		{"RTT dependencies refresh without consumer submissions", testNestedRenderTargets},
		{"Stream upload fallback and context recreation", testStreamUploadFallbacks},
	});
}

int runOsgBenchmark()
{
	using Clock = std::chrono::steady_clock;
	std::cout << "workload,update_us,frame_us\n";
	for (int mode = 0; mode < 3; ++mode)
	{
		unittest::OsgTestContext gui;
		auto& manager = gui.render();
		manager.getGuiRoot()->setUpdateCallback(nullptr);
		RenderContext first(gui.root);
		first.graphics->setBeforeSwapCallback({});
		first.graphics->setSyncToVBlank(false);
		std::unique_ptr<RenderContext> second;
		if (mode == 2)
		{
			second = std::make_unique<RenderContext>(gui.root);
			second->graphics->setBeforeSwapCallback({});
			second->graphics->setSyncToVBlank(false);
		}
		std::array<MyGUI::OsgVertexBuffer, 128> buffers;
		for (auto& buffer : buffers)
			fillQuad(buffer, 0xffffffff);
		double updateTime = 0;
		double frameTime = 0;
		constexpr int warmup = 60;
		constexpr int frames = 300;
		for (int frame = 0; frame < warmup + frames; ++frame)
		{
			const auto start = Clock::now();
			manager.begin();
			for (auto& buffer : buffers)
			{
				if (mode != 0)
					fillQuad(buffer, (frame % 2) ? 0xff0000ff : 0xff00ff00);
				manager.doRender(&buffer, nullptr, 6);
			}
			manager.end();
			const auto updated = Clock::now();
			first.viewer->frame();
			if (second)
				second->viewer->frame();
			const auto finished = Clock::now();
			if (frame >= warmup)
			{
				updateTime += std::chrono::duration<double, std::micro>(updated - start).count();
				frameTime += std::chrono::duration<double, std::micro>(finished - start).count();
			}
		}
		std::cout << (mode == 0		  ? "static"
						  : mode == 1 ? "animated"
									  : "animated_two_contexts")
				  << ',' << updateTime / frames << ',' << frameTime / frames << '\n';
	}
	return 0;
}
