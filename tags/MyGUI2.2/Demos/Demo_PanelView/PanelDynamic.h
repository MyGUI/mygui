/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_DYNAMIC_H__
#define __PANEL_DYNAMIC_H__

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelDynamic : public wraps::BasePanelViewItem
	{
	public:
		PanelDynamic();

		virtual void initialise();
		virtual void shutdown();

		void setVisibleCount(size_t _count);

	private:
		MyGUI::VectorWidgetPtr mItemsText;
		MyGUI::VectorWidgetPtr mItemsEdit;

	};

} // namespace demo

#endif // __PANEL_DYNAMIC_H__
