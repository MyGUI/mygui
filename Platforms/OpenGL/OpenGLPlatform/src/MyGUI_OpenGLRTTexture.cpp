/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_OpenGLRTTexture.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDiagnostic.h"

#include <MyGUI_GL.h>

namespace MyGUI
{

	OpenGLRTTexture::OpenGLRTTexture(unsigned int _texture) :
		mTextureId(_texture)
	{
		int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &mWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &mHeight);
		glBindTexture(GL_TEXTURE_2D, 0);

		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffers(1, &mFBOID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);

		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		glGenRenderbuffers(1, &mRBOID);
		glBindRenderbuffer(GL_RENDERBUFFER, mRBOID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLRTTexture::~OpenGLRTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffers(1, &mFBOID);
			mFBOID = 0;
		}
		if (mRBOID != 0)
		{
			glDeleteRenderbuffers(1, &mRBOID);
			mRBOID = 0;
		}
	}

	void OpenGLRTTexture::begin()
	{
		glPushAttrib(GL_VIEWPORT_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);

		glViewport(0, 0, mWidth, mHeight);

		OpenGLRenderManager::getInstance().begin();
		glOrtho(-1, 1, 1, -1, -1, 1);
		glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);
	}

	void OpenGLRTTexture::end()
	{
		OpenGLRenderManager::getInstance().end();

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

		glPopAttrib();
	}

	void OpenGLRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
