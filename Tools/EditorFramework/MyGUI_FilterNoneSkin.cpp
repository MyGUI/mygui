/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "MyGUI_Precompiled.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CommonStateInfo.h"

#if defined(MYGUI_OGRE_PLATFORM)
	#include <MyGUI_OgreRenderManager.h>
	#include <MyGUI_OgreTexture.h>
	#include <MyGUI_OgreVertexBuffer.h>
	#include <OgreTextureUnitState.h>
#elif defined(MYGUI_OGRENEXT_PLATFORM)
	#include <MyGUI_OgreNextRenderManager.h>
	#include <MyGUI_OgreNextTexture.h>
	#include <OgreMaterial.h>
	#include <OgreTechnique.h>
	#include <OgrePass.h>
	#include <OgreTextureUnitState.h>
	#include <OgreHlmsSamplerblock.h>
#elif defined(MYGUI_OPENGL_PLATFORM)
	#include <MyGUI_OpenGLRenderManager.h>
	#include <MyGUI_OpenGLTexture.h>
	#include <MyGUI_GL.h>
	#include <SDL.h>
#elif defined(MYGUI_OPENGL3_PLATFORM)
	#include <MyGUI_OpenGL3RenderManager.h>
	#include <MyGUI_OpenGL3Texture.h>
	#include <MyGUI_GL.h>
	#include <SDL.h>
#elif defined(MYGUI_DIRECTX_PLATFORM)
	#include <MyGUI_DirectXRenderManager.h>
	#include <d3d9.h>
#elif defined(MYGUI_DIRECTX11_PLATFORM)
	#include <MyGUI_DirectX11RenderManager.h>
	#include <d3d11.h>
#elif defined(MYGUI_VULKAN_PLATFORM)
	#include <MyGUI_VulkanRenderManager.h>
#endif

namespace MyGUI
{

	FilterNone::FilterNone()
	{
		mSeparate = true;

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32 && (defined(MYGUI_OPENGL_PLATFORM) || defined(MYGUI_OPENGL3_PLATFORM))
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(1);
		}
#endif
	}

	void FilterNone::_setAlign(const IntSize& _oldsize)
	{
		mCurrentCoord.set(0, 0, mCroppedParent->getWidth(), mCroppedParent->getHeight());
		mAlign = Align::Stretch;

		Base::_setAlign(_oldsize);
	}

	void FilterNone::doManualRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
#if defined(MYGUI_OGRE_PLATFORM)

		if (OgreRenderManager::getInstancePtr()->getManualRender())
			OgreRenderManager::getInstancePtr()->begin();

		OgreRenderManager::getInstancePtr()->setManualRender(true);

		if (_texture)
		{
			OgreTexture* texture = static_cast<OgreTexture*>(_texture);
			Ogre::TexturePtr texture_ptr = texture->getOgreTexture();
			if (texture_ptr)
			{
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setTexture(0, true, texture_ptr);

	#if OGRE_VERSION < MYGUI_DEFINE_VERSION(14, 0, 0)
				MYGUI_SUPPRESS_MSVC(4996)
				MYGUI_SUPPRESS_GCC("-Wdeprecated-declarations")
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setTextureUnitFiltering(
					0,
					Ogre::FO_NONE,
					Ogre::FO_NONE,
					Ogre::FO_NONE);
				MYGUI_UNSUPPRESS_GCC()
				MYGUI_UNSUPPRESS_MSVC()
	#else
				Ogre::Sampler sampler;
				sampler.setFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setSampler(0, sampler);
	#endif
			}
		}

		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		OgreRenderManager::getInstancePtr()->getRenderSystem()->_render(*operation);
#elif defined(MYGUI_OGRENEXT_PLATFORM)
		if (_texture)
		{
			OgreNextTexture* texture = static_cast<OgreNextTexture*>(_texture);
			Ogre::MaterialPtr material = texture->getMaterial();
			if (material)
			{
				Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
				Ogre::TextureUnitState* tu = pass->getTextureUnitState(0);
				if (tu)
				{
					const Ogre::HlmsSamplerblock* prevBlock = tu->getSamplerblock();
					Ogre::HlmsSamplerblock pointBlock;
					if (prevBlock)
						pointBlock = *prevBlock;
					pointBlock.mMinFilter = Ogre::FO_NONE;
					pointBlock.mMagFilter = Ogre::FO_NONE;
					pointBlock.mMipFilter = Ogre::FO_NONE;
					tu->setSamplerblock(pointBlock);

					if (prevBlock)
						OgreNextRenderManager::getInstancePtr()->deferSamplerRestore(tu, prevBlock);

					OgreNextRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
					return;
				}
			}
		}
		OgreNextRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_OPENGL_PLATFORM)
		if (_texture)
		{
			OpenGLTexture* texture = static_cast<OpenGLTexture*>(_texture);
			glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		OpenGLRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_OPENGL3_PLATFORM)
		if (_texture)
		{
			OpenGL3Texture* texture = static_cast<OpenGL3Texture*>(_texture);
			glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		OpenGL3RenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_DIRECTX_PLATFORM)
		DirectXRenderManager::getInstancePtr()->mpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		DirectXRenderManager::getInstancePtr()->mpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		DirectXRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_DIRECTX11_PLATFORM)
		DirectX11RenderManager* rm = DirectX11RenderManager::getInstancePtr();
		ID3D11SamplerState* prev = rm->mSamplerState;
		rm->mSamplerState = rm->mPointSamplerState;
		rm->doRender(_buffer, _texture, _count);
		rm->mSamplerState = prev;
#elif defined(MYGUI_VULKAN_PLATFORM)
		VulkanRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#endif
	}

}
