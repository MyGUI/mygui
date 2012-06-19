/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#include <d3dx9.h>
#include "MyGUI_DummyRTTexture.h"
#include "MyGUI_DummyRenderManager.h"

namespace MyGUI
{

	DummyRTTexture::DummyRTTexture(IDirect3DDevice9* _device, IDirect3DTexture9* _texture) :
		mpD3DDevice(_device),
		mpTexture(_texture),
		mpRenderSurface(NULL),
		mpBackBuffer(NULL)
	{
		mpTexture->GetSurfaceLevel(0, &mpRenderSurface);

		D3DSURFACE_DESC info;
		mpTexture->GetLevelDesc(0, &info);
		int width = info.Width;
		int height = info.Height;

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	DummyRTTexture::~DummyRTTexture()
	{
		if (mpRenderSurface != nullptr)
		{
			mpRenderSurface->Release();
			mpRenderSurface = nullptr;
		}
	}

	void DummyRTTexture::begin()
	{
		mpD3DDevice->GetRenderTarget(0, &mpBackBuffer);

		mpD3DDevice->SetRenderTarget(0, mpRenderSurface);
		mpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET,
			D3DCOLOR_RGBA(0, 0, 0, 0), 1, 0);

		mpD3DDevice->BeginScene();
	}

	void DummyRTTexture::end()
	{
		mpD3DDevice->EndScene();

		mpD3DDevice->SetRenderTarget(0, mpBackBuffer);
		mpBackBuffer->Release();
	}

	void DummyRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DummyRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
