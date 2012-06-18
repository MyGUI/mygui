/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Config.h"
#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public value struct IntCoord
		{
			int left, top, width, height;

			IntCoord( int _left, int _top, int _width, int _height ) : left( _left ), top( _top ), width( _width ), height( _height ) { }

			static bool operator == ( IntCoord lvalue, IntCoord rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.width == rvalue.width && lvalue.height == rvalue.height );
			}
			virtual bool Equals(IntCoord other)
			{
				return *this == other;
			}
			static bool operator != ( IntCoord lvalue, IntCoord rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE IntCoord IntCoord;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IntCoord&>
		{
			typedef IntCoord Type;
			inline static const IntCoord& To(const MyGUI::IntCoord& _value)
			{
				return reinterpret_cast<const IntCoord&>(_value);
			}
			inline static MyGUI::IntCoord& From(IntCoord& _value)
			{
				return reinterpret_cast<MyGUI::IntCoord&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntCoord>
		{
			typedef IntCoord Type;
			inline static const IntCoord& To(const MyGUI::IntCoord& _value)
			{
				return reinterpret_cast<const IntCoord&>(_value);
			}
			inline static MyGUI::IntCoord& From(IntCoord& _value)
			{
				return reinterpret_cast<MyGUI::IntCoord&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntCoord&>
		{
			typedef IntCoord % Type;
			inline static IntCoord % To(MyGUI::IntCoord& _value)
			{
				return reinterpret_cast<IntCoord&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
