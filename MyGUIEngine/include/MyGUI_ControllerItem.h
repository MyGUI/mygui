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

	/** Base interface for controllers */
	class MYGUI_EXPORT ControllerItem
	{
	public:
		virtual ~ControllerItem() {};

		virtual void prepareItem(WidgetPtr _widget) = 0;
		//virtual void replaseItem(WidgetPtr _widget, ControllerItem * _item) = 0;
		virtual bool addTime(WidgetPtr _widget, float _time) = 0;
		virtual const std::string & getType() = 0;

		/** Event : Before controller started working.\n
			signature : void method(MyGUI::WidgetPtr _sender)\n
			@param _sender widget under control
		*/
		delegates::CDelegate1<WidgetPtr> eventPreAction;

		/** Event : Controller updated (called every frame).\n
			signature : void method(MyGUI::WidgetPtr _sender)\n
			@param _sender widget under control
		*/
		delegates::CDelegate1<WidgetPtr> eventUpdateAction;

		/** Event : After controller finished working.\n
			signature : void method(MyGUI::WidgetPtr _sender)\n
			@param _sender widget under control
		*/
		delegates::CDelegate1<WidgetPtr> eventPostAction;

	};

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_ITEM_H__
