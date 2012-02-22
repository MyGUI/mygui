/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef __CONTROL_PANEL_H__
#define __CONTROL_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "PointerContextManager.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(ControlPanel, "ControlPanel.layout");
	class ControlPanel :
		public wraps::BaseLayout
	{
	public:
		ControlPanel(PointerContextManager* _manager);

		void injectKeyPress(MyGUI::KeyCode _key);

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mBusy, "Busy");
		MyGUI::Button* mBusy;
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mRepair, "Repair");
		MyGUI::Button* mRepair;
		ATTRIBUTE_FIELD_WIDGET_NAME(ControlPanel, mAttack, "Attack");
		MyGUI::Button* mAttack;

		PointerContextManager* mPointerContextManager;
	};

} // namespace demo

#endif // __CONTROL_PANEL_H__
