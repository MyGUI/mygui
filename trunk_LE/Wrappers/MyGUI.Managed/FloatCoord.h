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

		public value struct FloatCoord
		{
			float left, top, width, height;

			FloatCoord( float _left, float _top, float _width, float _height ) : left( _left ), top( _top ), width( _width ), height( _height ) { }

			static bool operator == ( FloatCoord lvalue, FloatCoord rvalue )
			{
				return ( lvalue.left == rvalue.left && lvalue.top == rvalue.top && lvalue.width == rvalue.width && lvalue.height == rvalue.height );
			}
			virtual bool Equals(FloatCoord other)
			{
				return *this == other;
			}
			static bool operator != ( FloatCoord lvalue, FloatCoord rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE FloatCoord FloatCoord;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::FloatCoord&>
		{
			typedef FloatCoord Type;
			inline static const FloatCoord& To(const MyGUI::FloatCoord& _value)
			{
				return reinterpret_cast<const FloatCoord&>(_value);
			}
			inline static MyGUI::FloatCoord& From(FloatCoord& _value)
			{
				return reinterpret_cast<MyGUI::FloatCoord&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatCoord>
		{
			typedef FloatCoord Type;
			inline static const FloatCoord& To(const MyGUI::FloatCoord& _value)
			{
				return reinterpret_cast<const FloatCoord&>(_value);
			}
			inline static MyGUI::FloatCoord& From(FloatCoord& _value)
			{
				return reinterpret_cast<MyGUI::FloatCoord&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatCoord&>
		{
			typedef FloatCoord % Type;
			inline static FloatCoord % To(MyGUI::FloatCoord& _value)
			{
				return reinterpret_cast<FloatCoord&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
