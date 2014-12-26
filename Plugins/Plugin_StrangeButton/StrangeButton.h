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

	class MYGUI_EXPORT_DLL StrangeButton :
		public MyGUI::Button
	{
		MYGUI_RTTI_DERIVED( StrangeButton )

	public:
		StrangeButton();

	protected:
		virtual void onMouseLostFocus(MyGUI::Widget* _new);
		virtual void onMouseSetFocus(MyGUI::Widget* _old);
	};

} // namespace plugin

#endif // STRANGE_BUTTON_H_
