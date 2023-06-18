/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f96377f4_5267_4e1d_a47b_7f8f402870c4_
#define _f96377f4_5267_4e1d_a47b_7f8f402870c4_

#include "MyGUI.h"

namespace MyGUI::utility
{

	template<>
	inline std::string toString<UString>(UString _value)
	{
		return _value;
	}

	template<>
	inline UString parseValue<UString>(std::string_view _value)
	{
		return UString(_value);
	}

	template<>
	inline bool parseComplex<UString>(std::string_view _value, UString& arg)
	{
		arg.assign(_value);
		return true;
	}

}

#endif
