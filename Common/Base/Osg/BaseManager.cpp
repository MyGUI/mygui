#include "Precompiled.h"
#include "BaseManager.h"
#include "MyGUI_FileSystemUtility.h"

#include <SDL_syswm.h>

#include <osg/GL>
#include <osg/GraphicsContext>
#include <osg/Image>
#include <osg/Viewport>
#include <osgDB/Registry>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/PolygonMode>

// When osgViewer/osgDB are linked statically, the linker drops the object files that
// register the X11 windowing system interface and the osgDB plugins. Referencing the
// exported symbols below forces those object files to be pulled in.
#if MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
extern "C" void graphicswindow_X11();
#endif

USE_OSGPLUGIN(png)
USE_OSGPLUGIN(freetype)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(tga)
USE_OSGPLUGIN(bmp)
USE_OSGPLUGIN(dds)
USE_OSGPLUGIN(osg)

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#include <osgViewer/api/Win32/GraphicsWindowWin32>
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
	#include <osgViewer/api/X11/GraphicsWindowX11>
#endif

#include <vector>
#include <functional>
#include <utility>
#include <stdexcept>

namespace base
{

	class FrameCaptureCallback : public osg::GraphicsContext::SwapCallback
	{
	public:
		explicit FrameCaptureCallback(std::function<void()> _capture) :
			mCapture(std::move(_capture))
		{
		}

		void swapBuffersImplementation(osg::GraphicsContext* _context) override
		{
			mCapture();
			_context->swapBuffersImplementation();
		}

	private:
		std::function<void()> mCapture;
	};

	BaseManager::BaseManager() :
		SdlBaseManager(SDL_WINDOW_OPENGL)
	{
	}

	BaseManager::~BaseManager() = default;

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		(void)_windowed;

		mRenderWidth = _width;
		mRenderHeight = _height;

		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = _width;
		traits->height = _height;
		traits->windowName = "MyGUI Render Window";
		traits->windowDecoration = true;
		traits->doubleBuffer = true;
		traits->readDISPLAY();
		traits->sampleBuffers = 0;
		traits->samples = 0;

		// attach the OSG graphics context to the existing SDL window
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE)
		{
			std::cerr << "Failed to SDL_GetWindowWMInfo: " << SDL_GetError() << std::endl;
			throw std::runtime_error("OSG application initialization failed");
		}

		// Referencing this symbol forces the linker to keep the object file that registers
		// the X11 windowing system interface when osgViewer is linked statically.
#if MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
		graphicswindow_X11();
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		traits->inheritedWindowData = new osgViewer::GraphicsWindowWin32::WindowData(wmInfo.info.win.window);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
		if (wmInfo.info.x11.window == 0)
		{
			std::cerr << "The OSG render system requires an X11 window, but the SDL window was created on a "
						 "different backend (e.g. Wayland). Set SDL_VIDEODRIVER=x11 to force X11."
					  << std::endl;
			throw std::runtime_error("OSG application initialization failed");
		}
		traits->inheritedWindowData = new osgViewer::GraphicsWindowX11::WindowData(wmInfo.info.x11.window);
#else
		std::cerr << "The OSG render system is not supported on this platform" << std::endl;
		return false;
#endif

		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
		if (!gc.valid())
		{
			std::cerr << "Failed to create the OSG graphics context" << std::endl;
			throw std::runtime_error("OSG application initialization failed");
		}

		gc->setSwapCallback(new FrameCaptureCallback([this] { captureFrame(); }));

		mViewer = new osgViewer::Viewer;
		mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
		mViewer->getCamera()->setGraphicsContext(gc);
		mViewer->getCamera()->setViewport(new osg::Viewport(0, 0, _width, _height));
		mViewer->getCamera()->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

		mSceneRoot = new osg::Group;
		mViewer->setSceneData(mSceneRoot);

		return true;
	}

	void BaseManager::destroyRender()
	{
		mViewer = nullptr;
		mSceneRoot = nullptr;
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::OsgPlatform();
		setupResources();
		mPlatform->initialise(mSceneRoot, mRenderWidth, mRenderHeight);
	}

	void BaseManager::destroyGuiPlatform()
	{
		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::drawOneFrame()
	{
		mViewer->frame();

		if (mScreenShotRequested)
		{
			mScreenShotRequested = false;

			osg::GraphicsContext* gc = mViewer->getCamera()->getGraphicsContext();
			std::vector<std::uint8_t> pixels(mRenderWidth * mRenderHeight * 4);
			gc->makeCurrent();
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			// Read the back buffer: reading GL_FRONT returns black with Mesa/llvmpipe.
			glReadBuffer(GL_BACK);
			glReadPixels(0, 0, mRenderWidth, mRenderHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			osg::ref_ptr<osg::Image> image = new osg::Image;
			image->setImage(
				mRenderWidth,
				mRenderHeight,
				1,
				GL_RGBA,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				pixels.data(),
				osg::Image::NO_DELETE);
			// osgDB::writeImageFile writes image rows bottom-up (the osg::Image convention),
			// so the raw GL_BACK readback (row 0 = bottom of the framebuffer) is written
			// to the PNG in the correct top-down order without any extra flipping.
			osgDB::writeImageFile(*image, MyGUI::utility::toUtf8(mScreenShotFile));
		}
	}

	void BaseManager::captureFrame()
	{
		if (!mCaptureRequested)
			return;
		const auto* traits = mViewer->getCamera()->getGraphicsContext()->getTraits();
		const int width = traits->width, height = traits->height;
		std::vector<std::uint8_t> pixels(size_t(width) * size_t(height) * 4);
		GLint packAlignment = 0;
		glGetIntegerv(GL_PACK_ALIGNMENT, &packAlignment);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
		glPixelStorei(GL_PACK_ALIGNMENT, packAlignment);
		std::vector<float> depth;
		if (mSceneDepthProbe)
		{
			for (const auto point :
				 {MyGUI::IntPoint(8, 8),
				  MyGUI::IntPoint(width - 9, 8),
				  MyGUI::IntPoint(8, height - 9),
				  MyGUI::IntPoint(width - 9, height - 9),
				  MyGUI::IntPoint(width / 2, height / 2)})
			{
				float sample = 0;
				glReadPixels(point.left, point.top, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &sample);
				depth.push_back(sample);
			}
		}
		const GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			failFrameCapture(
				"OSG OpenGL error during rendering/readback: " + std::to_string(error),
				error == GL_OUT_OF_MEMORY || error == 0x0507 /* GL_CONTEXT_LOST */);
		else
			completeFrameCapture(pixels.data(), width, height, size_t(width) * 4, false, true, depth);
	}

	void BaseManager::captureScreenshot()
	{
		if (mScreenShotRequested)
		{
			mScreenShotRequested = false;
			osg::GraphicsContext* gc = mViewer->getCamera()->getGraphicsContext();
			const int width = gc->getTraits()->width;
			const int height = gc->getTraits()->height;
			std::vector<std::uint8_t> pixels(width * height * 4);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			// Read the back buffer: reading GL_FRONT returns black with Mesa/llvmpipe.
			glReadBuffer(GL_BACK);
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

			osg::ref_ptr<osg::Image> image = new osg::Image;
			image->setImage(width, height, 1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data(), osg::Image::NO_DELETE);
			// The PNG writer expects bottom-up rows (the osg::Image convention),
			// so the raw GL_BACK readback (row 0 = bottom of the framebuffer) is written
			// to the PNG in the correct top-down order without any extra flipping.
			osgDB::writeImageFile(*image, MyGUI::utility::toUtf8(mScreenShotFile));
		}
	}

	bool BaseManager::setSceneDepthProbe(bool _enabled)
	{
		auto* context = mViewer->getCamera()->getGraphicsContext();
		if (_enabled && context->getTraits()->depth == 0)
			return false;
		mSceneDepthProbe = _enabled;
		mViewer->getCamera()->setClearDepth(_enabled ? 0.25 : 1.0);
		return true;
	}

	bool BaseManager::setHostileRenderState(bool _enabled)
	{
		auto* state = mSceneRoot->getOrCreateStateSet();
		state->setMode(
			GL_CULL_FACE,
			_enabled ? osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE : osg::StateAttribute::OFF);
		state->setAttributeAndModes(
			new osg::PolygonMode(
				osg::PolygonMode::FRONT_AND_BACK,
				_enabled ? osg::PolygonMode::LINE : osg::PolygonMode::FILL),
			osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		return true;
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		mViewer->getCamera()->getGraphicsContext()->resized(0, 0, _width, _height);
		mViewer->getCamera()->setViewport(0, 0, _width, _height);
	}

	void BaseManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

} // namespace base
