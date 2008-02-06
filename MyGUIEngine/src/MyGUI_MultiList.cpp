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
		mSortRowIndex(ITEM_NONE),
		mIsDirtySort(false)
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

	MultiList::~MultiList()
	{
		Gui::getInstance().removeFrameListener(this);
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

		// вставляем во все поля пустые, а потом присваиваем первому
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->insertItem(_index, "");
		}
		mVectorRowInfo.front().list->setItem(_index, _item);

		_insertSortIndex(_index);
		setDirtySort();
		//sortList();
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
		_eraseSortIndex(_index);
	}

	void MultiList::deleteAllItems()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->deleteAllItems();
		}
		_clearSortIndex();
	}

	size_t MultiList::getItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		return convertFromSort(mVectorRowInfo.front().list->getItemSelect());
	}

	void MultiList::resetItemSelect()
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->resetItemSelect();
		}
	}

	void MultiList::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		size_t index = convertFromSort(_index);
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->setItemSelect(index);
		}
	}

	//----------------------------------------------------------------------------------//
	// методы для работы с саб строками
	void MultiList::setSubItem(size_t _row, size_t _index, const Ogre::DisplayString & _item)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		mVectorRowInfo[_row].list->setItem(convertFromSort(_index), _item);

		// если мы попортили список с активным сортом, надо пересчитывать
		if (_row == mSortRowIndex) setDirtySort();
			//sortList();
	}

	const Ogre::DisplayString & MultiList::getSubItem(size_t _row, size_t _index)
	{
		MYGUI_ASSERT(false == mVectorRowInfo.empty(), "row not found");
		MYGUI_ASSERT(_index < mVectorRowInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		return mVectorRowInfo[_row].list->getItem(convertToSort(_index));
	}
	//----------------------------------------------------------------------------------//

	void MultiList::updateRows()
	{
		mWidthBar = 0;
		int index = 0;
		for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
			(*iter).list->setPosition(mWidthBar, mHeightButton, (*iter).width, mWidgetClient->getHeight() - mHeightButton);
			(*iter).button->setPosition(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).button->_setInternalData(index);

			mWidthBar += (*iter).width;
			index++;
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
		if (_widget->_getInternalData() == mSortRowIndex) {
			mSortUp = !mSortUp;
			redrawButtons();
			// если было недосортированно то сортируем
			if (mIsDirtySort) sortList();

			flipList();
		}
		else {
			mSortRowIndex = (size_t)_widget->_getInternalData();
			mSortUp = true;
			redrawButtons();
			sortList();
		}
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

	size_t MultiList::convertFromSort(size_t _index)
	{
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] == _index) return pos;
		}
		MYGUI_EXCEPT("index not found");
		return ITEM_NONE;
	}

	size_t MultiList::convertToSort(size_t _index)
	{
		MYGUI_DEBUG_ASSERT(_index < mToSortIndex.size(), "index out of range");
		return mToSortIndex[_index];
	}

	void MultiList::_insertSortIndex(size_t _index)
	{
		// все индексты что больше или равны вставляемому увеличиваем на 1
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] >= _index) mToSortIndex[pos]++;
		}
		// записываем индекс
		mToSortIndex.insert(mToSortIndex.begin() + _index, _index);
	}

	void MultiList::_eraseSortIndex(size_t _index)
	{
		size_t index = mToSortIndex[_index];
		// все индексты что меньше уменьшаем
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] < index) mToSortIndex[pos]--;
		}
		mToSortIndex.erase(mToSortIndex.begin() + _index);
	}

	void MultiList::_clearSortIndex()
	{
		mToSortIndex.clear();
	}

	void MultiList::flipList()
	{

		if (ITEM_NONE == mSortRowIndex) return;

		size_t end = mVectorRowInfo.front().list->getItemCount();
		if (0 == end) return;
		end --;
		size_t start = 0;

		Ogre::DisplayString tmp;
		tmp.reserve(64);
		size_t index1, index2;

		VectorSizeT vec;
		size_t size2 = mToSortIndex.size();
		vec.resize(size2);
		for (size_t pos=0; pos<size2; ++pos) vec[mToSortIndex[pos]] = pos;

		while (start < end) {

			for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
				tmp = (*iter).list->getItem(start);
				(*iter).list->setItem(start, (*iter).list->getItem(end));
				(*iter).list->setItem(end, tmp);
			}

			index1 = vec[start];
			index2 = vec[end];

			mToSortIndex[index1] = mToSortIndex[index2];
			mToSortIndex[index2] = start;

			vec[start] = vec[end];
			vec[end] = index1;

			start++;
			end--;
		}
	}

	void MultiList::sortList()
	{
		if (ITEM_NONE == mSortRowIndex) return;
		ListPtr list = mVectorRowInfo[mSortRowIndex].list;
		size_t count = list->getItemCount();
		if (0 == count) return;

		/*int i, j;

		Ogre::DisplayString tmp;
		int index1, index2, index_tmp;

		for ( i=0; i < count; i++) {  // цикл проходов, i - номер прохода
			tmp = list->getItem(i);
			index_tmp = convertFromSort(i);
			//x = a[i];
			// поиск места элемента в готовой последовательности 
			if (mSortUp) {
				for ( j=i-1; j>=0 && list->getItem(j) > tmp; j--) {

					list->setItem(j+1, list->getItem(j));

					index1 = convertFromSort(j+1);
					index2 = convertFromSort(j);

					mToSortIndex[index1] = mToSortIndex[index2];

					//a[j+1] = a[j];  	// сдвигаем элемент направо, пока не дошли
				}
				// место найдено, вставить элемент
				list->setItem(j+1, tmp);

				index1 = convertFromSort(j+1);
				mToSortIndex[index1] = index_tmp;
//				a[j+1] = x;
			}
			else {
				for ( j=i-1; j>=0 && list->getItem(j) < tmp; j--) {

					list->setItem(j+1, list->getItem(j));

					index1 = convertFromSort(j+1);
					index2 = convertFromSort(j);

					mToSortIndex[index1] = mToSortIndex[index2];

					//a[j+1] = a[j];  	// сдвигаем элемент направо, пока не дошли
				}
				// место найдено, вставить элемент
				list->setItem(j+1, tmp);

				index1 = convertFromSort(j+1);
				mToSortIndex[index1] = index_tmp;
//				a[j+1] = x;
			}
		}*/

		/*Ogre::DisplayString tmp;
		tmp.reserve(64);
		size_t index1, index2, index_tmp;

		for( size_t i=count-1; i>0; --i ) {
			for( size_t j=0; j<i; ++j ) {
				if (mSortUp) {
					if ( list->getItem(j) <= list->getItem(j+1) ) continue;
				}
				else {
					if( list->getItem(j) >= list->getItem(j+1) ) continue;
				}

				//swapLine(j, j+1);
				for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
					tmp = (*iter).list->getItem(j);
					(*iter).list->setItem(j, (*iter).list->getItem(j+1));
					(*iter).list->setItem(j+1, tmp);
				}
				index1 = convertFromSort(j);
				index2 = convertFromSort(j+1);

				index_tmp = mToSortIndex[index1];
				mToSortIndex[index1] = mToSortIndex[index2];
				mToSortIndex[index2] = index_tmp;
			}
		}*/

		Ogre::DisplayString tmp;
		tmp.reserve(64);
		size_t index1, index2;//, index_tmp;

		VectorSizeT vec;
		size_t size2 = mToSortIndex.size();
		vec.resize(size2);
		for (size_t pos=0; pos<size2; ++pos) vec[mToSortIndex[pos]] = pos;

		if (mSortUp) {
			for( size_t i=count-1; i>0; --i ) {
				for( size_t j=0; j<i; ++j ) {
					if ( list->getItem(j) <= list->getItem(j+1) ) continue;

					for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
						tmp = (*iter).list->getItem(j);
						(*iter).list->setItem(j, (*iter).list->getItem(j+1));
						(*iter).list->setItem(j+1, tmp);
					}

					index1 = vec[j];
					index2 = vec[j+1];

					//index_tmp = mToSortIndex[index1];
					mToSortIndex[index1] = mToSortIndex[index2];
					mToSortIndex[index2] = j;//index_tmp;

					//index_tmp = vec[j];
					vec[j] = vec[j+1];
					vec[j+1] = index1;//index_tmp;
				}
			}
		}
		else {
			for( size_t i=count-1; i>0; --i ) {
				for( size_t j=0; j<i; ++j ) {
					if ( list->getItem(j) >= list->getItem(j+1) ) continue;

					for (VectorRowInfo::iterator iter=mVectorRowInfo.begin(); iter!=mVectorRowInfo.end(); ++iter) {
						tmp = (*iter).list->getItem(j);
						(*iter).list->setItem(j, (*iter).list->getItem(j+1));
						(*iter).list->setItem(j+1, tmp);
					}

					index1 = vec[j];
					index2 = vec[j+1];

					//index_tmp = mToSortIndex[index1];
					mToSortIndex[index1] = mToSortIndex[index2];
					mToSortIndex[index2] = j;//index_tmp;

					//index_tmp = vec[j];
					vec[j] = vec[j+1];
					vec[j+1] = index1;//index_tmp;
				}
			}
		}

		mIsDirtySort = false;
	}

	void MultiList::setDirtySort()
	{
		if (mIsDirtySort) return;
		Gui::getInstance().addFrameListener(this);
		mIsDirtySort = true;
	}

	void MultiList::_frameEntered(float _frame)
	{
		if (false == mIsDirtySort) {
			Gui::getInstance().removeFrameListener(this);
			return;
		}
		sortList();
	}

} // namespace MyGUI
