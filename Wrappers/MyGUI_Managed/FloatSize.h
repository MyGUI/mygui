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

		public value struct FloatSize
		{
			float width, height;

			FloatSize( float _width, float _height ) : width( _width ), height( _height ) { }

			static bool operator == ( FloatSize lvalue, FloatSize rvalue )
			{
				return ( lvalue.width == rvalue.width && lvalue.height == rvalue.height );
			}
			virtual bool Equals(FloatSize other)
			{
				return *this == other;
			}
			static bool operator != ( FloatSize lvalue, FloatSize rvalue )
			{
				return !(lvalue == rvalue);
			}

		};

		#else

		typedef MMYGUI_EXTERNAL_NAMESPACE FloatSize FloatSize;

		#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::FloatSize&>
		{
			typedef FloatSize Type;
			inline static const FloatSize& To(const MyGUI::FloatSize& _value)
			{
				return reinterpret_cast<const FloatSize&>(_value);
			}
			inline static MyGUI::FloatSize& From(FloatSize& _value)
			{
				return reinterpret_cast<MyGUI::FloatSize&>(_value);
			}
		};
		template <> struct Convert<MyGUI::FloatSize>
		{
			typedef FloatSize Type;
			inline static const FloatSize& To(const MyGUI::FloatSize& _value)
			{
				return reinterpret_cast<const FloatSize&>(_value);
			}
			inline static MyGUI::FloatSize& From(FloatSize& _value)
			{
				return reinterpret_cast<MyGUI::FloatSize&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
