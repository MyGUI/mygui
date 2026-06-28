#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	class OpenGLESRTTexture : public IRenderTarget
	{
	public:
		OpenGLESRTTexture(unsigned int _texture);
		~OpenGLESRTTexture() override;

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
