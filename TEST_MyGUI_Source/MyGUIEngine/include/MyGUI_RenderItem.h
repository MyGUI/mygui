/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_RENDER_ITEM_H__
#define __MYGUI_RENDER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgrePrerequisites.h>
#include <OgreRenderOperation.h>
#include <OgreRenderSystem.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>

namespace MyGUI
{

	class DrawItem;
	typedef std::vector<DrawItem *> VectorDrawItem;

	class _MyGUIExport RenderItem
	{
	public:
		RenderItem(const std::string& _texture);
		~RenderItem();

		void _render();
		void _resize(const FloatSize& _size);

		inline const std::string& getTextureName() { return mTextureName; }

		inline void addDrawItem(DrawItem * _item, size_t _count)
		{
			mDrawItems.push_back(_item);
			mNeedVertexCount += _count;
			mOutDate = true;
		}

		inline void removeDrawItem(DrawItem * _item, size_t _count)
		{
			for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter) {
				if ((*iter) == _item) {
					mDrawItems.erase(iter);
					mNeedVertexCount -= _count;
					mOutDate = true;
					break;
				}
			}
		}

		inline void outOfDate() { mOutDate = true; }

		inline float getMaximumDepth() {return mMaximumDepth;}

		inline float getPixScaleX() {return mPixScaleX;}
		inline float getPixScaleY() {return mPixScaleY;}

		inline float getHOffset() {return mHOffset;}
		inline float getVOffset() {return mVOffset;}

	private:
		void initRenderState();

		void createVertexBuffer();
		void destroyVertexBuffer();

		void resizeVertexBuffer();

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

		// размер пикселя в относительных координатах
		float mPixScaleX;
		float mPixScaleY;

		// смещение для того, чтобы тексель попал в пиксель
        float mHOffset;
        float mVOffset;

		// координата зю
		float mMaximumDepth;

	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_ITEM_H__
