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

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public value struct Colour
		{

			float red, green, blue, alpha;

			Colour( float _red, float _green, float _blue ) : red( _red ), green( _green ), blue( _blue ), alpha( 1 ) { }

			Colour( float _red, float _green, float _blue, float _alpha ) : red( _red ), green( _green ), blue( _blue ), alpha( _alpha ) { }

			static bool operator == ( Colour lvalue, Colour rvalue )
			{
				return ( lvalue.red == rvalue.red && lvalue.green == rvalue.green && lvalue.blue == rvalue.blue && lvalue.alpha == rvalue.alpha );
			}
			virtual bool Equals(Colour other)
			{
				return *this == other;
			}
			static bool operator != ( Colour lvalue, Colour rvalue )
			{
				return !(lvalue == rvalue);
			}

			static Colour Red = Colour(1, 0, 0, 1);
			static Colour Green = Colour(0, 1, 0, 1);
			static Colour Blue = Colour(0, 0, 1, 1);

			static Colour Black = Colour(0, 0, 0, 1);
			static Colour White = Colour(1, 1, 1, 1);
			static Colour Zero = Colour(0, 0, 0, 0);

		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::Colour&>
		{
			typedef Colour Type;
			inline static const Colour& To(const MyGUI::Colour& _value)
			{
				return reinterpret_cast<const Colour&>(_value);
			}
			inline static MyGUI::Colour& From(Colour& _value)
			{
				return reinterpret_cast<MyGUI::Colour&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
