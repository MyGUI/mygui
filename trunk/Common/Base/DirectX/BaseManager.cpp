/*!
	@file
	@author		Albert Semenov
	@date		05/2009
	@module
*/

#include "precompiled.h"
#include "BaseManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#	include <windows.h>
#endif

// имя класса окна
const char * WND_CLASS_NAME = "MyGUI_Demo_window";

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
			if (baseManager)
				baseManager->_windowResized();
			break;
		}

		case WM_CLOSE:
		{
			base::BaseManager *baseManager = (base::BaseManager*)GetWindowLongPtr(hWnd, GWL_USERDATA);
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

	BaseManager::BaseManager() :
		mGUI(nullptr),
		mPlatform(nullptr),
		mInfo(nullptr),
		mFocusInfo(nullptr),
		hWnd(0),
		mD3d(nullptr),
		mDevice(nullptr),
		mExit(false),
		mResourceFileName("core.xml")
	{
	}

	BaseManager::~BaseManager()
	{
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
		hWnd = CreateWindow(wc.lpszClassName, TEXT("MyGUI Demo [Direct3D9]"), WS_POPUP,
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

		createRender(width, height, windowed);

		windowAdjustSettings(hWnd, width, height, !mD3dpp.Windowed);

		createInput((size_t)hWnd);
		_windowResized();

		createGui();

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

		destroyGui();

		destroyInput();

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
		mPlatform = new MyGUI::DirectXPlatform();
		mPlatform->initialise(mDevice);

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
		if (mDevice)
		{
			mD3dpp.BackBufferWidth = _width;
			mD3dpp.BackBufferHeight = _height;
			mDevice->Reset(&mD3dpp);
		}
	}

	// тестовый вариант загрузки моделей
	/*ID3DXBuffer *g_pD3DXMaterials;
	DWORD       g_dwNumMaterials;
	ID3DXMesh   *g_pD3DXMesh;
	D3DXMATERIAL      *g_pMaterials      = NULL;
	D3DMATERIAL9      *g_pMaterialList = NULL;
	IDirect3DTexture9 **g_pTextureList;*/

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		// инициализация direct3d
		mD3d = Direct3DCreate9(D3D_SDK_VERSION);

		D3DDISPLAYMODE d3ddm;
		mD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		memset(&mD3dpp, 0, sizeof(mD3dpp));
		mD3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		mD3dpp.EnableAutoDepthStencil = TRUE;
		mD3dpp.BackBufferCount  = 1;
		mD3dpp.BackBufferFormat = d3ddm.Format;
		mD3dpp.BackBufferWidth  = _width;
		mD3dpp.BackBufferHeight = _height;
		mD3dpp.hDeviceWindow = hWnd;
		mD3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		mD3dpp.Windowed = _windowed;

		if (FAILED(mD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &mD3dpp, &mDevice)))
		{
			//OutException("fatal error!", "failed create d3d9 mDevice");
			return false;
		}

		// тестовый вариант загрузки моделей
		/*if (FAILED(D3DXLoadMeshFromX(
			"tiger.x",
			D3DXMESH_SYSTEMMEM,
			mDevice,
			NULL,
			&g_pD3DXMaterials,
			NULL,
			&g_dwNumMaterials,
			&g_pD3DXMesh)))
		{
			// Произошла ошибка
			//return false;
		}

		// Получаем указатель на список материалов
		g_pMaterials = (D3DXMATERIAL*)g_pD3DXMaterials->GetBufferPointer();

		if (g_pMaterials != NULL)
		{
			// Создаем массив структур данных материалов
			// для копирования в него данных
			g_pMaterialList = new D3DMATERIAL9[g_dwNumMaterials];

			// Создаем массив указателей на объекты текстуры
			g_pTextureList = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];

			// Копируем материалы
			for (DWORD i = 0; i < g_dwNumMaterials; i++)
			{
				g_pMaterialList[i] = g_pMaterials[i].MatD3D;

				// Делаем фоновую составляющую цвета такой же,
				// как и рассеиваемая
				g_pMaterialList[i].Ambient = g_pMaterialList[i].Diffuse;

				// Создаем и загружаем текстуры (если они есть)
				if (FAILED(D3DXCreateTextureFromFileA(
					mDevice,
					g_pMaterials[i].pTextureFilename,
					&g_pTextureList[i])))
				{
					g_pTextureList[i] = NULL;
				}
			}

			// Освобождаем буфер материалов, использовавшийся для загрузки
			g_pD3DXMaterials->Release();
		}
		else
		{
			// Если материалы не были загружены, создаем
			// материал по умолчанию
			g_dwNumMaterials = 1;

			// Создаем белый материал
			g_pMaterialList = new D3DMATERIAL9[1];
			g_pMaterialList[0].Diffuse.r = 1.0f;
			g_pMaterialList[0].Diffuse.g = 1.0f;
			g_pMaterialList[0].Diffuse.b = 1.0f;
			g_pMaterialList[0].Diffuse.a = 1.0f;
			g_pMaterialList[0].Ambient = g_pMaterialList[0].Diffuse;

			// Создаем пустую ссылку на текстуру
			g_pTextureList = new LPDIRECT3DTEXTURE9[1];
			g_pTextureList[0] = NULL;
		}*/

		return true;
	}

	void BaseManager::drawOneFrame()
	{
		// проверка состояния устройства
		HRESULT hr = mDevice->TestCooperativeLevel();
		if (SUCCEEDED(hr))
		{
			if (SUCCEEDED(mDevice->BeginScene()))
			{
				mDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);

				// тестовый вариант загрузки моделей
				/*D3DXMATRIXA16 matWorld;
				//FLOAT fAngle = 0 * (2.0f * D3DX_PI) / 2000.0f;

				// Мировая матрица
				D3DXMatrixIdentity(&matWorld);
				//D3DXMatrixRotationY(&matWorld, fAngle); // D3DXMatrixRotationY D3DXMatrixRotationZ
				mDevice->SetTransform(D3DTS_WORLD, &matWorld);

				D3DXMATRIXA16 matView;
				D3DXVECTOR3 vEyePt(3, 3, 3); // Глаз наблюдателя
				D3DXVECTOR3 vLookatPt(0, 0, 0); // Точка цели камеры
				D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // Верх камеры

				D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
				mDevice->SetTransform(D3DTS_VIEW, &matView);

				// Проекционная матрица
				D3DXMATRIXA16 matProj;
				D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f);
				mDevice->SetTransform(D3DTS_PROJECTION, &matProj);


				// Перебираем в цикле каждый материал в сетке
				for(DWORD i = 0; i < g_dwNumMaterials; i++)
				{
					// Устанавливаем материал и текстуру
					mDevice->SetMaterial(&g_pMaterialList[i]);
					mDevice->SetTexture(0, g_pTextureList[i]);

					// Рисуем подгруппу сетки
					g_pD3DXMesh->DrawSubset(i);
				}*/

				mPlatform->getRenderManagerPtr()->drawOneFrame();
				mDevice->EndScene();
			}
			mDevice->Present(NULL, NULL, 0, NULL);
		}
		else
		{
			if (hr == D3DERR_DEVICENOTRESET)
			{
				if (SUCCEEDED(mDevice->Reset(&mD3dpp)))
				{
					mPlatform->getRenderManagerPtr()->deviceReset();
					Sleep(10);
				}
			}
		}
	}

	void BaseManager::destroyRender()
	{
		if (mDevice)
		{
			mDevice->Release();
			mDevice = 0;
		}
		if (mD3d)
		{
			mD3d->Release();
			mD3d = 0;
		}
	}

} // namespace base
