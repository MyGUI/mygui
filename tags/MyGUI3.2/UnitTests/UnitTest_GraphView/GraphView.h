/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef __GRAPH_VIEW_H__
#define __GRAPH_VIEW_H__

#include <MyGUI.h>
#include "BaseGraphView.h"

namespace demo
{

	class GraphView :
		public wraps::BaseGraphView
	{
	public:
		GraphView() :
			wraps::BaseGraphView("GraphView.layout", nullptr)
		{
			setCanvasWidget("Canvas");
		}
	};

} // namespace demo

#endif // __GRAPH_VIEW_H__
