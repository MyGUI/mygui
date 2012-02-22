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

		public value struct IntRect
		{
			int left, top, right, bottom;

			IntRect( int _left, int _top, int _right, int _bottom ) : left( _left ), top( _top ), right( _right ), bottom( _bottom ) { }

			static bool operator == ( IntRect lvalue, IntRect rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.right == rvalue.right && lvalue.bottom == rvalue.bottom );
			}
			virtual bool Equals(IntRect other)
			{
				return *this == other;
			}
			static bool operator != ( IntRect lvalue, IntRect rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE IntRect IntRect;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IntRect&>
		{
			typedef IntRect Type;
			inline static const IntRect& To(const MyGUI::IntRect& _value)
			{
				return reinterpret_cast<const IntRect&>(_value);
			}
			inline static MyGUI::IntRect& From(IntRect& _value)
			{
				return reinterpret_cast<MyGUI::IntRect&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntRect>
		{
			typedef IntRect Type;
			inline static const IntRect& To(const MyGUI::IntRect& _value)
			{
				return reinterpret_cast<const IntRect&>(_value);
			}
			inline static MyGUI::IntRect& From(IntRect& _value)
			{
				return reinterpret_cast<MyGUI::IntRect&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntRect&>
		{
			typedef IntRect % Type;
			inline static IntRect % To(MyGUI::IntRect& _value)
			{
				return reinterpret_cast<IntRect&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
