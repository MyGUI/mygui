/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
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
		explicit EnumCharInfo() : mData(0) {}
		explicit EnumCharInfo(size_t _value, bool _colour = false) : mData(_colour ? (_value | 0xFF000000) : _value) {}
		explicit EnumCharInfo(float _value) : mDataFloat(_value) {}
		explicit EnumCharInfo(Font::GlyphInfo * _info) : mData((size_t)_info) {}

		inline size_t getValueSizeT() { return mData; }
		inline float getValueFloat() { return mDataFloat; }
		inline Ogre::RGBA getColour() { return (Ogre::RGBA) (mData & 0x00FFFFFF); }
		inline Font::GlyphInfo * getGlyphInfo() {return (Font::GlyphInfo *)mData;}
		inline bool isColour() { return (mData & 0xFF000000) == 0xFF000000; }

	private:
		union
		{
			size_t mData;
			float mDataFloat;
		};
	};

	typedef std::vector<EnumCharInfo> VectorCharInfo;
	typedef std::vector<VectorCharInfo> VectorLineInfo;

} // namespace MyGUI

#endif // __MYGUI_ENUM_CHAR_INFO_H__
