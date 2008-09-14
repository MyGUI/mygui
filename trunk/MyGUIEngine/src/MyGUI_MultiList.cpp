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
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( MultiList, Widget );

	MultiList::MultiList(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mHeightButton(0),
		//mWidgetClient(null),
		mButtonMain(null),
		mWidthBar(0),
		mLastMouseFocusIndex(ITEM_NONE),
		mSortUp(true),
		mSortColumnIndex(ITEM_NONE),
		mIsDirtySort(false),
		mWidthSeparator(0),
		mOffsetButtonSeparator(2)
	{
		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("SkinButton");
			if (iter != properties.end()) mSkinButton = iter->second;
			iter = properties.find("HeightButton");
			if (iter != properties.end()) mHeightButton = utility::parseInt(iter->second);
			if (mHeightButton < 0) mHeightButton = 0;

			iter = properties.find("SkinList");
			if (iter != properties.end()) mSkinList = iter->second;

			iter = properties.find("SkinButtonEmpty");
			if (iter != properties.end()) {
				mButtonMain = mWidgetClient->createWidget<Button>(iter->second,
					IntCoord(0, 0, mWidgetClient->getWidth(), mHeightButton), Align::Default);
			}

			iter = properties.find("WidthSeparator");
			if (iter != properties.end()) mWidthSeparator = utility::parseInt(iter->second);
			iter = properties.find("SkinSeparator");
			if (iter != properties.end()) mSkinSeparator = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
			}
		}
		// мона и без клиента
		if (null == mWidgetClient) mWidgetClient = this;
	}

	MultiList::~MultiList()
	{
		Gui::getInstance().eventFrameStart -= newDelegate(this, &MultiList::frameEntered);
		//Gui::getInstance().removeFrameListener(newDelegate(this, &MultiList::frameEntered));
	}

	//----------------------------------------------------------------------------------//
	// методы для работы со столбцами
	void MultiList::insertColumn(size_t _column, int _width, const Ogre::UTFString & _name)
	{
		// скрываем у крайнего скролл
		if (false == mVectorColumnInfo.empty()) 
			mVectorColumnInfo.back().list->setScrollVisible(false);
		else mSortColumnIndex = 0;

		ColumnInfo column;
		column.width = _width < 0 ? 0 : _width;

		column.list = mWidgetClient->createWidget<List>(mSkinList, IntCoord(), Align::Left | Align::VStretch);
		column.list->eventListChangePosition = newDelegate(this, &MultiList::notifyListChangePosition);
		column.list->eventListMouseItemFocus = newDelegate(this, &MultiList::notifyListChangeFocus);
		column.list->eventListChangeScroll = newDelegate(this, &MultiList::notifyListChangeScrollPosition);
		column.list->eventListSelectAccept = newDelegate(this, &MultiList::notifyListSelectAccept);

		column.button = mWidgetClient->createWidget<Button>(mSkinButton, IntCoord(), Align::Default);
		column.button->eventMouseButtonClick = newDelegate(this, &MultiList::notifyButtonClick);
		column.name = _name;

		// если уже были столбики, то делаем то же колличество полей
		if (false == mVectorColumnInfo.empty()) {
			size_t count = mVectorColumnInfo.front().list->getItemCount();
			for (size_t pos=0; pos<count; ++pos)
				column.list->addItem("");
		}

		if (_column > mVectorColumnInfo.size()) _column = mVectorColumnInfo.size();
		mVectorColumnInfo.insert(mVectorColumnInfo.begin() + _column, column);

		updateColumns();

		// показываем скролл нового крайнего
		mVectorColumnInfo.back().list->setScrollVisible(true);
	}

	void MultiList::setColumnName(size_t _column, const Ogre::UTFString & _name)
	{
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		mVectorColumnInfo[_column].name = _name;
		redrawButtons();
	}

	void MultiList::setColumnWidth(size_t _column, int _width)
	{
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		mVectorColumnInfo[_column].width = _width < 0 ? 0 : _width;
		updateColumns();
	}

	const Ogre::UTFString & MultiList::getColumnName(size_t _column)
	{
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		return mVectorColumnInfo[_column].name;
	}

	int MultiList::getColumnWidth(size_t _column)
	{
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		return mVectorColumnInfo[_column].width;
	}

	void MultiList::deleteColumn(size_t _column)
	{
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");

		ColumnInfo & info = mVectorColumnInfo[_column];

		WidgetManager & manager = WidgetManager::getInstance();
		manager.destroyWidget(info.button);
		manager.destroyWidget(info.list);

		mVectorColumnInfo.erase(mVectorColumnInfo.begin() + _column);

		if (mVectorColumnInfo.empty()) mSortColumnIndex = ITEM_NONE;
		else {
			mSortColumnIndex = 0;
			mSortUp = true;
			sortList();
		}

		updateColumns();
	}

	void MultiList::deleteAllColumns()
	{
		WidgetManager & manager = WidgetManager::getInstance();
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			manager.destroyWidget((*iter).button);
			manager.destroyWidget((*iter).list);
		}
		mVectorColumnInfo.clear();
		mSortColumnIndex = ITEM_NONE;

		updateColumns();
	}

	void MultiList::sortByColumn(size_t _column, bool _backward)
	{
		mSortColumnIndex = _column;
		if (_backward) {
			mSortUp = !mSortUp;
			redrawButtons();
			// если было недосортированно то сортируем
			if (mIsDirtySort) sortList();

			flipList();
		}
		else {
			mSortUp = true;
			redrawButtons();
			sortList();
		}
	}

	//------------------------------------------------------------------------------//
	// методы для работы со строками
	size_t MultiList::getItemCount()
	{
		if (mVectorColumnInfo.empty()) return 0;
		return mVectorColumnInfo.front().list->getItemCount();
	}

	void MultiList::insertItem(size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		if (ITEM_NONE == _index) _index = mVectorColumnInfo.front().list->getItemCount();

		// вставляем во все поля пустые, а потом присваиваем первому
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->insertItem(_index, "");
		}
		mVectorColumnInfo.front().list->setItem(_index, _item);

		_insertSortIndex(_index);
		setDirtySort();
	}

	void MultiList::setItem(size_t _index, const Ogre::UTFString & _item)
	{
		setSubItem(0, _index, _item);
	}

	const Ogre::UTFString & MultiList::getItem(size_t _index)
	{
		return getSubItem(0, _index);
	}

	void MultiList::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		MYGUI_ASSERT(_index < mVectorColumnInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->deleteItem(convertToSort(_index));
		}
		_eraseSortIndex(_index);
	}

	void MultiList::deleteAllItems()
	{
		if (mVectorColumnInfo.empty()) return;
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->deleteAllItems();
		}
		_clearSortIndex();
	}

	size_t MultiList::getItemSelect()
	{
		if (mVectorColumnInfo.empty()) return ITEM_NONE;
		size_t item = mVectorColumnInfo.front().list->getItemSelect();
		return (ITEM_NONE == item) ? ITEM_NONE : convertFromSort(item);
	}

	void MultiList::resetItemSelect()
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->resetItemSelect();
		}
	}

	void MultiList::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		MYGUI_ASSERT(_index < mVectorColumnInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		size_t index = convertToSort(_index);
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->setItemSelect(index);
		}
	}

	//----------------------------------------------------------------------------------//
	// методы для работы с саб строками
	void MultiList::setSubItem(size_t _column, size_t _index, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		MYGUI_ASSERT(_index < mVectorColumnInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		mVectorColumnInfo[_column].list->setItem(convertToSort(_index), _item);

		// если мы попортили список с активным сортом, надо пересчитывать
		if (_column == mSortColumnIndex) setDirtySort();
	}

	const Ogre::UTFString & MultiList::getSubItem(size_t _column, size_t _index)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		MYGUI_ASSERT(_index < mVectorColumnInfo.begin()->list->getItemCount(), "index " << _index <<" out of range");
		return mVectorColumnInfo[_column].list->getItem(convertToSort(_index));
	}

	size_t MultiList::findItem(size_t _column, const Ogre::UTFString & _item)
	{
		MYGUI_ASSERT(false == mVectorColumnInfo.empty(), "multilist has no columns");
		MYGUI_ASSERT(_column < mVectorColumnInfo.size(), "column " << _column <<" out of range");
		return convertFromSort(mVectorColumnInfo[_column].list->findItem(_item));
	}
	//----------------------------------------------------------------------------------//

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
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter)
			if (_widget != (*iter).list) (*iter).list->setItemSelect(_position);

		// наш евент
		eventListChangePosition(this, _position);
	}

	void MultiList::notifyListSelectAccept(MyGUI::WidgetPtr _widget, size_t _position)
	{
		// наш евент
		eventListSelectAccept(this, _position);
	}

	void MultiList::notifyListChangeFocus(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (_widget != (*iter).list) {
				if (ITEM_NONE != mLastMouseFocusIndex) (*iter).list->_setItemFocus(mLastMouseFocusIndex, false);
				if (ITEM_NONE != _position) (*iter).list->_setItemFocus(_position, true);
			}
		}
		mLastMouseFocusIndex = _position;
	}

	void MultiList::notifyListChangeScrollPosition(MyGUI::WidgetPtr _widget, size_t _position)
	{
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (_widget != (*iter).list)
				(*iter).list->setScrollPosition(_position);
		}
	}

	void MultiList::notifyButtonClick(MyGUI::WidgetPtr _widget)
	{
		size_t index = (size_t)_widget->_getInternalData();
		sortByColumn(index, index == mSortColumnIndex);
	}

	void MultiList::redrawButtons()
	{
		size_t pos = 0;
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			if (pos == mSortColumnIndex) {
				if (mSortUp) (*iter).button->setImageIndex(SORT_UP);
				else (*iter).button->setImageIndex(SORT_DOWN);
			}
			else (*iter).button->setImageIndex(SORT_NONE);
			(*iter).button->setCaption((*iter).name);
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
		if (_index == ITEM_NONE) return ITEM_NONE;
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
		// запоминаем чтобы позже удалить
		size_t index = convertToSort(_index);
		// все индексы что больше уменьшаем
		for (size_t pos=0; pos<mToSortIndex.size(); ++pos) {
			if (mToSortIndex[pos] > _index) mToSortIndex[pos]--;
		}
		mToSortIndex.erase(mToSortIndex.begin() + index);
	}

	void MultiList::_clearSortIndex()
	{
		mToSortIndex.clear();
	}

	void MultiList::flipList()
	{

		if (ITEM_NONE == mSortColumnIndex) return;

		size_t end = mVectorColumnInfo.front().list->getItemCount();
		if (0 == end) return;
		end --;
		size_t start = 0;

		Ogre::UTFString tmp;
		tmp.reserve(64);
		size_t index1, index2;

		VectorSizeT vec;
		size_t size2 = mToSortIndex.size();
		vec.resize(size2);
		for (size_t pos=0; pos<size2; ++pos) vec[mToSortIndex[pos]] = pos;

		while (start < end) {

			for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
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
		if (ITEM_NONE == mSortColumnIndex) return;
		ListPtr list = mVectorColumnInfo[mSortColumnIndex].list;
		size_t count = list->getItemCount();
		if (0 == count) return;

		VectorSizeT vec;
		size_t size = mToSortIndex.size();
		vec.resize(size);
		for (size_t pos=0; pos<size; ++pos) vec[mToSortIndex[pos]] = pos;

		struct Keeper
		{
			inline void keep(VectorSizeT & vec, VectorSizeT & vec2, VectorColumnInfo & info, size_t _index)
			{
				text.resize(info.size());
				std::vector<Ogre::UTFString>::iterator itext = text.begin();
				for (VectorColumnInfo::iterator iter=info.begin(); iter!=info.end(); ++iter, ++itext) {
					(*itext) = (*iter).list->getItem(_index);
				}
				index1 = _index;
				index2 = vec2[_index];
			}

			inline void restore(VectorSizeT & vec, VectorSizeT & vec2, VectorColumnInfo & info, size_t _index)
			{
				std::vector<Ogre::UTFString>::iterator itext = text.begin();
				for (VectorColumnInfo::iterator iter=info.begin(); iter!=info.end(); ++iter, ++itext) {
					(*iter).list->setItem(_index, *itext);
				}
				vec[vec2[_index]] = index1;
				vec2[_index] = index2;
			}

			inline void swap(VectorSizeT & vec, VectorSizeT & vec2, VectorColumnInfo & info, size_t _index1, size_t _index2)
			{
				for (VectorColumnInfo::iterator iter=info.begin(); iter!=info.end(); ++iter) {
					(*iter).list->setItem(_index1, (*iter).list->getItem(_index2));
				}
				vec[vec2[_index1]] = _index2;
				vec2[_index1] = vec2[_index2];
			}

			std::vector<Ogre::UTFString> text;
			size_t index1, index2;
		};

		Keeper keeper;

		// shell sort
		for(int step = (int)count/2; step>0 ; step >>= 1) {
			for( int i=0; i<(int)(count-step); ++i ) {
				int j = i;
				bool compare;
				if (operatorLess.empty()) compare = list->getItem(j) < list->getItem(j+step);
				else operatorLess(this, mSortColumnIndex, list->getItem(j), list->getItem(j+step), compare);
				while ( (j >= 0) && (compare^mSortUp) ){
					keeper.keep(mToSortIndex, vec, mVectorColumnInfo, j);
					keeper.swap(mToSortIndex, vec, mVectorColumnInfo, j, j+step);
					keeper.restore(mToSortIndex, vec, mVectorColumnInfo, j+step);
					--j;
					if (j >= 0)
					{
						if (operatorLess.empty()) compare = list->getItem(j) < list->getItem(j+step);
						else operatorLess(this, mSortColumnIndex, list->getItem(j), list->getItem(j+step), compare);
					}
				}
			}
		}

		/*if (mSortUp) {
			for( ; step>0 ; ) {
					for( size_t i=0; i<(count-step); ++i ) {
							int j = (int)i;
							while ( (j>=0) && (list->getItem(j) > list->getItem(j+step)) ){
									keeper.keep(mToSortIndex, vec, mVectorColumnInfo, j);
									keeper.swap(mToSortIndex, vec, mVectorColumnInfo, j, j+step);
									keeper.restore(mToSortIndex, vec, mVectorColumnInfo, j+step);
									--j;
							}
					}
					step >>= 1;
			}
		}
		else {
			//...
		}*/






		mIsDirtySort = false;
	}

	void MultiList::setDirtySort()
	{
		if (mIsDirtySort) return;
		Gui::getInstance().eventFrameStart += newDelegate(this, &MultiList::frameEntered);
		//Gui::getInstance().addFrameListener(newDelegate(this, &MultiList::frameEntered), this);
		mIsDirtySort = true;
	}

	void MultiList::frameEntered(float _frame)
	{
		if (false == mIsDirtySort) {
			Gui::getInstance().eventFrameStart -= newDelegate(this, &MultiList::frameEntered);
			//Gui::getInstance().removeFrameListener(newDelegate(this, &MultiList::frameEntered));
			return;
		}
		sortList();
	}

	WidgetPtr MultiList::getSeparator(size_t _index)
	{
		if (!mWidthSeparator || mSkinSeparator.empty()) return null;
		// последний столбик
		if (_index == mVectorColumnInfo.size()-1) return null;

		while (_index >= mSeparators.size()) {
			WidgetPtr separator = mWidgetClient->createWidget<Widget>(mSkinSeparator, IntCoord(), Align::Default);
			mSeparators.push_back(separator);
		}

		return mSeparators[_index];
	}

	void MultiList::updateColumns()
	{
		mWidthBar = 0;
		int index = 0;
		for (VectorColumnInfo::iterator iter=mVectorColumnInfo.begin(); iter!=mVectorColumnInfo.end(); ++iter) {
			(*iter).list->setPosition(mWidthBar, mHeightButton, (*iter).width, mWidgetClient->getHeight() - mHeightButton);
			(*iter).button->setPosition(mWidthBar, 0, (*iter).width, mHeightButton);
			(*iter).button->_setInternalData(index);

			mWidthBar += (*iter).width;

			// промежуток между листами
			WidgetPtr separator = getSeparator(index);
			if (separator) {
				separator->setPosition(mWidthBar, 0, mWidthSeparator, mWidgetClient->getHeight());
			}

			mWidthBar += mWidthSeparator;
			index++;
		}

		redrawButtons();
		updateOnlyEmpty();
	}


} // namespace MyGUI
