/*!
	@file
	@author		Albert Semenov
	@date		03/2010
*/
/*
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
#ifndef __MYGUI_I_EVENT_CALLER_H__
#define __MYGUI_I_EVENT_CALLER_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class IEventCaller
	{
	public:
		IEventCaller() { }
		virtual ~IEventCaller() { }

		virtual void invoke(UIElement* _this, Widget* _sender, EventInfo* _info, EventArgs* _args) { }
	};

	template <typename WidgetType, typename ArgsType>
	class EventCaller : public IEventCaller
	{
	public:
		typedef void (WidgetType::*Method)( Widget*, EventInfo*, ArgsType* );

		EventCaller() : mMethod(nullptr) { }
		EventCaller(Method _method) : mMethod(_method) { }
		virtual ~EventCaller() { }

		virtual void invoke(UIElement* _this, Widget* _sender, EventInfo* _info, EventArgs* _args)
		{
			if (mMethod != nullptr)
			{
				WidgetType* resiver = _this->castType<WidgetType>(false);
				if (resiver != nullptr)
				{
					ArgsType* args = _args->castType<ArgsType>();
					(resiver->*mMethod)(_sender, _info, args);
				}
			}
		}

	private:
		Method mMethod;
	};

} // namespace MyGUI

#endif // __MYGUI_I_EVENT_CALLER_H__
