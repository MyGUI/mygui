/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_VIEW_H_
#define GRAPH_VIEW_H_

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

#endif // GRAPH_VIEW_H_
