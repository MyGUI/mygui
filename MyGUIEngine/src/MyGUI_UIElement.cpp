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

#include "MyGUI_Precompiled.h"
#include "MyGUI_UIElement.h"
#include "MyGUI_Widget.h"
#include "MyGUI_IEventCaller.h"

namespace MyGUI
{

	UIElement::MapInfo UIElement::mEvents;

	UIElement::UIElement()
	{
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::registerEvent(const char* _name, bool _tunnel, bool _bubble, IEventCaller* _caller)
	{
		EventType info(_name, _tunnel, _bubble);
		mEvents[_name] = std::make_pair(info, _caller);
	}

	void UIElement::unregisterEvent(const char* _name)
	{
		MapInfo::iterator item = mEvents.find(_name);
		if (item != mEvents.end())
		{
			delete item->second.second;
			mEvents.erase(item);
		}
	}

	void UIElement::raiseEvent(const std::string& _name, EventArgs* _args)
	{
		MapInfo::iterator entry = mEvents.find(_name);
		if (entry != mEvents.end())
		{
			Widget* widget = static_cast<Widget*>(this);
			EventType type = entry->second.first;
			IEventCaller* caller = entry->second.second;
			EventInfo info(widget, type);

			if (type.isTunnel())
			{
				onRaiseEvent(widget, &info, _args, caller);
			}
			else if (type.isBubble())
			{
				bool handled = onSendEvent(widget, &info, _args, caller);
				if (!handled)
					onRaiseEvent(widget, &info, _args, caller);
			}
			else
			{
				onSendEvent(widget, &info, _args, caller);
			}
		}
		else
		{
			// log
		}
	}

	bool UIElement::onRaiseEvent(Widget* _sender, EventInfo* _info, EventArgs* _args, IEventCaller* _caller)
	{
		if (_info->getEventType().isTunnel())
		{
			bool handled = false;
			Widget* parent = _sender->getParent();
			if (parent != nullptr)
				handled = parent->onRaiseEvent(parent, _info, _args, _caller);

			if (!handled)
				handled = onSendEvent(_sender, _info, _args, _caller);

			return handled;
		}
		else if (_info->getEventType().isBubble())
		{
			Widget* parent = _sender->getParent();
			if (parent != nullptr)
			{
				bool handled = parent->onSendEvent(parent, _info, _args, _caller);
				if (!handled)
					parent->onRaiseEvent(parent, _info, _args, _caller);
			}
		}

		return false;
	}

	bool UIElement::onSendEvent(Widget* _sender, EventInfo* _info, EventArgs* _args, IEventCaller* _caller)
	{
		MapHandlerDelegate::iterator entry = mHandlers.find(_info->getEventType().getName());
		if (entry != mHandlers.end())
			entry->second(_sender, _info, _args);

		if (_caller != nullptr)
			_caller->invoke(this, _sender, _info, _args);

		//MYGUI_OUT(_sender->getName());

		return _info->getHandled();
	}

	void UIElement::addEventHandler(const std::string& _name, RoutedEventHandler::IDelegate* _delegate)
	{
		MapHandlerDelegate::iterator entry = mHandlers.find(_name);
		if (entry == mHandlers.end())
			entry = mHandlers.insert(std::pair<std::string, RoutedEventHandler>(_name, RoutedEventHandler())).first;

		entry->second += _delegate;
	}

	void UIElement::removeEventHandler(const std::string& _name, RoutedEventHandler::IDelegate* _delegate)
	{
		MapHandlerDelegate::iterator entry = mHandlers.find(_name);
		if (entry != mHandlers.end())
		{
			entry->second -= _delegate;
			if (entry->second.empty())
				mHandlers.erase(entry);
		}
	}

} // namespace MyGUI
