/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_ALIGN_H__
#define __MMYGUI_ALIGN_H__

#include <MyGUI.h>
#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

#define MMYGUI__DECLARE_ALIGN(T) \
		static Align T = Align(MyGUI::Align::T);

	public value struct Align
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(Align);
		MMYGUI_DECLARE_EQUALS(Align);

		MMYGUI__DECLARE_ALIGN(HCenter);
		MMYGUI__DECLARE_ALIGN(VCenter);
		MMYGUI__DECLARE_ALIGN(Center);

		MMYGUI__DECLARE_ALIGN(Left);
		MMYGUI__DECLARE_ALIGN(Right);
		MMYGUI__DECLARE_ALIGN(HStretch);

		MMYGUI__DECLARE_ALIGN(Top);
		MMYGUI__DECLARE_ALIGN(Bottom);
		MMYGUI__DECLARE_ALIGN(VStretch);

		MMYGUI__DECLARE_ALIGN(Stretch);
		MMYGUI__DECLARE_ALIGN(Default);

		explicit Align( int _value ) : value( _value) { }

		static Align operator | ( Align lvalue, Align rvalue ) { return Align( lvalue.value | rvalue.value ); }

		static bool operator == ( Align lvalue, Align rvalue ) { return ( lvalue.value == rvalue.value ); }

	private:
		int value;
	};

} // namespace MMyGUI

#endif // __MMYGUI_ALIGN_H__
