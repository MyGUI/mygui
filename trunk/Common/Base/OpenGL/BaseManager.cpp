/*!
	@file
	@author		Albert Semenov
	@date		05/2009
*/


#include "Precompiled.h"
#include <windows.h>

#ifdef MYGUI_CHECK_MEMORY_LEAKS
#	undef new
#	undef delete
#endif

#include <gdiplus.h>
#include "BaseManager.h"

#include <GL/gl.h>
#include <GL/glu.h>

//for image loader
#include <malloc.h>

#ifdef __MINGW32__
using namespace Gdiplus;
#endif

// имя класса окна
const char* WND_CLASS_NAME = "MyGUI_Demo_window";

LRESULT CALLBACK DXWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
		break;
	}

	case WM_MOVE:
	case WM_SIZE:
	{
		base::BaseManager* baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (baseManager)
			baseManager->_windowResized();
		break;
	}

	case WM_CLOSE:
	{
		base::BaseManager* baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (baseManager)
			baseManager->quit();
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	}
	return 0;
}

namespace base
{

	ULONG_PTR gdiplusToken;

	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		hWnd(0),
		hDC(0),
		hRC(0),
		mExit(false),
		mResourceFileName("MyGUI_Core.xml")
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	BaseManager::~BaseManager()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	void BaseManager::_windowResized()
	{
		RECT rect = { 0, 0, 0, 0 };
		GetClientRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		resizeRender(width, height);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->setViewSize(width, height);

		setInputViewSize(width, height);
	}

	bool BaseManager::create(int _width, int _height)
	{
		const unsigned int width = _width;
		const unsigned int height = _height;
		bool windowed = true;

		// регистрируем класс окна
		WNDCLASS wc =
		{
			0, (WNDPROC)DXWndProc, 0, 0, GetModuleHandle(NULL), LoadIcon(NULL, MAKEINTRESOURCE(1001)),
			LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, TEXT(WND_CLASS_NAME),
		};
		RegisterClass(&wc);

		// создаем главное окно
		hWnd = CreateWindow(wc.lpszClassName, TEXT("OpenGL Render Window"), WS_POPUP,
			0, 0, 0, 0, GetDesktopWindow(), NULL, wc.hInstance, this);
		if (!hWnd)
		{
			//OutException("fatal error!", "failed create window");
			return false;
		}

	#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		char buf[MAX_PATH];
		::GetModuleFileNameA(0, (LPCH)&buf, MAX_PATH);
		HINSTANCE instance = ::GetModuleHandleA(buf);
		HICON hIconSmall = static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 32, 32, LR_DEFAULTSIZE));
		HICON hIconBig = static_cast<HICON>(LoadImage(instance, MAKEINTRESOURCE(1001), IMAGE_ICON, 256, 256, LR_DEFAULTSIZE));
		if (hIconSmall)
			::SendMessageA(hWnd, WM_SETICON, 0, (LPARAM)hIconSmall);
		if (hIconBig)
			::SendMessageA(hWnd, WM_SETICON, 1, (LPARAM)hIconBig);
	#endif

		hInstance = wc.hInstance;

		windowAdjustSettings(hWnd, width, height, !windowed);

		if (!createRender(width, height, windowed))
		{
			return false;
		}

		createGui();

		createInput((size_t)hWnd);

		createPointerManager((size_t)hWnd);

		createScene();

		_windowResized();

		return true;
	}

	void BaseManager::run()
	{
		MSG msg;
		while (true)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (mExit)
				break;
			else if (msg.message == WM_QUIT)
				break;

			captureInput();
			drawOneFrame();

			if (GetActiveWindow() != hWnd)
				::Sleep(50);
		}
	}

	void BaseManager::destroy()
	{
		destroyScene();

		destroyPointerManager();

		destroyInput();

		destroyGui();

		destroyRender();

		if (hWnd)
		{
			DestroyWindow(hWnd);
			hWnd = 0;
		}

		UnregisterClass(WND_CLASS_NAME, hInstance);
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
	}

	void BaseManager::destroyGui()
	{
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

	size_t BaseManager::getWindowHandle()
	{
		return (size_t)hWnd;
	}

	void BaseManager::setWindowCaption(const std::wstring& _text)
	{
		SetWindowTextW(hWnd, _text.c_str());
	}

	void BaseManager::prepare()
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void BaseManager::windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen)
	{
		// стиль окна
		HWND hwndAfter = 0;
		unsigned long style = 0;
		unsigned long style_ex = 0;

		RECT rc = { 0, 0, width, height };

		if (fullScreen)
		{
			style = WS_POPUP | WS_VISIBLE;
			style_ex = GetWindowLongPtr(hWnd, GWL_EXSTYLE) | (WS_EX_TOPMOST);
			hwndAfter = HWND_TOPMOST;
		}
		else
		{
			style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
			style_ex = GetWindowLongPtr(hWnd, GWL_EXSTYLE) & (~WS_EX_TOPMOST);
			hwndAfter = HWND_NOTOPMOST;
			AdjustWindowRect(&rc, style, false);
		}

		SetWindowLongPtr(hWnd, GWL_STYLE, style);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, style_ex);

		int desk_width  = GetSystemMetrics(SM_CXSCREEN);
		int desk_height = GetSystemMetrics(SM_CYSCREEN);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		int x = fullScreen ? 0 : (desk_width  - w) / 2;
		int y = fullScreen ? 0 : (desk_height - h) / 2;

		SetWindowPos(hWnd, hwndAfter, x, y, w, h, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
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

	void BaseManager::resizeRender(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;

		glViewport(0, 0, _width, _height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f, (GLfloat)_width / (GLfloat)_height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		BYTE bits = 16;

		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | // Format Must Support Window
			PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
			PFD_DOUBLEBUFFER, // Must Support Double Buffering
			PFD_TYPE_RGBA, // Request An RGBA Format
			bits, // Select Our Color Depth
			0, 0, 0, 0, 0, 0, // Color Bits Ignored
			0, // No Alpha Buffer
			0, // Shift Bit Ignored
			0, // No Accumulation Buffer
			0, 0, 0, 0, // Accumulation Bits Ignored
			16, // 16Bit Z-Buffer (Depth Buffer)
			0, // No Stencil Buffer
			0, // No Auxiliary Buffer
			PFD_MAIN_PLANE, // Main Drawing Layer
			0, // Reserved
			0, 0, 0 // Layer Masks Ignored
		};

		GLuint pixel_format;

		hDC = GetDC(hWnd);
		if (!hDC)
		{
			return false;
		}

		pixel_format = ChoosePixelFormat(hDC, &pfd);
		if (!pixel_format)
		{
			return false;
		}

		if (!SetPixelFormat(hDC, pixel_format, &pfd))
		{
			return false;
		}

		hRC = wglCreateContext(hDC);
		if (!hRC)
		{
			return false;
		}

		if (!wglMakeCurrent(hDC, hRC))
		{
			return false;
		}

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		return true;
	}

	void BaseManager::drawOneFrame()
	{
		// First we clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Then we reset the modelview matrix
		glLoadIdentity();

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		SwapBuffers(hDC);
	}

	void BaseManager::destroyRender()
	{
		if (hRC)
		{
			if (!wglMakeCurrent(NULL, NULL))
			{
			}
			if (!wglDeleteContext(hRC))
			{
			}
			hRC = 0;
		}

		if (hDC && !ReleaseDC(hWnd, hDC))
		{
			hDC = 0;
		}
	}

	void convertRawData(Gdiplus::BitmapData* _out_data, void* _result, size_t _size, MyGUI::PixelFormat _format)
	{
		size_t num = 0;

		if (_format == MyGUI::PixelFormat::L8)
		{
			num = 1;
		}
		if (_format == MyGUI::PixelFormat::L8A8)
		{
			num = 2;
		}
		if (_format == MyGUI::PixelFormat::R8G8B8)
		{
			num = 3;
		}
		else if (_format == MyGUI::PixelFormat::R8G8B8A8)
		{
			num = 4;
		}
		else
		{
			return;
		}

		unsigned char* ptr_source = (unsigned char*)_out_data->Scan0;
		unsigned char* ptr_dest = (unsigned char*)_result;

		size_t stride_source = _out_data->Stride;
		size_t stride_dest = _out_data->Width * num;

		if (stride_dest == stride_source)
		{
			memcpy(_result, _out_data->Scan0, _size);
		}
		else
		{
			for (unsigned int y = 0; y < _out_data->Height; ++y)
			{
				memcpy(ptr_dest, ptr_source, stride_dest);
				ptr_dest += stride_dest;
				ptr_source += stride_source;
			}
		}
	}

	void* BaseManager::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::OpenGLDataManager::getInstance().getDataPath(_filename);

		void* result = 0;

		Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(MyGUI::UString(fullname).asWStr_c_str());
		if (image)
		{
			_width = image->GetWidth();
			_height = image->GetHeight();
			Gdiplus::PixelFormat format = image->GetPixelFormat();

			if (format == PixelFormat24bppRGB)
				_format = MyGUI::PixelFormat::R8G8B8;
			else if (format == PixelFormat32bppARGB)
				_format = MyGUI::PixelFormat::R8G8B8A8;
			else
				_format = MyGUI::PixelFormat::Unknow;

			if (_format != MyGUI::PixelFormat::Unknow)
			{
				Gdiplus::Rect rect(0, 0, _width, _height);
				Gdiplus::BitmapData out_data;
				image->LockBits(&rect, Gdiplus::ImageLockModeRead, format, &out_data);

				size_t size = out_data.Height * out_data.Stride;
				result = new unsigned char[size];

				convertRawData(&out_data, result, size, _format);

				image->UnlockBits(&out_data);
			}

			delete image;
		}

		return result;
	}

	void BaseManager::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename)
	{
		Gdiplus::PixelFormat format;
		int bpp;

		if (_format == MyGUI::PixelFormat::R8G8B8A8)
		{
			bpp = 4;
			format = PixelFormat32bppARGB;
		}
		else if (_format == MyGUI::PixelFormat::R8G8B8)
		{
			bpp = 3;
			format = PixelFormat24bppRGB;
		}
		else if (_format == MyGUI::PixelFormat::L8A8)
		{
			bpp = 2;
			format = PixelFormat16bppGrayScale;
		}
		else
		{
			MYGUI_LOG(Error, "Unsuitable texture format for saving.");
			return;
		}

		UINT num, size;
		Gdiplus::GetImageEncodersSize(&num, &size);

		Gdiplus::ImageCodecInfo* imageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(size);
		GetImageEncoders(num, size, imageCodecInfo);

		CLSID* pngClsid = NULL;
		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(imageCodecInfo[j].MimeType, L"image/png") == 0)
			{
				pngClsid = &imageCodecInfo[j].Clsid;
				break;
			}
		}

		if (pngClsid == NULL)
		{
			MYGUI_LOG(Error, "png codec not found");
			return;
		}

		Gdiplus::Bitmap image(_width, _height, bpp * _width, format, (BYTE*)_texture);

		HRESULT res = image.Save(MyGUI::UString(_filename).asWStr_c_str(), pngClsid, NULL);
		if (res != S_OK)
			MYGUI_LOG(Error, "Texture saving error. result = " << res);
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
