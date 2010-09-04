/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __PANEL_VIEW_WINDOW_H__
#define __PANEL_VIEW_WINDOW_H__

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

#endif // __PANEL_VIEW_WINDOW_H__
