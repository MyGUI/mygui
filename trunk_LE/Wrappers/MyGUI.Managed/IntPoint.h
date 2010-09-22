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

		public value struct IntPoint
		{
			int left, top;

			IntPoint( int _left, int _top ) : left( _left ), top( _top ) { }

			static bool operator == ( IntPoint lvalue, IntPoint rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top );
			}
			virtual bool Equals(IntPoint other)
			{
				return *this == other;
			}
			static bool operator != ( IntPoint lvalue, IntPoint rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE IntPoint IntPoint;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IntPoint&>
		{
			typedef IntPoint Type;
			inline static const IntPoint& To(const MyGUI::IntPoint& _value)
			{
				return reinterpret_cast<const IntPoint&>(_value);
			}
			inline static MyGUI::IntPoint& From(IntPoint& _value)
			{
				return reinterpret_cast<MyGUI::IntPoint&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntPoint>
		{
			typedef IntPoint Type;
			inline static const IntPoint& To(const MyGUI::IntPoint& _value)
			{
				return reinterpret_cast<const IntPoint&>(_value);
			}
			inline static MyGUI::IntPoint& From(IntPoint& _value)
			{
				return reinterpret_cast<MyGUI::IntPoint&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
