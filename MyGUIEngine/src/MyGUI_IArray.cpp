/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "MyGUI_Prerequest.h"
#include "MyGUI_Any.h"

namespace MyGUI
{

	// этот класс не дл€ использовани€, а дл€ соблюдени€ некоего общего интерфейса дл€ всех классов контейнеров


	class IArray
	{

	public:

		//------------------------------------------------------------------------------//
		// обобщеный интерфейс дл€ работы с элементами

		// типы дл€ массива
		typedef void * ItemType;
		typedef const std::string & NameType;

		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() { return 0; }

		//! Insert an item into a array at a specified position
		ItemType insertItemAt(size_t _index, NameType _name, Any _data = Any::Null) { return null; }
		//! Insert an item into a array
		ItemType insertItem(ItemType _to, NameType _name, Any _data = Any::Null) { return insertItemAt(getItemIndex(_to), _name, _data); }

		//! Replace an item at a specified position
		ItemType replaceItemAt(size_t _index, NameType _name, Any _data = Any::Null) { return null; }
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



		//! Get item from specified position
		ItemType getItemAt(size_t _index) { return null; }

		//! Get item index
		size_t getItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndex(ItemType _item) { return ITEM_NONE; }

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(NameType _name) { return ITEM_NONE; }

		//! Search item, returns the item of the first occurrence in array or null if item not found
		ItemType findItemWith(NameType _name) { return null; }


		//------------------------------------------------------------------------------//
		// манипул€ции выделени€ми

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getItemIndexSelected() { return ITEM_NONE; }

		//! Get selected item (null if none selected)
		ItemType getItemSelected() { return getItemIndexSelected() != ITEM_NONE ? getItemAt(getItemIndexSelected()) : null; }

		//! Select specified _index
		void setItemSelectedAt(size_t _index) { }

		//! Select item
		void setItemSelected(ItemType _item) { setItemSelectedAt(getItemIndex(_item)); }

		//! Clear item selection
		void clearItemSelected() { setItemSelectedAt(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипул€ции данными

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
		ValueType * getItemDataAt(size_t _index, bool _throw = true) { return null; }

		//! Get item data
		template <typename ValueType>
		ValueType * getItemData(ItemType _item, bool _throw = true)
		{
			return getItemData<ValueType>(getItemIndex(_item), _throw);
		}


		//------------------------------------------------------------------------------//
		// манипул€ции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, NameType _name);

		//! Replace an item name
		void setItemName(ItemType _item, NameType _name) { setItemNameAt(getItemIndex(_item), _name); }

		//! Get item name from specified position
		NameType getItemNameAt(size_t _index) { return ""; }

		//! Get item name
		NameType getItemName(ItemType _item) { return getItemNameAt(getItemIndex(_item)); }


		//------------------------------------------------------------------------------//
		// манипул€ции выдимостью

		//! Move all elements so specified becomes visible
		void beginToItemAt(size_t _index) { }

		//! Move all elements so specified becomes visible
		void beginToItem(ItemType _item) { beginToItemAt(getItemIndex(_item)); }

		//! Move all elements so first becomes visible
		void beginToItemFirst() { if (getItemCount()) beginToItemAt(0); }

		//! Move all elements so last becomes visible
		void beginToItemLast() { if (getItemCount()) beginToItemAt(getItemCount() - 1); }

		//! Move all elements so selected becomes visible
		void beginToItemSelected() { if (getItemIndexSelected() != ITEM_NONE) beginToItemAt(getItemIndexSelected()); }

		//------------------------------------------------------------------------------//

	};

} // namespace MyGUI
