/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
