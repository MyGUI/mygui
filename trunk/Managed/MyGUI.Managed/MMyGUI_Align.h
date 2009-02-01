/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"

namespace MMyGUI
{

	public value struct Align
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(Align);
		MMYGUI_DECLARE_EQUALS(Align);

		MMYGUI_DECLARE_ENUM(Align, HCenter);
		MMYGUI_DECLARE_ENUM(Align, VCenter);
		MMYGUI_DECLARE_ENUM(Align, Center);

		MMYGUI_DECLARE_ENUM(Align, Left);
		MMYGUI_DECLARE_ENUM(Align, Right);
		MMYGUI_DECLARE_ENUM(Align, HStretch);

		MMYGUI_DECLARE_ENUM(Align, Top);
		MMYGUI_DECLARE_ENUM(Align, Bottom);
		MMYGUI_DECLARE_ENUM(Align, VStretch);

		MMYGUI_DECLARE_ENUM(Align, Stretch);
		MMYGUI_DECLARE_ENUM(Align, Default);

		explicit Align( int _value ) : value( _value) { }

		static Align operator | ( Align lvalue, Align rvalue ) { return Align( lvalue.value | rvalue.value ); }

		static bool operator == ( Align lvalue, Align rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI
