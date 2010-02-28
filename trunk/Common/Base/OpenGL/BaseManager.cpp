/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
*/

#include "precompiled.h"
#include "BaseManager.h"

#include <gl\gl.h>
#include <gl\glu.h>

//for image loader
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

// имя класса окна
const char * WND_CLASS_NAME = "MyGUI_Demo_window";

LRESULT CALLBACK DXWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
		{
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
			break;
		}

		case WM_MOVE:
		case WM_SIZE:
		{
			base::BaseManager *baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (baseManager)
				baseManager->_windowResized();
			break;
		}

		case WM_CLOSE:
		{
			base::BaseManager *baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
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
		mInfo(nullptr),
		mFocusInfo(nullptr),
		hWnd(0),
		hDC(0),
		hRC(0),
		mExit(false),
		mResourceFileName("core.xml")
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

	bool BaseManager::create()
	{
		// регистрируем класс окна
		WNDCLASS wc = {
			0, (WNDPROC)DXWndProc, 0, 0, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
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

		hInstance = wc.hInstance;
		const unsigned int width = 1024;
		const unsigned int height = 768;
		bool windowed = true;

		if (!createRender(width, height, windowed))
		{
			return false;
		}

		windowAdjustSettings(hWnd, width, height, !windowed);

		createInput((size_t)hWnd);
		createGui();

		_windowResized();

		createPointerManager((size_t)hWnd);

		createScene();

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

			if (GetActiveWindow() == hWnd)
			{
				captureInput();
				updateFPS();
				drawOneFrame();
			}
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
				bool root = false;
				if (node->findAttribute("root") != "")
				{
					root = MyGUI::utility::parseBool(node->findAttribute("root"));
					if (root) mRootMedia = node->getContent();
				}
				addResourceLocation(node->getContent(), false);
			}
		}
	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::OpenGLPlatform();
		mPlatform->initialise(this);

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise(mResourceFileName);

		mInfo = new diagnostic::StatisticInfo();
	}

	void BaseManager::destroyGui()
	{
		if (mGUI)
		{
			if (mInfo)
			{
				delete mInfo;
				mInfo = nullptr;
			}

			if (mFocusInfo)
			{
				delete mFocusInfo;
				mFocusInfo = nullptr;
			}

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

	void BaseManager::setWindowCaption(const std::string & _text)
	{
		SetWindowText(hWnd, _text.c_str());
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, bool _recursive)
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
			style_ex = GetWindowLong(hWnd, GWL_EXSTYLE) | (WS_EX_TOPMOST);
			hwndAfter = HWND_TOPMOST;
		}
		else
		{
			style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
			style_ex = GetWindowLong(hWnd, GWL_EXSTYLE) &(~WS_EX_TOPMOST);
			hwndAfter = HWND_NOTOPMOST;
			AdjustWindowRect(&rc, style, false);
		}

		SetWindowLong(hWnd, GWL_STYLE, style);
		SetWindowLong(hWnd, GWL_EXSTYLE, style_ex);

		int desk_width  = GetSystemMetrics(SM_CXSCREEN);
		int desk_height = GetSystemMetrics(SM_CYSCREEN);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		int x = fullScreen ? 0 : (desk_width  - w) / 2;
		int y = fullScreen ? 0 : (desk_height - h) / 2;

		SetWindowPos(hWnd, hwndAfter, x, y, w, h, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}

	void BaseManager::updateFPS()
	{
		if (mInfo)
		{
			// calc FPS
			static MyGUI::Timer timer;
			const unsigned long interval = 1000; 
			static int count_frames = 0;
			int accumulate = timer.getMilliseconds();
			if (accumulate > interval)
			{
				mInfo->change("FPS", (int)((unsigned long)count_frames * 1000 / accumulate));
				mInfo->update();

				count_frames = 0;
				timer.reset();
			}
			count_frames ++;
		}
	}

	void BaseManager::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseMove(_absx, _absy, _absz);
	}

	void BaseManager::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMousePress(_absx, _absy, _id);
	}

	void BaseManager::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!mGUI)
			return;

		mGUI->injectMouseRelease(_absx, _absy, _id);
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
		else if (_key == MyGUI::KeyCode::F12)
		{
			if (mFocusInfo == nullptr)
				mFocusInfo = new diagnostic::InputFocusInfo();

			bool visible = mFocusInfo->getFocusVisible();
			mFocusInfo->setFocusVisible(!visible);
		}

		mGUI->injectKeyPress(_key, _text);
	}

	void BaseManager::injectKeyRelease(MyGUI::KeyCode _key)
	{
		if (!mGUI)
			return;

		mGUI->injectKeyRelease(_key);
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		if (_height == 0)	
			_height = 1;

		glViewport(0, 0, _width, _height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45.0f, (GLfloat)_width/(GLfloat)_height, 0.1f, 100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();		
	}

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		int bits = 16;

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

		if (!(hDC = GetDC(hWnd)))
		{
			return false;
		}
	
		if (!(pixel_format = ChoosePixelFormat(hDC, &pfd)))
		{
			return false;
		}

		if (!SetPixelFormat(hDC, pixel_format, &pfd))
		{
			return false;
		}

		if (!(hRC = wglCreateContext(hDC)))
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
			for (unsigned int y=0; y<_out_data->Height; ++y)
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

} // namespace base
