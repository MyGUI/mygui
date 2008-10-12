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

	enum DropState {
		DROP_START,
		DROP_END,
		DROP_MISS,
		DROP_ACCEPT,
		DROP_REFUSE
	};

	// структура информации об индексах дропа
	struct ItemDropInfo
	{
		ItemDropInfo() :
			reseiver(null),
			reseiver_index(ITEM_NONE),
			sender(null),
			sender_index(ITEM_NONE)
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
			reseiver = null;
			reseiver_index = ITEM_NONE;
			sender = null;
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

	struct DropWidgetInfo
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

	struct DropWidgetState
	{
		DropWidgetState(size_t _index) :
			index(_index),
			accept(false),
			refuse(false),
			update(true)
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
