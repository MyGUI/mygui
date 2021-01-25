/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_BITWISE_H_
#define MYGUI_BITWISE_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class Bitwise
	{
	public:
		/** Returns the closest power-of-two number greater or equal to value.
		*/
		template<typename Type>
		static inline Type firstPO2From(Type _value)
		{
			--_value;
			_value |= _value >> 16;
			_value |= _value >> 8;
			_value |= _value >> 4;
			_value |= _value >> 2;
			_value |= _value >> 1;
			++_value;
			return _value;
		}

		/** Determines whether the number is power-of-two or not. */
		template<typename Type>
		static inline bool isPO2(Type _value)
		{
			return (_value & (_value - 1)) == 0;
		}

	};

} // namespace MyGUI

#endif // MYGUI_BITWISE_H_
