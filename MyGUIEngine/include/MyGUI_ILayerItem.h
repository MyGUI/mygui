/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_LAYER_ITEM_H_
#define MYGUI_I_LAYER_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

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

#endif // MYGUI_I_LAYER_ITEM_H_
