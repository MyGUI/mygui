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
			reseiver_data(null),
			sender(null),
			sender_index(ITEM_NONE),
			sender_data(null)
		{
		}

		ItemDropInfo(WidgetPtr _sender, size_t _sender_index, void * _sender_data, WidgetPtr _reseiver, size_t _reseiver_index, void * _reseiver_data) :
			sender(_sender),
			sender_index(_sender_index),
			sender_data(_sender_data),
			reseiver(_reseiver),
			reseiver_index(_reseiver_index),
			reseiver_data(_reseiver_data)
		{
		}

		void set(WidgetPtr _sender, size_t _sender_index, void * _sender_data, WidgetPtr _reseiver, size_t _reseiver_index, void * _reseiver_data)
		{
			sender = _sender;
			sender_index = _sender_index;
			sender_data = _sender_data;
			reseiver = _reseiver;
			reseiver_index = _reseiver_index;
			reseiver_data = _reseiver_data;
		}

		void reset()
		{
			reseiver = null;
			reseiver_index = ITEM_NONE;
			reseiver_data = null;
			sender = null;
			sender_index = ITEM_NONE;
			sender_data = null;
		}

		// посылающий виджет 
		WidgetPtr sender;
		// индекс посылающего виджета
		size_t sender_index;
		// ассоциированные данные отправителя
		void * sender_data;

		// принимающий виджет
		WidgetPtr reseiver;
		// индекс принимающего виджета
		size_t reseiver_index;
		// ассоциированные данные получателя
		void * reseiver_data;
	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_DROP_INFO_H__
