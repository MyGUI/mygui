/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
#endif

namespace MyGUI
{

	FilterNone::FilterNone() :
		SubSkin()
	{
		mSeparate = true;
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
		if (OgreRenderManager::getInstancePtr()->getManualRender())
			OgreRenderManager::getInstancePtr()->begin();

		OgreRenderManager::getInstancePtr()->setManualRender(true);

		if (_texture)
		{
			OgreTexture* texture = static_cast<OgreTexture*>(_texture);
			Ogre::TexturePtr texture_ptr = texture->getOgreTexture();
			if (!texture_ptr.isNull())
			{
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setTexture(0, true, texture_ptr);
				OgreRenderManager::getInstancePtr()->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_POINT);
			}
		}

		OgreVertexBuffer* buffer = static_cast<OgreVertexBuffer*>(_buffer);
		Ogre::RenderOperation* operation = buffer->getRenderOperation();
		operation->vertexData->vertexCount = _count;

		OgreRenderManager::getInstancePtr()->getRenderSystem()->_render(*operation);
#endif
	}

} // namespace MyGUI
