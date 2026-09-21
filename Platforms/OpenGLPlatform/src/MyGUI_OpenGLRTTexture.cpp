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
		GLint textureBinding = 0, drawFramebuffer = 0, readFramebuffer = 0, renderbuffer = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding);
		const bool separate = OpenGLRenderManager::getInstance().isSeparateFramebufferSupported();
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &drawFramebuffer);
		if (separate)
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &renderbuffer);
		int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureId);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &mWidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &mHeight);
		glBindTexture(GL_TEXTURE_2D, textureBinding);
		MYGUI_PLATFORM_ASSERT(mWidth > 0 && mHeight > 0, "Render target texture has no storage");

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
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRBOID);

		const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (separate)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
		}
		else
			glBindFramebuffer(GL_FRAMEBUFFER, drawFramebuffer);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			glDeleteRenderbuffers(1, &mRBOID);
			glDeleteFramebuffers(1, &mFBOID);
			MYGUI_PLATFORM_EXCEPT("Incomplete render target framebuffer: " << status);
		}
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
		TargetState state;
		glGetIntegerv(GL_VIEWPORT, state.viewport);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &state.drawFramebuffer);
		if (OpenGLRenderManager::getInstance().isSeparateFramebufferSupported())
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &state.readFramebuffer);
		mStates.push_back(state);

		// Save raster state before changing the viewport or clear colour.
		OpenGLRenderManager::getInstance().begin();
		glBindFramebuffer(GL_FRAMEBUFFER, mFBOID);
		glViewport(0, 0, mWidth, mHeight);
		glOrtho(-1, 1, 1, -1, -1, 1);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRTTexture::end()
	{
		MYGUI_PLATFORM_ASSERT(!mStates.empty(), "Unbalanced render target pass");
		OpenGLRenderManager::getInstance().end();
		const auto state = mStates.back();
		mStates.pop_back();
		if (OpenGLRenderManager::getInstance().isSeparateFramebufferSupported())
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, state.drawFramebuffer);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, state.readFramebuffer);
		}
		else
			glBindFramebuffer(GL_FRAMEBUFFER, state.drawFramebuffer);
		glViewport(state.viewport[0], state.viewport[1], state.viewport[2], state.viewport[3]);
	}

	void OpenGLRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
