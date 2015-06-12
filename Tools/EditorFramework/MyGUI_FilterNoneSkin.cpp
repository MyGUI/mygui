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

#ifdef MYGUI_OGRE_PLATFORM
#include <MyGUI_OgreRenderManager.h>
#include <MyGUI_OgreTexture.h>
#include <MyGUI_OgreVertexBuffer.h>
#elif MYGUI_OPENGL_PLATFORM
#include <MyGUI_OpenGlRenderManager.h>
#elif MYGUI_DIRECTX_PLATFORM
#include <MyGUI_DirectXRenderManager.h>
#elif MYGUI_DIRECTX11_PLATFORM
#include <MyGUI_DirectX11RenderManager.h>
#elif MYGUI_OPENGL3_PLATFORM
#include <MyGUI_OpenGl3RenderManager.h>
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

	FilterNone::~FilterNone()
	{
	}

	void FilterNone::_setAlign(const IntSize& _oldsize)
	{
		mCurrentCoord.set(0, 0, mCroppedParent->getWidth(), mCroppedParent->getHeight());
		mAlign = Align::Stretch;

		Base::_setAlign(_oldsize);
	}

	void FilterNone::doManualRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
#ifdef MYGUI_OGRE_PLATFORM
		if (Ogre2RenderManager::getInstancePtr()->getManualRender())
			Ogre2RenderManager::getInstancePtr()->begin();

		Ogre2RenderManager::getInstancePtr()->setManualRender(true);

		if (_texture)
		{
			Ogre2Texture* texture = static_cast<Ogre2Texture*>(_texture);
			Ogre::TexturePtr texture_ptr = texture->getOgreTexture();
			if (!texture_ptr.isNull())
			{
				Ogre2RenderManager::getInstancePtr()->getRenderSystem()->_setTexture(0, true, texture_ptr);
				Ogre2RenderManager::getInstancePtr()->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);
			}
		}

		Ogre2VertexBuffer* buffer = static_cast<Ogre2VertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		Ogre2RenderManager::getInstancePtr()->getRenderSystem()->_render(*operation);
#elif MYGUI_OPENGL_PLATFORM
		OpenGLRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif MYGUI_DIRECTX_PLATFORM
		DirectXRenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif MYGUI_DIRECTX11_PLATFORM
		DirectX11RenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#elif MYGUI_OPENGL3_PLATFORM
    OpenGL3RenderManager::getInstancePtr()->doRender(_buffer, _texture, _count);
#endif
	}

}
