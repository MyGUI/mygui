/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_Marshaling.h"

namespace MMyGUI
{

#ifndef MMYGUI_USING_INTERFACE

	public value struct IntPoint
	{
		MMYGUI_DECLARE_EQUALS(IntPoint)

		int left, top;

		IntPoint( int _left, int _top ) : left( _left ), top( _top ) { }

		static bool operator == ( IntPoint lvalue, IntPoint rvalue )
        {
            return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top );
        }

	};

#endif // MMYGUI_USING_INTERFACE

	template <> struct Convert<const MyGUI::IntPoint&>
	{
		typedef IntPoint Type;
		inline static const IntPoint& To(const MyGUI::IntPoint& _value) { return reinterpret_cast<const IntPoint&>(_value); }
		inline static MyGUI::IntPoint& From(IntPoint& _value) { return reinterpret_cast<MyGUI::IntPoint&>(_value); }
	};
	template <> struct Convert<MyGUI::IntPoint>
	{
		typedef IntPoint Type;
		inline static const IntPoint& To(const MyGUI::IntPoint& _value) { return reinterpret_cast<const IntPoint&>(_value); }
		inline static MyGUI::IntPoint& From(IntPoint& _value) { return reinterpret_cast<MyGUI::IntPoint&>(_value); }
	};

} // namespace MMyGUI
