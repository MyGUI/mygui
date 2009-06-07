/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "precompiled.h"
#include "BaseManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#endif

// имя класса окна
	const char * WND_CLASS_NAME = "MyGUI_DirectX_Demo_window";

  LRESULT CALLBACK DXWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch(uMsg)
    {
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

	BaseManager * BaseManager::m_instance = nullptr;
	BaseManager & BaseManager::getInstance()
	{
		assert(m_instance);
		return *m_instance;
	}

	BaseManager::BaseManager() :
		mViewport(nullptr),
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr),
		hwnd(0),
		d3d(nullptr),
		device(nullptr)
	{
		assert(!m_instance);
		m_instance = this;
	}

	BaseManager::~BaseManager()
	{
		m_instance = nullptr;
	}

	void BaseManager::createInput()
	{
	}

	void BaseManager::destroyInput()
	{
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
	hwnd = CreateWindow(wc.lpszClassName, TEXT("MyGUI Demo [Direct3D9]"), WS_POPUP,
      0, 0, 0, 0, GetDesktopWindow(), NULL, wc.hInstance, NULL);
    if (!hwnd) {
      //OutException("fatal error!", "failed create window");
      return false;
    }

	hInstance = wc.hInstance;

    // инициализация direct3d
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DDISPLAYMODE d3ddm;
    d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

    d3dpp;
    memset(&d3dpp, 0, sizeof(d3dpp));
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.EnableAutoDepthStencil = FALSE;
    d3dpp.BackBufferCount  = 1;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferWidth  = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dpp.Windowed = TRUE;

    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
      D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
    {
      //OutException("fatal error!", "failed create d3d9 device");
      return false;
    }
    window_adjust_settings(hwnd, 800, 600, !d3dpp.Windowed);

		createInput();
		createGui();
		createScene();

		return true;
	}

	void BaseManager::run()
	{
    MSG msg;
    for (;;)
    {
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      if (msg.message == WM_QUIT)
        break;
      if (GetActiveWindow() == hwnd)
      {
        if (GetAsyncKeyState(VK_ESCAPE))
          break;
        // проверка состояния устройства
        HRESULT hr = device->TestCooperativeLevel();
        if (SUCCEEDED(hr))
        {
          if (SUCCEEDED(device->BeginScene()))
          {
            device->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF305080, 1.0f, 0);
            //gui->draw();
            device->EndScene();
          }
          device->Present(NULL, NULL, 0, NULL);
        }
        else
        {
          if (hr == D3DERR_DEVICENOTRESET)
          {
            //gui->deviceLost();
            if (SUCCEEDED(device->Reset(&d3dpp)))
            {
              //gui->deviceReset();
              Sleep(10);
            }
          }
        }
      }
    }
	}

	void BaseManager::destroy() // очищаем все параметры каркаса приложения
	{

		destroyScene();
		destroyGui();


		if (mViewport)
		{
			delete mViewport;
			mViewport = nullptr;
		}


    if (device) { device->Release(); device = 0; }
    if (d3d) { d3d->Release(); d3d = 0; }

    if (hwnd)
    {
      DestroyWindow(hwnd);
      hwnd = 0;
    }
    //MyGUI_d.lib DirectXRenderSystem_d.lib
    UnregisterClass(WND_CLASS_NAME, hInstance);

	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::DirectXPlatform();
		mPlatform->initialise(device);

		addResourceLocation("../../Media", "General", "FileSystem", false);
		addResourceLocation("../../Media/MyGUI_Media", "General", "FileSystem", false);

		mGUI = new MyGUI::Gui();
		mGUI->initialise();

		//mInfo = new statistic::StatisticInfo();
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
	/*#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		size_t windowHnd = 0;
		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		::SetWindowTextA((HWND)windowHnd, _text.c_str());
	#endif*/
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
		/*static MyGUI::StaticImagePtr image = nullptr;
		if (image == nullptr) image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		image->setImageTexture(_filename);
		image->setNeedMouseFocus(false);*/
	}

	void BaseManager::setDescriptionText(const MyGUI::UString & _text)
	{
		/*MyGUI::EditPtr text = nullptr;
		if (text == nullptr)
		{
			const MyGUI::IntSize& view_size = mGUI->getViewSize();
			MyGUI::WidgetPtr panel = mGUI->createWidget<MyGUI::Widget>("PanelSmall", view_size.width, -128, 400, 128, MyGUI::Align::Default, "Statistic");
			text = panel->createWidget<MyGUI::Edit>("WordWrapSimple", 10, 10, 380, 108, MyGUI::Align::Default);
			//text->setTextColour(MyGUI::Colour(0, 1, 0, 1));
			MyGUI::StaticImagePtr image = panel->createWidget<MyGUI::StaticImage>(MyGUI::WidgetStyle::Popup, "StaticImage", MyGUI::IntCoord(view_size.width-48, 0, 48, 48), MyGUI::Align::Default, "Back");
			image->setItemResource("pic_CoreMessageIcon");
			image->setItemGroup("Icons");
			image->setItemName("Quest");

			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerEdgeHide::getClassTypeName());
			MyGUI::ControllerEdgeHide* controller = item->castType<MyGUI::ControllerEdgeHide>();
			controller->setTime(0.5);
			MyGUI::ControllerManager::getInstance().addItem(panel, controller);
		}
		text->setCaption(_text);*/
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, const std::string & _group, const std::string & _type, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _group, _recursive);
	}

  // эта функция устанавливает размеры окна и применяет нужный стиль, вынесено в отдельную функцию
  // для того, чтобы окно не висело с черным фоном долго, то есть, сначала инициализируется все, и только потом показывается окно
  // P.S. просто мне так нравится больше, можно переделать
  void BaseManager::window_adjust_settings(HWND hWnd, int width, int height, bool fullScreen)
  {
    // сохраняем параметры десктопа, для позиционирования окна
    static int desk_width  = GetSystemMetrics(SM_CXSCREEN);
    static int desk_height = GetSystemMetrics(SM_CYSCREEN);

    // стиль окна
    HWND hwndAfter;
    unsigned long style, style_ex;

    RECT rc;
    SetRect(&rc, 0, 0, width, height);

    if (fullScreen)
    {
      style     = WS_POPUP | WS_VISIBLE;
      style_ex  = GetWindowLong(hWnd, GWL_EXSTYLE) | (WS_EX_TOPMOST);
      hwndAfter = HWND_TOPMOST;
    }
    else
    {
      style     = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
      style_ex  = GetWindowLong(hWnd, GWL_EXSTYLE) &(~WS_EX_TOPMOST);
      hwndAfter = HWND_NOTOPMOST;
    }

    SetWindowLong(hWnd, GWL_STYLE,   style);
    SetWindowLong(hWnd, GWL_EXSTYLE, style_ex);

    unsigned int x, y, w, h;
    w = rc.right - rc.left;
    h = rc.bottom - rc.top;
    x = fullScreen ? 0 : (desk_width  - w) / 2;
    y = fullScreen ? 0 : (desk_height - h) / 2;

    SetWindowPos(hWnd, hwndAfter, x, y, w, h, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
  }

} // namespace base
