/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef __MYGUI_DIRECTX11_RTTEXTURE_H__
#define __MYGUI_DIRECTX11_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct IDirect3DSurface9;

namespace MyGUI
{
	class DirectX11Texture;

	class DirectX11RTTexture :
		public IRenderTarget
	{
	public:
		DirectX11RTTexture( DirectX11Texture* texture, DirectX11RenderManager* manager );
		virtual ~DirectX11RTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		ID3D11DepthStencilView* oldDepthStencil;
		ID3D11RenderTargetView* oldRenderTarget;
		D3D11_VIEWPORT          oldViewport;
		DirectX11Texture*       mTexture;
		DirectX11RenderManager* mManager;
		ID3D11RenderTargetView* mRenderTarget;
		RenderTargetInfo        mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RTTEXTURE_H__
