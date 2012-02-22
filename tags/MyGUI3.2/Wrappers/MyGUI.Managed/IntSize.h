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

		public value struct IntSize
		{
			int width, height;

			IntSize( int _width, int _height ) : width( _width ), height( _height ) { }

			static bool operator == ( IntSize lvalue, IntSize rvalue )
			{
				return ( lvalue.width == rvalue.width && lvalue.height == rvalue.height );
			}
			virtual bool Equals(IntSize other)
			{
				return *this == other;
			}
			static bool operator != ( IntSize lvalue, IntSize rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE IntSize IntSize;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::IntSize&>
		{
			typedef IntSize Type;
			inline static const IntSize& To(const MyGUI::IntSize& _value)
			{
				return reinterpret_cast<const IntSize&>(_value);
			}
			inline static MyGUI::IntSize& From(IntSize& _value)
			{
				return reinterpret_cast<MyGUI::IntSize&>(_value);
			}
		};
		template <> struct Convert<MyGUI::IntSize>
		{
			typedef IntSize Type;
			inline static const IntSize& To(const MyGUI::IntSize& _value)
			{
				return reinterpret_cast<const IntSize&>(_value);
			}
			inline static MyGUI::IntSize& From(IntSize& _value)
			{
				return reinterpret_cast<MyGUI::IntSize&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
