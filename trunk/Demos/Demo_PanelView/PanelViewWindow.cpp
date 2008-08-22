/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#include "PanelViewWindow.h"

PanelViewWindow::PanelViewWindow() :
	BaseLayout("PanelView.layout")
{
}

void PanelViewWindow::initialise()
{
	loadLayout();

	assignWidget(mScrollView, "scroll_View");
	mPanelView.attach(mScrollView);

	if (mMainWidget->getWidgetType() == MyGUI::Window::_getType()) {
		static_cast<MyGUI::WindowPtr>(mMainWidget)->eventWindowChangeCoord = MyGUI::newDelegate(this, &PanelView::notifyWindowChangeCoord);
	}
}

void PanelViewWindow::notifyWindowChangeCoord(MyGUI::WidgetPtr _sender)
{
	mPanelView.updateView();
}
