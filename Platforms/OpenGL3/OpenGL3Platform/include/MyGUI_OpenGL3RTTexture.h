/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef MYGUI_OPENGL3_RTTEXTURE_H_
#define MYGUI_OPENGL3_RTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class OpenGL3RTTexture :
		public IRenderTarget
	{
	public:
		OpenGL3RTTexture(unsigned int _texture);
		virtual ~OpenGL3RTTexture();

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

    int mSavedViewport[4];
    
    unsigned int mFBOID;
		unsigned int mRBOID;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_RTTEXTURE_H_
