/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MACROS_H_
#define MYGUI_MACROS_H_

#include "MyGUI_Prerequest.h"
#include <limits>
#include <stddef.h>

namespace MyGUI
{

	const size_t ITEM_NONE = (std::numeric_limits<size_t>::max)();
	const int DEFAULT = -1;
	const float ALPHA_MAX = 1.0f;
	const float ALPHA_MIN = 0.0f;

	//FIXME заменить на шаблоны
#define MYGUI_FLAG_NONE  0
#define MYGUI_FLAG(num)  (1<<(num))

} // namespace MyGUI

#endif // MYGUI_MACROS_H_
