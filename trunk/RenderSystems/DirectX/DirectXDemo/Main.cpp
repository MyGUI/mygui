/*!
@file
@author		Albert Semenov
@date		04/2009
@module
*/

#include "MyGUI.h"
#include "MyGUI_DirectXRenderManager.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
  int main(int argc, char **argv);
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT argc) { return main(1, &strCmdLine); }
  void OutException(const char * _caption, const char * _message) { ::MessageBox( NULL, _message, _caption, MB_OK | MB_ICONERROR | MB_TASKMODAL); }
#else
  void OutException(const char * _caption, const char * _message) { std::cerr << _caption << " : " << _message; }
#endif

  /* --------------------------------------------------------- */
  /*    я просто не знаю, как это все будет выглядеть потом,
  /*    потому делаю пока что вот так вот тупо :)
  /*  тут создается окно и инициализируется d3d,
  /*  потом создается гуи
  /* --------------------------------------------------------- */

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

  // эта функция устанавливает размеры окна и применяет нужный стиль, вынесено в отдельную функцию
  // для того, чтобы окно не висело с черным фоном долго, то есть, сначала инициализируется все, и только потом показывается окно
  // P.S. просто мне так нравится больше, можно переделать
  void window_adjust_settings(HWND hWnd, int width, int height, bool fullScreen)
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

  // имя класса окна
#define WND_CLASS_NAME "MyGUI_DirectX_Demo_window"

  int main(int argc, char **argv)
  {
    IDirect3D9 *d3d;
    IDirect3DDevice9 *device;

    // регистрируем класс окна
    WNDCLASS wc = {
      0, (WNDPROC)DXWndProc, 0, 0, GetModuleHandle(NULL), LoadIcon(NULL, IDI_APPLICATION),
      LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, TEXT(WND_CLASS_NAME),
    };
    RegisterClass(&wc);

    // создаем главное окно
    HWND hwnd = CreateWindow(wc.lpszClassName, TEXT("MyGUI Demo [Direct3D9]"), WS_POPUP,
      0, 0, 0, 0, GetDesktopWindow(), NULL, wc.hInstance, NULL);
    if (!hwnd) {
      OutException("fatal error!", "failed create window");
      return 0;
    }

    // инициализация direct3d
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DDISPLAYMODE d3ddm;
    d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

    D3DPRESENT_PARAMETERS d3dpp;
    memset(&d3dpp, 0, sizeof(d3dpp));
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.EnableAutoDepthStencil = FALSE;
    d3dpp.BackBufferCount  = 1;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferWidth  = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dpp.Windowed = FALSE;

    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
      D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &device)))
    {
      OutException("fatal error!", "failed create d3d9 device");
      return 0;
    }

    MyGUI::DirectXRenderManager* render = new MyGUI::DirectXRenderManager();
    render->initialise();

    MyGUI::Gui* gui = new MyGUI::Gui();
    gui->initialise();

    window_adjust_settings(hwnd, 800, 600, !d3dpp.Windowed);

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

    /*
    gui->shutdown();
    delete gui;
    gui = nullptr;
    */

    render->shutdown();
    delete render;
    render = nullptr;

    if (device) { device->Release(); device = 0; }
    if (d3d) { d3d->Release(); d3d = 0; }

    if (hwnd)
    {
      DestroyWindow(hwnd);
      hwnd = nullptr;
    }
    //MyGUI_d.lib DirectXRenderSystem_d.lib
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
  }

#ifdef __cplusplus
}
#endif
