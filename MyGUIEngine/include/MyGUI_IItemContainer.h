/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_ITEM_CONTAINER_H__
#define __MYGUI_I_ITEM_CONTAINER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	class IItem;

	class MYGUI_EXPORT IItemContainer
	{
	public:
		IItemContainer() { }
		virtual ~IItemContainer() { }

		virtual size_t _getItemCount()
		{
			return 0;
		}

		virtual void _addItem(const MyGUI::UString& _name) { }

		virtual void _removeItemAt(size_t _index) { }

		virtual Widget* _getItemAt(size_t _index)
		{
			return nullptr;
		}

		virtual void _setItemNameAt(size_t _index, const UString& _name) { }
		virtual const UString& _getItemNameAt(size_t _index)
		{
			return Constants::getEmptyUString();
		}

		virtual void _setItemSelected(IItem* _item) { }
	};

} // namespace MyGUI

#endif // __MYGUI_I_ITEM_CONTAINER_H__
