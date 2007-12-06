/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TYPES_H__
#define __MYGUI_TYPES_H__

#include "MyGUI_Prerequest.h"
#include <Ogre.h>
#include "utility.h"
#include "TPoint.h"
#include "TSize.h"
#include "TRect.h"

namespace MyGUI
{

	// определяем типы
	typedef types::TPoint<int> IntPoint;
	typedef types::TPoint<float> FloatPoint;

	typedef types::TSize<int> IntSize;
	typedef types::TSize<float> FloatSize;

	typedef types::TRect<int> IntRect;
	typedef types::TRect<float> FloatRect;

} // namespace MyGUI

namespace util
{
	// определяем парсеры для наших типов в пространстве util для однообразности
	//inline MyGUI::IntPoint parseIntPoint(const std::string & _value) {return parseValueEx2<MyGUI::IntPoint, int>(_value);}
	//inline MyGUI::FloatPoint parseFloatPoint(const std::string & _value) {return parseValueEx2<MyGUI::FloatPoint, float>(_value);}

	//inline MyGUI::IntSize parseIntSize(const std::string & _value) {return parseValueEx2<MyGUI::IntSize, int>(_value);}
	//inline MyGUI::FloatSize parseFloatSize(const std::string & _value) {return parseValueEx2<MyGUI::FloatSize, float>(_value);}

	//inline MyGUI::IntRect parseIntRect(const std::string & _value) {return parseValueEx4<MyGUI::IntRect, int>(_value);}
	//inline MyGUI::FloatRect parseFloatRect(const std::string & _value) {return parseValueEx4<MyGUI::FloatRect, float>(_value);}

	namespace templates
	{
		template <class T>
		Ogre::ColourValue parseColour(const std::string& _value)
		{
			if (_value.empty()) return Ogre::ColourValue::ZERO;
			if (_value[0] == '#') {
				unsigned long ret = 0;
				if (0 == sscanf(_value.c_str(), "#%X", &ret)) return Ogre::ColourValue( ((float)(ret&0xFF0000))*(1/(255*256*256)), ((float)(ret&0xFF00))*(1/(255*256)), ((float)(ret&0xFF))*(1/255));
			}
			else {
				float red, green, blue, alpha = 1;
				std::istringstream str(_value);
				str >> red >> green >> blue;
				if (str.fail()) return Ogre::ColourValue::ZERO;
				str >> alpha;
				return Ogre::ColourValue(red, green, blue, alpha);
			}
//			std::vector<std::string> vec = util::split(_value);
//			if (vec.size() == 1) {
//				unsigned long ret = 0;
//				if (0 == sscanf(vec[0].c_str(), "#%X", &ret)) return Ogre::ColourValue( ((float)(ret&0xFF0000))*(1/(255*256*256)), ((float)(ret&0xFF00))*(1/(255*256)), ((float)(ret&0xFF))*(1/255));
//			}
//			else if (vec.size() == 3) return Ogre::ColourValue(parseFloat(vec[0]), parseFloat(vec[1]), parseFloat(vec[2]));
//			else if (vec.size() == 4) return Ogre::ColourValue(parseFloat(vec[0]), parseFloat(vec[1]), parseFloat(vec[2]), parseFloat(vec[3]));
			return Ogre::ColourValue::ZERO;
		}
	} // namespace templates

	inline Ogre::ColourValue parseColour(const std::string& _value) {return templates::parseColour<void>(_value);}

} // namespace util

#endif // __MYGUI_TYPES_H__