/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#ifndef __ITEM_DROP_INFO_H__
#define __ITEM_DROP_INFO_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{
	struct DDItemInfo
	{
		DDItemInfo(const MyGUI::DDItemInfo& _info) :
			sender(*_info.sender->getUserData<wraps::BaseLayout*>()),
			sender_index(_info.sender_index),
			receiver(_info.receiver ? *_info.receiver->getUserData<wraps::BaseLayout*>() : nullptr),
			receiver_index(_info.receiver_index)
		{
		}

		wraps::BaseLayout* sender;
		size_t sender_index;

		wraps::BaseLayout* receiver;
		size_t receiver_index;
	};

} // namespace wraps

#endif // __ITEM_DROP_INFO_H__
