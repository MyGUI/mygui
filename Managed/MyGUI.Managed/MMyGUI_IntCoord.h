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

	public value struct IntCoord
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(IntCoord);
		MMYGUI_DECLARE_EQUALS(IntCoord)

		int left, top, width, height;

		IntCoord( int _left, int _top, int _width, int _height ) : left( _left ), top( _top ), width( _width ), height( _height ) { }

		static bool operator == ( IntCoord lvalue, IntCoord rvalue )
        {
            return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.width == rvalue.width && lvalue.height == rvalue.height );
        }

	};

} // namespace MMyGUI
