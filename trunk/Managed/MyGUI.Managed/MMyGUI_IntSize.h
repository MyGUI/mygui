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

	public value struct IntSize
	{
		MMYGUI_DECLARE_EQUALS(IntSize)

		int width, height;

		IntSize( int _width, int _height ) : width( _width ), height( _height ) { }

		static bool operator == ( IntSize lvalue, IntSize rvalue )
        {
            return ( lvalue.width == rvalue.width && lvalue.height == rvalue.height );
        }

	};

	template <> struct Convert<const MyGUI::IntSize&>
	{
		typedef IntSize Type;
		inline static const IntSize& To(const MyGUI::IntSize& _value) { return reinterpret_cast<const IntSize&>(_value); }
		inline static MyGUI::IntSize& From(IntSize& _value) { return reinterpret_cast<MyGUI::IntSize&>(_value); }
	};

} // namespace MMyGUI
