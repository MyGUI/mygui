/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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
#ifndef __MYGUI_ENUM_CHAR_INFO_H__
#define __MYGUI_ENUM_CHAR_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Font.h"

namespace MyGUI
{

	// вспомогательный класс для хранения информации о символе
	class EnumCharInfo
	{
	public:
		EnumCharInfo() : mData(0) {}
		explicit EnumCharInfo(size_t _value, bool _colour = false) : mData(_colour ? (_value | 0xFF000000) : _value) {}
		explicit EnumCharInfo(float _value) : mDataFloat(_value) {}
		explicit EnumCharInfo(Font::GlyphInfo * _info) : mData((size_t)_info) {}

		size_t getValueSizeT() { return mData; }
		float getValueFloat() { return mDataFloat; }
		Ogre::RGBA getColour() { return (Ogre::RGBA) (mData & 0x00FFFFFF); }
		Font::GlyphInfo * getGlyphInfo() {return (Font::GlyphInfo *)mData;}
		bool isColour() { return (mData & 0xFF000000) == 0xFF000000; }

	private:
		union
		{
			size_t mData;
			float mDataFloat;
		};
	};

	struct LineInfo
	{
		LineInfo() { }

		LineInfo(size_t _count, size_t _length, float _real_length) :
			count(_count),
			length(_length),
			real_length(_real_length)
		{ }

		void set(size_t _count, size_t _length, float _real_length)
		{
			count = _count;
			length = _length;
			real_length = _real_length;
		}

		// колличество символов, без учета цвета
		size_t count;
		// длинна строки в пикселях
		size_t length;
		// длинна строки в реальных координатах
		float real_length;
	};

	typedef std::vector<EnumCharInfo> VectorCharInfo;
	typedef std::pair<LineInfo, VectorCharInfo> PairVectorCharInfo;
	typedef std::vector<PairVectorCharInfo> VectorLineInfo;

} // namespace MyGUI

#endif // __MYGUI_ENUM_CHAR_INFO_H__
