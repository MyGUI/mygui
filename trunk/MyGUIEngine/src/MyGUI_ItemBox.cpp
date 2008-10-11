/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
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

	MYGUI_RTTI_CHILD_IMPLEMENT( ItemBox, Widget );

	ItemBox::ItemBox(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mWidgetScroll(null),
		mLineTop(0),
		mOffsetTop(0),
		mIsFocus(false),
		mCountItems(0),
		mScrollRange(0),
		mScrollPosition(0),
		mIndexSelect(ITEM_NONE),
		mIndexActive(ITEM_NONE),
		mIndexAccept(ITEM_NONE),
		mIndexRefuse(ITEM_NONE),
		mOldDrop(null),
		mDropResult(false),
		mCurrentSender(null),
		mStartDrop(false),
		mNeedDrop(false),
		mDropSenderIndex(ITEM_NONE),
		mAlignVert(true)
	{
		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		const MapString & properties = _info->getProperties();
		if (false == properties.empty()) {
			MapString::const_iterator iter = properties.find("AlignVert");
			if (iter != properties.end()) mAlignVert = utility::parseBool(iter->second);
		}

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
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
		// ��� ��� ������, �� ��������� ���� �� �����������
		if ((null == mWidgetScroll) && (null == mWidgetClient)) mWidgetClient = this;
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// ����������� ������ ��� �����������
		mWidgetClient->requestGetContainer = newDelegate(this, &ItemBox::requestGetContainer);

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
			if (mCountLines) mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * getWidgetHeight(mWidgetClient, mAlignVert) / mSizeItem.height / mCountLines );
		}
	}

	void ItemBox::updateFromResize(const IntSize& _size)
	{
		int old_count = mCountItemInLine;
		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
		resetContainer(true);
	}

	void ItemBox::notifyScrollChangePosition(WidgetPtr _sender, size_t _index)
	{
		mScrollPosition = (int)_index;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateAllVisible(old != mLineTop);

		resetContainer(true);
	}

	void ItemBox::updateMetrics()
	{
		IntCoord coord(0, 0, 1, 1);
		// ���������� ������ ������
		requestCoordWidgetItem(this, coord, false);
		convertWidgetCoord(coord, mAlignVert);

		mSizeItem = coord.size();
		MYGUI_ASSERT((mSizeItem.width > 0 && mSizeItem.height > 0), "(mSizeItem.width > 0 && mSizeItem.height > 0)  at requestCoordWidgetItem");

		// ����������� ������� �� ����� ������
		mCountItemInLine = getWidgetWidth(mWidgetClient, mAlignVert) / mSizeItem.width;
		if (1 > mCountItemInLine) mCountItemInLine = 1;

		// ����������� �����
		mCountLines = (int)mCountItems / mCountItemInLine;
		if (0 != ((int)mCountItems % mCountItemInLine)) mCountLines ++;

		// ����������� ����� ������� ����������� � ������� �����
		mCountLineVisible = getWidgetHeight(mWidgetClient, mAlignVert) / mSizeItem.height;
		if (0 != (getWidgetHeight(mWidgetClient, mAlignVert) % mSizeItem.height)) mCountLineVisible++;
		mCountLineVisible ++; // � �� ���� ������ ��� ������������ ���������
	}

	void ItemBox::updateScroll()
	{
		// ����������� ������� � ��������
		mScrollRange = (mSizeItem.height * mCountLines) - getWidgetHeight(mWidgetClient, mAlignVert);

		// ��������� ��������� �������
		bool change = false;
		if (mWidgetScroll) {
			if ((mScrollRange <= 0) || (getWidgetLeft(mWidgetScroll, mAlignVert) <= getWidgetLeft(mWidgetClient, mAlignVert))) {
				if (mWidgetScroll->isShow()) {
					change = true;
					mWidgetScroll->hide();
					// ����������� ���������� ���� �� ������ ������
					setWidgetSize(mWidgetClient, 
						getWidgetWidth(mWidgetClient, mAlignVert) + getWidgetWidth(mWidgetScroll, mAlignVert),
						getWidgetHeight(mWidgetClient, mAlignVert), mAlignVert);
				}
			}
			else if (false == mWidgetScroll->isShow()) {
				change = true;
				setWidgetSize(mWidgetClient,
					getWidgetWidth(mWidgetClient, mAlignVert) - getWidgetWidth(mWidgetScroll, mAlignVert),
					getWidgetHeight(mWidgetClient, mAlignVert), mAlignVert);
				mWidgetScroll->show();
			}
		}

		// ���� ������ ���������, �� �������������
		if (change) {
			updateMetrics();
			mScrollRange = (mSizeItem.height * mCountLines) - getWidgetHeight(mWidgetClient, mAlignVert);
		}
		
		// ��������� ������� ������� �������
		int count_pix = ((mCountLines - mLineTop) * mSizeItem.height) - mOffsetTop;

		if ((count_pix < getWidgetHeight(mWidgetClient, mAlignVert)) && ((mLineTop != 0) || (mOffsetTop != 0))) {
			// ������� �������, �������� � ����
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
		int start = (mLineTop * mCountItemInLine);
		int count = (mCountLineVisible * mCountItemInLine) + start;

		size_t iwid = 0; // ������ �������
		for (int pos = start; pos<count; ++pos, ++iwid) {
			// ������ ��� �������
			if (pos >= mCountItems) break;

			WidgetPtr item = getItemWidget(iwid);
			setWidgetPosition(item, ((int)iwid % mCountItemInLine) * mSizeItem.width,
				(((int)iwid / mCountItemInLine) * mSizeItem.height)  - mOffsetTop,
				mSizeItem.width, mSizeItem.height, mAlignVert);

			item->show();

			if (_redraw) {

				ItemInfo data(pos, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);

				requestUpdateWidgetItem(this, item, data);
			}

		}

		// ��� ������� ��� ����, �� �� ���� �� ������
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid]->hide();
			iwid ++;
		}

	}

	WidgetPtr ItemBox::getItemWidget(size_t _index)
	{
		// ��� ��� ������ �������, ���� �������
		if (_index == mVectorItems.size()) {

			WidgetPtr item = mWidgetClient->createWidget<Widget>("Default", IntCoord(0, 0, mSizeItem.width, mSizeItem.height), Align::Default);

			// �������� ������ �� �������� �������
			requestCreateWidgetItem(this, item);

			item->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			item->eventMouseSetFocus = newDelegate(this, &ItemBox::notifyMouseSetFocus);
			item->eventMouseLostFocus = newDelegate(this, &ItemBox::notifyMouseLostFocus);
			item->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			item->eventMouseButtonReleased = newDelegate(this, &ItemBox::notifyMouseButtonReleased);
			item->eventMouseButtonDoubleClick = newDelegate(this, &ItemBox::notifyMouseButtonDoubleClick);
			item->eventMouseDrag = newDelegate(this, &ItemBox::notifyMouseDrag);
			item->requestGetContainer = newDelegate(this, &ItemBox::requestGetContainer);
			item->eventKeyButtonPressed = newDelegate(this, &ItemBox::notifyKeyButtonPressed);
			item->eventKeyButtonReleased = newDelegate(this, &ItemBox::notifyKeyButtonReleased);

			item->_setInternalData((size_t)mVectorItems.size());

			mVectorItems.push_back(item);
		}

		// ����������� ������ ���������������
		MYGUI_ASSERT_RANGE(_index, mVectorItems.size(), "ItemBox::getItemWidget");

		return mVectorItems[_index];
	}

	void ItemBox::onMouseWheel(int _rel)
	{
		notifyMouseWheel(null, _rel);

		// !!! ����������� �������� � ����� ������
		Widget::onMouseWheel(_rel);
	}

	void ItemBox::onKeySetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		setState("select");

		// !!! ����������� �������� � ����� ������
		Widget::onKeySetFocus(_old);
	}

	void ItemBox::onKeyLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		setState("normal");

		// !!! ����������� �������� � ����� ������
		Widget::onKeyLostFocus(_new);
	}

	void ItemBox::notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::notifyMouseSetFocus");

		mIndexActive = index;
		ItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

		requestUpdateWidgetItem(this, mVectorItems[*_sender->_getInternalData<size_t>()], data);
		mIndexActive = index;
	}

	void ItemBox::notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		// ��� �������� �����
		if (mIndexActive == ITEM_NONE) return;

		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::notifyMouseLostFocus");

		mIndexActive = ITEM_NONE;
		ItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

		requestUpdateWidgetItem(this, mVectorItems[*_sender->_getInternalData<size_t>()], data);
		mIndexActive = ITEM_NONE;
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

		// ���������� ������ ���������
		// ��� ��� ��� ���������, ���� ����� ���������� ��� �����
		resetCurrentActiveItem();

		mScrollPosition = offset;
		int old = mLineTop;
		mLineTop = mScrollPosition / mSizeItem.height;
		mOffsetTop = mScrollPosition % mSizeItem.height;

		_updateScrollWidget();
		_updateAllVisible(old != mLineTop);

		// ������ ���� � ������������ �����
		if (false == mNeedDrop) {
			findCurrentActiveItem();
		}

		resetContainer(true);
	}

	void ItemBox::resetCurrentActiveItem()
	{
		// ���������� ������ ���������
		if (mIndexActive != ITEM_NONE) {
			size_t start = (size_t)(mLineTop * mCountItemInLine);
			size_t index = mIndexActive;
			mIndexActive = ITEM_NONE;

			// ���� �����, �� ���������
			if ((mIndexActive >= start) && (mIndexActive < (start + mVectorItems.size()))) {
				ItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

				requestUpdateWidgetItem(this, mVectorItems[mIndexActive - start], data);
			}
		}
	}

	void ItemBox::findCurrentActiveItem()
	{
		MYGUI_DEBUG_ASSERT(mIndexActive == ITEM_NONE, "use resetCurrentActiveItem() before findCurrentActiveItem()");

		const IntPoint& point = InputManager::getInstance().getMousePosition();

		// ������� ��������� ���������� ����
		const IntRect& rect = mWidgetClient->getAbsoluteRect();
		if ((point.left < rect.left) || (point.left > rect.right) || (point.top < rect.top) || (point.top > rect.bottom)) {
			return;
		}

		for (size_t pos=0; pos<mVectorItems.size(); ++pos) {
			WidgetPtr item = mVectorItems[pos];
			const IntRect& rect = item->getAbsoluteRect();
			if ((point.left>= rect.left) && (point.left <= rect.right) && (point.top>= rect.top) && (point.top <= rect.bottom)) {

				size_t index = *item->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				// ��� �������� ������ ����� ���� ������, ��� ��� ����� ���������� ������� ������
				if (index < mItemsInfo.size()) {

					mIndexActive = index;
					ItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

					requestUpdateWidgetItem(this, item, data);
				}

				break;
			}
		}
	}

	void ItemBox::requestGetContainer(WidgetPtr _sender, WidgetPtr & _list, size_t & _index)
	{
		if (_sender == mWidgetClient) {
			_list = this;
			_index = ITEM_NONE;
		}
		else {
			size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
			if (index < mItemsInfo.size()) {
				_list = this;
				_index = index;
			}
		}
	}

	void ItemBox::setContainerItemInfo(size_t _index, bool _set, bool _accept)
	{
		if (_index == ITEM_NONE) return;
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::setContainerItemInfo");

		mIndexAccept = (_set && _accept ) ? _index : ITEM_NONE;
		mIndexRefuse = (_set && !_accept) ? _index : ITEM_NONE;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			ItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);

			requestUpdateWidgetItem(this, mVectorItems[_index - start], data);
		}
	}

	void ItemBox::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size() , "ItemBox::setItemData");
		mItemsInfo[_index].data = _data;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {

			ItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, true, false);
			requestUpdateWidgetItem(this, mVectorItems[_index - start], data);

		}

		resetContainer(true);
	}

	void ItemBox::insertItemAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ItemBox::insertItemAt");
		if (_index == ITEM_NONE) _index = mItemsInfo.size();

		resetContainer(false);

		resetCurrentActiveItem();

		mItemsInfo.insert(mItemsInfo.begin() + _index, ItemDataInfo(_data));
		mCountItems++;

		// ����������� ����� ������ ���������
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

		resetContainer(false);
		resetCurrentActiveItem();

		mItemsInfo.erase(mItemsInfo.begin() + _index);
		mCountItems --;

		// ����������� ����� ������ ���������
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
		resetContainer(false);

		mItemsInfo.clear();
		mCountItems = 0;

		mIndexSelect = ITEM_NONE;
		mIndexActive = ITEM_NONE;

		updateMetrics();
		updateScroll();

		_updateAllVisible(true);
	}

	void ItemBox::setItemSelectedAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "ItemBox::setItemSelectedAt");
		if (_index == mIndexSelect) return;

		size_t start = (size_t)(mLineTop * mCountItemInLine);

		// ���������� ������ ���������
		if (mIndexSelect != ITEM_NONE) {

			size_t index = mIndexSelect;
			mIndexSelect = ITEM_NONE;

			if ((index >= start) && (index < (start + mVectorItems.size()))) {
				ItemInfo data(index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestUpdateWidgetItem(this, mVectorItems[index - start], data);
			}
		}

		mIndexSelect = _index;
		if (mIndexSelect != ITEM_NONE) {

			if ((_index >= start) && (_index < (start + mVectorItems.size()))) {
				ItemInfo data(_index, mIndexSelect, mIndexActive, mIndexAccept, mIndexRefuse, false, false);
				requestUpdateWidgetItem(this, mVectorItems[_index - start], data);
			}
		}

	}

	void ItemBox::notifyInvalideDrop(WidgetPtr _sender)
	{
		const IntPoint& point = InputManager::getInstance().getMousePosition();
		notifyMouseDrag(null, point.left, point.top);
	}

	void ItemBox::notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if ( MB_Left == _id) {
			size_t old = mIndexSelect;

			// ���������� ���� ��� �����
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();

			if (_sender == mWidgetClient) {
				// ���������� ���������
				setItemSelectedAt(ITEM_NONE);
			}
			else {
				// ������ �����������
				mDropSenderIndex = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				MYGUI_ASSERT_RANGE(mDropSenderIndex, mItemsInfo.size(), "ItemBox::notifyMouseButtonPressed");

				// ���������� �������
				setItemSelectedAt(mDropSenderIndex);
			}

			// ����������, ����� ��������� ������� ������
			mCurrentSender = null;
			mStartDrop = false;
			// �������� ������ �������, ���� �������� ������
			mClickInWidget = InputManager::getInstance().getLastLeftPressed() - _sender->getAbsolutePosition();

			// �������� �������
			eventMouseItemActivate(mWidgetEventSender, mIndexSelect);
			// ����� ������� �������� ������ ��� �������� ���������
			if (old != mIndexSelect) eventChangeItemPosition(mWidgetEventSender, mIndexSelect);
		}
		// ���� ������ ������ ������� � ��� ���� �� ����������
		else {
			endDrop(true);
		}
			

		eventNotifyItem(this, NotifyItemData(getIndexByWidget(_sender), NOTIFY_MOUSE_PRESSED, _left, _top, _id));
	}

	void ItemBox::notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		if ( MB_Left == _id) {
			endDrop(false);
		}

		eventNotifyItem(this, NotifyItemData(getIndexByWidget(_sender), NOTIFY_MOUSE_RELEASED, _left, _top, _id));
	}

	void ItemBox::endDrop(bool _reset)
	{
		if (mStartDrop) {
			if (mItemDrag) mItemDrag->hide();

			// ���������� ������ ���������
			if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

			if (_reset) mDropResult = false;
			eventEndDrop(this, mDropInfo, mDropResult);
			eventDropState(this, DROP_END);
			enableToolTip(true);

			// ���������� ���� ��� �����
			mStartDrop = false;
			mDropResult = false;
			mOldDrop = null;
			mDropInfo.reset();
			mDropSenderIndex = ITEM_NONE;
		}
	}

	void ItemBox::notifyMouseButtonDoubleClick(WidgetPtr _sender)
	{
		size_t index = *_sender->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::notifyMouseButtonDoubleClick");

		eventSelectItemAccept(this, index);
	}

	void ItemBox::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		// ����� �� �������� ������
		bool update = false;

		// ������ ��� ������� �������
		if (false == mStartDrop) {
			mStartDrop = true;
			mNeedDrop = false;
			update = true;
			// ������ �� �������� ����� �� �������
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			eventStartDrop(this, mDropInfo, mNeedDrop);

			if (mNeedDrop) {
				eventDropState(this, DROP_START);
				enableToolTip(false);
			}
			else {
				// ���������� ����� ���� (�� �����������)
				InputManager::getInstance().resetMouseCaptureWidget();
			}
		}

		// ���� �� �����
		if (false == mNeedDrop) {
			return;
		}

		// ������ ������, ��� ��� ���� ����
		WidgetPtr item = InputManager::getInstance().getWidgetFromPoint(_left, _top);

		if (null == mItemDrag) {
			// ���������� ������ ������
			IntCoord coord;
			mPointDragOffset = coord.point();
			requestCoordWidgetItem(this, coord, true);
			convertWidgetCoord(coord, mAlignVert);

			// ������� � ����������� �����
			mItemDrag = Gui::getInstance().createWidget<Widget>("Default", IntCoord(0, 0, coord.width, coord.height), Align::Default, "DragAndDrop");
			requestCreateWidgetItem(this, mItemDrag);
		}

		//IntPoint point = InputManager::getInstance().getLastLeftPressed() - _sender2->getAbsolutePosition();
		mItemDrag->setPosition(_left - mClickInWidget.left + mPointDragOffset.left, _top - mClickInWidget.top + mPointDragOffset.top);
		mItemDrag->show();

		// ���� �����, ������ ��� ����������
		if (mOldDrop == item) return;

		// ���������� ������ ���������
		if (mDropInfo.reseiver) mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		WidgetPtr reseiver = null;
		size_t reseiver_index = ITEM_NONE;
		// ���� ������ ��� ����
		if (item) {
			// ������ ������ �� ������ �� ������������� �������
			item->getContainer(reseiver, reseiver_index);
			if (reseiver) {
				// ������������� �� ���������� � ���������� �����
				reseiver->eventInvalideContainer = newDelegate(this, &ItemBox::notifyInvalideDrop);

				// ������ ������ �� ����������� �����
				mDropInfo.set(this, mDropSenderIndex, reseiver, reseiver_index);
				eventRequestDrop(this, mDropInfo, mDropResult);

				// ������������� ����� ���������
				mDropInfo.reseiver->setContainerItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
			}
		}
		// ��� ������� ��� ����
		else {
			mDropInfo.set(this, mDropSenderIndex, null, ITEM_NONE);
		}

		mOldDrop = item;
		DropState state;

		ItemInfo data;
		if (reseiver == null) {
			data.drag_accept = false;
			data.drag_refuse = false;
			state = DROP_MISS;
		}
		else if (mDropResult) {
			data.drag_accept = true;
			data.drag_refuse = false;
			state = DROP_ACCEPT;
		}
		else {
			data.drag_accept = false;
			data.drag_refuse = true;
			state = DROP_REFUSE;
		}
		data.select = false;
		data.active = false;

		data.index = mDropSenderIndex;
		data.update = update;
		data.drag = true;

		requestUpdateWidgetItem(this, mItemDrag, data);

		eventDropState(this, state);
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
		eventNotifyItem(this, NotifyItemData(getIndexByWidget(_sender), NOTIFY_KEY_PRESSED, _key, _char));
	}

	void ItemBox::notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key)
	{
		eventNotifyItem(this, NotifyItemData(getIndexByWidget(_sender), NOTIFY_KEY_RELEASED, _key));
	}

	size_t ItemBox::getIndexByWidget(WidgetPtr _widget)
	{
		MYGUI_ASSERT(_widget, "ItemBox::getIndexByWidget : Widget == null");
		if (_widget == mWidgetClient) return ITEM_NONE;
		MYGUI_ASSERT(_widget->getParent() == mWidgetClient, "ItemBox::getIndexByWidget : Widget is not child");

		size_t index = *_widget->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
		MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::getIndexByWidget");

		return index;
	}

	size_t ItemBox::getContainerIndex(const IntPoint & _point)
	{
		for (VectorWidgetPtr::iterator iter=mVectorItems.begin(); iter!=mVectorItems.end(); ++iter) {
			if ((*iter)->isShow()) {
				if ((*iter)->getAbsoluteRect().inside(_point)) {
					return getIndexByWidget(*iter);
				}
			}
		}
		return ITEM_NONE;
	}

	void ItemBox::resetContainer(bool _update)
	{
		// ����������� � ��������
		Widget::resetContainer(_update);

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
			if ((*iter)->isShow()) {
				size_t index = *(*iter)->_getInternalData<size_t>() + (mLineTop * mCountItemInLine);
				MYGUI_ASSERT_RANGE(index, mItemsInfo.size(), "ItemBox::getWidgetByIndex");

				if (index == _index) return (*iter);
			}
		}
		return null;
	}

} // namespace MyGUI
