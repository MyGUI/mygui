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

	MyGUI::WindowPtr window = mMainWidget->castType<MyGUI::Window>(false);
	if (window != null) {
		window->eventWindowChangeCoord = MyGUI::newDelegate(this, &PanelViewWindow::notifyWindowChangeCoord);
		mOldSize = window->getSize();
	}
}

void PanelViewWindow::shutdown()
{
	removeAllItems();
}

void PanelViewWindow::notifyWindowChangeCoord(MyGUI::WidgetPtr _sender)
{
	const MyGUI::IntSize & size = _sender->getSize();
	if (size != mOldSize) {
		mOldSize = size;
		mPanelView.setNeedUpdate();
	}
}
