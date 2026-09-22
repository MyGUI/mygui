/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include <d3d9.h>
#include "MyGUI_DirectXRTTexture.h"
#include "MyGUI_DirectXRenderManager.h"
#include "MyGUI_DirectXDiagnostic.h"

namespace MyGUI
{

	DirectXRTTexture::DirectXRTTexture(IDirect3DDevice9* _device, IDirect3DTexture9* _texture) :
		mpD3DDevice(_device),
		mpTexture(_texture),
		mpRenderSurface(nullptr),
		mpBackBuffer(nullptr)
	{
		D3DSURFACE_DESC info{};
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(mpTexture->GetLevelDesc(0, &info)), "Failed to query render target texture");
		MYGUI_PLATFORM_ASSERT((info.Usage & D3DUSAGE_RENDERTARGET) != 0, "Texture is not a render target");
		MYGUI_PLATFORM_ASSERT(
			SUCCEEDED(mpTexture->GetSurfaceLevel(0, &mpRenderSurface)),
			"Failed to get render target surface");
		int width = info.Width;
		int height = info.Height;

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);
	}

	DirectXRTTexture::~DirectXRTTexture()
	{
		if (mpBackBuffer)
		{
			if (mOwnScene)
				mpD3DDevice->EndScene();
			mpD3DDevice->SetRenderTarget(0, mpBackBuffer);
			mpD3DDevice->SetViewport(&mSavedViewport);
			mpBackBuffer->Release();
		}
		if (mpRenderSurface != nullptr)
		{
			mpRenderSurface->Release();
			mpRenderSurface = nullptr;
		}
	}

	void DirectXRTTexture::begin()
	{
		MYGUI_PLATFORM_ASSERT(!mpBackBuffer, "Render target is already active");
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(mpD3DDevice->GetViewport(&mSavedViewport)), "Failed to save viewport");
		MYGUI_PLATFORM_ASSERT(
			SUCCEEDED(mpD3DDevice->GetRenderTarget(0, &mpBackBuffer)),
			"Failed to save render target");
		try
		{
			MYGUI_PLATFORM_ASSERT(
				SUCCEEDED(mpD3DDevice->SetRenderTarget(0, mpRenderSurface)),
				"Failed to bind render target");
			MYGUI_PLATFORM_ASSERT(
				SUCCEEDED(mpD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1, 0)),
				"Failed to clear render target");
			// BeginScene returns INVALIDCALL if the host (or an outer RTT) already
			// owns a scene. Only end a scene we started, preserving standalone RTT use.
			const HRESULT sceneResult = mpD3DDevice->BeginScene();
			MYGUI_PLATFORM_ASSERT(
				SUCCEEDED(sceneResult) || sceneResult == D3DERR_INVALIDCALL,
				"Failed to begin render target scene");
			mOwnScene = SUCCEEDED(sceneResult);
		}
		catch (...)
		{
			mpD3DDevice->SetRenderTarget(0, mpBackBuffer);
			mpD3DDevice->SetViewport(&mSavedViewport);
			mpBackBuffer->Release();
			mpBackBuffer = nullptr;
			throw;
		}
	}

	void DirectXRTTexture::end()
	{
		MYGUI_PLATFORM_ASSERT(mpBackBuffer, "Render target is not active");
		const HRESULT sceneResult = mOwnScene ? mpD3DDevice->EndScene() : D3D_OK;
		mOwnScene = false;
		const HRESULT targetResult = mpD3DDevice->SetRenderTarget(0, mpBackBuffer);
		const HRESULT viewportResult = mpD3DDevice->SetViewport(&mSavedViewport);
		mpBackBuffer->Release();
		mpBackBuffer = nullptr;
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(sceneResult), "Failed to end render target scene");
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(targetResult), "Failed to restore render target");
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(viewportResult), "Failed to restore viewport");
	}

	void DirectXRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DirectXRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
