/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __GRAPH_CONNECTION_EXTENTION_H__
#define __GRAPH_CONNECTION_EXTENTION_H__

#include <MyGUI.h>
#include "BaseGraphConnection.h"

namespace demo
{

	class GraphConnectionExtension : public wraps::BaseGraphConnection
	{
	public:
		GraphConnectionExtension(MyGUI::WidgetPtr _widget) : BaseGraphConnection(_widget, "Extension")
		{
		}
	};

} // namespace demo

#endif // __GRAPH_CONNECTION_EXTENTION_H__
