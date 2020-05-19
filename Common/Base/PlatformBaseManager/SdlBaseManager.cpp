#include "Precompiled.h"
#include "SdlBaseManager.h"

#include <SDL2/SDL_syswm.h>

namespace base
{
	SdlBaseManager::SdlBaseManager()
	{
	}

	void SdlBaseManager::_windowResized(int w, int h)
	{
		resizeRender(w, h);

		if (mPlatformReady)
			MyGUI::RenderManager::getInstance().setViewSize(w, h);

		setInputViewSize(w, h);
	}

	bool SdlBaseManager::create(int _width, int _height)
	{
		// initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "Failed to initialize SDL2.";
			exit(1);
		}

		const int width = _width;
		const int height = _height;
		bool windowed = true;

		// create window and position it at the center of the screen
		SDL_DisplayMode currDisp;
		if (SDL_GetCurrentDisplayMode(0, &currDisp) != 0)
		{
			std::cerr << "Failed to retrieve screen info.";
			exit(1);
		}
		int left = (currDisp.w - width) / 2;
		int top = (currDisp.h - height) / 2;

		mSdlWindow = SDL_CreateWindow("MyGUI Render Window", left, top, width, height, SDL_WINDOW_RESIZABLE);
		if (mSdlWindow == nullptr)
		{
			std::cerr << "Failed to create SDL window.";
			exit(1);
		}
		mWindowOn = true;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// set icon
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version)
		if (SDL_GetWindowWMInfo(mSdlWindow, &wmInfo) == SDL_FALSE)
		{
			std::cerr << "Failed to SDL_GetWindowWMInfo.";
			exit(1);
		}
		size_t handle = (size_t)wmInfo.info.win.window;

		char buf[MAX_PATH];
		::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
		HINSTANCE instance = ::GetModuleHandleA(buf);
		HICON hIconSmall = static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 32, 32, LR_DEFAULTSIZE));
		HICON hIconBig = static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 256, 256, LR_DEFAULTSIZE));
		if (hIconSmall)
			::SendMessageA((HWND)handle, WM_SETICON, 0, (LPARAM)hIconSmall);
		if (hIconBig)
			::SendMessageA((HWND)handle, WM_SETICON, 1, (LPARAM)hIconBig);
#endif

		if (!createRender(width, height, windowed))
		{
			return false;
		}

		createGuiPlatform();
		mPlatformReady = true;
		createGui();

		createInput();

		createPointerManager();

		// this needs to be called before createScene() since some demos require
		// screen size to properly position the widgets
		_windowResized(width, height);

		createScene();
		loadPointerResources();

		return true;
	}

	void SdlBaseManager::run()
	{
#ifndef EMSCRIPTEN
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
				case SDL_MOUSEMOTION:
					mouseMoved(mEvent.motion);
					break;
				case SDL_MOUSEBUTTONDOWN:
					mousePressed(mEvent.button);
					break;
				case SDL_MOUSEBUTTONUP:
					mouseReleased(mEvent.button);
					break;
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
					case SDL_WINDOWEVENT_CLOSE:
						mExit = true;
						break;
					case SDL_WINDOWEVENT_RESIZED:
						_windowResized(mEvent.window.data1, mEvent.window.data2);
						break;
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_RESTORED:
					case SDL_WINDOWEVENT_EXPOSED:
					case SDL_WINDOWEVENT_MAXIMIZED:
						mWindowOn = true;
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_HIDDEN:
						mWindowOn = false;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

			mFpsCounter++;
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

	void SdlBaseManager::setupResources()
	{
		MyGUI::xml::Document doc;

		if (!doc.open(std::string("resources.xml")))
			doc.getLastError();

		MyGUI::xml::ElementPtr root = doc.getRoot();
		if (root == nullptr || root->getName() != "Paths")
			return;

		MyGUI::xml::ElementEnumerator node = root->getElementEnumerator();
		while (node.next())
		{
			if (node->getName() == "Path")
			{
				if (node->findAttribute("root") != "")
				{
					bool rootAttribute = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (rootAttribute)
						mRootMedia = node->getContent();
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

	bool SdlBaseManager::getWindowMaximized()
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

	MyGUI::IntCoord SdlBaseManager::getWindowCoord()
	{
		int left, top, width, height;
		SDL_GetWindowPosition(mSdlWindow, &left, &top);
		SDL_GetWindowSize(mSdlWindow, &width, &height);
		return MyGUI::IntCoord(left, top, width, height);
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
		else if (_key == MyGUI::KeyCode::SysRq)
		{
			makeScreenShot();
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

	void* SdlBaseManager::convertPixelData(SDL_Surface *_image, MyGUI::PixelFormat& _myGuiPixelFormat)
	{
		void* ret = nullptr;
		SDL_PixelFormat *format = _image->format;
		unsigned int bpp = format->BytesPerPixel;
		switch (bpp) {
		case 1:
			_myGuiPixelFormat = MyGUI::PixelFormat::L8;
			break;
		case 2:
			_myGuiPixelFormat = MyGUI::PixelFormat::L8A8;
			break;
		case 3:
			_myGuiPixelFormat = MyGUI::PixelFormat::R8G8B8;
			break;
		case 4:
			_myGuiPixelFormat = MyGUI::PixelFormat::R8G8B8A8;
			break;
		default:
			break;
		}
		SDL_LockSurface(_image);

		int pitchSrc = _image->pitch;	//the length of a row of pixels in bytes
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

	const std::string& SdlBaseManager::getRootMedia()
	{
		return mRootMedia;
	}

	void SdlBaseManager::setResourceFilename(const std::string& _flename)
	{
		mResourceFileName = _flename;
	}

} // namespace base
