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

	public value struct IntPoint
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(IntPoint);
		MMYGUI_DECLARE_EQUALS(IntPoint)

		int left, top;

		IntPoint( int _left, int _top ) : left( _left ), top( _top ) { }

		static bool operator == ( IntPoint lvalue, IntPoint rvalue )
        {
            return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top );
        }

	};

} // namespace MMyGUI
