/*!
	@file
	@author		Albert Semenov
	@date		05/2009
*/
#include "Precompiled.h"
#include "BaseManager.h"
#include "MyGUI_Diagnostic.h"

#include <SDL_image.h>
#include "GL/glew.h"

#ifdef MYGUI_CHECK_MEMORY_LEAKS
#	undef new
#	undef delete
#endif

// имя класса окна
const char* WND_CLASS_NAME = "MyGUI_Demo_window";

namespace base
{
	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		mWindow(nullptr),
		mContext(nullptr),
		mExit(false),
		mWindowOn(false),
		mResourceFileName("MyGUI_Core.xml")
	{
		// initialize SDL
		MYGUI_ASSERT(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_EVENTS) == 0, "Failed initializing SDL.");
		// initialize SDL_image
		MYGUI_ASSERT(IMG_Init(~0) != 0, "Failed to initializing SDL_image");
	}

	BaseManager::~BaseManager()
	{
		SDL_GL_DeleteContext(mContext);
		IMG_Quit();
		SDL_Quit();
	}

	void BaseManager::_windowResized( int w, int h )
{
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->setViewSize(w, h);

		setInputViewSize(w, h);
	}

	bool BaseManager::create(int _width, int _height)
	{
		const unsigned int width = _width;
		const unsigned int height = _height;
		bool windowed = true;

		// create window and position it at the center of the screen
		SDL_DisplayMode currDisp;
		MYGUI_ASSERT(SDL_GetCurrentDisplayMode(0, &currDisp) == 0, "Failed to retrieve screen info.");
		int left = (currDisp.w - width) / 2;
		int top = (currDisp.h - height) / 2;

		mWindow = SDL_CreateWindow("OpenGL Render Window", left, top, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		MYGUI_ASSERT(mWindow != nullptr, "Failed to create SDL window.");
		mContext = SDL_GL_CreateContext(mWindow);
		MYGUI_ASSERT(mContext != nullptr, "Failed to create SDL context.");
		mWindowOn = true;

		if (!createRender(width, height, windowed))
		{
			return false;
		}

		createGui();

		createInput();

		createPointerManager();

		// this needs to be called before createScene() since some demos require
		// screen size to properly position the widgets
		_windowResized(width, height);

		createScene();

		return true;
	}

	void BaseManager::run()
	{
		while (!mExit)
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
					default:
						break;
					}
					break;
				default:
					break;
				}
			}
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawOneFrame();
			if (!mWindowOn)
				SDL_Delay(50);
		}
	}

	void BaseManager::destroy()
	{
		destroyScene();

		destroyPointerManager();

		destroyInput();

		destroyGui();

		destroyRender();

	}

	void BaseManager::setupResources()
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
					bool root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root)
						mRootMedia = node->getContent();
				}
				addResourceLocation(node->getContent(), false);
			}
		}

		addResourceLocation(getRootMedia() + "/Common/Base");
	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OpenGLPlatform();
		mPlatform->initialise(this);

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise(mResourceFileName);

		SDL_StartTextInput();
	}

	void BaseManager::destroyGui()
	{
		SDL_StopTextInput();
		if (mGUI)
		{
			mGUI->shutdown();
			delete mGUI;
			mGUI = nullptr;
		}

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::setWindowMaximized(bool _value)
	{
		if (mWindow != nullptr && _value)
		{
			SDL_MaximizeWindow(mWindow);
		}
	}

	bool BaseManager::getWindowMaximized()
	{
		Uint32 windowState = SDL_GetWindowFlags(mWindow);
		return windowState & SDL_WINDOW_MAXIMIZED || windowState & SDL_WINDOW_FULLSCREEN;
	}

	void BaseManager::setWindowCoord(const MyGUI::IntCoord& _value)
	{
		if (_value.empty())
			return;

		MyGUI::IntCoord coord = _value;

		SDL_SetWindowPosition(mWindow, coord.left, coord.top);
	}

	MyGUI::IntCoord BaseManager::getWindowCoord()
	{
		int left, top, width, height;
		SDL_GetWindowPosition(mWindow, &left, &top);
		SDL_GetWindowSize(mWindow, &width, &height);
		return MyGUI::IntCoord(left, top, width, height);
	}

	void BaseManager::setWindowCaption(const std::wstring& _text)
	{
		MyGUI::UString title(_text);
		SDL_SetWindowTitle(mWindow, title.asUTF8_c_str());
	}

	void BaseManager::prepare()
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void BaseManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseMove(_absx, _absy, _absz);
	}

	void BaseManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMousePress(_absx, _absy, _id);
	}

	void BaseManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectMouseRelease(_absx, _absy, _id);
	}

	void BaseManager::injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text)
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

	void BaseManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		MyGUI::InputManager::getInstance().injectKeyRelease(_key);
	}

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		return true;
	}

	void BaseManager::drawOneFrame()
	{
		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		SDL_GL_SwapWindow(mWindow);
	}

	void BaseManager::destroyRender()
	{
	}

	void* BaseManager::convertPixelData(SDL_Surface *_image, MyGUI::PixelFormat& _myGuiPixelFormat)
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
		int bppSrc = pitchSrc / _image->w;
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

	void* BaseManager::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::OpenGLDataManager::getInstance().getDataPath(_filename);
		void* result = nullptr;
		SDL_Surface *image = nullptr;
		SDL_Surface *cvtImage = nullptr;		// converted surface with RGBA/RGB pixel format
		image = IMG_Load(fullname.c_str());
		if (image != nullptr) {
			_width = image->w;
			_height = image->h;

			int bpp = image->format->BytesPerPixel;
			if (bpp < 3) 
			{
				result = convertPixelData(image, _format);
			}
			else 
			{
				Uint32 pixelFmt = bpp == 3 ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_ARGB8888;
				cvtImage = SDL_ConvertSurfaceFormat(image, pixelFmt, 0);
				result = convertPixelData(cvtImage, _format);
				SDL_FreeSurface(cvtImage);
			}
			SDL_FreeSurface(image);
		}
		MYGUI_ASSERT(result != nullptr, "Failed to load image.");
		return result;
	}

	void BaseManager::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename)
	{

	}

	void BaseManager::quit()
	{
		mExit = true;
	}

	const std::string& BaseManager::getRootMedia()
	{
		return mRootMedia;
	}

	void BaseManager::setResourceFilename(const std::string& _flename)
	{
		mResourceFileName = _flename;
	}

} // namespace base
