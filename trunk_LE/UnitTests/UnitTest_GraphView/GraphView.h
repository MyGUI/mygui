/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
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
			MyGUI::CanvasPtr canvas = nullptr;
			assignWidget(canvas, "Canvas");
			wrapCanvas(canvas);

			assignWidget(mScrollView, "ScrollView");

			mScrollView->setCanvasSize(16, 16);
			canvas->setCoord(0, 0, 16, 16);

			eventChangeSize = MyGUI::newDelegate(this, &GraphView::notifyChangeSize);

			notifyChangeSize(this, MyGUI::IntSize());
		}

	private:
		void notifyChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size)
		{
			const MyGUI::IntCoord& coord = mScrollView->getClientCoord();
			if (_size.width < coord.width) _size.width = coord.width;
			if (_size.height < coord.height) _size.height = coord.height;
			mScrollView->setCanvasSize(_size);
		}

	private:
		MyGUI::ScrollViewPtr mScrollView;

	};

} // namespace demo

#endif // __GRAPH_VIEW_H__
