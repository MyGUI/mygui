/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_COLOUR_H__
#define __MYGUI_COLOUR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	struct MYGUI_EXPORT Colour
	{
	public:
		Colour();
		Colour(float _red, float _green, float _blue, float _alpha = 1);
		explicit Colour(const std::string& _value);

		Colour& operator = (Colour const& _value);
		bool operator == (Colour const& _value) const;
		bool operator != (Colour const& _value) const;

		void set(float _red, float _green, float _blue, float _alpha = 1);

		void clear();

		std::string print() const;

		static Colour parse(const std::string& _value);

		friend std::ostream& operator << (std::ostream& _stream, const Colour&  _value)
		{
			return operatorShiftLeft(_stream, _value);
		}

		friend std::istream& operator >> (std::istream& _stream, Colour&  _value)
		{
			return operatorShiftRight(_stream, _value);
		}

		static std::ostream& operatorShiftLeft(std::ostream& _stream, const Colour&  _value);
		static std::istream& operatorShiftRight(std::istream& _stream, Colour&  _value);

	public:
		float red;
		float green;
		float blue;
		float alpha;

		static const Colour Zero;
		static const Colour Black;
		static const Colour White;
		static const Colour Red;
		static const Colour Green;
		static const Colour Blue;
	};

} // namespace MyGUI

#endif // __MYGUI_COLOUR_H__
