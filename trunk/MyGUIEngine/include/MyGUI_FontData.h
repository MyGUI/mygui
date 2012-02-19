/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#ifndef __MYGUI_FONT_DATA_H__
#define __MYGUI_FONT_DATA_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT FontCodeType
	{
		enum Enum
		{
			Tab = 0x0009,
			LF = 0x000A,
			CR = 0x000D,
			Space = 0x0020,
			LatinStart = 0x0021,
			NEL = 0x0085,
			LatinEnd = 0x00A6,

			// The following are special code points. These are used represent displayable text elements that do not correspond to
			// any actual Unicode code point. To prevent collisions, they must be defined with values higher than that of the
			// highest valid Unicode code point (0x10FFFF as of Unicode 6.1).
			Selected = 0xFFFFFFFC, // Used for rendering text selections when they have input focus.
			SelectedBack = 0xFFFFFFFD, // Used for rendering text selections when they don't have input focus.
			Cursor = 0xFFFFFFFE, // Used for rendering the blinking text cursor.
			NotDefined = 0xFFFFFFFF // Used to render substitute glyphs for characters that aren't supported by the current font.
		};

		FontCodeType(Enum _value = NotDefined) :
			value(_value)
		{
		}

		friend bool operator == (FontCodeType const& a, FontCodeType const& b)
		{
			return a.value == b.value;
		}

		friend bool operator != (FontCodeType const& a, FontCodeType const& b)
		{
			return a.value != b.value;
		}

	private:
		Enum value;
	};

	// информация об одном символе
	struct GlyphInfo
	{
		GlyphInfo(
			Char _codePoint = 0U,
			float _width = 0.0f,
			float _height = 0.0f,
			float _advance = 0.0f,
			float _bearingX = 0.0f,
			float _bearingY = 0.0f,
			const FloatRect& _uvRect = FloatRect()) :
			codePoint(_codePoint),
			width(_width),
			height(_height),
			advance(_advance),
			bearingX(_bearingX),
			bearingY(_bearingY),
			uvRect(_uvRect)
		{
		}

		Char codePoint;
		float width;
		float height;
		float advance;
		float bearingX;
		float bearingY;
		FloatRect uvRect;
	};

	typedef std::vector<GlyphInfo> VectorGlyphInfo;

	// информация об диапазоне
	//FIXME move to std::pair
	class PairCodePoint
	{
	public:
		PairCodePoint() :
			first(0),
			last(0)
		{
		}

		PairCodePoint(Char _first, Char _last) :
			first(_first),
			last(_last)
		{
		}

		// проверяет входит ли символ в диапазон
		bool isExist(Char _code) const
		{
			return _code >= first && _code <= last;
		}

	public:
		Char first;
		Char last;
	};

	// инфомация о диапазоне символов
	class RangeInfo
	{
	public:
		RangeInfo() :
			first(0),
			last(0)
		{
		}

		RangeInfo(Char _first, Char _last) :
			first(_first),
			last(_last)
		{
			range.resize(last - first + 1);
		}

		// проверяет входит ли символ в диапазон
		bool isExist(Char _code) const
		{
			return _code >= first && _code <= last;
		}

		// возвращает указатель на глиф, или 0, если код не входит в диапазон
		GlyphInfo* getInfo(Char _code)
		{
			return isExist(_code) ? &range[_code - first] : nullptr;
		}

		void setInfo(Char _code, GlyphInfo* _value)
		{
			if (isExist(_code)) range[_code - first] = *_value;
		}

	public:
		Char first;
		Char last;
		VectorGlyphInfo range;
	};

	// FIXME move to resource font
	class PairCodeCoord
	{
	public:
		PairCodeCoord() :
			code(0)
		{
		}

		PairCodeCoord(Char _code, const IntCoord& _coord) :
			code(_code),
			coord(_coord)
		{
		}

		bool operator < (const PairCodeCoord& _value) const
		{
			return code < _value.code;
		}

	public:
		Char code;
		IntCoord coord;
	};

} // namespace MyGUI

#endif // __MYGUI_FONT_DATA_H__
