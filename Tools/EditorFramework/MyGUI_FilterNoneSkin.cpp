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
#elif defined(MYGUI_OPENGL_PLATFORM)
#include <MyGUI_OpenGLRenderManager.h>
#elif defined(MYGUI_DIRECTX_PLATFORM)
#include <MyGUI_DirectXRenderManager.h>
#elif defined(MYGUI_DIRECTX11_PLATFORM)
#include <MyGUI_DirectX11RenderManager.h>
#elif defined(MYGUI_OPENGL3_PLATFORM)
#include <MyGUI_OpenGL3RenderManager.h>
#endif

namespace MyGUI
{

	FilterNone::FilterNone() :
		SubSkin()
	{
#ifdef MYGUI_OGRE_PLATFORM
		mSeparate = true;
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
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
			}
		}

		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		OgreRenderManager::getInstancePtr()->getRenderSystem()->_render(*operation);
#elif defined(MYGUI_OPENGL_PLATFORM)
		OpenGLRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_DIRECTX_PLATFORM)
		DirectXRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_DIRECTX11_PLATFORM)
		DirectX11RenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif defined(MYGUI_OPENGL3_PLATFORM)
	OpenGL3RenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#endif
	}

}
