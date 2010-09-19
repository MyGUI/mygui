/*!
	@file
	@author		Albert Semenov
	@date		12/2008
*/
/*
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
