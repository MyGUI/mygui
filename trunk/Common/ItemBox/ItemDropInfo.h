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

namespace wraps
{

	struct ItemDropInfo
	{

		ItemDropInfo(const MyGUI::ItemDropInfo & _info) :
			sender(*_info.sender->getUserData<wraps::BaseLayout*>()),
			sender_index(_info.sender_index),
			reseiver(_info.reseiver ? *_info.reseiver->getUserData<wraps::BaseLayout*>() : null),
			reseiver_index(_info.reseiver_index)
		{
		}

		wraps::BaseLayout * sender;
		size_t sender_index;

		wraps::BaseLayout * reseiver;
		size_t reseiver_index;
	};

} // namespace wraps

#endif // __ITEM_DROP_INFO_H__
