/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_Colour.h"

namespace MyGUI
{

	const Colour Colour::Zero = Colour(0, 0, 0, 0);
	const Colour Colour::Black = Colour(0, 0, 0, 1);
	const Colour Colour::White = Colour(1, 1, 1, 1);
	const Colour Colour::Red = Colour(1, 0, 0, 1);
	const Colour Colour::Green = Colour(0, 1, 0, 1);
	const Colour Colour::Blue = Colour(0, 0, 1, 1);

	Colour::Colour() :
		red(1),
		green(1),
		blue(1),
		alpha(1)
	{
	}

	Colour::Colour( float _red, float _green, float _blue, float _alpha) :
		red(_red),
		green(_green),
		blue(_blue),
		alpha(_alpha)
	{
	}

	Colour::Colour(const std::string& _value)
	{
		*this = parse(_value);
	}

	Colour& Colour::operator = (Colour const& _value)
	{
		red = _value.red;
		green = _value.green;
		blue = _value.blue;
		alpha = _value.alpha;
		return *this;
	}

	bool Colour::operator == (Colour const& _value) const
	{
		return ((red == _value.red) && (green == _value.green) && (blue == _value.blue) && (alpha == _value.alpha));
	}

	bool Colour::operator != (Colour const& _value) const
	{
		return ! (*this == _value);
	}

	void Colour::set(float _red, float _green, float _blue, float _alpha)
	{
		red = _red;
		green = _green;
		blue = _blue;
		alpha = _alpha;
	}

	void Colour::clear()
	{
		red = green = blue = alpha = 0;
	}

	std::string Colour::print() const
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	Colour Colour::parse(const std::string& _value)
	{
		if (!_value.empty())
		{
			if (_value[0] == '#')
			{
				std::istringstream stream(_value.substr(1));
				int result = 0;
				stream >> std::hex >> result;
				if (!stream.fail())
				{
					return Colour( (unsigned char)( result >> 16 ) / 256.0f, (unsigned char)( result >> 8 ) / 256.0f, (unsigned char)( result ) / 256.0f );
				}
			}
			else
			{
				float red, green, blue;
				std::istringstream stream(_value);
				stream >> red >> green >> blue;
				if (!stream.fail())
				{
					float alpha = ALPHA_MAX;
					if (!stream.eof())
						stream >> alpha;
					return Colour(red, green, blue, alpha);
				}
			}
		}
		return Colour::Zero;
	}

	std::ostream& Colour::operatorShiftLeft(std::ostream& _stream, const Colour&  _value)
	{
		_stream << _value.red << " " << _value.green << " " << _value.blue << " " << _value.alpha;
		return _stream;
	}

	std::istream& Colour::operatorShiftRight(std::istream& _stream, Colour&  _value)
	{
		_value.clear();

		std::string value;
		_stream >> value;

		if (value.empty())
			return _stream;

		if (value[0] == '#')
		{
			_value = parse(value);
		}
		else
		{
			std::istringstream stream(value);
			stream >> _value.red;
			if (stream.fail())
				_value.clear();
			else
			{
				_stream >> _value.green >> _value.blue;
				if (!_stream.eof())
					_stream >> _value.alpha;
				else
					_value.alpha = 1;

				if (_stream.fail())
					_value.clear();
			}
		}

		return _stream;
	}

} // namespace MyGUI
