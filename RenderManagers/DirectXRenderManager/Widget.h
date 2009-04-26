/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "MyGUI_ILayerItem.h"
#include <vector>
#include "QuadDrawItem.h"

namespace MyGUI
{

	class Widget : public ILayerItem
	{
	public:
		Widget(const std::string& _texture);
		virtual ~Widget() { }

		virtual ILayerItem * getLayerItemByPoint(int _left, int _top);

		virtual void attachItemToNode(ILayer* _layer, ILayerNode* _node);
		virtual void detachFromLayer();
		virtual void upLayerItem();

		Widget* createChild(const std::string& _texture);

	private:
		ILayer* mLayer;
		ILayerNode* mNode;

		typedef std::vector<Widget*> VectorWidget;
		VectorWidget mChilds;

		typedef std::vector<QuadDrawItem*> VectorQuadDrawItem;
		VectorQuadDrawItem mItems;

		std::string mTexture;

	};

} // namespace MyGUI

#endif // __WIDGET_H__
