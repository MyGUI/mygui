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

	enum MYGUI_OBSOLETE_START("use : DropItemState")
	{
		//use DropItemState::None
		DROP_NONE,
		//use DropItemState::Start
		DROP_START,
		//use DropItemState::End
		DROP_END,
		//use DropItemState::Miss
		DROP_MISS,
		//use DropItemState::Accept
		DROP_ACCEPT,
		//use DropItemState::Refuse
		DROP_REFUSE
	} MYGUI_OBSOLETE_END;

	struct MYGUI_EXPORT DropItemState
	{
		enum DropState
		{
			None,
			Start,
			End,
			Miss,
			Accept,
			Refuse
		};

		int value;
		DropItemState() : value(None) { }
		DropItemState(int _value) : value(_value) { }
		friend bool operator == (DropItemState const & a, DropItemState const & b) { return a.value == b.value; }
		friend bool operator != (DropItemState const & a, DropItemState const & b) { return a.value != b.value; }
	};

	// структура информации об индексах дропа
	/** Inormation about drag'n'drop indexes */
	struct MYGUI_EXPORT ItemDropInfo
	{
		ItemDropInfo() :
			sender(nullptr),
			sender_index(ITEM_NONE),
			reseiver(nullptr),
			reseiver_index(ITEM_NONE)
		{
		}

		ItemDropInfo(WidgetPtr _sender, size_t _sender_index, WidgetPtr _reseiver, size_t _reseiver_index) :
			sender(_sender),
			sender_index(_sender_index),
			reseiver(_reseiver),
			reseiver_index(_reseiver_index)
		{
		}

		void set(WidgetPtr _sender, size_t _sender_index, WidgetPtr _reseiver, size_t _reseiver_index)
		{
			sender = _sender;
			sender_index = _sender_index;
			reseiver = _reseiver;
			reseiver_index = _reseiver_index;
		}

		void reset()
		{
			reseiver = nullptr;
			reseiver_index = ITEM_NONE;
			sender = nullptr;
			sender_index = ITEM_NONE;
		}

		// посылающий виджет
		WidgetPtr sender;
		// индекс посылающего виджета
		size_t sender_index;

		// принимающий виджет
		WidgetPtr reseiver;
		// индекс принимающего виджета
		size_t reseiver_index;
	};

	struct MYGUI_EXPORT DropWidgetInfo
	{
		DropWidgetInfo(WidgetPtr _item, const IntCoord & _dimension) :
			item(_item),
			dimension(_dimension)
		{
		}

		WidgetPtr item;
		IntCoord dimension;
	};
	typedef std::vector<DropWidgetInfo> VectorDropWidgetInfo;

	struct MYGUI_EXPORT DropWidgetState
	{
		DropWidgetState(size_t _index) :
			index(_index),
			update(true),
			accept(false),
			refuse(false)
		{ }

		// индекс этого элемента
		/** Index of element */
		size_t index;
		// изменилось не только состояние, но и содержимое
		/** State and interdan data changed */
		bool update;
		// айтем принимамет дроп
		/** Is widget accept drag */
		bool accept;
		// айтем не берет дроп
		/** Is widget refuse drag */
		bool refuse;
	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_DROP_INFO_H__
