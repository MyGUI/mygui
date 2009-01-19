/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_GRAPH_VIEW_H__
#define __BASE_GRAPH_VIEW_H__

#include <MyGUI.h>
#include "MyGUI_Canvas.h"
#include "BaseGraphNode.h"

namespace wraps
{

	class BaseGraphView : public BaseLayout
	{
	public:
		typedef std::vector<BaseGraphNode*> VectorGraphNode;

	public:
		BaseGraphView(const std::string & _layout, MyGUI::WidgetPtr _parent) :
	  		BaseLayout(_layout, _parent)
		{
			mCanvas = mMainWidget->castType<MyGUI::Canvas>();
		}

		template<typename NodeType>
		void createItem()
		{
			BaseGraphNode* node = new NodeType(this);
			mNodes.push_back(node);
		}

		void destroyItem(BaseGraphNode* _node)
		{
			VectorGraphNode::iterator item = std::find(mNodes.begin(), mNodes.end(), _node);
			MYGUI_ASSERT(item != mNodes.end(), "Item not found");
			delete *item;
			mNodes.erase(item);
		}

	private:
		MyGUI::CanvasPtr mCanvas;
		VectorGraphNode mNodes;
	};

} // namespace wraps

#endif // __BASE_GRAPH_VIEW_H__
