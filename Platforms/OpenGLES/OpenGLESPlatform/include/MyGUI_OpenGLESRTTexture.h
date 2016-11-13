#ifndef MYGUI_OPENGLES_RTTEXTURE_H__
#define MYGUI_OPENGLES_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class OpenGLESRTTexture :
		public IRenderTarget
	{
	public:
		OpenGLESRTTexture(unsigned int _texture);
		virtual ~OpenGLESRTTexture();

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

#endif // MYGUI_OPENGLES_RTTEXTURE_H__
