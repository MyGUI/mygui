/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __GRAPH_CONNECTION_SIMPLE_H__
#define __GRAPH_CONNECTION_SIMPLE_H__

#include <MyGUI.h>
#include "BaseGraphConnection.h"

namespace demo
{

	class GraphConnectionSimple : public wraps::BaseGraphConnection
	{
	public:
		GraphConnectionSimple(MyGUI::WidgetPtr _widget) : BaseGraphConnection(_widget, "Simple")
		{
		}
	};

} // namespace demo

#endif // __GRAPH_CONNECTION_SIMPLE_H__
