#include "D3D9Renderer.h"

bool D3D9Renderer::init(IDirect3DDevice9 *_pd3dDevice)
{
  mpD3DDevice = _pd3dDevice;

  return true;
}

void D3D9Renderer::lost()
{
}

bool D3D9Renderer::restored()
{
  return true;
}

void D3D9Renderer::shutdown()
{
  mpD3DDevice = 0;
}