#include "MyGUI_OpenGLESRTTexture.h"
#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESDiagnostic.h"

#include <GLES3/gl3.h>
#include "MyGUI_OpenGLESError.h"

namespace MyGUI
{

	OpenGLESRTTexture::OpenGLESRTTexture(unsigned int _texture, int _width, int _height) :
		mTextureId(_texture),
		mWidth(_width),
		mHeight(_height)
	{
		MYGUI_PLATFORM_ASSERT(mWidth > 0 && mHeight > 0, "Render target dimensions must be positive");
		GLint drawFramebuffer = 0, readFramebuffer = 0, renderbuffer = 0;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &renderbuffer);

		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffers(1, (GLuint*)&mFBOID);
		CHECK_GL_ERROR_DEBUG();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);
		CHECK_GL_ERROR_DEBUG();

		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		glGenRenderbuffers(1, (GLuint*)&mRBOID);
		CHECK_GL_ERROR_DEBUG();
		glBindRenderbuffer(GL_RENDERBUFFER, mRBOID);
		CHECK_GL_ERROR_DEBUG();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mWidth, mHeight);
		CHECK_GL_ERROR_DEBUG();
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		CHECK_GL_ERROR_DEBUG();

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);
		CHECK_GL_ERROR_DEBUG();

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOID);
		CHECK_GL_ERROR_DEBUG();

		const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			glDeleteRenderbuffers(1, &mRBOID);
			glDeleteFramebuffers(1, &mFBOID);
			MYGUI_PLATFORM_EXCEPT("Incomplete render target framebuffer: " << status);
		}
	}

	OpenGLESRTTexture::~OpenGLESRTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffers(1, (GLuint*)&mFBOID);
			CHECK_GL_ERROR_DEBUG();
			mFBOID = 0;
		}
		if (mRBOID != 0)
		{
			glDeleteRenderbuffers(1, (GLuint*)&mRBOID);
			CHECK_GL_ERROR_DEBUG();
			mRBOID = 0;
		}
	}

	void OpenGLESRTTexture::begin()
	{
		TargetState state;
		glGetIntegerv(GL_VIEWPORT, state.viewport);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &state.drawFramebuffer);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &state.readFramebuffer);
		glGetFloatv(GL_COLOR_CLEAR_VALUE, state.clearColour);
		mStates.push_back(state);

		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);
		CHECK_GL_ERROR_DEBUG();

		glViewport(0, 0, mWidth, mHeight);
		CHECK_GL_ERROR_DEBUG();

		OpenGLESRenderManager::getInstance().begin();
		CHECK_GL_ERROR_DEBUG();
		glClearColor(0, 0, 0, 0);
		CHECK_GL_ERROR_DEBUG();
		glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);
		CHECK_GL_ERROR_DEBUG();
	}

	void OpenGLESRTTexture::end()
	{
		MYGUI_PLATFORM_ASSERT(!mStates.empty(), "Unbalanced render target pass");
		OpenGLESRenderManager::getInstance().end();
		const auto state = mStates.back();
		mStates.pop_back();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, state.drawFramebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, state.readFramebuffer);
		glViewport(state.viewport[0], state.viewport[1], state.viewport[2], state.viewport[3]);
		glClearColor(state.clearColour[0], state.clearColour[1], state.clearColour[2], state.clearColour[3]);
		CHECK_GL_ERROR_DEBUG();
	}

	void OpenGLESRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLESRenderManager::getInstance().doRenderRtt(_buffer, _texture, _count);
	}

} // namespace MyGUI
