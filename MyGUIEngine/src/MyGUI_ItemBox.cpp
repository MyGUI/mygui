/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Common.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_Button.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetTranslate.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	ItemBox::ItemBox(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mWidgetScroll(nullptr),
		mScrollRange(0),
		mScrollPosition(0),
		mCountItems(0),
		mLineTop(0),
		mOffsetTop(0),
		mIndexSelect(ITEM_NONE),
		mIndexActive(ITEM_NONE),
		mIndexAccept(ITEM_NONE),
		mIndexRefuse(ITEM_NONE),
		mIsFocus(false),
		mItemDrag(nullptr),
		mAlignVert(true)
	{
		initialiseWidgetSkin(_info);
	}

	ItemBox::~ItemBox()
	{
		shutdownWidgetSkin();
	}

	void ItemBox::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void ItemBox::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;
		mDragLayer = "DragAndDrop";

		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("AlignVert");
			if (iter != properties.end()) mAlignVert = utility::parseBool(iter->second);
			iter = properties.find("DragLayer");
			if (iter != properties.end()) mDragLayer = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "VScroll") {
				MYGUI_DEBUG_ASSERT( ! mWidgetScroll, "widget already assigned");
				mWidgetScroll = (*iter)->castType<VScroll>();
				mWidgetScroll->eventScrollChangePosition = newDelegate(this, &ItemBox::notifyScrollChangePosition);
			}
			if (*(*iter)->_getInternalData<std::string>() == "HScroll") {
				MYGUI_DEBUG_ASSERT( ! mWidgetScroll, "widget already assigned");
				mWidgetScroll = (*iter)->castType<HScroll>();
				mWidgetScroll->eventScrollChangePosition = newDelegate(this, &ItemBox::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			}
		}
		// сли нет скрола, то клиенская зона не обязательно
		//if ((nullptr == mWidgetScroll) && (nullptr == mWidgetClient)) mWidgetClient = this;
		MYGUI_ASSERT(nullptr != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// подписываем клиент для драгэндропа
		mWidgetClient->_requestGetContainer = newDelegate(this, &ItemBox::_requestGetContainer);

		updateMetrics();
		updateScroll();
	}

	void ItemBox::shutdownWidgetSkin()
	{
		mWidgetScroll = nullptr;
		mWidgetClient = nullptr;
	}

	void ItemBox::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);
	}

	void ItemBox::setSize(const IntSize & _size)
	{
		IntSize size = getSize();

		Base::setSize(_size);

		updateFromResize(size);
	}

	void ItemBox::setCoord(const IntCoord & _coord)
	{
		IntSize size = getSize();

		Base::setCoord(_coord);

		updateFromResize(size);
	}

	void ItemBox::_updateScrollWidget()
	{
		if (mWidgetScroll) {
			mWidgetScroll->setScrollPosition(mScrollPosition);
			mWidgetScroll->setScrollRange(mScrollRange + 1);
			mWidgetScroll->setScrollPage(mSizeItem.height);
			mWidgetScroll->setScrollViewPage(mSizeItem.height);
			if (mCountLines) mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * getWidgetHeight(mWidgetClient, mAlignVert) / mSizeItem.height / mCountLines );
		}
	}

	void ItemBox::updateFromResize(const IntSize& _size)
	{
		// UNUSED
		// int old_count = mCountItemInLine;
		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
		_resetContainer(true);
	}

	void ItemBox::notifyScrollChangePosition(VScrollPtr _sender, size_t _index)
	{
		mScrollPosition = (int)_index;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateAllVisible(old != mLineTop);

		_resetContainer(true);
	}

	void ItemBox::updateMetrics()
	{
		IntCoord coord(0, 0, 1, 1);

		// спрашиваем размер иконок
		requestCoordItem(this, coord, false);

		convertWidgetCoord(coord, mAlignVert);

		mSizeItem = coord.size();
		MYGUI_ASSERT((mSizeItem.width > 0 && mSizeItem.height > 0), "(mSizeItem.width > 0 && mSizeItem.height > 0)  at requestCoordWidgetItem");

		// колличество айтемов на одной строке
		mCountItemInLine = getWidgetWidth(mWidgetClient, mAlignVert) / mSizeItem.width;
		if (1 > mCountItemInLine) mCountItemInLine = 1;

		// колличество строк
		mCountLines = mCountItems / mCountItemInLine;
		if (0 != (mCountItems % mCountItemInLine)) mCountLines ++;

		// колличество строк которые помещаються в видимую часть
		mCountLineVisible = getWidgetHeight(mWidgetClient, mAlignVert) / mSizeItem.height;
		if (0 != (getWidgetHeight(mWidgetClient, mAlignVert) % mSizeItem.height)) mCountLineVisible++;
		mCountLineVisible ++; // и на одну больше для попиксельной прокрутки
	}

	void ItemBox::updateScroll()
	{
		// размерность скролла в пикселях
		mScrollRange = (mSizeItem.height * mCountLines) - getWidgetHeight(mWidgetClient, mAlignVert);

		// тестируем видимость скролла
		bool change = false;
		if (mWidgetScroll) {
			if ((mScrollRange <= 0) || (getWidgetLeft(mWidgetScroll, mAlignVert) <= getWidgetLeft(mWidgetClient, mAlignVert))) {
				if (mWidgetScroll->isVisible()) {
					change = true;
					mWidgetScroll->setVisible(false);
					// увеличиваем клиентскую зону на ширину скрола
					setWidgetSize(mWidgetClient,
						getWidgetWidth(mWidgetClient, mAlignVert) + getWidgetWidth(mWidgetScroll, mAlignVert),
						getWidgetHeight(mWidgetClient, mAlignVert), mAlignVert);
				}
			}
			else if (false == mWidgetScroll->isVisible()) {
				change = true;
				setWidgetSize(mWidgetClient,
					getWidgetWidth(mWidgetClient, mAlignVert) - getWidgetWidth(mWidgetScroll, mAlignVert),
					getWidgetHeight(mWidgetClient, mAlignVert), mAlignVert);
				mWidgetScroll->setVisible(true);
			}
		}

		// если скролл изменился, то пересчитываем
		if (change) {
			updateMetrics();
			mScrollRange = (mSizeItem.height * mCountLines) - getWidgetHeight(mWidgetClient, mAlignVert);
		}

		// тестируем текущую позицию скролла
		int count_pix = ((mCountLines - mLineTop) * mSizeItem.height) - mOffsetTop;

		if ((count_pix < getWidgetHeight(mWidgetClient, mAlignVert)) && ((mLineTop != 0) || (mOffsetTop != 0))) {
			// считаем позицию, прижатую к низу
			mLineTop = mCountLines - (getWidgetHeight(mWidgetClient, mAlignVert) / mSizeItem.height) - 1;

			if (mLineTop < 0) {
				mLineTop = 0;
				mOffsetTop = 0;
				mScrollPosition = 0;
			}
			else {
				mOffsetTop = mSizeItem.height - (getWidgetHeight(mWidgetClient, mAlignVert) % mSizeItem.height);
				mScrollPosition = (mLineTop * mSizeItem.height) + mOffsetTop;
			}

		}

		_updateScrollWidget();
	}

	void ItemBox::_updateAllVisible(bool _redraw)
	{
		size_t start = (mLineTop * mCountItemInLine);
		size_t count = (mCountLineVisible * mCountItemInLine) + start;

		size_t iwid = 0; // индекс виджета
		for (size_t pos = start; pos<count; ++pos, ++iwid) {
			// дальше нет айтемов
			if (pos >= mCountItems) break;

			WidgetPtr item = getItemWidget(iwid);
			setWidgetCoord(item, ((int)iwid % mCountItemInLine) * mSizeItem.width,
				(((int)iwid / mCountItemInLine) * mSizeItem.height)  - mOffsetTop,
				mSizeItem.width, mSizeItem.height, mAlignVert);

			item->setVisible(true);

			if (_redraw) {

				IBDrawItemInfo data(pos, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);

				requestDrawItem(this, item, data);
			}

		}

		// все виджеты еще есть, то их надо бы скрыть
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid]->setVisible(false);
			iwid ++;
		}

	}

	WidgetPtr ItemBox::getItemWidget(size_t _index)
	{
		// еще нет такого виджета, нуно создать
		if (_index == mVectorItems.size()) {

			WidgetPtr item = mWidgetClient->createWidget<Widget>("Default", IntCoord(0, 0, mSizeItem.width, mSizeItem.height), Align::Default);

			// вызываем запрос на создание виджета
			requestCreateWidgetItem(this, item);

			item->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			item->eventRootMouseChangeFocus = newDelegate(this, &ItemBox::notifyRootMouseChangeFocus);
			item->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			item->eventMouseButtonReleased = newDelegate(this, &ItemBox::notifyMouseButtonReleased);
			item->eventMouseButtonDoubleClick = newDelegate(this, &ItemBox::notifyMouseButtonDoubleClick);
			item->eventMouseDrag = newDelegate(this, &ItemBox::notifyMouseDrag);
			item->_requestGetContainer = newDelegate(this, &ItemBox::_requestGetContainer);
			item->eventKeyButtonPressed = newDelegate(this, &ItemBox::notifyKeyButtonPressed);
			item->eventKeyButtonReleased = newDelegate(this, &ItemBox::notifyKeyButtonReleased);

			item->_setInternalData((size_t)mVectorItems.size());

			mVectorItems.push_back(item);
		}

		// запрашивать только последовательно
		MYGUI_ASSERT_RANGE(_index, mVectorItems.size(), "ItemBox::getItemWidget");

		return mVectorItems[_index];
	}

	void ItemBox::onMouseWheel(int _rel)
	{
		notifyMouseWheel(nullptr, _rel);

		Base::onMouseWheel(_rel);
	}

	void ItemBox::onKeySetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		setState("pushed");

		Base::onKeySetFocus(_old);
	}

	void ItemBox::onKeyLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		setState("normal");

		Base::onKeyLostFocus(_new);
	}

	void ItemBox::notifyMouseWheel(WidgetPtr _sender, int _rel)
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
		if (false == mNeedDrop) {
			findCurrentActiveItem();
		}

		_resetContainer(true);
	}

	void ItemBox::resetCurrentActiveItem()
	{
		// сбрасываем старую подсветку
		if (mIndexActive != ITEM_NONE) {
			size_t start = (size_t)(mLineTop * mCountItemInLine);
			size_t index = mIndexActive;
			mIndexActive = ITEM_NONE;

			// если видим, то обновляем
			if ((mIndexActive >= start) && (mIndexActive < (start + mVectorItems.size()))) {
				IBDrawItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

				requestDrawItem(this, mVectorItems[mIndexActive - start], data);
			}
		}
	}

	void ItemBox::findCurrentActiveItem()
	{
		MYGUI_DEBUG_ASSERT(mIndexActive == ITEM_NONE, "use : resetCurrentActiveItem() before findCurrentActiveItem()");

		const IntPoint& point = InputManager::getInstance().getMousePosition();

		// сначала проверяем клиентскую зону
		const IntRect& rect = mWidgetClient->getAbsoluteRect();
		if ((point.left < rect.left) || (point.left > rect.right) || (point.top < rect.top) || (point.top > rect.bottom)) {
			return;
		}

		for (size_t pos=0; pos<mVectorItems.size(); ++pos) {
			WidgetPtr item = mVectorItems[pos];
			const IntRect& rect = item->getAbsoluteRect();
			if ((point.left>= rect.left) && (point.left <= rect.right) && (point.top>= rect.top) && (point.top <= rect.bottom)) {

				size_t index = *item->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				// при переборе индекс может быть больше, так как может создасться сколько угодно
				if (index < mItemsInfo.size()) {

					mIndexActive = index;
					IBDrawItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

					requestDrawItem(this, item, data);
				}

				break;
			}
		}
	}

	void ItemBox::_requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index)
	{
		if (_sender == mWidgetClient) {
			_container = this;
			_index = ITEM_NONE;
		}
		else {
			size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
			if (index < mItemsInfo.size()) {
				_container = this;
				_index = index;
			}
		}
	}

	void ItemBox::_setContainerItemInfo(size_t _index, bool _set, bool _accept)
	{
		if (_index == ITEM_NONE) return;
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::_setContainerItemInfo");

		mIndexAccept = (_set && _accept ) ? _index : ITEM_NONE;
		mIndexRefuse = (_set && !_accept) ? _index : ITEM_NONE;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

			requestDrawItem(this, mVectorItems[_index - start], data);
		}
	}

	void ItemBox::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ItemBox::setItemData");
		mItemsInfo[_index].data = _data;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);
			requestDrawItem(this, mVectorItems[_index - start], data);

		}

		_resetContainer(true);
	}

	void ItemBox::insertItemAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ItemBox::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		_resetContainer(false);

		resetCurrentActiveItem();

		mItemsInfo.insert(mItemsInfo.begin() + _index, ItemDataInfo(_data));
		mCountItems++;

		// расчитываем новый индекс выделения
		if (mIndexSelect != ITEM_NONE) {
			if (mIndexSelect >= _index) {
				mIndexSelect ++;
			}
		}

		updateMetrics();
		updateScroll();

		findCurrentActiveItem();

		_updateAllVisible(true);
	}

	void ItemBox::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ItemBox::removeItemAt");

		_resetContainer(false);
		resetCurrentActiveItem();

		mItemsInfo.erase(mItemsInfo.begin() + _index);
		mCountItems --;

		// расчитываем новый индекс выделения
		if (mIndexSelect != ITEM_NONE) {
			if (mCountItems == 0) {
				mIndexSelect = ITEM_NONE;
			}
			else if ((mIndexSelect > _index) || (mIndexSelect == mCountItems)) {
				mIndexSelect --;
			}
		}

		updateMetrics();
		updateScroll();

		findCurrentActiveItem();

		_updateAllVisible(true);
	}

	void ItemBox::removeAllItems()
	{
		if (0 == mItemsInfo.size()) return;
		_resetContainer(false);

		mItemsInfo.clear();
		mCountItems = 0;

		mIndexSelect = ITEM_NONE;
		mIndexActive = ITEM_NONE;

		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
	}

	void ItemBox::redrawItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ItemBox::redrawItemAt");

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);
			requestDrawItem(this, mVectorItems[_index - start], data);

		}
	}

	void ItemBox::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "ItemBox::setIndexSelected");
		if (_index == mIndexSelect) return;

		size_t start = (size_t)(mLineTop * mCountItemInLine);

		// сбрасываем старое выделение
		if (mIndexSelect != ITEM_NONE) {

			size_t index = mIndexSelect;
			mIndexSelect = ITEM_NONE;

			if ((index >= start) && (index < (start + mVectorItems.size()))) {
				IBDrawItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestDrawItem(this, mVectorItems[index - start], data);
			}
		}

		mIndexSelect = _index;
		if (mIndexSelect != ITEM_NONE) {

			if ((_index >= start) && (_index < (start + mVectorItems.size()))) {
				IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestDrawItem(this, mVectorItems[_index - start], data);
			}
		}

	}

	void ItemBox::notifyMouseButtonDoubleClick(WidgetPtr _sender)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::notifyMouseButtonDoubleClick");

		eventSelectItemAccept(this, index);
	}

	void ItemBox::setItemBoxAlignVert(bool _vert)
	{
		if (mAlignVert == _vert) return;
		mAlignVert = _vert;

		mCountItemInLine = -1;
		updateFromResize(IntSize());
	}

	void ItemBox::notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyPressed, _key, _char));
	}

	void ItemBox::notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyReleased, _key));
	}

	size_t ItemBox::getIndexByWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(_widget, "ItemBox::getIndexByWidget : Widget == nullptr");
		if (_widget == mWidgetClient) return ITEM_NONE;
		MYGUI_ASSERT(_widget->getParent() == mWidgetClient, "ItemBox::getIndexByWidget : Widget is not child");

		size_t index = *_widget->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::getIndexByWidget");

		return index;
	}

	size_t ItemBox::_getContainerIndex(const IntPoint & _point)
	{
		for (VectorWidgetPtr::iterator iter=mVectorItems.begin(); iter!=mVectorItems.end(); ++iter) {
			if ((*iter)->isVisible()) {
				if ((*iter)->getAbsoluteRect().inside(_point)) {
					return getIndexByWidget(*iter);
				}
			}
		}
		return ITEM_NONE;
	}

	void ItemBox::_resetContainer(bool _update)
	{
		// обязательно у базового
		Base::_resetContainer(_update);

		if ( ! _update) {
			WidgetManager & instance = WidgetManager::getInstance();
			for (VectorWidgetPtr::iterator iter=mVectorItems.begin(); iter!=mVectorItems.end(); ++iter) {
				instance.unlinkFromUnlinkers(*iter);
			}
		}
	}

	WidgetPtr ItemBox::getWidgetByIndex(size_t _index)
	{
		for (VectorWidgetPtr::iterator iter=mVectorItems.begin(); iter!=mVectorItems.end(); ++iter) {
			if ((*iter)->isVisible()) {
				size_t index = *(*iter)->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::getWidgetByIndex");

				if (index == _index) return (*iter);
			}
		}
		return nullptr;
	}

	void ItemBox::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void ItemBox::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void ItemBox::onMouseDrag(int _left, int _top)
	{
		Base::onMouseDrag(_left, _top);
	}

	void ItemBox::removeDropItems()
	{
		if (mItemDrag) mItemDrag->setVisible(false);
	}

	void ItemBox::updateDropItems()
	{
		if (nullptr == mItemDrag) {
			// спрашиваем размер иконок
			IntCoord coord;
			mPointDragOffset = coord.point();

			requestCoordItem(this, coord, true);

			convertWidgetCoord(coord, mAlignVert);

			// создаем и запрашиваем детей
			mItemDrag = Gui::getInstance().createWidget<Widget>("Default", IntCoord(0, 0, coord.width, coord.height), Align::Default, mDragLayer);
			requestCreateWidgetItem(this, mItemDrag);
		}

		const IntPoint & point = InputManager::getInstance().getMousePosition();

		mItemDrag->setPosition(point.left - mClickInWidget.left + mPointDragOffset.left, point.top - mClickInWidget.top + mPointDragOffset.top);
		mItemDrag->setVisible(true);
	}

	void ItemBox::updateDropItemsState(const DDWidgetState & _state)
	{
		IBDrawItemInfo data;
		data.drop_accept = _state.accept;
		data.drop_refuse = _state.refuse;

		data.select = false;
		data.active = false;

		data.index = mDropSenderIndex;
		data.update = _state.update;
		data.drag = true;

		requestDrawItem(this, mItemDrag, data);
	}

	void ItemBox::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		mouseDrag();
	}

	void ItemBox::notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		mouseButtonPressed(_id);

		if ( MouseButton::Left == _id) {
			size_t old = mIndexSelect;

			if (_sender == mWidgetClient) {
				// сбрасываем выделение
				setIndexSelected(ITEM_NONE);
			}
			else {
				// индекс отправителя
				mDropSenderIndex = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				MYGUI_ASSERT_RANGE(mDropSenderIndex, mItemsInfo.size(), "ItemBox::notifyMouseButtonPressed");

				// выделенный елемент
				setIndexSelected(mDropSenderIndex);
			}

			// смещение внутри виджета, куда кликнули мышкой
			mClickInWidget = InputManager::getInstance().getLastLeftPressed() - _sender->getAbsolutePosition();

			// отсылаем событие
			eventMouseItemActivate(this, mIndexSelect);
			// смену позиции отсылаем только при реальном изменении
			if (old != mIndexSelect) eventChangeItemPosition(this, mIndexSelect);
		}

		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::MousePressed, _left, _top, _id));
	}

	void ItemBox::notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		mouseButtonReleased(_id);
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		// солличество айтемов может измениться
		if (index >= getItemCount()) return;
		eventNotifyItem(this, IBNotifyItemData(index, IBNotifyItemData::MouseReleased, _left, _top, _id));
	}

	void ItemBox::notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		if (_focus) {
			MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::notifyRootMouseChangeFocus");

			// сбрасываем старый
			if (mIndexActive != ITEM_NONE) {
				size_t old_index = mIndexActive;
				mIndexActive = ITEM_NONE;
				IBDrawItemInfo data(old_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestDrawItem(this, mVectorItems[old_index - (mLineTop * mCountItemInLine)], data);
			}

			mIndexActive = index;
			IBDrawItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
			requestDrawItem(this, mVectorItems[*_sender->_getInternalData<size_t>()], data);
		}
		else {
			// при сбросе виджет может быть уже скрыт, и соответсвенно отсутсвовать индекс
			// сбрасываем индекс, только если мы и есть актив
			if (index < mItemsInfo.size() && mIndexActive == index) {
				mIndexActive = ITEM_NONE;
				IBDrawItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestDrawItem(this, mVectorItems[*_sender->_getInternalData<size_t>()], data);
			}
		}
	}

} // namespace MyGUI
