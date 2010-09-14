/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#include "Precompiled.h"
#include "BiIndexData.h"

namespace unittest
{

	void BiIndexData::insertItemAt(size_t _index, const std::string& _name, size_t _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "BiIndexData::insertItemAt");
		if (_index == MyGUI::ITEM_NONE) _index = mItemsInfo.size();

		size_t index = BiIndexBase::insertItemAt(_index);
		mItemsInfo.insert(mItemsInfo.begin() + index, _name);
	}

	void BiIndexData::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "BiIndexData::removeItemAt");

		size_t index = BiIndexBase::removeItemAt(_index);
		mItemsInfo.erase(mItemsInfo.begin() + index);
	}

	void BiIndexData::removeAllItems()
	{
		BiIndexBase::removeAllItems();
		mItemsInfo.clear();
	}

	const std::string& BiIndexData::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "BiIndexData::getItemNameAt");
		size_t index = BiIndexBase::convertToBack(_index);
		return mItemsInfo[index];
	}

	void BiIndexData::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "BiIndexData::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "BiIndexData::swapItemsAt");

		BiIndexBase::swapItemsFaceAt(_index1, _index2);
	}

	void BiIndexData::swapSortItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "BiIndexData::swapSortItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "BiIndexData::swapSortItemsAt");

		BiIndexBase::swapItemsBackAt(_index1, _index2);
		std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);
	}

} // namespace unittest
