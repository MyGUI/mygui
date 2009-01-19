/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_GRAPH_NODE_H__
#define __BASE_GRAPH_NODE_H__

#include <MyGUI.h>
#include "BaseLayout\BaseLayout.h"

namespace wraps
{

	class BaseGraphNode : public BaseLayout
	{
	public:
		BaseGraphNode(const std::string & _layout, MyGUI::WidgetPtr _parent) :
	  		BaseLayout(_layout, _parent)
		{
		}
	};

} // namespace wraps

#endif // __BASE_GRAPH_NODE_H__
