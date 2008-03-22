/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_ITEM_H__
#define __MYGUI_CONTROLLER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport ControllerItem
	{
	public:

		virtual void prepareItem(WidgetPtr _widget) = 0;
		virtual void replaseItem(WidgetPtr _widget, ControllerItem * _item) = 0;
		virtual bool addTime(WidgetPtr _widget, float _time) = 0;
		virtual const std::string & getType() = 0;

		// делегат, который будет вызван, перед началом работы контроллера
		delegates::CDelegate1<WidgetPtr> eventPreAction;

		// делегат, который будет вызван, после завершения работы контроллера
		delegates::CDelegate1<WidgetPtr> eventPostAction;

	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_ITEM_H__
