/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/
/*
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
#include "MyGUI_ListBox.h"
#include "MyGUI_Button.h"

namespace MyGUI
{

	ListBox::ListBox() :
		mHeightLine(0)
	{
		requestCreateWidgetItem = MyGUI::newDelegate(this, &ListBox::notifyCreateWidgetItem);
		requestDrawItem = MyGUI::newDelegate(this, &ListBox::notifyDrawItem);
	}

	void ListBox::initialiseOverride()
	{
		Base::initialiseOverride();

		mHeightLine = 20;
		mChangeContentByResize = false;

		if (isUserString("SkinLine"))
			mSkinLine = getUserString("SkinLine");
		if (isUserString("HeightLine"))
			mHeightLine = utility::parseValue<int>(getUserString("HeightLine"));

		_setScrollViewPage(mHeightLine);
	}

	void ListBox::shutdownOverride()
	{
		Base::shutdownOverride();
	}

	void ListBox::notifyCreateWidgetItem(MyGUI::ListCtrl* _sender, MyGUI::Widget* _item)
	{
		const MyGUI::IntSize& size = _item->getSize();

		MyGUI::Button* text = _item->createWidget<MyGUI::Button>(mSkinLine, MyGUI::IntCoord(0, 0, size.width, size.height), MyGUI::Align::Stretch);

		text->setNeedMouseFocus(false);

		_item->setUserData(text);
	}

	void ListBox::notifyDrawItem(MyGUI::ListCtrl* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info, MyGUI::IntCoord& _coord)
	{
		MyGUI::Button* text = *_item->getUserData<MyGUI::Button*>();

		if (_info.update)
		{
			text->setCaption(mItemsInfo[_info.index]);

			MyGUI::IntSize size = text->getTextSize() + (text->getSize() - text->getTextRegion().size());
			size.height = mHeightLine;
			_coord.set(0, 0, size.width, size.height);
		}

		text->setStateSelected(_info.select);
		text->_setMouseFocus(_info.active);
	}

	void ListBox::insertItemAt(size_t _index, const UString& _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ListBox::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		mItemsInfo.insert(mItemsInfo.begin() + _index, _name);

		Base::insertItemAt(_index, _data);
	}

	void ListBox::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::removeItemAt");
		mItemsInfo.erase(mItemsInfo.begin() + _index);

		Base::removeItemAt(_index);
	}

	void ListBox::removeAllItems()
	{
		mItemsInfo.clear();

		Base::removeAllItems();
	}

	void ListBox::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "ListBox::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "ListBox::swapItemsAt");

		if (_index1 == _index2) return;

		std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);

		Base::redrawItemAt(_index1);
		Base::redrawItemAt(_index2);
	}

	size_t ListBox::findItemIndexWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos] == _name) return pos;
		}
		return ITEM_NONE;
	}

	void ListBox::setItemNameAt(size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::setItemNameAt");

		Base::redrawItemAt(_index);
	}

	const UString& ListBox::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::getItemNameAt");

		return mItemsInfo[_index];
	}

	void ListBox::beginToItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::beginToItemAt");

		//FIXME
	}

	size_t ListBox::getItemCount() const
	{
		return mItemsInfo.size();
	}

	void ListBox::addItem(const UString& _name, Any _data)
	{
		insertItemAt(ITEM_NONE, _name, _data);
	}

	size_t ListBox::getIndexSelected() const
	{
		return Base::getIndexSelected();
	}

	void ListBox::setIndexSelected(size_t _index)
	{
		Base::setIndexSelected(_index);
	}

	void ListBox::clearIndexSelected()
	{
		Base::clearIndexSelected();
	}

	void ListBox::setItemDataAt(size_t _index, Any _data)
	{
		Base::setItemDataAt(_index, _data);
	}

	//! Clear an item data at a specified position
	void ListBox::clearItemDataAt(size_t _index)
	{
		Base::clearItemDataAt(_index);
	}

	void ListBox::beginToItemFirst()
	{
		if (getItemCount())
			beginToItemAt(0);
	}

	void ListBox::beginToItemLast()
	{
		if (getItemCount())
			beginToItemAt(getItemCount() - 1);
	}

	void ListBox::beginToItemSelected()
	{
		if (getIndexSelected() != ITEM_NONE)
			beginToItemAt(getIndexSelected());
	}

} // namespace MyGUI
