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

namespace MyGUI
{

	Ogre::String ItemBox::WidgetTypeName = "ItemBox";

	ItemBox::ItemBox(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mWidgetScroll(null),
		mWidgetClient(null),
		mTopIndex(0),
		mOffsetTop(0),
		mRangeIndex(-1),
		//mLastRedrawLine(0),
		//mIndexSelect(ITEM_NONE),
		mIsFocus(false),
		mCountItems(0)
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
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMousePressed);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			}
		}
		MYGUI_ASSERT(null != mWidgetScroll, "Child VScroll not found in skin (ItemBox must have VScroll)");//???
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// ������ ��������
		/*const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("SkinLine");
		if (iter != param.end()) mSkinLine = iter->second;
		MYGUI_ASSERT(false == mSkinLine.empty(), "SkinLine property or skin not found (ItemBox must have SkinLine property)");

		iter = param.find("HeightLine");
		if (iter != param.end()) mHeightLine = utility::parseInt(iter->second);
		if (mHeightLine < 1) mHeightLine = 1;

*/
		mWidgetScroll->setScrollPage((size_t)mSizeItem.height);

		mSizeItem.set(50, 50);
		mCountItems = 150;

		updateMetrics();
		updateScroll();
		_redrawAllVisible();
		//updateLine();

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

	void ItemBox::updateFromResize(const IntSize& _size)
	{
		int old_count = mCountItemInLine;
		updateMetrics();
		updateScroll();

		// ���� ����������� ������� � ������ ����������, �� �������������� ���
		if (old_count == mCountItemInLine) {
			// ���� ����� ����� ������ �� ������ �� �����
			//if (_size.height >= mCoord.height) return;
		}

		notifyScrollChangePosition(null, mWidgetScroll->getScrollPosition());
		_redrawAllVisible();
	}

	void ItemBox::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _rel)
	{
		mOffsetTop = ((int)_rel % mSizeItem.height);

		// �������� � ������������� �������
		int offsetVert = 0 - mOffsetTop;
		int countInLine = 0;
		int offsetHorz = 0;

		// ������������ ���� �������
		for (size_t pos=0; pos<mVectorItems.size(); pos++) {
			if (countInLine >= mCountItemInLine) {
				offsetVert += mSizeItem.height;
				countInLine = 0;
				offsetHorz = 0;
			}

			mVectorItems[pos]->setPosition(IntPoint(offsetHorz, offsetVert));
			countInLine ++;
			offsetHorz += mSizeItem.width;
		}

		// ���� ������ ���������, �� �������������� �����
		int top = ((int)_rel / mSizeItem.height) * mCountItemInLine;
		if (top != mTopIndex) {
			mTopIndex = top;
			_redrawAllVisible();
		}

		// ������������� ��� ������ ������, ���� ��� ���������
		//_redrawItemRange(mLastRedrawLine);

	}

	void ItemBox::notifyMouseWheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if (mRangeIndex <= 0) return;

		int offset = (int)mWidgetScroll->getScrollPosition();
		if (_rel < 0) offset += mSizeItem.height;
		else  offset -= mSizeItem.height;

		if (offset >= mRangeIndex) offset = mRangeIndex;
		else if (offset < 0) offset = 0;

		if ((int)mWidgetScroll->getScrollPosition() == offset) return;

		mWidgetScroll->setScrollPosition(offset);
		notifyScrollChangePosition(null, offset);
	}

	void ItemBox::updateMetrics()
	{
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
		mRangeIndex = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();

		if ( (mRangeIndex < 1) || (mWidgetScroll->getLeft() <= mWidgetClient->getLeft()) ) {
			if (mWidgetScroll->isShow()) {
				mWidgetScroll->hide();
				// ����������� ���������� ���� �� ������ ������
				mWidgetClient->setSize(mWidgetClient->getWidth() + mWidgetScroll->getWidth(), mWidgetClient->getHeight());
			}
			mWidgetScroll->setScrollPosition(0);
			mWidgetScroll->setScrollRange(0);
			mTopIndex = 0;
			mOffsetTop = 0;
			return;
		}
		if (false == mWidgetScroll->isShow()) {
			mWidgetClient->setSize(mWidgetClient->getWidth() - mWidgetScroll->getWidth(), mWidgetClient->getHeight());
			mWidgetScroll->show();
		}

		// ������������ ������� �������
		size_t pos = mWidgetScroll->getScrollPosition();
		mWidgetScroll->setScrollRange(mRangeIndex + 1);
		// ���� ������� ����� �����������, �� ���������� �� � ����� �����
		if (pos >= mRangeIndex + 1) mWidgetScroll->setScrollPosition(mRangeIndex);
	}

	void ItemBox::_redrawAllVisible()
	{

		int count = mCountLineVisible * mCountItemInLine + mTopIndex;
		size_t iwid = 0; // ������ �������
		for (int pos = mTopIndex; pos<count; ++pos, ++iwid) {
			// ������ ��� �������
			if (pos >= mCountItems) break;

			WidgetPtr widget = getItemWidget(iwid);

			widget->show();
			redrawItem(widget, (size_t)pos);

		}

		// ��� ������� ��� ����, �� �� ���� �� ������
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid]->hide();
			iwid ++;
		}

		//MYGUI_OUT(mVectorItems.size());

	}

	// ��������� ����������� ������
	void ItemBox::redrawItem(WidgetPtr _widget, size_t _index)
	{
		_widget->setCaption(utility::toString(_index));
	}

	WidgetPtr ItemBox::getItemWidget(size_t _index)
	{
		// ��� ��� ������ �������, ���� �������
		if (_index >= mVectorItems.size()) {
			int pos = mTopIndex + (int)mVectorItems.size();
			WidgetPtr widget = mWidgetClient->createWidgetT("Widget", "Edit", IntCoord((pos % mCountItemInLine) * mSizeItem.width, ((pos / mCountItemInLine) * mSizeItem.height)  + mOffsetTop, mSizeItem.width, mSizeItem.height), ALIGN_DEFAULT);
			widget->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			mVectorItems.push_back(widget);
			return mVectorItems.back();
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

	/*void ItemBox::_onKeyButtonPressed(int _key, Char _char)
	{
		// ����� ��������� �����, ��������������� �������� �������� �������
		if (getItemCount() == 0) {

			// !!! ����������� �������� � ����� ������
			Widget::_onKeyButtonPressed(_key, _char);
			return;
		}

		size_t sel = mIndexSelect;

		if (_key == KC_UP) {

			if (sel != 0) {
				if (sel == ITEM_NONE) sel = 0;
				else sel --;
			}

		} else if (_key == KC_DOWN) {

			if (sel == ITEM_NONE) sel = 0;
			else sel ++;

			if (sel >= getItemCount()) {
				// ������ ��������
				sel = mIndexSelect;
			}

		} else if (_key == KC_HOME) {

			if (sel != 0) sel = 0;

		} else if (_key == KC_END) {

			if (sel != (getItemCount() - 1)) {
				sel = getItemCount() - 1;
			}

		} else if (_key == KC_PGUP) {

			if (sel != 0) {
				if (sel == ITEM_NONE) sel = 0;
				else {
					size_t page = mWidgetClient->getHeight() / mHeightLine;
					if (sel <= page) sel = 0;
					else sel -= page;
				}
			}

		} else if (_key == KC_PGDOWN) {

			if (sel != (getItemCount() - 1)) {
				if (sel == ITEM_NONE) sel = 0;
				else {
					sel += mWidgetClient->getHeight() / mHeightLine;
					if (sel >= getItemCount()) sel = getItemCount() - 1;
				}
			}

		} else if (_key == KC_RETURN) {
			if (sel != ITEM_NONE) {
				eventItemBoxSelectAccept(this);

				Widget::_onKeyButtonPressed(_key, _char);
				// �������, ��� ��� �������� ����������� �����
				return;
			}

		}

		if (sel != mIndexSelect) {
			if ( false == isItemVisible(sel)) beginToIndex(sel);
			setItemSelect(sel);
			// ���������� �������
			eventItemBoxChangePosition(this, mIndexSelect);
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void ItemBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		if (_sender == mWidgetScroll) return;

		// ���� ������� ������, �� ����������
		if (_sender == mWidgetClient) {

			if (mIndexSelect != ITEM_NONE) {
				_selectIndex(mIndexSelect, false);
				mIndexSelect = ITEM_NONE;
				eventItemBoxChangePosition(this, mIndexSelect);
			}
			eventItemBoxMouseItemActivate(this, mIndexSelect);

		// ���� �� ������, �� �����������
		} else {
			size_t index = (size_t)_sender->_getInternalData() + mTopIndex;

			if (mIndexSelect != index) {
				_selectIndex(mIndexSelect, false);
				_selectIndex(index, true);
				mIndexSelect = index;
				eventItemBoxChangePosition(this, mIndexSelect);
			}
			eventItemBoxMouseItemActivate(this, mIndexSelect);

		}
	}*/

	/*void ItemBox::updateLine(bool _reset)
	{
		// ����������
		if (_reset) {
			mOldSize.clear();
			mLastRedrawLine = 0;
		}

		// ������� �������
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// ���� ������ ������������� �� ��������� �������
		if (mOldSize.height < mCoord.height) {

			int height = (int)mVectorItems.size() * mHeightLine - mOffsetTop;

			// �� ��� ���, ���� �� ��������� ������������� �����������, � ������ �� ���� ������
			while ( (height <= (mWidgetClient->getHeight() + mHeightLine)) && (mVectorItems.size() < mStringArray.size()) ) {
				// ������� �����
				WidgetPtr line = mWidgetClient->createWidgetT("Button", mSkinLine, 0, height, mWidgetClient->getWidth(), mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// ������������� �� ������ ��� �������
				line->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMousePressed);
				line->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				// ����������� ���������� �����, ��� �������� ��������
				line->_setInternalData((int)mVectorItems.size());
				// � ���������
				mVectorItems.push_back(line);
				height += mHeightLine;
			};

			// ��������� �� ����������� �� ������ ��������� ������
			if (position >= mRangeIndex) {

				// ������ ���� ���������� � ������
				if (mRangeIndex <= 0) {

					// ��������, ���� ����
					if (position || mOffsetTop || mTopIndex) {

						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;
						mLastRedrawLine = 0; // ����� ��� ��������������

						// �����������
						int offset = 0;
						for (size_t pos=0; pos<mVectorItems.size(); pos++) {
							mVectorItems[pos]->setPosition(0, offset);
							offset += mHeightLine;
						}
					}

				} else {

					// ��������� ������ � ������ �������
					int count = mWidgetClient->getHeight() / mHeightLine;
					mOffsetTop = mHeightLine - (mWidgetClient->getHeight() % mHeightLine);

					if (mOffsetTop == mHeightLine) {
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mStringArray.size() - count - 1;

					// �����������
					int offset = 0 - mOffsetTop;
					for (size_t pos=0; pos<mVectorItems.size(); pos++) {
						mVectorItems[pos]->setPosition(0, offset);
						offset += mHeightLine;
					}

					// ����������� ���������, ������ ���� ������������
					position = top * mHeightLine + mOffsetTop;

					// ���� ����� ���������, �� �������������� �����
					if (top != mTopIndex) {
						mTopIndex = top;
						_redrawItemRange();
					}

				}
			}

			// ���������� ������, �� ��������� ���� ������, ��������� ����� �����
			_redrawItemRange(mLastRedrawLine);

		} // if (old_cy < mCoord.height)

		// ������������ ��������� �������
		mWidgetScroll->setScrollPosition(position);

		mOldSize.width = mCoord.width;
		mOldSize.height = mCoord.height;
	}

	void ItemBox::_redrawItemRange(size_t _start)
	{
		// �������������� �����, ������ ��, ��� �����
		size_t pos = _start;
		for (; pos<mVectorItems.size(); pos++) {
			// ������ � ����� �������
			size_t index = pos + (size_t)mTopIndex;

			// �� ����� �������� ������� ������
			if (index >= mStringArray.size()) {
				// ���������� ��������� �������������� �����
				mLastRedrawLine = pos;
				break;
			}
			if (mVectorItems[pos]->getTop() > mWidgetClient->getHeight()) {
				// ���������� ��������� �������������� �����
				mLastRedrawLine = pos;
				break;
			}

			// ���� ��� �����, �� �������
			if (false == mVectorItems[pos]->isShow()) mVectorItems[pos]->show();
			// ��������� �����
			mVectorItems[pos]->setCaption(mStringArray[index]);

			// ���� ����� �������� ,�� �������
			if (index == mIndexSelect) {
				if (!static_cast<ButtonPtr>(mVectorItems[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mVectorItems[pos])->setButtonPressed(true);
			} else {
				if (static_cast<ButtonPtr>(mVectorItems[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mVectorItems[pos])->setButtonPressed(false);
			}
		}

		// ���� ���� ���� ������, �� ������ ������������ �����
		if (pos >= mVectorItems.size()) mLastRedrawLine = pos;
	}

	// �������������� ������
	void ItemBox::_redrawItem(size_t _index)
	{
		// �������
		if (_index < (size_t)mTopIndex) return;
		_index -= (size_t)mTopIndex;
		// ���� �������
		if (_index > mLastRedrawLine) return;
		// ��������������
		mVectorItems[_index]->setCaption(mStringArray[_index + mTopIndex]);
	}

	void ItemBox::insertItemString(size_t _index, const Ogre::UTFString & _item)
	{
		if (_index > mStringArray.size()) _index = mStringArray.size();
		// ��������� ���������
		_insertString(_index, _item);

		// ���� ����, �� ������ ���������� �������
		if ( (mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect) ) mIndexSelect++;

		// ������, �� ������� �������� ��������
		if ( (_index <= (size_t)mTopIndex) && (mRangeIndex > 0) ) {
			mTopIndex ++;
			// ������������ ��������� �������
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			mRangeIndex += mHeightLine;

		} else {

			// ���������� ��������� ������
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// ������, ����� ���������� �������� ��������, ���� ���� ������ (������ ��� ��� ��������� ����� �� ���� ������� ������)
			if (mWidgetClient->getHeight() < (offset - mHeightLine)) {
				// ������������ ��������� �������
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				mRangeIndex += mHeightLine;

			// ������ � ������� �������
			} else {

				// ��������� ���
				updateScroll();
				updateLine(true);

				// ����� ���� ��� ����������� �� ����������� �����������
			}
		}

	}

	void ItemBox::deleteItemString(size_t _index)
	{
		// �������, �� ��������
		MYGUI_ASSERT(_index < mStringArray.size(), "deleteItemString: index '" << _index << "' out of range");

		// ������� ��������� ������
		_deleteString(_index);

		// ���� ����, �� ������ ���������� �������
		if (mStringArray.empty()) mIndexSelect = ITEM_NONE;
		else if (mIndexSelect != ITEM_NONE) {
			if (_index < mIndexSelect) mIndexSelect--;
			else if ( (_index == mIndexSelect) && (mIndexSelect == (mStringArray.size())) ) mIndexSelect--;
		}

		// ���� �������� ����� ������ , �� �������� �������
		if (mVectorItems.size() > mStringArray.size()) {
			mVectorItems[mStringArray.size()]->hide();
		}

		// ������, �� ������� �������� ��������
		if (_index < (size_t)mTopIndex) {
			mTopIndex --;
			// ������������ ��������� �������
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			mRangeIndex -= mHeightLine;

		} else {

			// ���������� ��������� ��������� ������
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// ������, ����� ���������� �������� ��������
			if (mWidgetClient->getHeight() < offset) {
				// ������������ ��������� �������
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				mRangeIndex -= mHeightLine;

			// ������ � ������� �������
			} else {

				// ��������� ���
				updateScroll();
				updateLine(true);

				// ����� ���� ��� ����������� �� ����������� �����������
			}
		}
	}

	void ItemBox::_deleteString(size_t _index)
	{
		for (size_t pos=_index+1; pos<mStringArray.size(); pos++) {
			mStringArray[pos-1] = mStringArray[pos];
		}
		mStringArray.pop_back();
	}

	void ItemBox::_insertString(size_t _index, const Ogre::UTFString & _item)
	{
		mStringArray.push_back("");
		for (size_t pos=mStringArray.size()-1; pos > _index; pos--) {
			mStringArray[pos] = mStringArray[pos-1];
		}
		mStringArray[_index] = _item;
	}

	void ItemBox::setItemSelect(size_t _index)
	{
		if (mIndexSelect == _index) return;
		_selectIndex(mIndexSelect, false);
		_selectIndex(_index, true);
		mIndexSelect = _index;
	}

	void ItemBox::_selectIndex(size_t _index, bool _select)
	{
		if (_index >= mStringArray.size()) return;
		// �� ����� ������
		if (_index < (size_t)mTopIndex) return;
		// ���������� ��������� ������
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;
		// ������, ����� ���������� �������� ��������
		if (mWidgetClient->getHeight() < offset) return;

		static_cast<ButtonPtr>(mVectorItems[_index-mTopIndex])->setButtonPressed(_select);
	}

	void ItemBox::beginToIndex(size_t _index)
	{
		if (_index >= mStringArray.size()) return;
		if (mRangeIndex <= 0) return;

		int offset = (int)_index * mHeightLine;
		if (offset >= mRangeIndex) offset = mRangeIndex;

		if ((int)mWidgetScroll->getScrollPosition() == offset) return;

		mWidgetScroll->setScrollPosition(offset);
		notifyScrollChangePosition(null, offset);
	}

	// ����� �� �� �������, ��������� ��� ���
	bool ItemBox::isItemVisible(size_t _index, bool _fill)
	{
		// ���� �������� ���, �� �� ��� �� ����� (� ��� ����� ����� �� ������ ���)
		if (_index >= mStringArray.size()) return false;
		// ���� ������ ���, �� �� ������� �����
		if (mRangeIndex <= 0) return true;

		// ������, �� ������� �������� ��������
		if (_index < (size_t)mTopIndex) return false;

		// ������ ��� ������� ����������
		if (_index == (size_t)mTopIndex) {
			if ( (mOffsetTop != 0) && (_fill) ) return false; // ��� ����� ��������� ���������
			return true;
		}

		// ���������� ��������� ������
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

		// ������, ����� ���������� �������� ��������
		if (mWidgetClient->getHeight() < offset) return false;

		// ���� �� ����� � ��� ����� �����
		if ((mWidgetClient->getHeight() < (offset + mHeightLine)) && (_fill) ) return false;

		return true;
	}
*/
} // namespace MyGUI
