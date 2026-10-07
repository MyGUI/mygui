/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_OpenGL3RTTexture.h"
#include "MyGUI_OpenGL3RenderManager.h"
#include "MyGUI_OpenGL3Diagnostic.h"

#include <MyGUI_GL.h>

namespace MyGUI
{

	OpenGL3RTTexture::OpenGL3RTTexture(unsigned int _texture) :
		mTextureId(_texture)
	{
		GLint textureBinding = 0, drawFramebuffer = 0, readFramebuffer = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
		int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &mWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &mHeight);
		glBindTexture(GL_TEXTURE_2D, textureBinding);

		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffers(1, &mFBOID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
	}

	OpenGL3RTTexture::~OpenGL3RTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffers(1, &mFBOID);
			mFBOID = 0;
		}
	}

	void OpenGL3RTTexture::begin()
	{
		glGetIntegerv(GL_VIEWPORT, mSavedViewport); // save current viewport
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &mSavedDrawFramebuffer);
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &mSavedReadFramebuffer);
		glGetFloatv(GL_COLOR_CLEAR_VALUE, mSavedClearColour);

		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);

		glViewport(0, 0, mWidth, mHeight);

		OpenGL3RenderManager::getInstance().begin();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGL3RTTexture::end()
	{
		OpenGL3RenderManager::getInstance().end();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mSavedDrawFramebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mSavedReadFramebuffer);
		glClearColor(mSavedClearColour[0], mSavedClearColour[1], mSavedClearColour[2], mSavedClearColour[3]);

		glViewport(mSavedViewport[0], mSavedViewport[1], mSavedViewport[2], mSavedViewport[3]); // restore old viewport
	}

	void OpenGL3RTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGL3RenderManager::getInstance().doRenderRtt(_buffer, _texture, _count);
	}

} // namespace MyGUI
