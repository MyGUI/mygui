/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef MIRROR_LIST_H_
#define MIRROR_LIST_H_

#include "MyGUI.h"

namespace unittest
{

	class Mirror_List
	{

	public:
		//------------------------------------------------------------------------------//
		// манипул€ции айтемами

		//! Get number of items
		size_t getItemCount() const
		{
			return mItemsInfo.size();
		}

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, const MyGUI::UString& _name, MyGUI::Any _data = MyGUI::Any::Null)
		{
			MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ListBox::insertItemAt");
			if (_index == MyGUI::ITEM_NONE) _index = mItemsInfo.size();
			mItemsInfo.insert(mItemsInfo.begin() + _index, ItemInfo(_name, _data));
		}

		//! Add an item to the end of a array
		void addItem(const MyGUI::UString& _name, MyGUI::Any _data = MyGUI::Any::Null)
		{
			insertItemAt(MyGUI::ITEM_NONE, _name, _data);
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::removeItemAt");
			mItemsInfo.erase(mItemsInfo.begin() + _index);
		}

		//! Remove all items
		void removeAllItems()
		{
			mItemsInfo.clear();
		}

		//! Swap items at a specified position
		void swapItemsAt(size_t _index1, size_t _index2)
		{
			MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "ListBox::swapItemsAt");
			MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "ListBox::swapItemsAt");
			if (_index1 == _index2) return;
			std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);
		}

		//! Search item, returns the position of the first occurrence in array or ITEM_NONE if item not found
		size_t findItemIndexWith(const MyGUI::UString& _name)
		{
			for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
			{
				if (mItemsInfo[pos].name == _name) return pos;
			}
			return MyGUI::ITEM_NONE;
		}


		//------------------------------------------------------------------------------//
		// манипул€ции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, MyGUI::Any _data)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::setItemDataAt");
			mItemsInfo[_index].data = _data;
		}

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index)
		{
			setItemDataAt(_index, MyGUI::Any::Null);
		}

		//! Get item data from specified position
		template <typename ValueType>
		ValueType* getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		//------------------------------------------------------------------------------//
		// манипул€ции отображением

		//! Replace an item name at a specified position
		void setItemNameAt(size_t _index, const MyGUI::UString& _name)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::setItemNameAt");
			mItemsInfo[_index].name = _name;
		}

		//! Get item name from specified position
		const MyGUI::UString& getItemNameAt(size_t _index)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::getItemNameAt");
			return mItemsInfo[_index].name;
		}

	private:

		struct ItemInfo
		{
			ItemInfo(const MyGUI::UString& _name, MyGUI::Any& _data) :
				name(_name),
				data(_data)
			{
			}

			MyGUI::UString name;
			MyGUI::Any data;
		};

		typedef std::vector<ItemInfo> VectorItemInfo;
		VectorItemInfo mItemsInfo;

	};

} // namespace unittest

#endif // MIRROR_LIST_H_
