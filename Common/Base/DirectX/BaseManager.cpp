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
		case WM_CREATE:
		{
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
			break;
		}

		case WM_MOVE:
		case WM_SIZE:
		{
			base::BaseManager *baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
			if (baseManager) baseManager->windowResized();
				break;
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
		device(nullptr),
		m_exit(false)
	{
		assert(!m_instance);
		m_instance = this;
	}

	BaseManager::~BaseManager()
	{
		m_instance = nullptr;
	}

	void BaseManager::windowResized()
	{
		if (mInputManager)
		{
			int mLeft, mTop, mWidth, mHeight;
			RECT rc;
			GetWindowRect(hwnd, &rc);
			mTop    = rc.top;
			mLeft   = rc.left;
			mWidth  = mLeft + rc.right;
			mHeight = mTop + rc.bottom;

			if (mGUI)
			{
				mGUI->resizeWindow(MyGUI::IntSize(mWidth, mHeight));
			}

			if (mMouse)
			{
				const OIS::MouseState &ms = mMouse->getMouseState();
				ms.width = mWidth;
				ms.height = mHeight;
			}
		}
	}

	void BaseManager::createInput() // создаем систему ввода
	{
		OIS::ParamList pl;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		void *pData = &windowHnd;
		HWND *pHWnd = (HWND*)pData;
		*pHWnd = hwnd;

		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		mInputManager = OIS::InputManager::createInputSystem( pl );

		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
		mKeyboard->setEventCallback(this);

		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
		mMouse->setEventCallback(this);

		windowResized();
	}

	void BaseManager::destroyInput() // удаляем систему ввода
	{
		if( mInputManager )
		{
			if (mMouse)
			{
				mInputManager->destroyInputObject( mMouse );
				mMouse = nullptr;
			}
			if (mKeyboard)
			{
				mInputManager->destroyInputObject( mKeyboard );
				mKeyboard = nullptr;
			}
			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = nullptr;
		}
	}

	bool BaseManager::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (mGUI) mGUI->injectMouseMove(arg);
		return true;
	}

	bool BaseManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (mGUI) mGUI->injectMousePress(arg, id);
		return true;
	}

	bool BaseManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (mGUI) mGUI->injectMouseRelease(arg, id);
		return true;
	}

	bool BaseManager::keyPressed( const OIS::KeyEvent &arg )
	{
		if ( arg.key == OIS::KC_ESCAPE )
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return false;
		}
		else if ( arg.key == OIS::KC_SYSRQ )
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
					return true;
				}
				file = MyGUI::utility::toString("screenshot_", ++num, ".png");
				stream.open(file.c_str());
			} while (stream.is_open());
			return true;
		}
		else if ( arg.key == OIS::KC_F12)
		{
			if (mGUI) MyGUI::InputManager::getInstance().setShowFocus(!MyGUI::InputManager::getInstance().getShowFocus());
		}

		if (mGUI) mGUI->injectKeyPress(arg);
		return true;
	}

	bool BaseManager::keyReleased( const OIS::KeyEvent &arg )
	{
		if (mGUI) mGUI->injectKeyRelease( arg );
		return true;
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
		if (!hwnd)
		{
			//OutException("fatal error!", "failed create window");
			return false;
		}

		hInstance = wc.hInstance;

		// инициализация direct3d
		d3d = Direct3DCreate9(D3D_SDK_VERSION);

		D3DDISPLAYMODE d3ddm;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		const unsigned int width = 1024;
		const unsigned int height = 768;

		d3dpp;
		memset(&d3dpp, 0, sizeof(d3dpp));
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.BackBufferCount  = 1;
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.BackBufferWidth  = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.Windowed = TRUE;

		if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
		{
			//OutException("fatal error!", "failed create d3d9 device");
			return false;
		}

		window_adjust_settings(hwnd, width, height, !d3dpp.Windowed);

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
			if (m_exit)
				break;
			else if (msg.message == WM_QUIT)
				break;

			if (GetActiveWindow() == hwnd)
			{
				if (mMouse) mMouse->capture();
				mKeyboard->capture();
				mGUI->injectFrameEntered(1.0f);

				// проверка состояния устройства
				HRESULT hr = device->TestCooperativeLevel();
				if (SUCCEEDED(hr))
				{
					if (SUCCEEDED(device->BeginScene()))
					{
						device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
						mPlatform->getRenderManagerPtr()->render();
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

		destroyInput();

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

	void BaseManager::setupResources()
	{
		addResourceLocation("../../Media", false);
		addResourceLocation("../../Media/MyGUI_Media", false);
	}

	void BaseManager::createGui()
	{
		mPlatform = new MyGUI::DirectXPlatform();
		mPlatform->initialise(device);

		setupResources();

		mGUI = new MyGUI::Gui();
		mGUI->initialise();

		mInfo = new statistic::StatisticInfo();
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
		SetWindowText(hwnd, _text.c_str());
	}

	void BaseManager::setWallpaper(const std::string & _filename)
	{
		static MyGUI::StaticImagePtr image = nullptr;
		if (image == nullptr) image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(MyGUI::IntPoint(), mGUI->getViewSize()), MyGUI::Align::Stretch, "Back");
		image->setImageTexture(_filename);
		image->setNeedMouseFocus(false);
	}

	void BaseManager::setDescriptionText(const MyGUI::UString & _text)
	{
		MyGUI::EditPtr text = nullptr;
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
		text->setCaption(_text);
	}

	void BaseManager::prepare(int argc, char **argv)
	{
	}

	void BaseManager::addResourceLocation(const std::string & _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

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
			style = WS_POPUP | WS_VISIBLE;
			style_ex = GetWindowLong(hWnd, GWL_EXSTYLE) | (WS_EX_TOPMOST);
			hwndAfter = HWND_TOPMOST;
		}
		else
		{
			style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
			style_ex = GetWindowLong(hWnd, GWL_EXSTYLE) &(~WS_EX_TOPMOST);
			hwndAfter = HWND_NOTOPMOST;
			AdjustWindowRect(&rc, style, false);
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
