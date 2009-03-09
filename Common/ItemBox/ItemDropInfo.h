/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __ITEM_DROP_INFO_H__
#define __ITEM_DROP_INFO_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	struct DDItemInfo
	{

		DDItemInfo(const MyGUI::DDItemInfo & _info) :
			sender(*_info.sender->getUserData<wraps::BaseLayout*>()),
			sender_index(_info.sender_index),
			receiver(_info.receiver ? *_info.receiver->getUserData<wraps::BaseLayout*>() : nullptr),
			receiver_index(_info.receiver_index)
		{
		}

		wraps::BaseLayout * sender;
		size_t sender_index;

		wraps::BaseLayout * receiver;
		size_t receiver_index;
	};

} // namespace wraps

#endif // __ITEM_DROP_INFO_H__
