/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef MYGUI_OPENGL_RTTEXTURE_H_
#define MYGUI_OPENGL_RTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class OpenGLRTTexture : public IRenderTarget
	{
	public:
		OpenGLRTTexture(unsigned int _texture);
		~OpenGLRTTexture() override;

		void begin() override;
		void end() override;

		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		const RenderTargetInfo& getInfo() const override
		{
			return mRenderTargetInfo;
		}

	private:
		RenderTargetInfo mRenderTargetInfo;
		unsigned int mTextureId;
		int mWidth{0};
		int mHeight{0};
		unsigned int mFBOID{0};
		unsigned int mRBOID{0};
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL_RTTEXTURE_H_
