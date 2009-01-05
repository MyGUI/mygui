/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __PANEL_VIEW_WINDOW_H__
#define __PANEL_VIEW_WINDOW_H__

#include "PanelView.h"
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class PanelViewWindow : public wraps::BaseLayout2
	{
	public:
		PanelViewWindow();
		PanelView* getPanelView() { return mPanelView; }

	private:
		void notifyWindowChangeCoord(MyGUI::WidgetPtr _sender);

	private:
		MyGUI::IntSize mOldSize;
		PanelView* mPanelView;
	};

} // namespace demo

#endif // __PANEL_VIEW_WINDOW_H__
