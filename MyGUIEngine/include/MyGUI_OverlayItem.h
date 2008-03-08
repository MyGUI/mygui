/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_OVERLAY_ITEM_H__
#define __MYGUI_OVERLAY_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DrawItem.h"

#include <OgreRenderSystem.h>

namespace MyGUI
{
	typedef std::pair<DrawItem *, size_t> DrawItemInfo;
	typedef std::vector<DrawItemInfo> VectorDrawItem;

	class _MyGUIExport OverlayItem
	{

	public:
		OverlayItem(const Ogre::String& _material);
		void updatePositionGeometry();
		void updateView();

		void addDrawItem(DrawItem * _item, size_t _count);
		void removeDrawItem(DrawItem * _item, size_t _count);

		// обновить буфер
		inline void outOfDate() { mOutOfDate = true; }

    private:
		void memoryAllocation();

	private:
		Ogre::RenderOperation mRenderOp;
		bool mOutOfDate;
		VectorDrawItem mVectorDrawItem;
		size_t mNeedVertex;
		size_t mCountVertex;

		float mPixelScaleX, mPixelScaleY;

	};

} // namespace MyGUI

#endif // __MYGUI_OVERLAY_ITEM_H__
