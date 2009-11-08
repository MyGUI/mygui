/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#ifndef __MYGUI_RENDER_ITEM_H__
#define __MYGUI_RENDER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgrePrerequisites.h>
#include <OgreRenderOperation.h>
#include <OgreRenderSystem.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	enum { VERTEX_IN_QUAD = 6 };

	class LayerItemKeeper;
	class DrawItem;
	typedef std::pair<DrawItem *, size_t> DrawItemInfo;
	typedef std::vector<DrawItemInfo> VectorDrawItem;


	class MYGUI_EXPORT IRenderItem
	{
	public:
		IRenderItem(LayerItemKeeper * _parent) { }
		virtual ~IRenderItem() { }

		virtual void _render(bool _update) = 0;
		virtual bool empty() = 0;
	};


	class MYGUI_EXPORT RenderItem : public IRenderItem
	{
	public:
		RenderItem(const std::string& _texture, LayerItemKeeper * _parent);
		virtual ~RenderItem();

		virtual void _render(bool _update);

		const std::string& getTextureName() { return mTextureName; }

		void addDrawItem(DrawItem * _item, size_t _count);

		void removeDrawItem(DrawItem * _item);

		void reallockDrawItem(DrawItem * _item, size_t _count);

		void setTextureName(const std::string& _texture);

		void outOfDate() { mOutDate = true; }

		size_t getVertexCount() { return mVertexCount; }
		size_t getNeedVertexCount() { return mNeedVertexCount; }

		virtual bool empty() { return getNeedVertexCount() == 0; }

	private:
		void initRenderState();

		void createVertexBuffer();
		void destroyVertexBuffer();

		void resizeVertexBuffer();
		void initialise();

	private:
		std::string mTextureName;

		size_t mNeedVertexCount;
		size_t mVertexCount;

		bool mOutDate;
		VectorDrawItem mDrawItems;

		Ogre::RenderSystem* mRenderSystem;
		Ogre::TextureManager* mTextureManager;

		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;

		Ogre::TextureUnitState::UVWAddressingMode mTextureAddressMode;
		Ogre::LayerBlendModeEx mColorBlendMode, mAlphaBlendMode;

		LayerItemKeeper * mParent;
		LayerManager * mLayerManager;

		size_t mCountVertex;
		bool mIsInitialise;

	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_ITEM_H__
