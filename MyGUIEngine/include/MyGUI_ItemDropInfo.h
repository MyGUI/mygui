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

} // namespace MyGUI

#endif // __MYGUI_ITEM_DROP_INFO_H__
