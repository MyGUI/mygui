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
#ifndef __MYGUI_EVENT_TYPE_H__
#define __MYGUI_EVENT_TYPE_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class EventType
	{
	public:
		enum EventPolicy
		{
			Direct,
			Tunnel,
			Bubble
		};

		EventType() : mPolicy(Direct) { }
		EventType(const std::string& _name, EventPolicy _policy) : mName(_name), mPolicy(_policy) { }
		~EventType() { }

		const std::string& getName() const { return mName; }
		EventPolicy getPolicy() const { return mPolicy; }

	private:
		std::string mName;
		EventPolicy mPolicy;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_TYPE_H__
