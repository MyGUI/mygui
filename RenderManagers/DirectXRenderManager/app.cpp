#include <windows.h>
#include <d3d9.h>

#include "D3D9Renderer.h"

unsigned int screen_width  = 800;
unsigned int screen_height = 600;
bool full_screen = true;

static HWND hWnd;
static bool bActive;

IDirect3D9 *pd3dObject;
IDirect3DDevice9 *pd3dDevice;
D3DPRESENT_PARAMETERS md3dpp;

D3D9Renderer guiRenderer;

bool initD3DDevice()
{
  if (0 == (pd3dObject = Direct3DCreate9(D3D_SDK_VERSION)))
    return false;
  D3DDISPLAYMODE d3ddm;
  if (FAILED(pd3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    return false;
  memset(&md3dpp, 0, sizeof(md3dpp));
  md3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  md3dpp.EnableAutoDepthStencil = FALSE;
  md3dpp.BackBufferCount        = 1;
  md3dpp.BackBufferFormat       = d3ddm.Format;
  md3dpp.BackBufferWidth        = screen_width;
  md3dpp.BackBufferHeight       = screen_height;
  md3dpp.hDeviceWindow          = hWnd;
  md3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  md3dpp.Windowed               = (full_screen != true);
  HRESULT hr = pd3dObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
    hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &md3dpp, &pd3dDevice);
  if (FAILED(hr) || (NULL == pd3dDevice))
    return false;
  if (!guiRenderer.init(pd3dDevice))
    return false;

  return true;
}

bool drawGUIengine()
{
  HRESULT hr = pd3dDevice->TestCooperativeLevel();
  if (SUCCEEDED(hr))
  {
    pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF404080, 1.0f, 0);
    if (SUCCEEDED(pd3dDevice->BeginScene()))
    {

      // draw scene

      // draw gui
      //guiEngine.render();

      pd3dDevice->EndScene();
    }
    pd3dDevice->Present(NULL, NULL, NULL, NULL);
  }
  else
  {
    if (hr == D3DERR_DEVICENOTRESET)
    {
      guiRenderer.lost(); // release render targets
      if (SUCCEEDED(pd3dDevice->Reset(&md3dpp)))
      {
        if (!guiRenderer.restored())
          return false;
        Sleep(50);
      }
    }
  }

  return true;
}

void shutdownD3DDevice()
{
  guiRenderer.shutdown();
  if (pd3dDevice) {
    pd3dDevice->Release();
    pd3dDevice = 0;
  }
  if (pd3dObject) {
    pd3dObject->Release();
    pd3dObject = 0;
  }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
  case WM_KEYDOWN:
    {
      if (wParam == VK_ESCAPE)
        SendMessage(hwnd, WM_CLOSE, 0, 0);
      break;
    }
  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      BeginPaint(hwnd, &ps);
      bActive = !IsRectEmpty(&ps.rcPaint);
      EndPaint(hwnd, &ps);
      break;
    }
  case WM_ACTIVATEAPP:
    {
      bActive = (wParam != WA_INACTIVE);
      break;
    }
  case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }
  default:
    {
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
  }
  return NULL;
}

int APIENTRY WinMain(HINSTANCE hThisInst, HINSTANCE hLastInst,
                     LPSTR lpCmdLine, int nCmdShow)
{

  WNDCLASS window_class = {
    0, (WNDPROC)WndProc, 0, 0, hThisInst,
    LoadIcon(NULL, IDI_APPLICATION),
    LoadCursor(NULL, IDC_ARROW),
    (HBRUSH)GetStockObject(BLACK_BRUSH),
    NULL, TEXT("MyGUI_test_window"),
  };

  RegisterClass(&window_class);
  hWnd = CreateWindow(window_class.lpszClassName, TEXT("MyGUI D3D9 Render"),
    WS_POPUP, 0, 0, 0, 0, GetDesktopWindow(), NULL, hThisInst, NULL);
  SetForegroundWindow(hWnd);
  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  // direct3d9 initialise
  if (!initD3DDevice())
  {
    MessageBox(NULL, TEXT("Direct3D9 initialise failed!"),
      TEXT("Fatal error!"), MB_TASKMODAL | MB_ICONERROR);
    return 0;
  }

  // adjust window style and size
  unsigned long window_style;
  unsigned long window_style_extension;

  HWND hwndAfter;
  int x, y, w, h;

  window_style_extension = GetWindowLong(hWnd, GWL_EXSTYLE);
  if (full_screen)
  {
    hwndAfter    = HWND_TOPMOST;
    window_style = WS_POPUP | WS_VISIBLE;
    window_style_extension  |= (WS_EX_TOPMOST);
  }
  else
  {
    hwndAfter    = HWND_NOTOPMOST;
    window_style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    window_style_extension  &=(~WS_EX_TOPMOST);
  }

  SetWindowLong(hWnd, GWL_STYLE, window_style);
  SetWindowLong(hWnd, GWL_EXSTYLE, window_style_extension);
  RECT window_rect = { 0, 0, screen_width, screen_height };

  AdjustWindowRect(&window_rect, FALSE, window_style);

  w = window_rect.right - window_rect.left;
  h = window_rect.bottom - window_rect.top;
  x = full_screen ? 0 : (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
  y = full_screen ? 0 : (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

  SetWindowPos(hWnd, hwndAfter, x, y, w, h, SWP_FRAMECHANGED);

  MSG msg;
  while (1)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (msg.message == WM_QUIT)
      break;

    if (bActive)
    {
      bool bQuit = !drawGUIengine();
      if ( bQuit ) break;
    }
    else
    {
      WaitMessage();
    }
  }

  shutdownD3DDevice();
  ChangeDisplaySettings(NULL, 0);

  if (hWnd) {
    DestroyWindow(hWnd);
    hWnd = 0;
  }

  UnregisterClass(window_class.lpszClassName, hThisInst);
  return 0;
}