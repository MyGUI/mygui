/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f96377f4_5267_4e1d_a47b_7f8f402870c4_
#define _f96377f4_5267_4e1d_a47b_7f8f402870c4_

#include "MyGUI.h"

namespace MyGUI
{
	namespace utility
	{

		template<>
		inline std::string toString<UString>(UString _value)
		{
			return _value;
		}

		template<>
		inline UString parseValue(const std::string& _value)
		{
			return _value;
		}

	}
}

#endif
