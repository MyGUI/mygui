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
		static MYGUI_FORCEINLINE Type firstPO2From(Type _value)
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
		static MYGUI_FORCEINLINE bool isPO2(Type _value)
		{
			return (_value & (_value - 1)) == 0;
		}

		/** Returns the number of bits a pattern must be shifted right by to
		remove right-hand zeros.
		*/
		template<typename Type>
		static MYGUI_FORCEINLINE size_t getBitShift(Type _mask)
		{
			if (_mask == 0)
				return 0;

			size_t result = 0;
			while ((_mask & 1) == 0)
			{
				++result;
				_mask >>= 1;
			}
			return result;
		}
	};

} // namespace MyGUI

#endif // MYGUI_BITWISE_H_
