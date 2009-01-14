/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_COLOUR_H__
#define __MMYGUI_COLOUR_H__

#include <MyGUI.h>

#include "MMyGUI_Common.h"

namespace MMyGUI
{

	public value struct Colour
	{

		MMYGUI_MANAGED_NATIVE_CONVERSIONS_FOR_VALUE(Colour);
		MMYGUI_DECLARE_EQUALS(Colour)

		int red, green, blue, alpha;

		Colour( int _red, int _green, int _blue, int _alpha ) : red( _red ), green( _green ), blue( _blue ), alpha( _alpha ) { }

		static bool operator == ( Colour lvalue, Colour rvalue )
        {
            return ( lvalue.red == rvalue.red && lvalue.green == rvalue.green && lvalue.blue == rvalue.blue && lvalue.alpha == rvalue.alpha );
        }

	};

} // namespace MMyGUI

#endif // __MMYGUI_COLOUR_H__
