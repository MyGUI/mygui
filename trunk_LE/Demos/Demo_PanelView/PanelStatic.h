/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __PANEL_STATIC_H__
#define __PANEL_STATIC_H__

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

#endif // __PANEL_STATIC_H__
