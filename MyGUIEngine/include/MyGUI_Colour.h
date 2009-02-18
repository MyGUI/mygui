/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_COLOUR_H__
#define __MYGUI_COLOUR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

#include <OgreColourValue.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

		struct MYGUI_EXPORT Colour
		{
			float red, green, blue, alpha;

			static const Colour Zero;
			static const Colour Black;
			static const Colour White;
			static const Colour Red;
			static const Colour Green;
			static const Colour Blue;

			Colour() : red( 1 ), green( 1 ), blue( 1 ), alpha( 1 ) { }
			Colour( float _red, float _green, float _blue, float _alpha = 1 ) : red( _red ), green( _green ), blue( _blue ), alpha( _alpha ) { }
			explicit Colour(const std::string& _value) { *this = parse(_value); }


			Colour & operator=( Colour const & _value )
			{
				red = _value.red;
				green = _value.green;
				blue = _value.blue;
				alpha = _value.alpha;
				return *this;
			}

			// Ogre
			Colour(const Ogre::ColourValue& _value) : red( _value.r ), green( _value.g ), blue( _value.b ), alpha( _value.a ) { }
			friend bool operator == (Ogre::ColourValue  const & a, Colour const & b) { return b == a; }
			friend bool operator != (Ogre::ColourValue  const & a, Colour const & b) { return !(b == a); }
			Ogre::ColourValue toColourValue() { return Ogre::ColourValue(red, green, blue, alpha); }

			uint32 toColourARGB()
			{
				uint32 val32 = uint8(alpha * 255);
				val32 <<= 8;
				val32 += uint8(red * 255);
				val32 <<= 8;
				val32 += uint8(green * 255);
				val32 <<= 8;
				val32 += uint8(blue * 255);
				return val32;
			}

			bool operator==( Colour const & _value ) const
			{
				return ((red == _value.red) && (green == _value.green) && (blue == _value.blue) && (alpha == _value.alpha));
			}

			bool operator!=( Colour const & _value ) const
			{
				return ! (*this == _value);
			}

			void set( float _red, float _green, float _blue, float _alpha = 1 )
			{
				red = _red;
				green = _green;
				blue = _blue;
				alpha = _alpha;
			}

			void clear()
			{
				red = green = blue = alpha = 0;
			}

			std::string print() const
			{
				std::ostringstream stream;
				stream << *this;
				return stream.str();
			}

			static Colour parse(const std::string& _value)
			{
				if (!_value.empty()) {
					if (_value[0] == '#') {
						std::istringstream stream(_value.substr(1));
						int result = 0;
						stream >> std::hex >> result;
						if (!stream.fail()) {
							return Colour( (unsigned char)( result >> 16 ) / 256.0f, (unsigned char)( result >> 8 ) / 256.0f, (unsigned char)( result ) / 256.0f );
						}
					}
					else {
						float red, green, blue, alpha = 1;
						std::istringstream stream(_value);
						stream >> red >> green >> blue;
						if (!stream.fail()) {
							stream >> alpha;
							return Colour(red, green, blue, alpha);
						}
					}
				}
				return Colour(0, 0, 0, 0);
			}

			friend std::ostream& operator << ( std::ostream& _stream, const Colour&  _value )
			{
				_stream << _value.red << " " << _value.green << " " << _value.blue << " " << _value.alpha;
				return _stream;
			}

			friend std::istream& operator >> ( std::istream& _stream, Colour&  _value )
			{
				_stream >> _value.red >> _value.green >> _value.blue >> _value.alpha;
				if (_stream.fail()) _value.clear();
				return _stream;
			}

		};

} // namespace MyGUI

#endif // __MYGUI_COLOUR_H__
