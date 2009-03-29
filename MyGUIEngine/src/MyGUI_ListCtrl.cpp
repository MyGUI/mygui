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
#include "MyGUI_ListCtrl.h"
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

	ListCtrl::ListCtrl(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
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
		mVScroll(nullptr),
		mHScroll(nullptr),
		mShowHScroll(true),
		mShowVScroll(true),
		mVRange(0),
		mHRange(0)
	{
		initialiseWidgetSkin(_info);
	}

	ListCtrl::~ListCtrl()
	{
		shutdownWidgetSkin();
	}

	void ListCtrl::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void ListCtrl::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		// нам нужен фокус клавы
		mNeedKeyFocus = true;
		mDragLayer = "DragAndDrop";

		const MapString & properties = _info->getProperties();
		if (false == properties.empty())
		{
			MapString::const_iterator iter = properties.find("DragLayer");
			if (iter != properties.end()) mDragLayer = iter->second;
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "VScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = (*iter)->castType<HScroll>();
				mVScroll->eventScrollChangePosition = newDelegate(this, &ListCtrl::notifyScrollChangePosition);
			}
			if (*(*iter)->_getInternalData<std::string>() == "HScroll")
			{
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = (*iter)->castType<HScroll>();
				mHScroll->eventScrollChangePosition = newDelegate(this, &ListCtrl::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "Client")
			{
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ListCtrl::notifyMouseWheel);
				mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ListCtrl::notifyMouseButtonPressed);
			}
		}
		// сли нет скрола, то клиенская зона не обязательно
		MYGUI_ASSERT(nullptr != mWidgetClient, "Child Widget Client not found in skin (ListCtrl must have Client)");

		// подписываем клиент для драгэндропа
		mWidgetClient->_requestGetContainer = newDelegate(this, &ListCtrl::_requestGetContainer);

		updateMetrics();
		updateScroll();
	}

	void ListCtrl::shutdownWidgetSkin()
	{
		mWidgetClient = nullptr;
		mVScroll = nullptr;
		mHScroll = nullptr;
	}

	void ListCtrl::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);
	}

	void ListCtrl::setSize(const IntSize & _size)
	{
		IntSize size = getSize();

		Base::setSize(_size);

		updateFromResize(size);
	}

	void ListCtrl::setCoord(const IntCoord & _coord)
	{
		IntSize size = getSize();

		Base::setCoord(_coord);

		updateFromResize(size);
	}

	void ListCtrl::updateFromResize(const IntSize& _size)
	{
		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
		_resetContainer(true);
	}

	void ListCtrl::_updateScrollWidget()
	{
		/*if (mWidgetScroll)
		{
			mWidgetScroll->setScrollPosition(mScrollPosition);
			mWidgetScroll->setScrollRange(mScrollRange + 1);
			mWidgetScroll->setScrollPage(mSizeItem.height);
			mWidgetScroll->setScrollViewPage(mSizeItem.height);
			if (mCountLines) mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * mWidgetClient->getHeight() / mSizeItem.height / mCountLines );
		}*/
	}

	void ListCtrl::notifyScrollChangePosition(VScrollPtr _sender, size_t _index)
	{
		/*mScrollPosition = (int)_index;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateAllVisible(old != mLineTop);

		_resetContainer(true);*/
	}

	void ListCtrl::updateMetrics()
	{
		/*IntCoord coord(0, 0, 50, 50);

		mSizeItem = coord.size();
		MYGUI_ASSERT((mSizeItem.width > 0 && mSizeItem.height > 0), "(mSizeItem.width > 0 && mSizeItem.height > 0)  at requestCoordWidgetItem");

		// колличество айтемов на одной строке
		mCountItemInLine = mWidgetClient->getWidth() / mSizeItem.width;
		if (1 > mCountItemInLine) mCountItemInLine = 1;

		// колличество строк
		mCountLines = mCountItems / mCountItemInLine;
		if (0 != (mCountItems % mCountItemInLine)) mCountLines ++;

		// колличество строк которые помещаються в видимую часть
		mCountLineVisible = mWidgetClient->getHeight() / mSizeItem.height;
		if (0 != (mWidgetClient->getHeight() % mSizeItem.height)) mCountLineVisible++;
		mCountLineVisible ++; // и на одну больше для попиксельной прокрутки*/
	}

	void ListCtrl::updateScroll()
	{
		// размерность скролла в пикселях
		/*mScrollRange = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();

		// тестируем видимость скролла
		bool change = false;
		if (mWidgetScroll) {
			if ((mScrollRange <= 0) || (mWidgetScroll->getLeft() <= mWidgetClient->getHeight())) {
				if (mWidgetScroll->isVisible()) {
					change = true;
					mWidgetScroll->setVisible(false);
					// увеличиваем клиентскую зону на ширину скрола
					mWidgetClient->setSize(
						mWidgetClient->getWidth() + mWidgetScroll->getWidth(),
						mWidgetClient->getHeight());
				}
			}
			else if (false == mWidgetScroll->isVisible()) {
				change = true;
				mWidgetClient->setSize(
					mWidgetClient->getWidth() - mWidgetScroll->getWidth(),
					mWidgetClient->getHeight());
				mWidgetScroll->setVisible(true);
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

		_updateScrollWidget();*/
	}

	void ListCtrl::_updateAllVisible(bool _redraw)
	{
		size_t start = (mLineTop * mCountItemInLine);
		size_t count = (mCountLineVisible * mCountItemInLine) + start;

		size_t iwid = 0; // индекс виджета
		for (size_t pos = start; pos<count; ++pos, ++iwid) {
			// дальше нет айтемов
			if (pos >= mCountItems) break;

			WidgetPtr item = getItemWidget(iwid);
			item->setCoord(((int)iwid % mCountItemInLine) * mSizeItem.width,
				(((int)iwid / mCountItemInLine) * mSizeItem.height)  - mOffsetTop,
				mSizeItem.width, mSizeItem.height);

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

	WidgetPtr ListCtrl::getItemWidget(size_t _index)
	{
		// еще нет такого виджета, нуно создать
		if (_index == mVectorItems.size())
		{

			WidgetPtr item = mWidgetClient->createWidget<Widget>("Default", IntCoord(0, 0, mSizeItem.width, mSizeItem.height), Align::Default);

			// вызываем запрос на создание виджета
			requestCreateWidgetItem(this, item);

			item->eventMouseWheel = newDelegate(this, &ListCtrl::notifyMouseWheel);
			item->eventRootMouseChangeFocus = newDelegate(this, &ListCtrl::notifyRootMouseChangeFocus);
			item->eventMouseButtonPressed = newDelegate(this, &ListCtrl::notifyMouseButtonPressed);
			item->eventMouseButtonReleased = newDelegate(this, &ListCtrl::notifyMouseButtonReleased);
			item->eventMouseButtonDoubleClick = newDelegate(this, &ListCtrl::notifyMouseButtonDoubleClick);
			item->eventMouseDrag = newDelegate(this, &ListCtrl::notifyMouseDrag);
			item->_requestGetContainer = newDelegate(this, &ListCtrl::_requestGetContainer);
			item->eventKeyButtonPressed = newDelegate(this, &ListCtrl::notifyKeyButtonPressed);
			item->eventKeyButtonReleased = newDelegate(this, &ListCtrl::notifyKeyButtonReleased);

			item->_setInternalData((size_t)mVectorItems.size());

			mVectorItems.push_back(item);
		}

		// запрашивать только последовательно
		MYGUI_ASSERT_RANGE(_index, mVectorItems.size(), "ListCtrl::getItemWidget");

		return mVectorItems[_index];
	}

	void ListCtrl::onMouseWheel(int _rel)
	{
		notifyMouseWheel(nullptr, _rel);

		Base::onMouseWheel(_rel);
	}

	void ListCtrl::onKeySetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		setState("pushed");

		Base::onKeySetFocus(_old);
	}

	void ListCtrl::onKeyLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		setState("normal");

		Base::onKeyLostFocus(_new);
	}

	void ListCtrl::notifyMouseWheel(WidgetPtr _sender, int _rel)
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

	void ListCtrl::resetCurrentActiveItem()
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

	void ListCtrl::findCurrentActiveItem()
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

	void ListCtrl::_requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index)
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

	void ListCtrl::_setContainerItemInfo(size_t _index, bool _set, bool _accept)
	{
		if (_index == ITEM_NONE) return;
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListCtrl::_setContainerItemInfo");

		mIndexAccept = (_set && _accept ) ? _index : ITEM_NONE;
		mIndexRefuse = (_set && !_accept) ? _index : ITEM_NONE;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

			requestDrawItem(this, mVectorItems[_index - start], data);
		}
	}

	void ListCtrl::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ListCtrl::setItemData");
		mItemsInfo[_index].data = _data;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);
			requestDrawItem(this, mVectorItems[_index - start], data);

		}

		_resetContainer(true);
	}

	void ListCtrl::insertItemAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ListCtrl::insertItemAt");
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

	void ListCtrl::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ListCtrl::removeItemAt");

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

	void ListCtrl::removeAllItems()
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

	void ListCtrl::redrawItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ListCtrl::redrawItemAt");

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			IBDrawItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);
			requestDrawItem(this, mVectorItems[_index - start], data);

		}
	}

	void ListCtrl::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "ListCtrl::setIndexSelected");
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

	void ListCtrl::notifyMouseButtonDoubleClick(WidgetPtr _sender)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ListCtrl::notifyMouseButtonDoubleClick");

		eventSelectItemAccept(this, index);
	}

	void ListCtrl::notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyPressed, _key, _char));
	}

	void ListCtrl::notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyReleased, _key));
	}

	size_t ListCtrl::getIndexByWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(_widget, "ListCtrl::getIndexByWidget : Widget == nullptr");
		if (_widget == mWidgetClient) return ITEM_NONE;
		MYGUI_ASSERT(_widget->getParent() == mWidgetClient, "ListCtrl::getIndexByWidget : Widget is not child");

		size_t index = *_widget->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ListCtrl::getIndexByWidget");

		return index;
	}

	size_t ListCtrl::_getContainerIndex(const IntPoint & _point)
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

	void ListCtrl::_resetContainer(bool _update)
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

	WidgetPtr ListCtrl::getWidgetByIndex(size_t _index)
	{
		for (VectorWidgetPtr::iterator iter=mVectorItems.begin(); iter!=mVectorItems.end(); ++iter) {
			if ((*iter)->isVisible()) {
				size_t index = *(*iter)->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ListCtrl::getWidgetByIndex");

				if (index == _index) return (*iter);
			}
		}
		return nullptr;
	}

	void ListCtrl::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void ListCtrl::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void ListCtrl::onMouseDrag(int _left, int _top)
	{
		Base::onMouseDrag(_left, _top);
	}

	void ListCtrl::removeDropItems()
	{
		if (mItemDrag) mItemDrag->setVisible(false);
	}

	void ListCtrl::updateDropItems()
	{
		if (nullptr == mItemDrag)
		{
			// спрашиваем размер иконок
			IntCoord coord(0, 0, 50, 50);
			mPointDragOffset = coord.point();

			// создаем и запрашиваем детей
			mItemDrag = Gui::getInstance().createWidget<Widget>("Default", IntCoord(0, 0, coord.width, coord.height), Align::Default, mDragLayer);
			requestCreateWidgetItem(this, mItemDrag);
		}

		const IntPoint & point = InputManager::getInstance().getMousePosition();

		mItemDrag->setPosition(point.left - mClickInWidget.left + mPointDragOffset.left, point.top - mClickInWidget.top + mPointDragOffset.top);
		mItemDrag->setVisible(true);
	}

	void ListCtrl::updateDropItemsState(const DDWidgetState & _state)
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

	void ListCtrl::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		mouseDrag();
	}

	void ListCtrl::notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
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
				MYGUI_ASSERT_RANGE(mDropSenderIndex, mItemsInfo.size(), "ListCtrl::notifyMouseButtonPressed");

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

	void ListCtrl::notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		mouseButtonReleased(_id);
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::MouseReleased, _left, _top, _id));
	}

	void ListCtrl::notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		if (_focus) {
			MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ListCtrl::notifyRootMouseChangeFocus");

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

	/*void ListCtrl::updateScroll()
	{
		IntSize textSize = mText->getTextSize();

		// вертикальный текст не помещается
		if (textSize.height > mText->getHeight()) {
			if (mVScroll != nullptr) {
				if (( ! mVScroll->isVisible()) && (mShowVScroll)) {
					mVScroll->setVisible(true);
					mWidgetClient->setSize(mWidgetClient->getWidth() - mVScroll->getWidth(), mWidgetClient->getHeight());

					// размер текста может измениться
					textSize = mText->getTextSize();

					if (mHScroll != nullptr) {
						mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

						// если показали вертикальный скрол бар, уменьшилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет показа
						if ((textSize.width > mText->getWidth()) && ( ! mHScroll->isVisible()) && (mShowHScroll)) {
							mHScroll->setVisible(true);
							mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() - mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

							// размер текста может измениться
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}
		// вертикальный текст помещается
		else {
			if (mVScroll != nullptr) {
				if (mVScroll->isVisible()) {
					mVScroll->setVisible(false);
					mWidgetClient->setSize(mWidgetClient->getWidth() + mVScroll->getWidth(), mWidgetClient->getHeight());

					// размер текста может измениться
					textSize = mText->getTextSize();

					if (mHScroll != nullptr) {
						mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

						// если скрыли вертикальный скрол бар, увеличилось вью по горизонтали,
						// пересчитываем горизонтальный скрол на предмет скрытия
						if ((textSize.width <= mText->getWidth()) && (mHScroll->isVisible())) {
							mHScroll->setVisible(false);
							mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() + mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

							// размер текста может измениться
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}


		// горизонтальный текст не помещается
		if (textSize.width > mText->getWidth()) {
			if (mHScroll != nullptr) {
				if (( ! mHScroll->isVisible()) && (mShowHScroll)) {
					mHScroll->setVisible(true);
					mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() - mHScroll->getHeight());

					// размер текста может измениться
					textSize = mText->getTextSize();

					if (mVScroll != nullptr) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

						// если показали горизонтальный скрол бар, уменьшилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет показа
						if ((textSize.height > mText->getHeight()) && ( ! mVScroll->isVisible()) && (mShowVScroll)) {
							mVScroll->setVisible(true);
							mWidgetClient->setSize(mWidgetClient->getWidth() - mVScroll->getWidth(), mWidgetClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

							// размер текста может измениться
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}
		// горизонтальный текст помещается
		else {
			if (mHScroll != nullptr) {
				if (mHScroll->isVisible()) {
					mHScroll->setVisible(false);
					mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() + mHScroll->getHeight());

					// размер текста может измениться
					textSize = mText->getTextSize();

					if (mVScroll != nullptr) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

						// если скрыли горизонтальный скрол бар, увеличилось вью по вертикали,
						// пересчитываем вертикальный скрол на предмет скрытия
						if ((textSize.height <= mText->getHeight()) && (mVScroll->isVisible())) {
							mVScroll->setVisible(false);
							mWidgetClient->setSize(mWidgetClient->getWidth() + mVScroll->getWidth(), mWidgetClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

							// размер текста может измениться
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}

		mVRange = (mText->getHeight() >= textSize.height) ? 0 : textSize.height - mText->getHeight();
		mHRange = (mText->getWidth() >= textSize.width) ? 0 : textSize.width - mText->getWidth();

		size_t page = getFontHeight();
		if (mVScroll != nullptr) {
			mVScroll->setScrollPage(page);
			mVScroll->setScrollViewPage(mCoord.width > (int)page ? mCoord.width : page);
			mVScroll->setScrollRange(mVRange + 1);
			if (textSize.height) mVScroll->setTrackSize(int (float(mVScroll->getLineSize() * mText->getHeight()) / float(textSize.height)));
		}
		if (mHScroll != nullptr) {
			mHScroll->setScrollPage(page);
			mHScroll->setScrollViewPage(mCoord.height > (int)page ? mCoord.height : page);
			mHScroll->setScrollRange(mHRange + 1);
			if (textSize.width) mHScroll->setTrackSize(int (float(mHScroll->getLineSize() * mText->getWidth()) / float(textSize.width)));
		}

	}*/

} // namespace MyGUI
