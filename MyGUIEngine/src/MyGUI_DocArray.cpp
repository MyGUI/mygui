/*!
	@file
	@author		Albert Semenov
	@date		10/2008
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

#include "MyGUI_Precompiled.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_Any.h"

namespace MyGUI
{
	namespace demonstrate
	{

		// этот класс не для использования, а для соблюдения некоего общего интерфейса для всех классов контейнеров
		/** This class not for use. It's for observance of a certain general interface for all classes of containers */

		class IArray
		{

		public:

			//------------------------------------------------------------------------------//
			// обобщеный интерфейс для работы с элементами

			// типы для массива
			typedef void * ItemType;
			typedef const std::string NameType;

			//------------------------------------------------------------------------------//
			// манипуляции айтемами

			//! Get number of items
			size_t getItemCount() { return 0; }

			//! Insert an item into a array at a specified position
			ItemType insertItemAt(size_t _index, NameType _name, Any _data = Any::Null) { return nullptr; }
			//! Insert an item into a array
			ItemType insertItem(ItemType _to, NameType _name, Any _data = Any::Null) { return insertItemAt(getItemIndex(_to), _name, _data); }

			//! Replace an item at a specified position
			ItemType replaceItemAt(size_t _index, NameType _name, Any _data = Any::Null) { return nullptr; }
			//! Replace an item
			ItemType replaceItem(ItemType _replace, NameType _name, Any _data = Any::Null) { return replaceItemAt(getItemIndex(_replace), _name, _data); }

			//! Add an item to the end of a array
			ItemType addItem(NameType _name, Any _data = Any::Null) { return insertItemAt(ITEM_NONE, _name, _data); }

			//! Remove item at a specified position
			void removeItemAt(size_t _index) { }

			//! Remove item
			void removeItem(ItemType _item) { removeItemAt(getItemIndex(_item)); }

			//! Remove all items
			void removeAllItems() { }

			//! Swap items at a specified positions
			void swapItemsAt(size_t _index1, size_t _index2) { }

			//! Swap items
			void swapItems(ItemType _item1, ItemType _item2) { }



			//! Get item from specified position
			ItemType getItemAt(size_t _index) { return nullptr; }

			//! Get item index
			size_t getItemIndex(ItemType _item) { return ITEM_NONE; }

			//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
			size_t findItemIndex(ItemType _item) { return ITEM_NONE; }

			//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
			size_t findItemIndexWith(NameType _name) { return ITEM_NONE; }

			//! Search item, returns the item of the first occurrence in array or nullptr if item not found
			ItemType findItemWith(NameType _name) { return nullptr; }


			//------------------------------------------------------------------------------//
			// манипуляции выделениями

			//! Get index of selected item (ITEM_NONE if none selected)
			size_t getIndexSelected() { return ITEM_NONE; }

			//! Get selected item (nullptr if none selected)
			ItemType getItemSelected() { return getIndexSelected() != ITEM_NONE ? getItemAt(getIndexSelected()) : nullptr; }

			//! Select specified _index
			void setIndexSelected(size_t _index) { }

			//! Select item
			void setItemSelected(ItemType _item) { setIndexSelected(getItemIndex(_item)); }

			//! Clear item selection
			void clearIndexSelected() { setIndexSelected(ITEM_NONE); }


			//------------------------------------------------------------------------------//
			// манипуляции данными

			//! Replace an item data at a specified position
			void setItemDataAt(size_t _index, Any _data) { }

			//! Replace an item data
			void setItemData(ItemType _item, Any _data) { setItemDataAt(getItemIndex(_item), _data); }

			//! Clear an item data at a specified position
			void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

			//! Clear an item data
			void clearItemData(ItemType _item) { clearItemDataAt(getItemIndex(_item)); }

			//! Get item data from specified position
			template <typename ValueType>
			ValueType * getItemDataAt(size_t _index, bool _throw = true) { return nullptr; }

			//! Get item data
			template <typename ValueType>
			ValueType * getItemData(ItemType _item, bool _throw = true)
			{
				return getItemData<ValueType>(getItemIndex(_item), _throw);
			}


			//------------------------------------------------------------------------------//
			// манипуляции отображением

			//! Replace an item name at a specified position
			void setItemNameAt(size_t _index, NameType _name);

			//! Replace an item name
			void setItemName(ItemType _item, NameType _name) { setItemNameAt(getItemIndex(_item), _name); }

			//! Get item name from specified position
			NameType getItemNameAt(size_t _index) { return NameType(); }

			//! Get item name
			NameType getItemName(ItemType _item) { return getItemNameAt(getItemIndex(_item)); }


			//------------------------------------------------------------------------------//
			// манипуляции выдимостью

			//! Move all elements so specified becomes visible
			void beginToItemAt(size_t _index) { }

			//! Move all elements so specified becomes visible
			void beginToItem(ItemType _item) { beginToItemAt(getItemIndex(_item)); }

			//! Move all elements so first becomes visible
			void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

			//! Move all elements so last becomes visible
			void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

			//! Move all elements so selected becomes visible
			void beginToItemSelected() { if (getIndexSelected() != ITEM_NONE) beginToItemAt(getIndexSelected()); }

			//------------------------------------------------------------------------------//

		};

	} // namespace demonstrate
} // namespace MyGUI
