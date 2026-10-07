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

	class OpenGL3RTTexture : public IRenderTarget
	{
	public:
		OpenGL3RTTexture(unsigned int _texture, int _width, int _height);
		~OpenGL3RTTexture() override;

		void begin() override;
		void end() override;

		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;

		const RenderTargetInfo& getInfo() const override
		{
			return mRenderTargetInfo;
		}

	private:
		RenderTargetInfo mRenderTargetInfo;
		int mWidth{0};
		int mHeight{0};

		int mSavedViewport[4]{};
		int mSavedDrawFramebuffer{}, mSavedReadFramebuffer{};
		float mSavedClearColour[4]{};

		unsigned int mFBOID{0};
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_RTTEXTURE_H_
