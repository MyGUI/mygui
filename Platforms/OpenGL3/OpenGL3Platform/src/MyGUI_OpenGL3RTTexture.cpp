/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_OpenGL3RTTexture.h"
#include "MyGUI_OpenGL3RenderManager.h"
#include "MyGUI_OpenGL3Diagnostic.h"

#include "GL/glew.h"

namespace MyGUI
{

	OpenGL3RTTexture::OpenGL3RTTexture(unsigned int _texture) :
		mTextureID(_texture),
		mWidth(0),
		mHeight(0),
		mFBOID(0),
		mRBOID(0)
	{
		int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureID);
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
		glGenFramebuffersEXT(1, &mFBOID);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOID);

		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		glGenRenderbuffersEXT(1, &mRBOID);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, mRBOID);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mWidth, mHeight);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTextureID, 0);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mRBOID);

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	OpenGL3RTTexture::~OpenGL3RTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffersEXT(1, &mFBOID);
			mFBOID = 0;
		}
		if (mRBOID != 0)
		{
			glDeleteRenderbuffersEXT(1, &mRBOID);
			mRBOID = 0;
		}
	}

	void OpenGL3RTTexture::begin()
	{
    glGetIntegerv(GL_VIEWPORT, mSavedViewport); // save current viewport
    
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBOID);

		glViewport(0, 0, mWidth, mHeight);

		OpenGL3RenderManager::getInstance().begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

	void OpenGL3RTTexture::end()
	{
		OpenGL3RenderManager::getInstance().end();

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind

    glViewport(mSavedViewport[0], mSavedViewport[1], mSavedViewport[2], mSavedViewport[3]); // restore old viewport
  }

	void OpenGL3RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGL3RenderManager::getInstance().doRenderRTT(_buffer, _texture, _count);
	}

} // namespace MyGUI
