/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef PANEL_DYNAMIC_H_
#define PANEL_DYNAMIC_H_

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelDynamic :
		public wraps::BasePanelViewItem
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

#endif // PANEL_DYNAMIC_H_
