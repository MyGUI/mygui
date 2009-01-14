/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_INT_POINT_H__
#define __MMYGUI_INT_POINT_H__

#include <MyGUI.h>

#include "MMyGUI_Common.h"

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

#endif // __MMYGUI_INT_POINT_H__
