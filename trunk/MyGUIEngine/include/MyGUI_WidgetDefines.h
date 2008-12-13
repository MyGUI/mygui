/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_DEFINES_H__
#define __MYGUI_WIDGET_DEFINES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"

namespace MyGUI
{

	class Widget;
	//typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> VectorWidgetPtr;
	typedef std::map<std::string, WidgetPtr> MapWidgetPtr;
	typedef Enumerator<VectorWidgetPtr> EnumeratorWidgetPtr;

} // namespace MyGUI

#endif // __MYGUI_WIDGET_DEFINES_H__
