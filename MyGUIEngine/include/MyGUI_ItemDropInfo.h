/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
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
			Start,
			End,
			Miss,
			Accept,
			Refuse
		};

		DDItemState(Enum _value = None) : value(_value) { }

		friend bool operator == (DDItemState const & a, DDItemState const & b) { return a.value == b.value; }
		friend bool operator != (DDItemState const & a, DDItemState const & b) { return a.value != b.value; }

	private:
		Enum value;
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

		DDItemInfo(WidgetPtr _sender, size_t _sender_index, WidgetPtr _receiver, size_t _receiver_index) :
			sender(_sender),
			sender_index(_sender_index),
			receiver(_receiver),
			receiver_index(_receiver_index)
		{
		}

		void set(WidgetPtr _sender, size_t _sender_index, WidgetPtr _receiver, size_t _receiver_index)
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

		// посылающий виджет
		WidgetPtr sender;
		// индекс посылающего виджета
		size_t sender_index;

		// принимающий виджет
		WidgetPtr receiver;
		// индекс принимающего виджета
		size_t receiver_index;
	};

	struct MYGUI_EXPORT DDWidgetState
	{
		DDWidgetState(size_t _index) :
			index(_index),
			update(true),
			accept(false),
			refuse(false)
		{ }

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
