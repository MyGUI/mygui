/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef PANEL_STATIC_H_
#define PANEL_STATIC_H_

#include <MyGUI.h>
#include "PanelView/BasePanelViewItem.h"

namespace demo
{

	class PanelStatic : public wraps::BasePanelViewItem
	{
	public:
		PanelStatic();

		void initialise() override;
		void shutdown() override;
	};

} // namespace demo

#endif // PANEL_STATIC_H_
