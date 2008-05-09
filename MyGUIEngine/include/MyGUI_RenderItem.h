/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
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

namespace MyGUI
{

	enum {VERTEX_IN_QUAD = 6};

	class LayerItemKeeper;
	class DrawItem;
	typedef std::pair<DrawItem *, size_t> DrawItemInfo;
	typedef std::vector<DrawItemInfo> VectorDrawItem;


	class _MyGUIExport RenderItem
	{
	public:
		RenderItem(const std::string& _texture, LayerItemKeeper * _parent);
		~RenderItem();

		void _render(bool _update);
		//void _resize(const FloatSize& _size);

		inline const std::string& getTextureName() { return mTextureName; }

		inline void addDrawItem(DrawItem * _item, size_t _count)
		{

// проверяем только в дебаге
#if MYGUI_DEBUG_MODE == 1
			for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter) {
				MYGUI_ASSERT((*iter).first != _item, "DrawItem exist");
			}
#endif

			mDrawItems.push_back(DrawItemInfo(_item, _count));
			mNeedVertexCount += _count;
			mOutDate = true;
		}

		void removeDrawItem(DrawItem * _item);

		inline void reallockDrawItem(DrawItem * _item, size_t _count)
		{
			for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter) {
				if ((*iter).first == _item) {
					// если нужно меньше, то ниче не делаем
					if ((*iter).second < _count) {
						mNeedVertexCount -= (*iter).second;
						mNeedVertexCount += _count;
						(*iter).second = _count;
					}
					return;
				}
			}
			MYGUI_EXCEPT("DrawItem not found");
		}

		inline void setTextureName(const std::string& _texture)
		{
			MYGUI_DEBUG_ASSERT(mNeedVertexCount == 0, "change texture only empty buffer");
			mTextureName = _texture;
		}

		inline void outOfDate() { mOutDate = true; }

		//inline float getMaximumDepth() {return mMaximumDepth;}

		/*inline float getPixScaleX() {return mPixScaleX;}
		inline float getPixScaleY() {return mPixScaleY;}

		inline float getHOffset() {return mHOffset;}
		inline float getVOffset() {return mVOffset;}

		inline float getAspectCoef() {return mAspectCoef;}*/

		inline size_t getVertexCount() {return mVertexCount;}
		inline size_t getNeedVertexCount() {return mNeedVertexCount;}

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
		/*float mPixScaleX;
		float mPixScaleY;

		// смещение для того, чтобы тексель попал в пиксель
        float mHOffset;
        float mVOffset;

		float mAspectCoef;

		// координата зю
		float mMaximumDepth;*/

		LayerItemKeeper * mParent;
		LayerManager * mLayerManager;

	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_ITEM_H__
