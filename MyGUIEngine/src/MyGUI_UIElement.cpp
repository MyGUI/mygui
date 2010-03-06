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

namespace MyGUI
{

	UIElement::MapInfo UIElement::mEvents;

	UIElement::UIElement()
	{
	}

	UIElement::~UIElement()
	{
	}

	void UIElement::registerEvent(const std::string& _name, bool _tunnel, bool _bubble)
	{
		EventType info(_name, _tunnel, _bubble);
		mEvents[_name] = info;
	}

	void UIElement::unregisterEvent(const std::string& _name)
	{
		mEvents.erase(_name);
	}

	void UIElement::raiseEvent(const std::string& _name, EventArgs* _args)
	{
		MapInfo::iterator entry = mEvents.find(_name);
		if (entry != mEvents.end())
		{
			Widget* widget = static_cast<Widget*>(this);
			EventType type = entry->second;
			EventInfo info(widget, type);

			if (type.isTunnel())
			{
				onRaiseEvent(widget, &info, _args);
			}
			else if (type.isBubble())
			{
				bool handled = onEvent(widget, &info, _args);
				if (!handled)
					onRaiseEvent(widget, &info, _args);
			}
			else
			{
				onEvent(widget, &info, _args);
			}
		}
		else
		{
			// log
		}
	}

	bool UIElement::onRaiseEvent(Widget* _sender, EventInfo* _info, EventArgs* _args)
	{
		if (_info->getEventType().isTunnel())
		{
			bool handled = false;
			Widget* parent = _sender->getParent();
			if (parent != nullptr)
				handled = parent->onRaiseEvent(parent, _info, _args);

			if (!handled)
				handled = onEvent(_sender, _info, _args);

			return handled;
		}
		else if (_info->getEventType().isBubble())
		{
			Widget* parent = _sender->getParent();
			if (parent != nullptr)
			{
				bool handled = parent->onEvent(parent, _info, _args);
				if (!handled)
					parent->onRaiseEvent(parent, _info, _args);
			}
		}

		return false;
	}

	bool UIElement::onEvent(Widget* _sender, EventInfo* _info, EventArgs* _args)
	{
		MapHandlerDelegate::iterator entry = mHandlers.find(_info->getEventType().getName());
		if (entry != mHandlers.end())
			entry->second(_sender, _info, _args);

		return _info->getHandled();
	}

	void UIElement::addEventHandler(const std::string& _name, RoutedEventHandlerDelegate::IDelegate* _delegate)
	{
		MapHandlerDelegate::iterator entry = mHandlers.find(_name);
		if (entry == mHandlers.end())
			entry = mHandlers.insert(std::pair<std::string, RoutedEventHandlerDelegate>(_name, RoutedEventHandlerDelegate())).first;

		entry->second += _delegate;
	}

	void UIElement::removeEventHandler(const std::string& _name, RoutedEventHandlerDelegate::IDelegate* _delegate)
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
