/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(ControlPanel, "ControlPanel.layout");
	class ControlPanel :
		public wraps::BaseLayout
	{
	public:
		ControlPanel();

	private:
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mBusy, "Busy");
		MyGUI::Button* mBusy;
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mRepare, "Repare");
		MyGUI::Button* mRepare;
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mAttack, "Attack");
		MyGUI::Button* mAttack;
	};

} // namespace demo

#endif // __CONTROL_PANEL_H__
