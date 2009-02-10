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

MMYGUI_BEGIN_NAMESPACE

//#ifndef MMYGUI_USING_INTERFACE

public value struct Colour
{

	MMYGUI_DECLARE_EQUALS(Colour)

	int red, green, blue, alpha;

	Colour( int _red, int _green, int _blue, int _alpha ) : red( _red ), green( _green ), blue( _blue ), alpha( _alpha ) { }

	static bool operator == ( Colour lvalue, Colour rvalue )
    {
        return ( lvalue.red == rvalue.red && lvalue.green == rvalue.green && lvalue.blue == rvalue.blue && lvalue.alpha == rvalue.alpha );
    }

	static Colour Red = Colour(1, 0, 0, 1);
	static Colour Green = Colour(0, 1, 0, 1);
	static Colour Blue = Colour(0, 0, 1, 1);

	static Colour Black = Colour(0, 0, 0, 1);
	static Colour White = Colour(1, 1, 1, 1);
	static Colour Zero = Colour(0, 0, 0, 0);

};

//#endif // MMYGUI_USING_INTERFACE

template <> struct Convert<const MyGUI::Colour&>
{
	typedef Colour Type;
	inline static const Colour& To(const MyGUI::Colour& _value) { return reinterpret_cast<const Colour&>(_value); }
	inline static MyGUI::Colour& From(Colour& _value) { return reinterpret_cast<MyGUI::Colour&>(_value); }
};

MMYGUI_END_NAMESPACE
