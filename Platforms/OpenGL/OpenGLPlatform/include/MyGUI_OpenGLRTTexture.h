/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef __MYGUI_OPENGL_RTTEXTURE_H__
#define __MYGUI_OPENGL_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class OpenGLRTTexture :
		public IRenderTarget
	{
	public:
		OpenGLRTTexture(unsigned int _texture);
		virtual ~OpenGLRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo()
		{
			return mRenderTargetInfo;
		}

	private:
		RenderTargetInfo mRenderTargetInfo;
		unsigned int mTextureID;
		int mWidth;
		int mHeight;
		unsigned int mFBOID;
		unsigned int mRBOID;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_RTTEXTURE_H__
