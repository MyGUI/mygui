/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_DROP_INFO_H__
#define __ITEM_DROP_INFO_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "CellView.h"
#include "ItemData.h"

struct ItemDropInfo
{
	ItemDropInfo(const MyGUI::ItemDropInfo & _info) :
		sender(*_info.sender->getUserData<BaseLayout*>()),
		sender_index(_info.sender_index),
		sender_data((ItemData*)_info.sender_data),
		reseiver(_info.reseiver ? *_info.reseiver->getUserData<BaseLayout*>() : null),
		reseiver_index(_info.reseiver_index),
		reseiver_data((ItemData*)_info.reseiver_data)
	{
	}

	BaseLayout * sender;
	size_t sender_index;
	ItemData * sender_data;

	BaseLayout * reseiver;
	size_t reseiver_index;
	ItemData * reseiver_data;
};

#endif // __ITEM_DROP_INFO_H__
