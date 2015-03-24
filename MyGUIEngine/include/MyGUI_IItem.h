/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_ITEM_H_
#define MYGUI_I_ITEM_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class IItemContainer;

	class MYGUI_EXPORT IItem
	{
	public:
		IItem() { }
		virtual ~IItem() { }

		virtual IItemContainer* _getItemContainer()
		{
			return nullptr;
		}
	};

} // namespace MyGUI

#endif // MYGUI_I_ITEM_H_
