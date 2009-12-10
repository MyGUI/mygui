/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/

#ifndef __MYGUI_DIRECTX_RTTEXTURE_H__
#define __MYGUI_DIRECTX_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

//struct IDirect3DDevice9;
struct IDirect3DTexture9;

namespace MyGUI
{

	class DirectXRTTexture :
		public IRenderTarget
	{
	public:
		DirectXRTTexture(IDirect3DTexture9* _texture);
		virtual ~DirectXRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo() { return mRenderTargetInfo; }

	private:
		//IDirect3DDevice9 *mpD3DDevice;
	    IDirect3DTexture9 *mpTexture;
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RTTEXTURE_H__
