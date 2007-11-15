#ifndef _TYPES_H_
#define _TYPES_H_

#include "Prerequest.h"
#include "utility.h"
#include <Ogre.h>
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
	inline MyGUI::IntPoint parseIntPoint(const std::string & _value) {return parseValueEx2<MyGUI::IntPoint, int>(_value);}
	inline MyGUI::FloatPoint parseFloatPoint(const std::string & _value) {return parseValueEx2<MyGUI::FloatPoint, float>(_value);}

	inline MyGUI::IntSize parseIntSize(const std::string & _value) {return parseValueEx2<MyGUI::IntSize, int>(_value);}
	inline MyGUI::FloatSize parseFloatSize(const std::string & _value) {return parseValueEx2<MyGUI::FloatSize, float>(_value);}

	inline MyGUI::IntRect parseIntRect(const std::string & _value) {return parseValueEx4<MyGUI::IntRect, int>(_value);}
	inline MyGUI::FloatRect parseFloatRect(const std::string & _value) {return parseValueEx4<MyGUI::FloatRect, float>(_value);}

	namespace templates
	{
		template <class T>
		Ogre::ColourValue parseColour(const std::string& _value)
		{
			std::vector<std::string> vec = util::split(_value);
			if (vec.size() == 1) {
				unsigned long ret = 0;
				if (0 == sscanf(vec[0].c_str(), "#%X", &ret)) return Ogre::ColourValue( ((float)(ret&0xFF0000))*(1/(255*256*256)), ((float)(ret&0xFF00))*(1/(255*256)), ((float)(ret&0xFF))*(1/255));
			}
			else if (vec.size() == 3) return Ogre::ColourValue(parseFloat(vec[0]), parseFloat(vec[1]), parseFloat(vec[2]));
			else if (vec.size() == 4) return Ogre::ColourValue(parseFloat(vec[0]), parseFloat(vec[1]), parseFloat(vec[2]), parseFloat(vec[3]));
			return Ogre::ColourValue::ZERO;
		}
	} // namespace templates

	inline Ogre::ColourValue parseColour(const std::string& _value) {return templates::parseColour<void>(_value);}

} // namespace util

#endif // _TYPES_H_