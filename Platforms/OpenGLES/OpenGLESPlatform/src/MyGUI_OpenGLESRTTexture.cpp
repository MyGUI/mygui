#include "MyGUI_OpenGLESRTTexture.h"
#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESDiagnostic.h"

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

#include "platform.h"

namespace MyGUI
{

	OpenGLESRTTexture::OpenGLESRTTexture(unsigned int _texture) :
		mTextureID(_texture),
		mWidth(0),
		mHeight(0),
		mFBOID(0),
		mRBOID(0)
	{
		//int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		CHECK_GL_ERROR_DEBUG();
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, (GLint *)&mWidth);
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, (GLint *)&mHeight);
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR_DEBUG();

		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffers(1, (GLuint*) &mFBOID);
		CHECK_GL_ERROR_DEBUG();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);
		CHECK_GL_ERROR_DEBUG();

		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		glGenRenderbuffers(1, (GLuint*) &mRBOID);
		CHECK_GL_ERROR_DEBUG();
		glBindRenderbuffer(GL_RENDERBUFFER, mRBOID);
		CHECK_GL_ERROR_DEBUG();
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
		CHECK_GL_ERROR_DEBUG();
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		CHECK_GL_ERROR_DEBUG();

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);
		CHECK_GL_ERROR_DEBUG();

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOID);
		CHECK_GL_ERROR_DEBUG();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CHECK_GL_ERROR_DEBUG();
	}

	OpenGLESRTTexture::~OpenGLESRTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffers(1, (GLuint*) &mFBOID);
			CHECK_GL_ERROR_DEBUG();
			mFBOID = 0;
		}
		if (mRBOID != 0)
		{
			glDeleteRenderbuffers(1, (GLuint*) &mRBOID);
			CHECK_GL_ERROR_DEBUG();
			mRBOID = 0;
		}
	}

	void OpenGLESRTTexture::begin()
	{
		//glPushAttrib(GL_VIEWPORT_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);
		CHECK_GL_ERROR_DEBUG();

		glViewport(0, 0, mWidth, mHeight);
		CHECK_GL_ERROR_DEBUG();

		OpenGLESRenderManager::getInstance().begin();
		CHECK_GL_ERROR_DEBUG();
		glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
		CHECK_GL_ERROR_DEBUG();
	}

	void OpenGLESRTTexture::end()
	{
		OpenGLESRenderManager::getInstance().end();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
		CHECK_GL_ERROR_DEBUG();
	}

	void OpenGLESRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLESRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
