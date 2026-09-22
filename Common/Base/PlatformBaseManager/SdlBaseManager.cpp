#include "Precompiled.h"
#include "SdlBaseManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#include <SDL_syswm.h>
	#include <shellapi.h>
#endif

#include <filesystem>
#include <stdexcept>
#include <cstring>
#include "MyGUI_FileSystemUtility.h"

namespace base
{

	// number of frames to render before capturing a screenshot from the command line
	static constexpr int cScreenShotFrames = 2;

	SdlBaseManager::SdlBaseManager(uint32_t _windowFlags) :
		mWindowFlags(_windowFlags)
	{
	}

	void SdlBaseManager::_windowResized(int w, int h)
	{
		int scaledW = static_cast<int>(w / mWindowScale);
		int scaledH = static_cast<int>(h / mWindowScale);

		if (mPlatformReady)
			MyGUI::RenderManager::getInstance().setViewSize(scaledW, scaledH);

		setInputViewSize(scaledW, scaledH);
		resizeRender(w, h);
	}

	bool SdlBaseManager::create(int _width, int _height)
	{
		// initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << std::endl;
			exit(1);
		}

		float dpiScale = 1.0f;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		// Skip DPI scaling while rendering for the screenshot comparison.
		if (!isScreenShotMode() && !mFixedPixels)
		{
			float ddpi;
			if (SDL_GetDisplayDPI(0, &ddpi, nullptr, nullptr) == 0 && ddpi > 0)
				dpiScale = ddpi / 96.0f;
		}
#endif
		mWindowScale = dpiScale;
		setDpiScale(mWindowScale);

		const int width = static_cast<int>(_width * mWindowScale);
		const int height = static_cast<int>(_height * mWindowScale);
		bool windowed = true;

		// create window and position it at the center of the screen
		SDL_DisplayMode currDisp;
		if (SDL_GetCurrentDisplayMode(0, &currDisp) != 0)
		{
			std::cerr << "Failed to retrieve screen info: " << SDL_GetError() << std::endl;
			exit(1);
		}
		int left = (currDisp.w - _width) / 2;
		int top = (currDisp.h - _height) / 2;

		uint32_t windowFlags = mWindowFlags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
		// Screenshot references use one drawable pixel per logical coordinate, including on Retina displays.
		if (isScreenShotMode())
		{
			windowFlags |= SDL_WINDOW_HIDDEN;
			windowFlags &= ~SDL_WINDOW_ALLOW_HIGHDPI;
		}

		if (mHiddenWindow)
			windowFlags |= SDL_WINDOW_HIDDEN;
		if (mFixedPixels)
			windowFlags &= ~SDL_WINDOW_ALLOW_HIGHDPI;

		setupRenderWindow();
		mSdlWindow = SDL_CreateWindow(mApplicationName.c_str(), left, top, width, height, windowFlags);
		if (mSdlWindow == nullptr)
		{
			std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
			exit(1);
		}
		mWindowOn = true;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// set icon
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE)
		{
			std::cerr << "Failed to SDL_GetWindowWMInfo: " << SDL_GetError() << std::endl;
			exit(1);
		}
		size_t handle = (size_t)wmInfo.info.win.window;

		char buf[MAX_PATH];
		::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
		HINSTANCE instance = ::GetModuleHandleA(buf);
		HICON hIconSmall =
			static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 32, 32, LR_DEFAULTSIZE));
		HICON hIconBig =
			static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 256, 256, LR_DEFAULTSIZE));
		if (hIconSmall)
			::SendMessageA((HWND)handle, WM_SETICON, 0, (LPARAM)hIconSmall);
		if (hIconBig)
			::SendMessageA((HWND)handle, WM_SETICON, 1, (LPARAM)hIconBig);
#endif

		setupBinaryDir();

		if (!createRender(width, height, windowed))
		{
			return false;
		}
#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE || defined(__EMSCRIPTEN__)
		// Retina and browser DPI scaling enlarge the drawable while window and mouse coordinates stay logical.
		// Query after creating the rendering context, before Gui initialises its font resources.
		int windowWidth = 0;
		int drawableWidth = 0;
		SDL_GetWindowSize(mSdlWindow, &windowWidth, nullptr);
		SDL_GetWindowSizeInPixels(mSdlWindow, &drawableWidth, nullptr);
		if (windowWidth > 0 && drawableWidth > 0)
			dpiScale = static_cast<float>(drawableWidth) / windowWidth;
#endif
#ifndef __EMSCRIPTEN__
		SDL_GL_SetSwapInterval(mEnableVSync ? 1 : 0);
#endif

		createGuiPlatform();
		mPlatformReady = true;
		createGui(dpiScale);
		createInput();
		createPointerManager();

		if (dpiScale != 1.0f)
			MYGUI_LOG(Info, "Using DPI scale: " << dpiScale);

		// this needs to be called before createScene() since some demos require
		// screen size to properly position the widgets
		_windowResized(width, height);

		createScene();
		loadPointerResources();

		if (isScreenShotMode())
		{
			// fire single deterministic update and disable further updates
			MyGUI::Gui::getInstance().eventFrameStart(0.1f);
			MyGUI::Gui::getInstance().eventFrameStart.clear();
		}
		return true;
	}

	void SdlBaseManager::run()
	{
#ifndef __EMSCRIPTEN__
		while (!mExit)
#endif
		{
			bool closeRequested = false;
			while (SDL_PollEvent(&mEvent) != 0)
			{
				switch (mEvent.type)
				{
					// keyboard events
				case SDL_KEYDOWN:
					mKeyCode = mEvent.key.keysym.sym;
					keyPressed(mKeyCode, nullptr);
					break;
				case SDL_TEXTINPUT:
					mKeyCode = SDLK_UNKNOWN;
					keyPressed(mKeyCode, &mEvent.text);
					break;
				case SDL_KEYUP:
					keyReleased(mEvent.key);
					break;
					// mouse events
				case SDL_MOUSEMOTION: mouseMoved(mEvent.motion); break;
				case SDL_MOUSEBUTTONDOWN: mousePressed(mEvent.button); break;
				case SDL_MOUSEBUTTONUP: mouseReleased(mEvent.button); break;
				case SDL_MOUSEWHEEL:
					mouseWheelMoved(mEvent.wheel);
					break;
					// drop file events
				case SDL_DROPFILE: break;
				case SDL_QUIT:
					closeRequested = true;
					break;
					// windows events
				case SDL_WINDOWEVENT:
					switch (mEvent.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE:
						if (mSdlWindow && mEvent.window.windowID == SDL_GetWindowID(mSdlWindow))
							closeRequested = true;
						break;
					case SDL_WINDOWEVENT_RESIZED: _windowResized(mEvent.window.data1, mEvent.window.data2); break;
					case SDL_WINDOWEVENT_FOCUS_GAINED: mWindowOn = true; break;
					case SDL_WINDOWEVENT_FOCUS_LOST: mWindowOn = false; break;
					default: break;
					}
					break;
				default: break;
				}
			}

			// A window close and SDL_QUIT can arrive together. Ask only once per frame so a second
			// request cannot dismiss the confirmation dialog opened by the first.
			if (closeRequested && !mExit && onWindowClose())
				quit();

			mFpsCounter++;

			if (mScreenShotExit)
			{
				--mScreenShotFramesLeft;
				if (mScreenShotFramesLeft == 0)
					mScreenShotRequested = true;
				if (mScreenShotFramesLeft < 0)
					mExit = true;
			}

			drawOneFrame();
			if (!mWindowOn)
				SDL_Delay(50);
		}
	}

	void SdlBaseManager::destroy()
	{
		destroyScene();
		destroyPointerManager();
		destroyInput();
		destroyGui();
		mPlatformReady = false;
		destroyRender();
		SDL_DestroyWindow(mSdlWindow);
		mSdlWindow = nullptr;
		SDL_Quit();
	}

	void SdlBaseManager::setWindowOptions(bool _hidden, bool _fixedPixels)
	{
		if (mSdlWindow)
			throw std::logic_error("Set window options before creation");
		mHiddenWindow = _hidden;
		mFixedPixels = _fixedPixels;
	}

	bool SdlBaseManager::isWindowHidden() const
	{
		return mSdlWindow && (SDL_GetWindowFlags(mSdlWindow) & SDL_WINDOW_HIDDEN) != 0;
	}

	MyGUI::IntSize SdlBaseManager::getDrawableSize() const
	{
		int width = 0, height = 0;
		if (mSdlWindow)
			SDL_GetWindowSizeInPixels(mSdlWindow, &width, &height);
		return {width, height};
	}

	void SdlBaseManager::resizeWindow(int _width, int _height)
	{
		SDL_SetWindowSize(mSdlWindow, _width, _height);
		SDL_PumpEvents();
		_windowResized(_width, _height);
	}

	void SdlBaseManager::requestFrameCapture()
	{
		if (mCaptureRequested)
			throw std::logic_error("A frame capture is already pending");
		mFrameCapture = {};
		mFrameCapture.status = FrameCapture::Status::Pending;
		mCaptureRequested = true;
	}

	const SdlBaseManager::FrameCapture& SdlBaseManager::getFrameCapture() const
	{
		return mFrameCapture;
	}

	void SdlBaseManager::completeFrameCapture(
		const void* _pixels,
		int _width,
		int _height,
		size_t _pitch,
		bool _bgra,
		bool _bottomUp,
		const std::vector<float>& _depth)
	{
		if (!mCaptureRequested)
			return;
		if (!_pixels || _width <= 0 || _height <= 0 || _pitch < size_t(_width) * 4)
		{
			failFrameCapture("Invalid frame readback");
			return;
		}
		mFrameCapture.width = _width;
		mFrameCapture.height = _height;
		mFrameCapture.depth = _depth;
		mFrameCapture.rgba.resize(size_t(_width) * size_t(_height) * 4);
		const auto* source = static_cast<const std::uint8_t*>(_pixels);
		for (int y = 0; y < _height; ++y)
		{
			auto* row = mFrameCapture.rgba.data() + size_t(y) * size_t(_width) * 4;
			std::memcpy(row, source + size_t(_bottomUp ? _height - 1 - y : y) * _pitch, size_t(_width) * 4);
			if (_bgra)
				for (int x = 0; x < _width; ++x)
					std::swap(row[x * 4], row[x * 4 + 2]);
		}
		mFrameCapture.status = FrameCapture::Status::Complete;
		mCaptureRequested = false;
	}

	void SdlBaseManager::failFrameCapture(std::string_view _error, bool _fatal)
	{
		mFrameCapture.status = FrameCapture::Status::Failed;
		mFrameCapture.error = _error;
		mFrameCapture.fatal = _fatal;
		mCaptureRequested = false;
	}

	void SdlBaseManager::setupBinaryDir()
	{
		char* basePath = SDL_GetBasePath();
		if (basePath)
		{
			mBinaryDir = MyGUI::utility::toPath(basePath).parent_path();
			SDL_free(basePath);
		}
		else
		{
			mBinaryDir = std::filesystem::current_path();
		}
	}

	void SdlBaseManager::setWorkingDirectoryToBinary()
	{
		setupBinaryDir();
		std::filesystem::current_path(mBinaryDir);
	}

	void SdlBaseManager::restoreWindowIfMinimized()
	{
		if (mSdlWindow && (SDL_GetWindowFlags(mSdlWindow) & SDL_WINDOW_MINIMIZED) != 0)
			SDL_RestoreWindow(mSdlWindow);
	}

	void SdlBaseManager::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open(mBinaryDir / "resources.xml"))
		{
			std::cerr << "Failed to load resources.xml: " << doc.getLastError() << std::endl;
			exit(1);
		}

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				const auto path = MyGUI::utility::toPath(node->getContent());
				if (!node->findAttribute("root").empty())
				{
					bool rootAttribute = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (rootAttribute)
					{
						mRootMedia = mBinaryDir / path;
					}
				}
				addResourceLocation(path, false);
			}
		}

		addResourceLocation(getRootMedia() / "Common/Base");
	}

	MyGUI::MapString SdlBaseManager::getStatistic()
	{
		MyGUI::MapString statistics;
		statistics["FPS"] = MyGUI::utility::toString(mFpsCounter);
		mFpsCounter = 0;
		return statistics;
	}

	void SdlBaseManager::createGui(float _dpiScale)
	{
		mGUI = new MyGUI::Gui();
		mGUI->setDpiScale(_dpiScale);
		mGUI->initialise(mResourceFileName);

		SDL_StartTextInput();
	}

	void SdlBaseManager::destroyGui()
	{
		SDL_StopTextInput();
		if (mGUI)
		{
			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		destroyGuiPlatform();
	}

	void SdlBaseManager::setWindowMaximized(bool _value)
	{
		if (mSdlWindow != nullptr && _value)
		{
			SDL_MaximizeWindow(mSdlWindow);
		}
	}

	bool SdlBaseManager::getWindowMaximized() const
	{
		Uint32 windowState = SDL_GetWindowFlags(mSdlWindow);
		return windowState & SDL_WINDOW_MAXIMIZED || windowState & SDL_WINDOW_FULLSCREEN;
	}

	void SdlBaseManager::setWindowCoord(const MyGUI::IntCoord& _value)
	{
		if (_value.empty())
			return;

		MyGUI::IntCoord coord = _value;

		SDL_SetWindowPosition(mSdlWindow, coord.left, coord.top);
	}

	MyGUI::IntCoord SdlBaseManager::getWindowCoord() const
	{
		int left;
		int top;
		int width;
		int height;
		SDL_GetWindowPosition(mSdlWindow, &left, &top);
		SDL_GetWindowSize(mSdlWindow, &width, &height);
		return {left, top, width, height};
	}

	void SdlBaseManager::setWindowCaption(const MyGUI::UString& _text)
	{
		SDL_SetWindowTitle(mSdlWindow, _text.asUTF8().c_str());
	}

	void SdlBaseManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}

	void SdlBaseManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}

	void SdlBaseManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}

	void SdlBaseManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
	{
		if (!mGUI)
			return;

		if (_key == MyGUI::KeyCode::Escape)
		{
			mExit = true;
			return;
		}

		MyGUI::InputManager::getInstance().injectKeyPress(_key, _text);
	}

	void SdlBaseManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectKeyRelease(_key);
	}

	void* SdlBaseManager::convertPixelData(SDL_Surface* _image, MyGUI::PixelFormat& _myGuiPixelFormat)
	{
		void* ret = nullptr;
		unsigned int bpp = _image->format->BytesPerPixel;
		switch (bpp)
		{
		case 1: _myGuiPixelFormat = MyGUI::PixelFormat::L8; break;
		case 2: _myGuiPixelFormat = MyGUI::PixelFormat::L8A8; break;
		case 3: _myGuiPixelFormat = MyGUI::PixelFormat::R8G8B8; break;
		case 4: _myGuiPixelFormat = MyGUI::PixelFormat::R8G8B8A8; break;
		default: break;
		}
		SDL_LockSurface(_image);

		int pitchSrc = _image->pitch; //the length of a row of pixels in bytes
		size_t size = _image->h * pitchSrc;
		ret = new unsigned char[size];
		unsigned char* ptr_source = (unsigned char*)_image->pixels;
		unsigned char* ptr_dst = (unsigned char*)ret;
		int pitchDst = _image->w * bpp;
		if (pitchSrc == pitchDst)
		{
			memcpy(ret, _image->pixels, size);
		}
		else
		{
			for (unsigned int y = 0; y < (unsigned int)_image->h; ++y)
			{
				memcpy(ptr_dst, ptr_source, pitchDst);
				ptr_dst += pitchDst;
				ptr_source += pitchSrc;
			}
		}

		SDL_UnlockSurface(_image);
		return ret;
	}

	void SdlBaseManager::quit()
	{
		mExit = true;
	}

	const std::filesystem::path& SdlBaseManager::getRootMedia() const
	{
		return mRootMedia;
	}

	void SdlBaseManager::setResourceFilename(std::string_view _flename)
	{
		mResourceFileName = _flename;
	}

	void SdlBaseManager::makeScreenShot()
	{
		std::filesystem::path file;
		do
		{
			static size_t num = 0;
			const size_t max_shot = 100;
			if (num == max_shot)
			{
				MYGUI_LOG(Info, "The limit of screenshots is exceeded : " << max_shot);
				return;
			}
			file = MyGUI::utility::toString("screenshot_", ++num, ".png");
		} while (std::filesystem::exists(file));
		mScreenShotFile = file;
		mScreenShotRequested = true;
	}

	void SdlBaseManager::setCommandLine(int _argc, char** _argv)
	{
		VectorPath args;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		int argc = 0;
		wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (argv != nullptr)
		{
			args.assign(argv, argv + argc);
			LocalFree(argv);
		}
#else
		args.reserve(_argc);
		for (int i = 0; i < _argc; ++i)
			args.push_back(MyGUI::utility::toPath(_argv[i]));
#endif

		if (!args.empty() && !args.front().stem().empty())
			mApplicationName =
				"MyGUI " + MyGUI::utility::toUtf8(args.front().stem()) + " (" MYGUI_RENDER_PLATFORM_NAME ")";

		for (size_t i = 1; i < args.size(); ++i)
		{
			if (args[i] == "--screenshot")
			{
				mScreenShotFile = std::filesystem::absolute((i + 1 < args.size()) ? args[++i] : "screenshot.png");
				mScreenShotExit = true;
				mScreenShotFramesLeft = cScreenShotFrames;
				std::cerr << "Screenshot will be saved to " << MyGUI::utility::toUtf8(mScreenShotFile) << std::endl;
				continue;
			}

			const auto& file = args[i];
			std::error_code ec;
			if (std::filesystem::is_regular_file(file, ec))
				mParams.push_back(std::filesystem::absolute(file));
		}
	}

	const SdlBaseManager::VectorPath& SdlBaseManager::getParams() const
	{
		return mParams;
	}

	bool SdlBaseManager::isScreenShotMode() const
	{
		return mScreenShotExit;
	}

} // namespace base
