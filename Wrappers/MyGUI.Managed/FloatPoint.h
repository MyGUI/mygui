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

		public value struct FloatPoint
		{
			float left, top;

			FloatPoint( float _left, float _top ) : left( _left ), top( _top ) { }

			static bool operator == ( FloatPoint lvalue, FloatPoint rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top );
			}
			virtual bool Equals(FloatPoint other)
			{
				return *this == other;
			}
			static bool operator != ( FloatPoint lvalue, FloatPoint rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE FloatPoint FloatPoint;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::FloatPoint&>
		{
			typedef FloatPoint Type;
			inline static const FloatPoint& To(const MyGUI::FloatPoint& _value)
			{
				return reinterpret_cast<const FloatPoint&>(_value);
			}
			inline static MyGUI::FloatPoint& From(FloatPoint& _value)
			{
				return reinterpret_cast<MyGUI::FloatPoint&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatPoint>
		{
			typedef FloatPoint Type;
			inline static const FloatPoint& To(const MyGUI::FloatPoint& _value)
			{
				return reinterpret_cast<const FloatPoint&>(_value);
			}
			inline static MyGUI::FloatPoint& From(FloatPoint& _value)
			{
				return reinterpret_cast<MyGUI::FloatPoint&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
