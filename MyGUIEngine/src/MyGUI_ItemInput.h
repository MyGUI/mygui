/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ITEM_INPUT_H_
#define MYGUI_ITEM_INPUT_H_

#include "MyGUI_IBItemInfo.h"
#include "MyGUI_InputEvent.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	enum class ItemInputScope
	{
		Item,
		ItemOrClient
	};

	inline Widget* consumeItemInput(const Widget& _owner, InputEventContext& _event, ItemInputScope _scope)
	{
		auto* target = _event.getTarget();
		if (target == nullptr || target == &_owner)
			return nullptr;

		const bool acceptsTarget = target == _owner.getClientWidget()
			? _scope == ItemInputScope::ItemOrClient
			: target->_getContainer() == &_owner;
		if (!acceptsTarget)
			return nullptr;

		_event.setHandled();
		return target;
	}

	inline IBNotifyItemData itemNotification(
		size_t _index,
		IBNotifyItemData::NotifyItem _kind,
		const MouseButtonEvent& _event)
	{
		const auto point = _event.getPosition();
		return {_index, _kind, point.left, point.top, _event.getButton()};
	}

	inline IBNotifyItemData itemNotification(size_t _index, const KeyPressEvent& _event)
	{
		return {_index, IBNotifyItemData::KeyPressed, _event.getKey(), _event.getCharacter()};
	}

	inline IBNotifyItemData itemNotification(size_t _index, const KeyReleaseEvent& _event)
	{
		return {_index, IBNotifyItemData::KeyReleased, _event.getKey()};
	}

}

#endif // MYGUI_ITEM_INPUT_H_
