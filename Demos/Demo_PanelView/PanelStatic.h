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

	class PanelStatic :
		public wraps::BasePanelViewItem
	{
	public:
		PanelStatic();

		virtual void initialise();
		virtual void shutdown();
	};

} // namespace demo

#endif // PANEL_STATIC_H_
