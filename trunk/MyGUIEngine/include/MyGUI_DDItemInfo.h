/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_ITEM_DROP_INFO_H__
#define __MYGUI_ITEM_DROP_INFO_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT DDItemState
	{
		enum Enum
		{
			None,
			Start, /**< start drag */
			End, /**< end drag (drop) */
			Miss, /**< drag DDContainer over empty space or widgets that don't have drag'n'drop */
			Accept, /**< drag DDContainer over another DDContainer that accept dropping on it */
			Refuse /**< drag DDContainer over another DDContainer that refuse dropping on it */
		};

		DDItemState(Enum _value = None) :
			mValue(_value)
		{
		}

		friend bool operator == (DDItemState const& a, DDItemState const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (DDItemState const& a, DDItemState const& b)
		{
			return a.mValue != b.mValue;
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		Enum mValue;
	};

	// структура информации об индексах дропа
	/** Inormation about drag'n'drop indexes */
	struct MYGUI_EXPORT DDItemInfo
	{
		DDItemInfo() :
			sender(nullptr),
			sender_index(ITEM_NONE),
			receiver(nullptr),
			receiver_index(ITEM_NONE)
		{
		}

		DDItemInfo(DDContainer* _sender, size_t _sender_index, DDContainer* _receiver, size_t _receiver_index) :
			sender(_sender),
			sender_index(_sender_index),
			receiver(_receiver),
			receiver_index(_receiver_index)
		{
		}

		void set(DDContainer* _sender, size_t _sender_index, DDContainer* _receiver, size_t _receiver_index)
		{
			sender = _sender;
			sender_index = _sender_index;
			receiver = _receiver;
			receiver_index = _receiver_index;
		}

		void reset()
		{
			sender = nullptr;
			sender_index = ITEM_NONE;
			receiver = nullptr;
			receiver_index = ITEM_NONE;
		}

		/** DDContainer that send this event (container from which we started drag) */
		DDContainer* sender;
		/** Index of sender container */
		size_t sender_index;

		/** DDContainer that receive dragged widget (container to which we want to drop) */
		DDContainer* receiver;
		/** Index of receiver container */
		size_t receiver_index;
	};

	struct MYGUI_EXPORT DDWidgetState
	{
		DDWidgetState(size_t _index) :
			index(_index),
			update(true),
			accept(false),
			refuse(false)
		{
		}

		/** Index of element */
		size_t index;
		/** State and internal data changed */
		bool update;
		/** Is widget accept drop */
		bool accept;
		/** Is widget refuse drop */
		bool refuse;
	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_DROP_INFO_H__
