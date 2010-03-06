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
#ifndef __MYGUI_EVENT_INFO_H__
#define __MYGUI_EVENT_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EventType.h"

namespace MyGUI
{

	class MYGUI_EXPORT EventInfo
	{
	public:
		EventInfo() : mWidgetSource(nullptr), mHandled(false) { }
		EventInfo(Widget* _source, const EventType& _type) : mWidgetSource(_source), mHandled(false), mType(_type) { }
		~EventInfo() { }

		bool getHandled() { return mHandled; }
		void setHanled(bool _value) { mHandled = _value; }

		Widget* getSource() { return mWidgetSource; }
		const EventType& getEventType() { return mType; }

	private:
		Widget* mWidgetSource;
		bool mHandled;
		EventType mType;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_INFO_H__
