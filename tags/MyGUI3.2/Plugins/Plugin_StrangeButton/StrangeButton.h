/*!
	@file
	@author		Denis Koronchik
	@date		09/2007
*/
#ifndef __STRANGE_BUTTON_H__
#define __STRANGE_BUTTON_H__

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

#endif // __STRANGE_BUTTON_H__
