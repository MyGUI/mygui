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
#include "MyGUI_Gui.h"

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
		//mItemDrag(null),
		mOldDrop(null),
		mDropResult(false),
		mCurrentSender(null),
		mStartDrop(false),
		mNeedDrop(false)
	{
		// ��� ����� ����� �����
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
		// ��� ��� ������, �� ��������� ���� �� �����������
		if ((null == mWidgetScroll) && (null == mWidgetClient)) mWidgetClient = this;
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// ����������� ������ ��� �����������
		mWidgetClient->_requestGetDragItemInfo = newDelegate(this, &ItemBox::requestGetDragItemInfo);

		// ������ ��������
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

		// ���� ����������� ������� � ������ ����������, �� �������������� ���
		if (old_count == mCountItemInLine) {
			// ���� ����� ����� ������ �� ������ �� �����
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
		// ���������� ������ ������
		requestCoordWidgetItem(this, coord, false);
		mSizeItem = coord.size();
		MYGUI_ASSERT((mSizeItem.width > 0 && mSizeItem.height > 0), "(mSizeItem.width > 0 && mSizeItem.height > 0)  at requestCoordWidgetItem");

		// ����������� ������� �� ����� ������
		mCountItemInLine = mWidgetClient->getWidth() / mSizeItem.width;
		if (1 > mCountItemInLine) mCountItemInLine = 1;

		// ����������� �����
		mCountLines = (int)mCountItems / mCountItemInLine;
		if (0 != ((int)mCountItems % mCountItemInLine)) mCountLines ++;

		// ����������� ����� ������� ����������� � ������� �����
		mCountLineVisible = mWidgetClient->getHeight() / mSizeItem.height;
		if (0 != (mWidgetClient->getHeight() % mSizeItem.height)) mCountLineVisible++;
		mCountLineVisible ++; // � �� ���� ������ ��� ������������ ���������
	}

	void ItemBox::updateScroll()
	{
		// ����������� ������� � ��������
		mScrollRange = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();

		// ��������� ��������� �������
		bool change = false;
		if (mWidgetScroll) {
			if ((mScrollRange <= 0) || (mWidgetScroll->getLeft() <= mWidgetClient->getLeft())) {
				if (mWidgetScroll->isShow()) {
					change = true;
					mWidgetScroll->hide();
					// ����������� ���������� ���� �� ������ ������
					mWidgetClient->setSize(mWidgetClient->getWidth() + mWidgetScroll->getWidth(), mWidgetClient->getHeight());
				}
			}
			else if (false == mWidgetScroll->isShow()) {
				change = true;
				mWidgetClient->setSize(mWidgetClient->getWidth() - mWidgetScroll->getWidth(), mWidgetClient->getHeight());
				mWidgetScroll->show();
			}
		}

		// ���� ������ ���������, �� �������������
		if (change) {
			updateMetrics();
			mScrollRange = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();
		}
		
		// ��������� ������� ������� �������
		int count_pix = ((mCountLines - mLineTop) * mSizeItem.height) - mOffsetTop;

		if ((count_pix < mWidgetClient->getHeight()) && ((mLineTop != 0) || (mOffsetTop != 0))) {
			// ������� �������, �������� � ����
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
		size_t iwid = 0; // ������ �������
		for (int pos = start; pos<count; ++pos, ++iwid) {
			// ������ ��� �������
			if (pos >= mCountItems) break;

			WidgetItemData & info = getItemWidget(iwid);
			info.item->setPosition(IntCoord((iwid % mCountItemInLine) * mSizeItem.width,
				((iwid / mCountItemInLine) * mSizeItem.height)  - mOffsetTop,
				mSizeItem.width, mSizeItem.height));

			info.item->show();
			if (_redraw) {
				ItemInfo & data = mItemsInfo[pos];
				data.update = true;
				requestUpdateWidgetItem(this, info, data);
			}

		}

		// ��� ������� ��� ����, �� �� ���� �� ������
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid].item->hide();
			iwid ++;
		}

	}

	WidgetItemData & ItemBox::getItemWidget(size_t _index)
	{
		// ����������� ������ ���������������
		MYGUI_DEBUG_ASSERT(_index <= mVectorItems.size(), "index out of range");

		// ��� ��� ������ �������, ���� �������
		if (_index == mVectorItems.size()) {

			// �������� ������ �� �������� �������
			WidgetItemData data;
			data.item = mWidgetClient->createWidget<Widget>("Default", IntCoord(0, 0, mSizeItem.width, mSizeItem.height), ALIGN_DEFAULT);

			requestCreateWidgetItem(this, data);
			//MYGUI_ASSERT(data.item, "not listen requestCreateWidgetItem");

			data.item->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			data.item->eventMouseSetFocus = newDelegate(this, &ItemBox::notifyMouseSetFocus);
			data.item->eventMouseLostFocus = newDelegate(this, &ItemBox::notifyMouseLostFocus);
			data.item->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMouseButtonPressed);
			data.item->eventMouseButtonReleased = newDelegate(this, &ItemBox::notifyMouseButtonReleased);
			data.item->eventMouseDrag = newDelegate(this, &ItemBox::notifyMouseDrag);
			data.item->_requestGetDragItemInfo = newDelegate(this, &ItemBox::requestGetDragItemInfo);

			data.item->_setInternalData((int)mVectorItems.size());
			mVectorItems.push_back(data);
		}

		return mVectorItems[_index];
	}

	void ItemBox::_onMouseWheel(int _rel)
	{
		notifyMouseWheel(null, _rel);

		// !!! ����������� �������� � ����� ������
		Widget::_onMouseWheel(_rel);
	}

	void ItemBox::_onKeySetFocus(WidgetPtr _old)
	{
		mIsFocus = true;
		setState("select");

		// !!! ����������� �������� � ����� ������
		Widget::_onKeySetFocus(_old);
	}

	void ItemBox::_onKeyLostFocus(WidgetPtr _new)
	{
		mIsFocus = false;
		setState("normal");

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyLostFocus(_new);
	}

	void ItemBox::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		ItemInfo & data = mItemsInfo[index];
		data.update = false;
		data.active = true;
		requestUpdateWidgetItem(this, mVectorItems[_sender->_getInternalData()], data);
		mIndexActive = index;
	}

	void ItemBox::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		// ��� �������� �����
		if (mIndexActive == ITEM_NONE) return;

		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		ItemInfo & data = mItemsInfo[index];
		data.update = false;
		data.active = false;
		requestUpdateWidgetItem(this, mVectorItems[_sender->_getInternalData()], data);
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
		findCurrentActiveItem();

	}

	void ItemBox::resetCurrentActiveItem()
	{
		// ���������� ������ ���������
		if (mIndexActive != ITEM_NONE) {
			size_t start = (size_t)(mLineTop * mCountItemInLine);
			ItemInfo & data = mItemsInfo[mIndexActive];
			data.update = false;
			data.active = false;
			if ((mIndexActive >= start) && (mIndexActive < (start + mVectorItems.size()))) {
				requestUpdateWidgetItem(this, mVectorItems[mIndexActive - start], data);
			}
			mIndexActive = ITEM_NONE;
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
			WidgetItemData & info = mVectorItems[pos];
			const IntRect& rect = info.item->getAbsoluteRect();
			if ((point.left>= rect.left) && (point.left <= rect.right) && (point.top>= rect.top) && (point.top <= rect.bottom)) {

				size_t index = (size_t)info.item->_getInternalData() + (mLineTop * mCountItemInLine);
				// ������ ����� ���� ������
				if (index < mItemsInfo.size()) {
					ItemInfo & data = mItemsInfo[index];
					data.update = false;
					data.active = true;
					requestUpdateWidgetItem(this, info, data);
					mIndexActive = index;
				}

				break;
			}
		}
	}

	void ItemBox::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// ����� �� �������� ������
		bool update = false;

		// ������� ������
		if (mCurrentSender != _sender) {
			mCurrentSender = _sender;
			update = true;
		}

		size_t index = (size_t)mCurrentSender->_getInternalData() + (mLineTop * mCountItemInLine);
		MYGUI_DEBUG_ASSERT(index < mItemsInfo.size(), "index out of range");

		if (false == mStartDrop) {
			mStartDrop = true;
			mNeedDrop = false;
			// ������ �� �������� ����� �� �������
			requestStartDrop(this, index, mNeedDrop);
		}

		// ���� �� �����
		if (false == mNeedDrop) {
			InputManager::getInstance().resetMouseCaptureWidget();
			return;
		}

		// ������ ������, ��� ��� ���� ����
		WidgetPtr item = InputManager::getInstance().getWidgetFromPoint(_left, _top);

		if (null == mItemDrag.item) {
			// ���������� ������ ������
			IntCoord coord;
			mPointDragOffset = coord.point();
			requestCoordWidgetItem(this, coord, true);

			// ������� � ����������� �����
			mItemDrag.item = Gui::getInstance().createWidget<Widget>("Default", IntCoord(0, 0, coord.width, coord.height), ALIGN_DEFAULT, "DragAndDrop");
			requestCreateWidgetItem(this, mItemDrag);
		}

		IntPoint point = InputManager::getInstance().getLastLeftPressed() - _sender->getAbsolutePosition();
		mItemDrag.item->setPosition(_left - point.left + mPointDragOffset.left, _top - point.top + mPointDragOffset.top);
		mItemDrag.item->show();

		// ���� �����, ������ ��� ����������
		if (mOldDrop == item) return;

		// ���������� ������ ���������
		if (mDropInfo.reseiver) mDropInfo.reseiver->_setDragItemInfo(mDropInfo.reseiver_index, false, false);

		mDropResult = false;
		WidgetPtr reseiver = null;
		size_t reseiver_index = ITEM_NONE;
		// ���� ������ ��� ����
		if (item) {
			// ������ ������ �� ������ �� ������������� �������
			item->_getDragItemInfo(reseiver, reseiver_index);
			if (reseiver) {
				// ������������� �� ���������� � ���������� �����
				reseiver->_eventInvalideDropInfo = newDelegate(this, &ItemBox::notifyInvalideDrop);

				// ������ ������ �� ����������� �����
				mDropInfo.set(this, index, 0, reseiver, reseiver_index, 0);
				requestDropItem(this, mDropInfo, mDropResult);

				// ������������� ����� ���������
				mDropInfo.reseiver->_setDragItemInfo(mDropInfo.reseiver_index, true, mDropResult);
			}
			else {
				mDropInfo.reset();
			}
		}
		// ��� ������� ��� ����
		else {
			mDropInfo.reset();
		}

		mOldDrop = item;

		// ����� �������
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
		requestUpdateWidgetItem(this, mItemDrag, data);

	}

	void ItemBox::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if ( ! _left) return;

		// ���������� ���� ��� �����
		mDropResult = false;
		mOldDrop = null;
		mDropInfo.reset();

		size_t index = (size_t)_sender->_getInternalData() + (mLineTop * mCountItemInLine);

		if (index < mItemsInfo.size()) setItemSelect(index);

		// ����������, ����� ��������� ������� ������
		mCurrentSender = null;
		mStartDrop = false;
	}

	void ItemBox::notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (mItemDrag.item) mItemDrag.item->hide();

		// ���������� ������ ���������
		if (mDropInfo.reseiver) mDropInfo.reseiver->_setDragItemInfo(mDropInfo.reseiver_index, false, false);

		// ���� ���� �������� �������
		if (mDropResult) eventDropAccept(this, mDropInfo);

		// ���������� ���� ��� �����
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
			requestUpdateWidgetItem(this, mVectorItems[_index - start], data);
		}
	}

	void ItemBox::setItem(size_t _index, void * _data)
	{
		MYGUI_ASSERT(_index < mItemsInfo.size() , "index '" << _index << " out of range '" << mItemsInfo.size() << "'");
		mItemsInfo[_index].data = _data;

		ItemInfo & data = mItemsInfo[_index];
		data.data = _data;

		data.update = true;

		size_t start = (size_t)(mLineTop * mCountItemInLine);
		if ((_index >= start) && (_index < (start + mVectorItems.size()))) {
			requestUpdateWidgetItem(this, mVectorItems[_index - start], data);
		}
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

		// ����������� ����� ������ ���������
		if (mIndexSelect != ITEM_NONE) {
			if (mIndexSelect >= _index) mIndexSelect ++;
		}

		// ����������� ������� � ���������
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

		// ����������� ����� ������ ���������
		if (mIndexSelect != ITEM_NONE) {
			if (mCountItems == 0) mIndexSelect = ITEM_NONE;
			else if ((mIndexSelect > _index) || (mIndexSelect == mCountItems)) mIndexSelect --;
		}

		// ����������� ������� � ���������
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

		// ���������� ������ ���������
		if (mIndexSelect != ITEM_NONE) {
			ItemInfo & data = mItemsInfo[mIndexSelect];
			data.update = false;
			data.select = false;
			if ((mIndexSelect >= start) && (mIndexSelect < (start + mVectorItems.size()))) {
				requestUpdateWidgetItem(this, mVectorItems[mIndexSelect - start], data);
			}
		}

		mIndexSelect = _index;
		if (mIndexSelect != ITEM_NONE) {
			ItemInfo & data = mItemsInfo[mIndexSelect];
			data.update = false;
			data.select = true;
			if ((mIndexSelect >= start) && (mIndexSelect < (start + mVectorItems.size()))) {
				requestUpdateWidgetItem(this, mVectorItems[mIndexSelect - start], data);
			}
		}
	}

	void ItemBox::notifyInvalideDrop(WidgetPtr _sender)
	{
		const IntPoint& point = InputManager::getInstance().getMousePosition();
		notifyMouseDrag(null, point.left, point.top);
	}

} // namespace MyGUI
