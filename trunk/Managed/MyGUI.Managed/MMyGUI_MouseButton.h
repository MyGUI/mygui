/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Marshaling.h"

MMYGUI_BEGIN_NAMESPACE

#ifndef MMYGUI_USING_INTERFACE

public enum struct MouseButton
{
	Left = MyGUI::MouseButton::Left,
	Right = MyGUI::MouseButton::Right,
	Middle = MyGUI::MouseButton::Middle,

	Button0 = MyGUI::MouseButton::Button0,
	Button1 = MyGUI::MouseButton::Button1,
	Button2 = MyGUI::MouseButton::Button2,
	Button3 = MyGUI::MouseButton::Button3,
	Button4 = MyGUI::MouseButton::Button4,
	Button5 = MyGUI::MouseButton::Button5,
	Button6 = MyGUI::MouseButton::Button6,
	Button7 = MyGUI::MouseButton::Button7,

	None = MyGUI::MouseButton::None
};

#else

typedef MMYGUI_EXTERNAL_NAMESPACE MouseButton MouseButton;

#endif // MMYGUI_USING_INTERFACE

template <> struct Convert<MyGUI::MouseButton>
{
	typedef MouseButton Type;
	inline static const MouseButton& To(const MyGUI::MouseButton& _value) { return reinterpret_cast<const MouseButton&>(_value); }
	inline static MyGUI::MouseButton& From(MouseButton& _value) { return reinterpret_cast<MyGUI::MouseButton&>(_value); }
};

MMYGUI_END_NAMESPACE
