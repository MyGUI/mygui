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

	class GraphView : public wraps::BaseGraphView
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
		}

	private:
		void notifyChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size)
		{
			const MyGUI::IntCoord& coord = mScrollView->getClientCoord();
			if (_size.width < coord.width) _size.width = coord.width;
			if (_size.height < coord.height) _size.height = coord.height;
			mScrollView->setCanvasSize(_size);
		}
		/*virtual void initialise()
		{
			//mMainWidget->setCaption(mName);
			//assignBase(mConnectionIn1, "ConnectionIn1");
			//assignBase(mConnectionOut1, "ConnectionOut1");
			//assignBase(mConnectionIn2, "ConnectionIn2");
			//assignBase(mConnectionOut2, "ConnectionOut2");
		}

		virtual void shutdown()
		{
		}*/

	private:
		MyGUI::ScrollViewPtr mScrollView;
		/*std::string mName;
		wraps::BaseGraphConnection * mConnectionIn1;
		wraps::BaseGraphConnection * mConnectionOut1;
		wraps::BaseGraphConnection* mConnectionIn2;
		wraps::BaseGraphConnection* mConnectionOut2;*/


	};

} // namespace demo

#endif // __GRAPH_VIEW_H__
