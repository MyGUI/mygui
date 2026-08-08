#include "Precompiled.h"
#include "SdlBaseManager.h"

#include <SDL_syswm.h>

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#include <shellapi.h>
#endif

#include <filesystem>
#include <fstream>

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
		int scaledW = static_cast<int>(w / mDpiScale);
		int scaledH = static_cast<int>(h / mDpiScale);

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

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		if (isScreenShotMode())
		{
			// Skip DPI scaling while rendering for the screenshot comparison.
			mDpiScale = 1.0f;
		}
		else
		{
			float ddpi;
			if (SDL_GetDisplayDPI(0, &ddpi, nullptr, nullptr) == 0 && ddpi > 0)
				mDpiScale = ddpi / 96.0f;
			else
				mDpiScale = 1.0f;
		}
#else
		mDpiScale = 1.0f;
#endif
		setDpiScale(mDpiScale);

		const int width = static_cast<int>(_width * mDpiScale);
		const int height = static_cast<int>(_height * mDpiScale);
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

		mSdlWindow = SDL_CreateWindow(
			"MyGUI Render Window",
			left,
			top,
			width,
			height,
			mWindowFlags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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
#ifndef __EMSCRIPTEN__
		SDL_GL_SetSwapInterval(mEnableVSync ? 1 : 0);
#endif

		createGuiPlatform();
		mPlatformReady = true;
		createGui();
		createInput();
		createPointerManager();

		if (mDpiScale != 1.0f)
			MYGUI_LOG(Info, "Using DPI scale: " << mDpiScale);

		// this needs to be called before createScene() since some demos require
		// screen size to properly position the widgets
		_windowResized(width, height);

		createScene();
		loadPointerResources();

		return true;
	}

	void SdlBaseManager::run()
	{
#ifndef __EMSCRIPTEN__
		while (!mExit)
#endif
		{
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
				case SDL_DROPFILE:
					break;
					// windows events
				case SDL_WINDOWEVENT:
					switch (mEvent.window.event)
					{
					case SDL_WINDOWEVENT_CLOSE: mExit = true; break;
					case SDL_WINDOWEVENT_RESIZED: _windowResized(mEvent.window.data1, mEvent.window.data2); break;
					case SDL_WINDOWEVENT_FOCUS_GAINED: mWindowOn = true; break;
					case SDL_WINDOWEVENT_FOCUS_LOST: mWindowOn = false; break;
					default: break;
					}
					break;
				default: break;
				}
			}

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

		destroyRender();

		SDL_Quit();
	}

	void SdlBaseManager::setupBinaryDir()
	{
		char* basePath = SDL_GetBasePath();
		if (basePath)
		{
			mBinaryDir = std::filesystem::path(basePath).parent_path();
			SDL_free(basePath);
		}
		else
		{
			mBinaryDir = std::filesystem::current_path();
		}
	}

	void SdlBaseManager::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open((mBinaryDir / "resources.xml").string()))
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
				if (!node->findAttribute("root").empty())
				{
					bool rootAttribute = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (rootAttribute)
					{
						mRootMedia = node->getContent();
						if (!std::filesystem::path(mRootMedia).is_absolute())
							mRootMedia = (mBinaryDir / mRootMedia).string();
					}
				}
				addResourceLocation(node->getContent(), false);
			}
		}

		addResourceLocation(getRootMedia() + "/Common/Base");
	}

	MyGUI::MapString SdlBaseManager::getStatistic()
	{
		MyGUI::MapString statistics;
		statistics["FPS"] = MyGUI::utility::toString(mFpsCounter);
		mFpsCounter = 0;
		return statistics;
	}

	void SdlBaseManager::createGui()
	{
		mGUI = new MyGUI::Gui();
		mGUI->setDpiScale(mDpiScale);
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

	void SdlBaseManager::setWindowCaption(const std::wstring& _text)
	{
		MyGUI::UString title(_text);
		SDL_SetWindowTitle(mSdlWindow, title.asUTF8_c_str());
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

	const std::string& SdlBaseManager::getRootMedia() const
	{
		return mRootMedia;
	}

	void SdlBaseManager::setResourceFilename(std::string_view _flename)
	{
		mResourceFileName = _flename;
	}

	void SdlBaseManager::makeScreenShot()
	{
		std::ifstream stream;
		std::string file;
		do
		{
			stream.close();
			static size_t num = 0;
			const size_t max_shot = 100;
			if (num == max_shot)
			{
				MYGUI_LOG(Info, "The limit of screenshots is exceeded : " << max_shot);
				return;
			}
			file = MyGUI::utility::toString("screenshot_", ++num, ".png");
			stream.open(file.c_str());
		} while (stream.is_open());
		mScreenShotFile = file;
		mScreenShotRequested = true;
	}

	void SdlBaseManager::setCommandLine(int _argc, char** _argv)
	{
#ifndef __EMSCRIPTEN__
		std::vector<std::wstring> args;
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
			args.push_back(MyGUI::UString(_argv[i]).asWStr());
	#endif

		for (size_t i = 1; i < args.size(); ++i)
		{
			if (args[i] == L"--screenshot")
			{
				const std::wstring file = (i + 1 < args.size()) ? args[++i] : L"screenshot.png";
				mScreenShotFile = std::filesystem::absolute(MyGUI::UString(file).asUTF8()).string();
				mScreenShotExit = true;
				mScreenShotFramesLeft = cScreenShotFrames;
				std::cerr << "Screenshot will be saved to " << mScreenShotFile << std::endl;
				continue;
			}

			const std::string file = MyGUI::UString(args[i]).asUTF8();
			std::error_code ec;
			if (std::filesystem::is_regular_file(file, ec))
				mParams.push_back(MyGUI::UString(std::filesystem::absolute(file).string()).asWStr());
		}
#endif
	}

	const SdlBaseManager::VectorWString& SdlBaseManager::getParams() const
	{
		return mParams;
	}

	bool SdlBaseManager::isScreenShotMode() const
	{
		return mScreenShotExit;
	}

} // namespace base
