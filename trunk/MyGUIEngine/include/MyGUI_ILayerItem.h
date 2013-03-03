/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_LAYER_ITEM_H__
#define __MYGUI_I_LAYER_ITEM_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class ILayer;
	class ILayerNode;

	class MYGUI_EXPORT ILayerItem
	{
	public:
		virtual ~ILayerItem() { }

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;
		virtual const IntCoord& getLayerItemCoord() const = 0;

		virtual void resizeLayerItemView(const IntSize& _oldView, const IntSize& _newView) = 0;

		virtual void attachItemToNode(ILayer* _layer, ILayerNode* _node) = 0;
		virtual void detachFromLayer() = 0;
		virtual void upLayerItem() = 0;
	};

} // namespace MyGUI

#endif // __MYGUI_I_LAYER_ITEM_H__
