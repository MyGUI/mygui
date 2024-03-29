/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef KEYBOARD_PANEL_H_
#define KEYBOARD_PANEL_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(KeyboardPanel, "Keyboard.layout");
	class KeyboardPanel : public wraps::BaseLayout
	{
	public:
		KeyboardPanel();

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(KeyboardPanel, mButtonEngine, "ButtonEngine");
		MyGUI::Button* mButtonEngine;
		ATTRIBUTE_FIELD_WIDGET_NAME(KeyboardPanel, mButtonGun, "ButtonGun");
		MyGUI::Button* mButtonGun;
		ATTRIBUTE_FIELD_WIDGET_NAME(KeyboardPanel, mButtonAmmo, "ButtonAmmo");
		MyGUI::Button* mButtonAmmo;
		ATTRIBUTE_FIELD_WIDGET_NAME(KeyboardPanel, mButtonArmor, "ButtonArmor");
		MyGUI::Button* mButtonArmor;
	};

} // namespace demo

#endif // KEYBOARD_PANEL_H_
