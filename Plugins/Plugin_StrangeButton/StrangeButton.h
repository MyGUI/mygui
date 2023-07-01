/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/
#ifndef STRANGE_BUTTON_H_
#define STRANGE_BUTTON_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Button.h"

namespace plugin
{

	class MYGUI_EXPORT_DLL StrangeButton : public MyGUI::Button
	{
		MYGUI_RTTI_DERIVED(StrangeButton)

	protected:
		void onMouseLostFocus(MyGUI::Widget* _new) override;
		void onMouseSetFocus(MyGUI::Widget* _old) override;
	};

} // namespace plugin

#endif // STRANGE_BUTTON_H_
