/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef MYGUI_DIRECTX11_RTTEXTURE_H_
#define MYGUI_DIRECTX11_RTTEXTURE_H_

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
		ID3D11DepthStencilView* mOldDepthStencil;
		ID3D11RenderTargetView* mOldRenderTarget;
		D3D11_VIEWPORT          mOdViewport;
		DirectX11Texture*       mTexture;
		DirectX11RenderManager* mManager;
		ID3D11RenderTargetView* mRenderTarget;
		RenderTargetInfo        mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // MYGUI_DIRECTX_RTTEXTURE_H_
