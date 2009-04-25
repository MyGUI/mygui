#ifndef __D3D9Renderer_h__
#define __D3D9Renderer_h__

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

class D3D9Renderer
{
  IDirect3DDevice9 *mpD3DDevice;
public:

  bool init(IDirect3DDevice9 *_pd3dDevice);
  void lost();
  bool restored();
  void shutdown();

};

#endif