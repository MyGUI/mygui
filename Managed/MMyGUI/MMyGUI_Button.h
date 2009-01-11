/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_BUTTON_H__
#define __MMYGUI_BUTTON_H__

#include <MyGUI.h>

#include "MMyGUI_Widget.h"

namespace MMyGUI
{

	public ref class Button : public Widget
	{

	public:
		//--------------------------------------------------------------------
		// базовые методы
		Button() : Widget() { }
		Button( MyGUI::Button* _native ) : Widget(_native) { }

		Button( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
		{
			CreateWidget(MyGUI::Button::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, "");
		}

		Button( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			CreateWidget(MyGUI::Button::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, _name);
		}

		Button( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align)
		{
			CreateWidget(MyGUI::Button::getClassTypeName(), _parent,  _skin, _coord, _align, "", "");
		}

		Button( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _name)
		{
			CreateWidget(MyGUI::Button::getClassTypeName(), _parent,  _skin, _coord, _align, "", _name);
		}

		//--------------------------------------------------------------------
		// методы паблик интерфейса
		property bool ButtonPresset
		{
			bool get() { return static_cast<MyGUI::Button*>(mNative)->getButtonPressed(); }
			void set(bool _value) { static_cast<MyGUI::Button*>(mNative)->setButtonPressed(_value); }
		}

	};

} // namespace MMyGUI

#endif // __MMYGUI_BUTTON_H__
