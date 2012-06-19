/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#ifndef __MYGUI_DUMMY_RTTEXTURE_H__
#define __MYGUI_DUMMY_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

//struct IDirect3DDevice9;
//struct IDirect3DTexture9;
//struct IDirect3DSurface9;

namespace MyGUI
{
	class DummyRTTexture :
		public IRenderTarget
	{
	public:
		DummyRTTexture(/*IDirect3DDevice9* _device, IDirect3DTexture9* _texture*/);
		virtual ~DummyRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		//IDirect3DDevice9* mpD3DDevice;
		//IDirect3DTexture9* mpTexture;
		//IDirect3DSurface9* mpRenderSurface;
		//IDirect3DSurface9* mpBackBuffer;
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // __MYGUI_DUMMY_RTTEXTURE_H__
