/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WIDGET_DEFINES_H_
#define MYGUI_WIDGET_DEFINES_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include <vector>
#include <map>
#include <string>

namespace MyGUI
{

	class Widget;
	typedef std::vector<Widget*> VectorWidgetPtr;
	typedef std::map<std::string, Widget*> MapWidgetPtr;
	typedef Enumerator<VectorWidgetPtr> EnumeratorWidgetPtr;

} // namespace MyGUI

#endif // MYGUI_WIDGET_DEFINES_H_
