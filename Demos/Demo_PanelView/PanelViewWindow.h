/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef PANEL_VIEW_WINDOW_H_
#define PANEL_VIEW_WINDOW_H_

#include "PanelView.h"
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class PanelViewWindow :
		public wraps::BaseLayout
	{
	public:
		PanelViewWindow() :
			BaseLayout("PanelView.layout"),
			mPanelView(nullptr)
		{
			assignBase(mPanelView, "scroll_View");
		}

		PanelView* getPanelView()
		{
			return mPanelView;
		}

	private:
		PanelView* mPanelView;
	};

} // namespace demo

#endif // PANEL_VIEW_WINDOW_H_
