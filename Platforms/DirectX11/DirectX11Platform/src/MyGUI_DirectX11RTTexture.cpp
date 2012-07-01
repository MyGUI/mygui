/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#include <d3dx11.h>
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11RTTexture.h"
#include "MyGUI_DirectX11Diagnostic.h"

namespace MyGUI
{

	DirectX11RTTexture::DirectX11RTTexture( DirectX11Texture* _texture, DirectX11RenderManager* _manager ) :
		mOldDepthStencil(nullptr),
		mOldRenderTarget(nullptr),
		mTexture(_texture),
		mManager(_manager),
		mRenderTarget(nullptr)
	{
		int width = mTexture->getWidth();
		int height = mTexture->getHeight();

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = 0.0f;
		mRenderTargetInfo.vOffset = 0.0f;
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0f / float(width);
		mRenderTargetInfo.pixScaleY = 1.0f / float(height);

		D3D11_RENDER_TARGET_VIEW_DESC desc;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = mManager->mpD3DDevice->CreateRenderTargetView(mTexture->mTexture, 0, &mRenderTarget);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Render Target View failed!");
	}

	DirectX11RTTexture::~DirectX11RTTexture()
	{
		if (mRenderTarget)
			mRenderTarget->Release();
	}

	void DirectX11RTTexture::begin()
	{
		UINT numViewports = 1;
		mManager->mpD3DContext->RSGetViewports(&numViewports, &mOdViewport);
		mManager->mpD3DContext->OMGetRenderTargets(1, &mOldRenderTarget, &mOldDepthStencil);

		D3D11_VIEWPORT vp;
		vp.Width = (float)mTexture->getWidth();
		vp.Height = (float)mTexture->getHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		mManager->mpD3DContext->RSSetViewports( 1, &vp );

		const float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		mManager->mpD3DContext->OMSetRenderTargets(1, &mRenderTarget, 0);
		mManager->mpD3DContext->ClearRenderTargetView(mRenderTarget, clearColor);
	}

	void DirectX11RTTexture::end()
	{
		mManager->mpD3DContext->OMSetRenderTargets(1, &mOldRenderTarget, mOldDepthStencil);
		if (mOldRenderTarget)
		{
			mOldRenderTarget->Release();
			mOldRenderTarget = nullptr;
		}
		if (mOldDepthStencil)
		{
			mOldDepthStencil->Release();
			mOldDepthStencil = nullptr;
		}
		mManager->mpD3DContext->RSSetViewports(1, &mOdViewport);
	}

	void DirectX11RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DirectX11RenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
