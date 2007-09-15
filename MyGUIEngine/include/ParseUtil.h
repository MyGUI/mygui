#ifndef _PARSEUTILS_H_
#define _PARSEUTILS_H_

#include "Prerequest.h"
#include <Ogre.h>

namespace widget
{

	template< class T >
	T parseValue( const std::string& _value )
	{
		std::istringstream str(_value);
		T ret = 0;
		str >> ret;
        return ret;
	}

	inline int parseInt(const std::string& _value) {return parseValue<int>(_value);}
	inline float parseFloat(const std::string& _value) {return parseValue<float>(_value);}

	inline Ogre::ColourValue parseColour(const std::string& _value)
	{
		const std::vector<std::string> & vec = util::split(_value);
		if (vec.size() == 1) {
			unsigned long ret = 0;
			if (vec[0].length() == 8) sscanf(vec[0].c_str(), "0x%X", &ret);
			else if (vec[0].length() == 7) sscanf(vec[0].c_str(), "#%X", &ret);
			else if (vec[0].length() == 6) sscanf(vec[0].c_str(), "%X", &ret);
			else return Ogre::ColourValue::ZERO;
			return Ogre::ColourValue( (((float)((ret>>16)&0xFF))/255.0), (((float)((ret>>8)&0xFF))/255.0), (((float)((ret)&0xFF))/255.0) );
		}
		if ((vec.size() == 3) || (vec.size() == 4)) return Ogre::ColourValue(parseFloat(vec[0]), parseFloat(vec[1]), parseFloat(vec[2])) ;
		return Ogre::ColourValue::ZERO;
	}

} // namespace widget

#endif
