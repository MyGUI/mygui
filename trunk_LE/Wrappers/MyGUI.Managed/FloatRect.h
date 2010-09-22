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

		public value struct FloatRect
		{
			float left, top, right, bottom;

			FloatRect( float _left, float _top, float _right, float _bottom ) : left( _left ), top( _top ), right( _right ), bottom( _bottom ) { }

			static bool operator == ( FloatRect lvalue, FloatRect rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.right == rvalue.right && lvalue.bottom == rvalue.bottom );
			}
			virtual bool Equals(FloatRect other)
			{
				return *this == other;
			}
			static bool operator != ( FloatRect lvalue, FloatRect rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE FloatRect FloatRect;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::FloatRect&>
		{
			typedef FloatRect Type;
			inline static const FloatRect& To(const MyGUI::FloatRect& _value)
			{
				return reinterpret_cast<const FloatRect&>(_value);
			}
			inline static MyGUI::FloatRect& From(FloatRect& _value)
			{
				return reinterpret_cast<MyGUI::FloatRect&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatRect>
		{
			typedef FloatRect Type;
			inline static const FloatRect& To(const MyGUI::FloatRect& _value)
			{
				return reinterpret_cast<const FloatRect&>(_value);
			}
			inline static MyGUI::FloatRect& From(FloatRect& _value)
			{
				return reinterpret_cast<MyGUI::FloatRect&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatRect&>
		{
			typedef FloatRect % Type;
			inline static FloatRect % To(MyGUI::FloatRect& _value)
			{
				return reinterpret_cast<FloatRect&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
