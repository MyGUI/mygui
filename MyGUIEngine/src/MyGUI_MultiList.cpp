/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#include "MyGUI_MultiList.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_Button.h"
#include "MyGUI_List.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	MultiList::MultiList(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mHeightButton(0),
		mWidgetClient(null),
		mButtonMain(null),
		mWidthBar(0),
		mLastMouseFocusIndex(ITEM_NONE),
		mSortUp(true),
		mSortRowIndex(ITEM_NONE)
	{
		// парсим свойства
		const MapString & param = _info->getParams();
		if (false == param.empty()) {
			MapString::const_iterator iter = param.find("SkinButton");
			if (iter != param.end()) mSkinButton = iter->second;
			iter = param.find("HeightButton");
			if (iter != param.end()) mHeightButton = utility::parseInt(iter->second);
			if (mHeightButton < 0) mHeightButton = 0;

			iter = param.find("SkinList");
			if (iter != param.end()) mSkinList = iter->second;

			iter = param.find("SkinButtonEmpty");
			if (iter != param.end()) {
				mButtonMain = mWidgetClient->createWidget<Button>(iter->second,
					IntCoord(0, 0, mWidgetClient->getWidth(), mHeightButton), ALIGN_DEFAULT);
			}
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
			}
		}
		// мона и без клиента
		if (null == mWidgetClient) mWidgetClient = this;

	}

	void MultiList::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
	}

	void MultiList::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
	}

	//----------------------------------------------------------------------------------//
	// методы для работы со столбцами
	void MultiList::insertRow(size_t _index, int _width, const Ogre::DisplayString & _name)
	{
		// скрываем у крайнего скролл
		if (false == mVectorRowInfo.empty()) 
			mVectorRowInfo.back().list->needVisibleScroll(false);
		else mSortRowIndex = 0;

		RowInfo row;
		row.width = _width < 0 ? 0 : _width;

		row.list = mWidgetClient->createWidget<List>(mSkinList, IntCoord(), ALIGN_LEFT | ALIGN_VSTRETCH);
		row.list->eventListChangePosition = newDelegate(this, &MultiList::notifyListChangePosition);
		row.list->eventListMouseItemFocus = newDelegate(this, &MultiList::notifyListChangeFocus);
		row.list->eventListChangeScroll = newDelegate(this, &MultiList::notifyListChangeScrollPosition);

		row.button = mWidgetClient->createWidget<Button>(mSkinButton, IntCoord(), ALIGN_DEFAULT);
		row.button->eventMouseButtonClick = newDelegate(this, &MultiList::notifyButtonClick);
		row.name = _name;

		// если уже были столбики, то делаем то же колличество полей
		if (false == mVectorRowInfo.empty()) {
			size_t count = mVectorRowInfo.front().list->getItemCount();
			for (size_t pos=0; pos<count; ++pos)
				row.list->addItem("");
		}

		if (_index > mVectorRowInfo.size()) _index = mVectorRowInfo.size();
		mVectorRowInfo.insert(mVectorRowInfo.begin() + _index, row);

		updateRows();

		// показываем скролл нового крайнего
		mVectorRowInfo.back().list->needVisibleScroll(true);
	}

	void MultiList::setRowName(size_t _index, const Ogre::DisplayString & _name)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		mVectorRowInfo[_index].name = _name;
		redrawButtons();
	}

	void MultiList::setRowWidth(size_t _index, int _width)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		mVectorRowInfo[_index].width = _width < 0 ? 0 : _width;

		updateRows();
	}

	const Ogre::DisplayString & MultiList::getRowName(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		return mVectorRowInfo[_index].name;
	}

	int MultiList::getRowWidth(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");
		return mVectorRowInfo[_index].width;
	}

	void MultiList::deleteRow(size_t _index)
	{
		MYGUI_ASSERT(_index < mVectorRowInfo.size(), "index " << _index <<" out of range");

		RowInfo & info = mVectorRowInfo[_index];
		Gui & gui = Gui::getInstance();
		gui.destroyWidget(info.button);
		gui.destroyWidget(info.list);

		mVectorRowInfo.erase(mVectorRowInfo.begin() + _index);

		if (mVectorRowInfo.empty()) mSortRowIndex = ITEM_NONE;
		else {
			mSortRowIndex = 0;
			mSortUp = true;
			sortList();
		}

		updateRows();
	}

	void MultiList::deleteAllRows()
	{
		Gui & gui = Gui::getInstance();
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			gui.destroyWidget((*iter).button);
			gui.destroyWidget((*iter).list);
		}
		mVectorRowInfo.clear();
		mSortRowIndex = ITEM_NONE;

		updateRows();
	}

	//------------------------------------------------------------------------------//
	// методы для работы со строками
	size_t MultiList::getItemCount()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return mVectorRowInfo.front().list->getItemCount();
	}

	void MultiList::insertItem(size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		if (ITEM_NONE == _index) _index = mVectorRowInfo.front().list->getItemCount();

		// отсортированное место для индекса
		size_t index = getInsertIndex(mSortRowIndex, mSortUp, mSortRowIndex == 0 ? _item : "");

		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->insertItem(index, "");
		}
		mVectorRowInfo.front().list->setItem(index, _item);

		// все индексты что больше или равны вставляемому увеличиваем на 1
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] >= index) mToSortIndex[pos]++;
		}

		// записываем реальный индекс
		mToSortIndex.insert(mToSortIndex.begin() + _index, index);

	}

	void MultiList::setItem(size_t _index, const Ogre::DisplayString & _item)
	{
		setSubItem(0, _index, _item);
	}

	const Ogre::DisplayString & MultiList::getItem(size_t _index)
	{
		return getSubItem(0, _index);
	}

	void MultiList::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->deleteItem(_index);
		}

		size_t index = mToSortIndex[_index];
		// все индексты что меньше уменьшаем
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] < index) mToSortIndex[pos]--;
		}
		mToSortIndex.erase(mToSortIndex.begin() + _index);
	}

	void MultiList::deleteAllItems()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->deleteAllItems();
		}
		mToSortIndex.clear();
	}

	size_t MultiList::getItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return mVectorRowInfo.front().list->getItemSelect();
	}

	void MultiList::resetItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->resetItemSelect();
	}

	void MultiList::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			(*iter).list->setItemSelect(_index);
	}

	//----------------------------------------------------------------------------------//
	// методы для работы с саб строками
	void MultiList::setSubItem(size_t _row, size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		mVectorRowInfo[_row].list->setItem(_index, _item);
	}

	const Ogre::DisplayString & MultiList::getSubItem(size_t _row, size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		return mVectorRowInfo[_row].list->getItem(_index);
	}
	//----------------------------------------------------------------------------------//

	void MultiList::updateRows()
	{
		mWidthBar = 0;
		int index = 0;
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).button->setPosition(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).list->setPosition(mWidthBar, mHeightButton, (*iter).width, mWidgetClient->getHeight() - mHeightButton);

			(*iter).button->_setInternalData(index++);

			mWidthBar += (*iter).width;
		}

		redrawButtons();
		updateOnlyEmpty();
	}

	void MultiList::updateOnlyEmpty()
	{
		if (null == mButtonMain) return;
		// кнопка, для заполнения пустоты
		if (mWidthBar >= mWidgetClient->getWidth()) mButtonMain->hide();
		else {
			mButtonMain->setPosition(mWidthBar, 0, mWidgetClient->getWidth()-mWidthBar, mHeightButton);
			mButtonMain->show();
		}
	}

	void MultiList::notifyListChangePosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter)
			if (_widget != (*iter).list) (*iter).list->setItemSelect(_position);
	}

	void MultiList::notifyListChangeFocus(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			if (_widget != (*iter).list) {
				if (ITEM_NONE != mLastMouseFocusIndex) (*iter).list->_setItemFocus(mLastMouseFocusIndex, false);
				if (ITEM_NONE != _position) (*iter).list->_setItemFocus(_position, true);
			}
		}
		mLastMouseFocusIndex = _position;
	}

	void MultiList::notifyListChangeScrollPosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			if (_widget != (*iter).list)
				(*iter).list->setScrollPosition(_position);
		}
	}

	void MultiList::notifyButtonClick(MyGUI::WidgetPtr _widget)
	{
		if (_widget->_getInternalData() == mSortRowIndex) mSortUp = !mSortUp;
		else {
			mSortRowIndex = (size_t)_widget->_getInternalData();
			mSortUp = true;
		}
		redrawButtons();
		sortList();
	}

	void MultiList::redrawButtons()
	{
		size_t pos = 0;
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			if (pos == mSortRowIndex) {
				if (mSortUp) (*iter).button->setCaption((*iter).name + "  ^");
				else (*iter).button->setCaption((*iter).name + "  v");
			}
			else (*iter).button->setCaption((*iter).name);
			pos++;
		}
	}

	void MultiList::sortList()
	{
		if (ITEM_NONE == mSortRowIndex) return;
		//std::string str1("test1");
		//std::string str2("test2");
		//MYGUI_OUT(str2 < str1);
		//std::list::sort(
		//mVectorRowInfo.so
	}

	size_t MultiList::getInsertIndex(size_t _row, bool _up, const Ogre::DisplayString& _item)
	{
		size_t index = 0;
		ListPtr list = mVectorRowInfo[_row].list;
		size_t count = list->getItemCount();

		for (; index<count; ++index) {
			if (_up) {
				if (list->getItem(index) > _item) break;
			}
			else {
				if (list->getItem(index) < _item) break;
			}
		}
		return index;
	}

} // namespace MyGUI
