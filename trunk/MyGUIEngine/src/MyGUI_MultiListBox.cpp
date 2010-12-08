/*!
	@file
	@author		Albert Semenov
	@date		04/2008
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
#include "MyGUI_MultiListBox.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_Button.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	MultiListBox::MultiListBox() :
		mHeightButton(0),
		mWidthBar(0),
		mButtonMain(nullptr),
		mLastMouseFocusIndex(ITEM_NONE),
		mSortUp(true),
		mSortColumnIndex(ITEM_NONE),
		mWidthSeparator(0),
		mOffsetButtonSeparator(2),
		mItemSelected(ITEM_NONE),
		mFrameAdvise(false),
		mClient(nullptr)
	{
	}

	void MultiListBox::initialiseOverride()
	{
		Base::initialiseOverride();

		std::string skinButtonEmpty;

		if (isUserString("SkinButton"))
			mSkinButton = getUserString("SkinButton");
		if (isUserString("SkinList"))
			mSkinList = getUserString("SkinList");
		if (isUserString("SkinButtonEmpty"))
			skinButtonEmpty = getUserString("SkinButtonEmpty");
		if (isUserString("SkinSeparator"))
			mSkinSeparator = getUserString("SkinSeparator");
		if (isUserString("HeightButton"))
			mHeightButton = utility::parseValue<int>(getUserString("HeightButton"));
		if (isUserString("WidthSeparator"))
			mWidthSeparator = utility::parseValue<int>(getUserString("WidthSeparator"));

		if (mHeightButton < 0)
			mHeightButton = 0;

		if (!skinButtonEmpty.empty())
			mButtonMain = mClient->createWidget<Button>(skinButtonEmpty,
				IntCoord(0, 0, mClient->getWidth(), mHeightButton), Align::Default);

		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			setWidgetClient(mClient);
		}

		if (nullptr == mClient)
			mClient = this;
	}

	void MultiListBox::shutdownOverride()
	{
		mClient = nullptr;

		Base::shutdownOverride();
	}

	//----------------------------------------------------------------------------------//
	// методы для работы со столбцами
	void MultiListBox::insertColumnAt(size_t _column, const UString& _name, int _width, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_column, mVectorColumnInfo.size(), "MultiListBox::insertColumnAt");
		if (_column == ITEM_NONE) _column = mVectorColumnInfo.size();

		// скрываем у крайнего скролл
		if (!mVectorColumnInfo.empty())
			mVectorColumnInfo.back().list->setScrollVisible(false);
		else mSortColumnIndex = ITEM_NONE;

		ColumnInfo column;
		column.width = _width < 0 ? 0 : _width;

		column.list = mClient->createWidget<ListBox>(mSkinList, IntCoord(), Align::Left | Align::VStretch);
		column.list->eventListChangePosition += newDelegate(this, &MultiListBox::notifyListChangePosition);
		column.list->eventListMouseItemFocus += newDelegate(this, &MultiListBox::notifyListChangeFocus);
		column.list->eventListChangeScroll += newDelegate(this, &MultiListBox::notifyListChangeScrollPosition);
		column.list->eventListSelectAccept += newDelegate(this, &MultiListBox::notifyListSelectAccept);

		column.button = mClient->createWidget<Button>(mSkinButton, IntCoord(), Align::Default);
		column.button->eventMouseButtonClick += newDelegate(this, &MultiListBox::notifyButtonClick);
		column.name = _name;
		column.data = _data;

		// если уже были столбики, то делаем то же колличество полей
		if (!mVectorColumnInfo.empty())
		{
			size_t count = mVectorColumnInfo.front().list->getItemCount();
			for (size_t pos = 0; pos < count; ++pos)
				column.list->addItem("");
		}

		mVectorColumnInfo.insert(mVectorColumnInfo.begin() + _column, column);

		updateColumns();

		// показываем скролл нового крайнего
		mVectorColumnInfo.back().list->setScrollVisible(true);
	}

	void MultiListBox::setColumnNameAt(size_t _column, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setColumnNameAt");
		mVectorColumnInfo[_column].name = _name;
		redrawButtons();
	}

	void MultiListBox::setColumnWidthAt(size_t _column, int _width)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setColumnWidthAt");
		mVectorColumnInfo[_column].width = _width < 0 ? 0 : _width;
		updateColumns();
	}

	const UString& MultiListBox::getColumnNameAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getColumnNameAt");
		return mVectorColumnInfo[_column].name;
	}

	int MultiListBox::getColumnWidthAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getColumnWidthAt");
		return mVectorColumnInfo[_column].width;
	}

	void MultiListBox::removeColumnAt(size_t _column)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::removeColumnAt");

		ColumnInfo& info = mVectorColumnInfo[_column];

		WidgetManager& manager = WidgetManager::getInstance();
		manager.destroyWidget(info.button);
		manager.destroyWidget(info.list);

		mVectorColumnInfo.erase(mVectorColumnInfo.begin() + _column);

		if (mVectorColumnInfo.empty())
		{
			mSortColumnIndex = ITEM_NONE;
			mItemSelected = ITEM_NONE;
		}
		else
		{
			mSortColumnIndex = ITEM_NONE;
			mSortUp = true;
			sortList();
		}

		updateColumns();
	}

	void MultiListBox::removeAllColumns()
	{
		WidgetManager& manager = WidgetManager::getInstance();
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			manager.destroyWidget((*iter).button);
			manager.destroyWidget((*iter).list);
		}
		mVectorColumnInfo.clear();
		mSortColumnIndex = ITEM_NONE;

		updateColumns();

		mItemSelected = ITEM_NONE;
	}

	void MultiListBox::sortByColumn(size_t _column, bool _backward)
	{
		mSortColumnIndex = _column;
		if (_backward)
		{
			mSortUp = !mSortUp;
			redrawButtons();
			// если было недосортированно то сортируем
			if (mFrameAdvise) sortList();

			flipList();
		}
		else
		{
			mSortUp = true;
			redrawButtons();
			sortList();
		}
	}

	size_t MultiListBox::getItemCount() const
	{
		if (mVectorColumnInfo.empty()) return 0;
		return mVectorColumnInfo.front().list->getItemCount();
	}

	void MultiListBox::removeAllItems()
	{
		BiIndexBase::removeAllItems();
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			(*iter).list->removeAllItems();
		}

		mItemSelected = ITEM_NONE;
	}

	/*size_t MultiListBox::getItemIndexSelected()
	{
		if (mVectorColumnInfo.empty()) return ITEM_NONE;
		size_t item = mVectorColumnInfo.front().list->getItemIndexSelected();
		return (ITEM_NONE == item) ? ITEM_NONE : BiIndexBase::convertToFace(item);
	}*/

	void MultiListBox::updateBackSelected(size_t _index)
	{
		if (_index == ITEM_NONE)
		{
			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->clearIndexSelected();
			}
		}
		else
		{
			//size_t index = BiIndexBase::convertToBack(_index);
			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->setIndexSelected(_index);
			}
		}
	}

	void MultiListBox::setIndexSelected(size_t _index)
	{
		if (_index == mItemSelected) return;

		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiListBox::setIndexSelected");
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::setIndexSelected");

		mItemSelected = _index;
		updateBackSelected(BiIndexBase::convertToBack(mItemSelected));
	}

	void MultiListBox::setSubItemNameAt(size_t _column, size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setSubItemAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::setSubItemAt");

		size_t index = BiIndexBase::convertToBack(_index);
		mVectorColumnInfo[_column].list->setItemNameAt(index, _name);

		// если мы попортили список с активным сортом, надо пересчитывать
		if (_column == mSortColumnIndex) frameAdvise(true);
	}

	const UString& MultiListBox::getSubItemNameAt(size_t _column, size_t _index)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::getSubItemNameAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::getSubItemNameAt");

		size_t index = BiIndexBase::convertToBack(_index);
		return mVectorColumnInfo[_column].list->getItemNameAt(index);
	}

	size_t MultiListBox::findSubItemWith(size_t _column, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::findSubItemWith");

		size_t index = mVectorColumnInfo[_column].list->findItemIndexWith(_name);
		return BiIndexBase::convertToFace(index);
	}
	//----------------------------------------------------------------------------------//

	void MultiListBox::updateOnlyEmpty()
	{
		if (nullptr == mButtonMain) return;
		// кнопка, для заполнения пустоты
		if (mWidthBar >= mClient->getWidth()) mButtonMain->setVisible(false);
		else
		{
			mButtonMain->setCoord(mWidthBar, 0, mClient->getWidth() - mWidthBar, mHeightButton);
			mButtonMain->setVisible(true);
		}
	}

	void MultiListBox::notifyListChangePosition(ListBox* _sender, size_t _position)
	{
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			if (_sender != (*iter).list) (*iter).list->setIndexSelected(_position);
		}

		updateBackSelected(_position);

		mItemSelected = BiIndexBase::convertToFace(_position);

		// наш евент
		eventListChangePosition(this, mItemSelected);
	}

	void MultiListBox::notifyListSelectAccept(ListBox* _sender, size_t _position)
	{
		// наш евент
		eventListSelectAccept(this, BiIndexBase::convertToFace(_position));
	}

	void MultiListBox::notifyListChangeFocus(ListBox* _sender, size_t _position)
	{
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			if (_sender != (*iter).list)
			{
				if (ITEM_NONE != mLastMouseFocusIndex) (*iter).list->_setItemFocus(mLastMouseFocusIndex, false);
				if (ITEM_NONE != _position) (*iter).list->_setItemFocus(_position, true);
			}
		}
		mLastMouseFocusIndex = _position;
	}

	void MultiListBox::notifyListChangeScrollPosition(ListBox* _sender, size_t _position)
	{
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			if (_sender != (*iter).list)
				(*iter).list->setScrollPosition(_position);
		}
	}

	void MultiListBox::notifyButtonClick(MyGUI::Widget* _sender)
	{
		size_t index = *_sender->_getInternalData<size_t>();
		sortByColumn(index, index == mSortColumnIndex);
	}

	void MultiListBox::redrawButtons()
	{
		size_t pos = 0;
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			if (pos == mSortColumnIndex)
			{
				if (mSortUp) setButtonImageIndex((*iter).button, SORT_UP);
				else setButtonImageIndex((*iter).button, SORT_DOWN);
			}
			else setButtonImageIndex((*iter).button, SORT_NONE);
			(*iter).button->setCaption((*iter).name);
			pos++;
		}
	}

	void MultiListBox::setButtonImageIndex(Button* _button, size_t _index)
	{
		ImageBox* image = _button->getImageBox();
		if ( nullptr == image ) return;
		if (image->getItemResource())
		{
			static const size_t CountIcons = 3;
			static const char* IconNames[CountIcons + 1] = { "None", "Up", "Down", "" };
			if (_index >= CountIcons) _index = CountIcons;
			image->setItemName(IconNames[_index]);
		}
		else
		{
			image->setItemSelect(_index);
		}
	}

	void MultiListBox::frameEntered(float _frame)
	{
		sortList();
	}

	void MultiListBox::frameAdvise(bool _advise)
	{
		if ( _advise )
		{
			if ( ! mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate( this, &MultiListBox::frameEntered );
				mFrameAdvise = true;
			}
		}
		else
		{
			if ( mFrameAdvise )
			{
				MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate( this, &MultiListBox::frameEntered );
				mFrameAdvise = false;
			}
		}
	}

	Widget* MultiListBox::getSeparator(size_t _index)
	{
		if (!mWidthSeparator || mSkinSeparator.empty()) return nullptr;
		// последний столбик
		if (_index == mVectorColumnInfo.size() - 1) return nullptr;

		while (_index >= mSeparators.size())
		{
			Widget* separator = mClient->createWidget<Widget>(mSkinSeparator, IntCoord(), Align::Default);
			mSeparators.push_back(separator);
		}

		return mSeparators[_index];
	}

	void MultiListBox::updateColumns()
	{
		mWidthBar = 0;
		size_t index = 0;
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			(*iter).list->setCoord(mWidthBar, mHeightButton, (*iter).width, mClient->getHeight() - mHeightButton);
			(*iter).button->setCoord(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).button->_setInternalData(index);

			mWidthBar += (*iter).width;

			// промежуток между листами
			Widget* separator = getSeparator(index);
			if (separator)
			{
				separator->setCoord(mWidthBar, 0, mWidthSeparator, mClient->getHeight());
			}

			mWidthBar += mWidthSeparator;
			index++;
		}

		redrawButtons();
		updateOnlyEmpty();
	}

	void MultiListBox::flipList()
	{
		if (ITEM_NONE == mSortColumnIndex) return;

		size_t last = mVectorColumnInfo.front().list->getItemCount();
		if (0 == last) return;
		last --;
		size_t first = 0;

		while (first < last)
		{
			BiIndexBase::swapItemsBackAt(first, last);
			for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
			{
				(*iter).list->swapItemsAt(first, last);
			}

			first++;
			last--;
		}

		updateBackSelected(BiIndexBase::convertToBack(mItemSelected));
	}

	bool MultiListBox::compare(ListBox* _list, size_t _left, size_t _right)
	{
		bool result = false;
		if (mSortUp) std::swap(_left, _right);
		if (requestOperatorLess.empty()) result = _list->getItemNameAt(_left) < _list->getItemNameAt(_right);
		else requestOperatorLess(this, mSortColumnIndex, _list->getItemNameAt(_left), _list->getItemNameAt(_right), result);
		return result;
	}

	void MultiListBox::sortList()
	{
		if (ITEM_NONE == mSortColumnIndex) return;

		ListBox* list = mVectorColumnInfo[mSortColumnIndex].list;

		size_t count = list->getItemCount();
		if (0 == count) return;

		// shell sort
		int first;
		size_t last;
		for (size_t step = count >> 1; step > 0 ; step >>= 1)
		{
			for (size_t i = 0; i < (count - step); i++)
			{
				first = (int)i;
				while (first >= 0)
				{
					last = first + step;
					if (compare(list, first, last))
					{
						BiIndexBase::swapItemsBackAt(first, last);
						for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
						{
							(*iter).list->swapItemsAt(first, last);
						}
					}
					first--;
				}
			}
		}

		frameAdvise(false);

		updateBackSelected(BiIndexBase::convertToBack(mItemSelected));
	}

	void MultiListBox::insertItemAt(size_t _index, const UString& _name, Any _data)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiListBox::insertItemAt");
		MYGUI_ASSERT_RANGE_INSERT(_index, mVectorColumnInfo.front().list->getItemCount(), "MultiListBox::insertItemAt");
		if (ITEM_NONE == _index) _index = mVectorColumnInfo.front().list->getItemCount();

		// если надо, то меняем выделенный элемент
		// при сортировке, обновится
		if ((mItemSelected != ITEM_NONE) && (_index <= mItemSelected)) mItemSelected ++;

		size_t index = BiIndexBase::insertItemAt(_index);

		// вставляем во все поля пустые, а потом присваиваем первому
		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			(*iter).list->insertItemAt(index, "");
		}
		mVectorColumnInfo.front().list->setItemNameAt(index, _name);
		mVectorColumnInfo.front().list->setItemDataAt(index, _data);

		frameAdvise(true);
	}

	void MultiListBox::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiListBox::removeItemAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::removeItemAt");

		size_t index = BiIndexBase::removeItemAt(_index);

		for (VectorColumnInfo::iterator iter = mVectorColumnInfo.begin(); iter != mVectorColumnInfo.end(); ++iter)
		{
			(*iter).list->removeItemAt(index);
		}

		// если надо, то меняем выделенный элемент
		size_t count = mVectorColumnInfo.begin()->list->getItemCount();
		if (count == 0) mItemSelected = ITEM_NONE;
		else if (mItemSelected != ITEM_NONE)
		{
			if (_index < mItemSelected) mItemSelected --;
			else if ((_index == mItemSelected) && (mItemSelected == count)) mItemSelected --;
		}
		updateBackSelected(BiIndexBase::convertToBack(mItemSelected));
	}

	void MultiListBox::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(0, mVectorColumnInfo.size(), "MultiListBox::removeItemAt");
		MYGUI_ASSERT_RANGE(_index1, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::swapItemsAt");

		// при сортированном, меняем только индексы
		BiIndexBase::swapItemsFaceAt(_index1, _index2);

		// при несортированном, нужно наоборот, поменять только данные
		// FIXME
	}

	void MultiListBox::setColumnDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.size(), "MultiListBox::setColumnDataAt");
		mVectorColumnInfo[_index].data = _data;
	}

	void MultiListBox::setSubItemDataAt(size_t _column, size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_column, mVectorColumnInfo.size(), "MultiListBox::setSubItemDataAt");
		MYGUI_ASSERT_RANGE(_index, mVectorColumnInfo.begin()->list->getItemCount(), "MultiListBox::setSubItemDataAt");

		size_t index = BiIndexBase::convertToBack(_index);
		mVectorColumnInfo[_column].list->setItemDataAt(index, _data);
	}

	size_t MultiListBox::getColumnCount() const
	{
		return mVectorColumnInfo.size();
	}

	void MultiListBox::addColumn(const UString& _name, int _width, Any _data)
	{
		insertColumnAt(ITEM_NONE, _name, _width, _data);
	}

	void MultiListBox::clearColumnDataAt(size_t _index)
	{
		setColumnDataAt(_index, Any::Null);
	}

	void MultiListBox::addItem(const UString& _name, Any _data)
	{
		insertItemAt(ITEM_NONE, _name, _data);
	}

	void MultiListBox::setItemNameAt(size_t _index, const UString& _name)
	{
		setSubItemNameAt(0, _index, _name);
	}

	const UString& MultiListBox::getItemNameAt(size_t _index)
	{
		return getSubItemNameAt(0, _index);
	}

	size_t MultiListBox::getIndexSelected() const
	{
		return mItemSelected;
	}

	void MultiListBox::clearIndexSelected()
	{
		setIndexSelected(ITEM_NONE);
	}

	void MultiListBox::setItemDataAt(size_t _index, Any _data)
	{
		setSubItemDataAt(0, _index, _data);
	}

	void MultiListBox::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	void MultiListBox::clearSubItemDataAt(size_t _column, size_t _index)
	{
		setSubItemDataAt(_column, _index, Any::Null);
	}

} // namespace MyGUI
