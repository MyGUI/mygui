/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __QUAD_DRAW_ITEM_H__
#define __QUAD_DRAW_ITEM_H__

#include "MyGUI_IDrawItem.h"
#include "MyGUI_IRenderItem.h"
#include "MyGUI_ILayerNode.h"
#include "RenderItem.h"

namespace MyGUI
{

	class QuadDrawItem : public IDrawItem
	{
	public:
		QuadDrawItem();
		virtual ~QuadDrawItem() { }

		virtual void createDrawItem(const std::string& _texture, ILayerNode* _keeper);
		virtual void destroyDrawItem();

		virtual void doRender(bool _update);

	private:
		RenderItem* mRenderItem;
	};

} // namespace MyGUI

#endif // __QUAD_DRAW_ITEM_H__
