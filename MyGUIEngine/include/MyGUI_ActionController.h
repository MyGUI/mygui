/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_ACTION_CONTROLLER_H__
#define __MYGUI_ACTION_CONTROLLER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	namespace action
	{

		void _MyGUIExport actionWidgetHide(WidgetPtr _widget);

		void _MyGUIExport actionWidgetDestroy(WidgetPtr _widget);

	} // namespace action

} // namespace MyGUI

#endif // __MYGUI_ACTION_CONTROLLER_H__
