/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_CastWidget.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_Button.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_InputManager.h"

namespace MyGUI
{

	Ogre::String ItemBox::WidgetTypeName = "ItemBox";

	ItemBox::ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mWidgetScroll(null),
		mWidgetClient(null),
		mLineTop(0),
		mOffsetTop(0),
		mIsFocus(false),
		mCountItems(0),
		mScrollRange(0),
		mScrollPosition(0),
		mIndexSelect(ITEM_NONE),
		mIndexActive(ITEM_NONE),
		mItemDrag(null),
		mOldDrop(null),
		mDropResult(false),
		mCurrentSender(null),
		mStartDrop(false),
		mNeedDrop(false)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "VScroll") {
				mWidgetScroll = castWidget<VScroll>(*iter);
				mWidgetScroll->eventScrollChangePosition = newDelegate(this, &ItemBox::notifyScrollChangePosition);
				//mWidgetScroll->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMousePressed);
			}
			else if ((*iter)->_getInternalString() == "Client") {
				mWidgetClient = (*iter);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			}
		}
		// сли нет скрола, то клиенская зона не обязательно
		if ((null == mWidgetScroll) && (null == mWidgetClient)) mWidgetClient = this;
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// подписываем клиент для драгэндропа
		mWidgetClient->_requestGetDragItemInfo = newDelegate(this, &ItemBox::requestGetDragItemInfo);

		// парсим свойства
		/*const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("SkinLine");
		if (iter != param.end()) mSkinLine = iter->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (ItemBox must have SkinLine property)");

		iter = param.find("HeightLine");
		if (iter != param.end()) mHeightLine = utility::parseInt(iter->second);
		if (mHeightLine < 1) mHeightLine = 1;*/

		updateMetrics();
		updateScroll();
		
	}

	void ItemBox::setSize(const IntSize& _size)
	{
		IntCoord old = mCoord;
		Widget::setSize(_size);
		updateFromResize(old.size());
	}

	void ItemBox::setPosition(const IntCoord& _coord)
	{
		IntCoord old = mCoord;
		Widget::setPosition(_coord);
		updateFromResize(old.size());
	}

	void ItemBox::_updateScrollWidget()
	{
		if (mWidgetScroll) {
			mWidgetScroll->setScrollPosition(mScrollPosition);
			mWidgetScroll->setScrollRange(mScrollRange + 1);
			mWidgetScroll->setScrollPage(mSizeItem.height);
			mWidgetScroll->setScrollViewPage(mSizeItem.height);
		}
	}

	void ItemBox::updateFromResize(const IntSize& _size)
	{
		int old_count = mCountItemInLine;
		updateMetrics();
		updateScroll();

		// если колличество айтемов в строке изменилось, то перерисовываем все
		if (old_count == mCountItemInLine) {
			// если строк стало меньшн то ничего не нужно
			if (_size.height >= mCoord.height) return;
		}

		_updateAllVisible(true);
	}

	void ItemBox::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _index)
	{
		mScrollPosition = _index;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateAllVisible(old != mLineTop);
	}

	void ItemBox::updateMetrics()
	{
		IntCoord coord(0, 0, 1, 1);
		// спрашиваем размер иконок
		requestCoordItem(this, mWidgetClient, coord, false);
		mSizeItem = coord.size();
		MYGUI_ASSERT((mSizeItem.width > 0 && mSizeItem.height > 0), "(mSizeItem.width > 0 && mSizeItem.height > 0)  at requestCoordItem");

		// колличество айтемов на одной строке
		mCountItemInLine = mWidgetClient->getWidth() / mSizeItem.width;
		if (1 > mCountItemInLine) mCountItemInLine = 1;

		// колличество строк
		mCountLines = (int)mCountItems / mCountItemInLine;
		if (0 != ((int)mCountItems % mCountItemInLine)) mCountLines ++;

		// колличество строк которые помещаються в видимую часть
		mCountLineVisible = mWidgetClient->getHeight() / mSizeItem.height;
		if (0 != (mWidgetClient->getHeight() % mSizeItem.height)) mCountLineVisible++;
		mCountLineVisible ++; // и на одну больше для попиксельной прокрутки
	}

	void ItemBox::updateScroll()
	{
		// размерность скролла в пикселях
		mScrollRange = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();

		// тестируем видимость скролла
		bool change = false;
		if (mWidgetScroll) {
			if ((mScrollRange <= 0) || (mWidgetScroll->getLeft() <= mWidgetClient->getLeft())) {
				if (mWidgetScroll->isShow()) {
					change = true;
					mWidgetScroll->hide();
					// увеличиваем клиентскую зону на ширину скрола
					mWidgetClient->setSize(mWidgetClient->getWidth() + mWidgetScroll->getWidth(), mWidgetClient->getHeight());
				}
			}
			else if (false == mWidgetScroll->isShow()) {
				change = true;
				mWidgetClient->setSize(mWidgetClient->getWidth() - mWidgetScroll->getWidth(), mWidgetClient->getHeight());
				mWidgetScroll->show();
			}
		}

		// если скролл изменился, то пересчитываем
		if (change) {
			updateMetrics();
			mScrollRange = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();
		}
		
		// тестируем текущую позицию скролла
		int count_pix = ((mCountLines - mLineTop) * mSizeItem.height) - mOffsetTop;

		if ((count_pix < mWidgetClient->getHeight()) && ((mLineTop != 0) || (mOffsetTop != 0))) {
			// считаем позицию, прижатую к низу
			mLineTop = mCountLines - (mWidgetClient->getHeight() / mSizeItem.height) - 1;

			if (mLineTop < 0) {
				mLineTop = 0;
				mOffsetTop = 0;
				mScrollPosition = 0;
			}
			else {
				mOffsetTop = mSizeItem.height - (mWidgetClient->getHeight() % mSizeItem.height);
				mScrollPosition = (mLineTop * mSizeItem.height) + mOffsetTop;
			}

		}

		_updateScrollWidget();
	}

	void ItemBox::_updateAllVisible(bool _redraw)
	{

		int start = (mLineTop * mCountItemInLine);
		int count = (mCountLineVisible * mCountItemInLine) + start;
		size_t iwid = 0; // индекс виджета
		for (int pos = start; pos<count; ++pos, ++iwid) {
			// дальше нет айтемов
			if (pos >= mCountItems) break;

			WidgetPtr widget = getItemWidget(iwid);
			widget->setPosition(IntCoord((iwid % mCountItemInLine) * mSizeItem.width,
				((iwid / mCountItemInLine) * mSizeItem.height)  - mOffsetTop,
				mSizeItem.width, mSizeItem.height));

			widget->show();
			if (_redraw) {
				ItemInfo & data = mItemsInfo[pos];
				data.update = true;
				requestUpdateItem(this, widget, data);
			}

		}

		// все виджеты еще есть, то их надо бы скрыть
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid]->hide();
			iwid ++;
		}

	}

	WidgetPtr ItemBox::getItemWidget(size_t _index)
	{
		// еще нет такого виджета, нуно создать
		if (_index >= mVectorItems.size()) {

			// вызываем запрос на создание виджета
			WidgetPtr widget = null;
			requestCreateItem(this, mWidgetClient, widget);
			MYGUI_ASSERT(widget, "not listen requestCreateItem");

			widget->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			widget->eventMouseSetFocus = newDelegate(this, &ItemBox::notifyMouseSetFocus);
			widget->eventMouseLostFocus = newDelegate(this, &ItemBox::notifyMouseLostFocus);
			widget->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			widget->eventMouseButtonReleased = newDelegate(this, &ItemBox::notifyMouseButtonReleased);
			widget->eventMouseDrag = newDelegate(this, &ItemBox::notifyMouseDrag);
			widget->_requestGetDragItemInfo = newDelegate(this, &ItemBox::requestGetDragItemInfo);

			widget->_setInternalData((int)mVectorItems.size());
			mVectorItems.push_back(widget);

			return mVectorItems.back();
		}
		return mVectorItems[_index];
	}

	void ItemBox::_onMouseWheel(int _rel)
	{
		notifyMouseWheel(null, _rel);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onMouseWheel(_rel);
	}

	void ItemBox::_onKeySetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		setState("select");

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeySetFocus(_old);
	}

	void ItemBox::_onKeyLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		setState("normal");

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeyLostFocus(_new);
	}

	void ItemBox::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		ItemInfo & data = mItemsInfo[index];
		data.update = false;
		data.active = true;
		requestUpdateItem(this, _sender, data);
		mIndexActive = index;
	}

	void ItemBox::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		// уже сбросили фокус
		if (mIndexActive == ITEM_NONE) return;

		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		ItemInfo & data = mItemsInfo[index];
		data.update = false;
		data.active = false;
		requestUpdateItem(this, _sender, data);
		mIndexActive = ITEM_NONE;
	}

	void ItemBox::notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if (mScrollRange <= 0) return;

		int offset = mScrollPosition;
		if (_rel < 0) offset += mSizeItem.height;
		else  offset -= mSizeItem.height;

		if (offset >= mScrollRange) offset = mScrollRange;
		else if (offset < 0) offset = 0;

		if (mScrollPosition == offset) return;

		// сбрасываем старую подсветку
		// так как при прокрутке, мышь может находиться над окном
		resetCurrentActiveItem();

		mScrollPosition = offset;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateScrollWidget();
		_updateAllVisible(old != mLineTop);

		// заново ищем и подсвечиваем айтем
		findCurrentActiveItem();

	}

	void ItemBox::resetCurrentActiveItem()
	{
		// сбрасываем старую подсветку
		if (mIndexActive != ITEM_NONE) {
			size_t start = (size_t)(mLineTop * mCountItemInLine);
			ItemInfo & data = mItemsInfo[mIndexActive];
			data.update = false;
			data.active = false;
			if ((mIndexActive >= start) && (mIndexActive < (start + mVectorItems.size()))) {
				requestUpdateItem(this, mVectorItems[mIndexActive - start], data);
			}
			mIndexActive = ITEM_NONE;
		}
	}

	void ItemBox::findCurrentActiveItem()
	{
		MYGUI_DEBUG_ASSERT(mIndexActive == ITEM_NONE, "use resetCurrentActiveItem() before findCurrentActiveItem()");

		const IntPoint& point = InputManager::getInstance().getMousePosition();

		// сначала проверяем клиентскую зону
		const IntRect& rect = mWidgetClient->getAbsoluteRect();
		if ((point.left < rect.left) || (point.left > rect.right) || (point.top < rect.top) || (point.top > rect.bottom)) {
			return;
		}

		for (size_t pos=0; pos<mVectorItems.size(); ++pos) {
			WidgetPtr widget = mVectorItems[pos];
			const IntRect& rect = widget->getAbsoluteRect();
			if ((point.left>= rect.left) && (point.left <= rect.right) && (point.top>= rect.top) && (point.top <= rect.bottom)) {

				size_t index = (size_t)widget->_getInternalData() + (mLineTop * mCountItemInLine);
				// индекс может быть больше
				if (index < mItemsInfo.size()) {
					ItemInfo & data = mItemsInfo[index];
					data.update = false;
					data.active = true;
					requestUpdateItem(this, widget, data);
					mIndexActive = index;
				}

				break;
			}
		}
	}

	void ItemBox::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// нужно ли обновить данные
		bool update = false;

		// текущий эмитер
		if (mCurrentSender != _sender) {
			mCurrentSender = _sender;
			update = true;
		}

		size_t index = (size_t)mCurrentSender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		if (false == mStartDrop) {
			mStartDrop = true;
			mNeedDrop = false;
			// запрос на нужность дропа по индексу
			requestStartDrop(this, index, mNeedDrop);
		}

		// дроп не нужен
		if (false == mNeedDrop) {
			InputManager::getInstance().resetMouseCaptureWidget();
			return;
		}

		// делаем запрос, над кем наша мыша
		WidgetPtr item = InputManager::getInstance().getWidgetFromPoint(_left, _top);

		if (null == mItemDrag) {
			requestCreateItem(this, null, mItemDrag);
			MYGUI_ASSERT(mItemDrag, "not listen requestCreateItem");
			// спрашиваем размер иконок
			IntCoord coord;
			requestCoordItem(this, mWidgetClient, coord, true);
			mItemDrag->setSize(coord.size());
			mPointDragOffset = coord.point();
		}

		IntPoint point = InputManager::getInstance().getLastLeftPressed() - _sender->getAbsolutePosition();
		mItemDrag->setPosition(_left - point.left + mPointDragOffset.left, _top - point.top + mPointDragOffset.top);
		mItemDrag->show();

		// если равно, значит уже спрашивали
		if (mOldDrop == item) return;

		// сбрасываем старую подсветку
		if (mDropInfo.reseiver) mDropInfo.reseiver->_setDragItemInfo(mDropInfo.index_reseiver, false, false);

		mDropResult = false;
		WidgetPtr reseiver = null;
		size_t index_reseiver = ITEM_NONE;
		// есть виджет под нами
		if (item) {
			// делаем запрос на индекс по произвольному виджету
			item->_getDragItemInfo(reseiver, index_reseiver);
			if (reseiver) {
				// подписываемся на информацию о валидности дропа
				reseiver->_eventInvalideDropInfo = newDelegate(this, &ItemBox::notifyInvalideDrop);

				// делаем запрос на возможность дропа
				mDropInfo.index = index;
				mDropInfo.reseiver = reseiver;
				mDropInfo.index_reseiver = index_reseiver;
				requestDropItem(this, mDropInfo, mDropResult);

				// устанавливаем новую подсветку
				mDropInfo.reseiver->_setDragItemInfo(mDropInfo.index_reseiver, true, mDropResult);
			}
			else {
				mDropInfo.reset();
			}
		}
		// нет виджета под нами
		else {
			mDropInfo.reset();
		}

		mOldDrop = item;

		// копию создаем
		ItemInfo data = mItemsInfo[index];
		data.update = update;
		data.drag = true;
		if (reseiver == null) {
			data.drag_accept = false;
			data.drag_refuse = false;
		}
		else if (mDropResult) {
			data.drag_accept = true;
			data.drag_refuse = false;
		}
		else {
			data.drag_accept = false;
			data.drag_refuse = true;
		}
		data.select = false;
		data.active = false;
		requestUpdateItem(this, mItemDrag, data);

	}

	void ItemBox::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if ( ! _left) return;

		// сбрасываем инфу для дропа
		mDropResult = false;
		mOldDrop = null;
		mDropInfo.reset();

		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);

		if (index < mItemsInfo.size()) setItemSelect(index);

		// сбрасываем, чтобы обновился дропный виджет
		mCurrentSender = null;
		mStartDrop = false;
	}

	void ItemBox::notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (mItemDrag) mItemDrag->hide();

		// сбрасываем старую подсветку
		if (mDropInfo.reseiver) mDropInfo.reseiver->_setDragItemInfo(mDropInfo.index_reseiver, false, false);

		// если дроп выполнен успешно
		if (mDropResult) eventDropAccept(this, mDropInfo);

		// сбрасываем инфу для дропа
		mDropResult = false;
		mOldDrop = null;
		mDropInfo.reset();
	}

	void ItemBox::requestGetDragItemInfo(WidgetPtr _sender, WidgetPtr & _list, size_t & _index)
	{
		if (_sender == mWidgetClient) {
			_list = this;
			_index = ITEM_NONE;
		}
		else {
			size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);
			if (index < mItemsInfo.size()) {
				_list = this;
				_index = index;
			}
		}
	}

	void ItemBox::_setDragItemInfo(size_t _index, bool _set, bool _accept)
	{
		if (_index == ITEM_NONE) return;
		MYGUI_DEBUG_ASSERT(_index < mItemsInfo.size(), "index out of range");

		ItemInfo & data = mItemsInfo[_index];
		data.update = false;
		if (false == _set) {
			data.drag_accept = false;
			data.drag_refuse = false;
		}
		else if (_accept) {
			data.drag_accept = true;
			data.drag_refuse = false;
		}
		else {
			data.drag_accept = false;
			data.drag_refuse = true;
		}
		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {
			requestUpdateItem(this, mVectorItems[_index - start], data);
		}
	}

	void ItemBox::setItem(size_t _index, void * _data)
	{
		MYGUI_ASSERT(_index < mItemsInfo.size() , "index '" << _index << " out of range '" << mItemsInfo.size() << "'");
		mItemsInfo[_index].data = _data;
	}

	void * ItemBox::getItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mItemsInfo.size() , "index '" << _index << " out of range '" << mItemsInfo.size() << "'");
		return mItemsInfo[_index].data;
	}

	void ItemBox::insertItem(size_t _index, void * _data)
	{
		MYGUI_ASSERT(((_index < mItemsInfo.size()) || (_index == ITEM_NONE)), "index '" << _index << " out of range '" << mItemsInfo.size() << "'");

		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		resetCurrentActiveItem();

		mItemsInfo.insert(mItemsInfo.begin() + _index, ItemInfo(mCountItems, _data));
		mCountItems++;

		// расчитываем новый индекс выделения
		if (mIndexSelect != ITEM_NONE) {
			if (mIndexSelect >= _index) mIndexSelect ++;
		}

		// подправляем индексы и выделение
		for (size_t pos=0; pos<mItemsInfo.size(); ++pos) {
			mItemsInfo[pos].index = pos;
			mItemsInfo[pos].select = mIndexSelect == pos;
		}

		updateMetrics();
		updateScroll();

		findCurrentActiveItem();

		_updateAllVisible(true);
	}

	void ItemBox::deleteItem(size_t _index)
	{
		MYGUI_ASSERT(_index < mItemsInfo.size() , "index '" << _index << " out of range '" << mItemsInfo.size() << "'");

		resetCurrentActiveItem();

		mItemsInfo.erase(mItemsInfo.begin() + _index);
		mCountItems --;

		// расчитываем новый индекс выделения
		if (mIndexSelect != ITEM_NONE) {
			if (mCountItems == 0) mIndexSelect = ITEM_NONE;
			else if ((mIndexSelect > _index) || (mIndexSelect == mCountItems)) mIndexSelect --;
		}

		// подправляем индексы и выделение
		for (size_t pos=0; pos<mItemsInfo.size(); ++pos) {
			mItemsInfo[pos].index = pos;
			mItemsInfo[pos].select = mIndexSelect == pos;
		}

		updateMetrics();
		updateScroll();

		findCurrentActiveItem();

		_updateAllVisible(true);
	}

	void ItemBox::deleteAllItems()
	{
		if (0 == mItemsInfo.size()) return;

		mItemsInfo.clear();
		mCountItems = 0;

		mIndexSelect = ITEM_NONE;
		mIndexActive = ITEM_NONE;

		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
	}

	void ItemBox::setItemSelect(size_t _index)
	{
		MYGUI_ASSERT(((_index < mItemsInfo.size()) || (_index == ITEM_NONE)), "index '" << _index << " out of range '" << mItemsInfo.size() << "'");
		if (_index == mIndexSelect) return;

		size_t start = (size_t)(mLineTop * mCountItemInLine);

		// сбрасываем старое выделение
		if (mIndexSelect != ITEM_NONE) {
			ItemInfo & data = mItemsInfo[mIndexSelect];
			data.update = false;
			data.select = false;
			if ((mIndexSelect >= start) && (mIndexSelect < (start + mVectorItems.size()))) {
				requestUpdateItem(this, mVectorItems[mIndexSelect - start], data);
			}
		}

		mIndexSelect = _index;
		if (mIndexSelect != ITEM_NONE) {
			ItemInfo & data = mItemsInfo[mIndexSelect];
			data.update = false;
			data.select = true;
			if ((mIndexSelect >= start) && (mIndexSelect < (start + mVectorItems.size()))) {
				requestUpdateItem(this, mVectorItems[mIndexSelect - start], data);
			}
		}
	}

	void ItemBox::notifyInvalideDrop(WidgetPtr _sender)
	{
		const IntPoint& point = InputManager::getInstance().getMousePosition();
		notifyMouseDrag(null, point.left, point.top);
	}

} // namespace MyGUI
