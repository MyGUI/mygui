/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "PanelViewWindow.h"

namespace demo
{

	PanelViewWindow::PanelViewWindow() : BaseLayout2("PanelView.layout")
	{
		assignBase(mPanelView, "scroll_View");

		MyGUI::WindowPtr window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != null) {
			window->eventWindowChangeCoord = MyGUI::newDelegate(this, &PanelViewWindow::notifyWindowChangeCoord);
			mOldSize = window->getSize();
		}
	}

	void PanelViewWindow::notifyWindowChangeCoord(MyGUI::WidgetPtr _sender)
	{
		const MyGUI::IntSize & size = _sender->getSize();
		if (size != mOldSize) {
			mOldSize = size;
			mPanelView->setNeedUpdate();
		}
	}

}