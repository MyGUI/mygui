/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_TYPES_H__
#define __MYGUI_TYPES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Utility.h"
#include <Ogre.h>
#include "TPoint.h"
#include "TSize.h"
#include "TRect.h"
#include "TCoord.h"

namespace MyGUI
{

	// определяем типы
	typedef types::TPoint<int> IntPoint;
	typedef types::TPoint<float> FloatPoint;

	typedef types::TSize<int> IntSize;
	typedef types::TSize<float> FloatSize;

	typedef types::TRect<int> IntRect;
	typedef types::TRect<float> FloatRect;

	typedef types::TCoord<int> IntCoord;
	typedef types::TCoord<float> FloatCoord;

	typedef std::map<std::string, std::string> MapString;
	typedef std::vector<std::string> VectorString;

	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned int uint;

	typedef wchar_t Char;

	namespace utility
	{
		namespace templates
		{
			template <class T>
			Ogre::ColourValue parseColour(const std::string& _value)
			{
				if (_value.empty()) return Ogre::ColourValue::ZERO;
				if (_value[0] == '#') {
					int ret = 0;
					if (1 == sscanf(_value.c_str(), "#%X", &ret))
						return Ogre::ColourValue( (unsigned char)( ret >> 16 ) / 256.0f, (unsigned char)( ret >> 8 ) / 256.0f, (unsigned char)( ret ) / 256.0f );
				}
				else {
					float red, green, blue, alpha = 1;
					std::istringstream str(_value);
					str >> red >> green >> blue;
					if (str.fail()) return Ogre::ColourValue::ZERO;
					str >> alpha;
					return Ogre::ColourValue(red, green, blue, alpha);
				}
				return Ogre::ColourValue::ZERO;
			}
		} // namespace templates

		inline Ogre::ColourValue parseColour(const std::string& _value) {return templates::parseColour<void>(_value);}

	} // namespace utility

} // namespace MyGUI

#endif // __MYGUI_TYPES_H__
