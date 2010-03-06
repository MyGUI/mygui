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
		EventType() : mTunnel(false), mBubble(false) { }
		EventType(const std::string& _name, bool _tunnel, bool _bubble) : mName(_name), mTunnel(_tunnel), mBubble(_bubble) { }
		~EventType() { }

		const std::string& getName() const { return mName; }
		bool isTunnel() const { return mTunnel; }
		bool isBubble() const { return mBubble; }

	private:
		std::string mName;
		bool mTunnel;
		bool mBubble;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_TYPE_H__
