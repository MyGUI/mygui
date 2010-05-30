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
#ifndef __MYGUI_UI_ELEMENT_H__
#define __MYGUI_UI_ELEMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IObject.h"
#include "MyGUI_EventInfo.h"
#include "MyGUI_EventType.h"
#include "MyGUI_EventArgs.h"

namespace MyGUI
{

	typedef delegates::CMultiDelegate3<Widget*, EventInfo*, EventArgs*> RoutedEventHandler;

	class MYGUI_EXPORT UIElement :
		public IObject
	{
		MYGUI_RTTI_DERIVED( UIElement )

	public:
		UIElement();
		virtual ~UIElement();

		static void registerEvent(const char* _name, EventType::EventPolicy _policy, IEventCaller* _caller = nullptr);
		static void unregisterEvent(const char* _name);

		void raiseEvent(const std::string& _name, EventArgs* _args);

		void addEventHandler(const std::string& _name, RoutedEventHandler::IDelegate* _delegate);
		void removeEventHandler(const std::string& _name, RoutedEventHandler::IDelegate* _delegate);

	private:
		bool onRaiseEvent(Widget* _sender, EventInfo* _info, EventArgs* _args, IEventCaller* _caller);
		bool onSendEvent(Widget* _sender, EventInfo* _info, EventArgs* _args, IEventCaller* _caller);

	private:
		typedef std::map<std::string, std::pair<EventType, IEventCaller*> > MapInfo;
		static MapInfo mEvents;

		typedef std::map<std::string, RoutedEventHandler> MapHandlerDelegate;
		MapHandlerDelegate mHandlers;
	};

} // namespace MyGUI

#endif // __MYGUI_UI_ELEMENT_H__
