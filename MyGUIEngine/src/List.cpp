
#include "List.h"
#include "ListParser.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<List> ListFactoryInstance("List"); }
	// ������ ������
	namespace parser { ListParser ListParserInstance; }

	List::List(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		mWidgetScroll(null), mWidgetClient(null),
		mOffsetTop(0),
		mTopIndex(0),
		mRangeIndex(-1),
		mLastRedrawLine(0),
		mIndexSelect(ITEM_NONE),
		mIsFocus(false),
		mOldCx(0), mOldCy(0)
	{
		std::string skinScroll, skinClient;
		FloatRect offsetScroll;
		FloatRect offsetClient;

		// ������ ��������
		const SkinParam & param = _info->getParams();
		if (!param.empty()) {
			SkinParam::const_iterator iter = param.find("SkinScroll");
			if (iter != param.end()) skinScroll = iter->second;
			iter = param.find("OffsetScroll");
			if (iter != param.end()) offsetScroll = util::parseFloatRect(iter->second);
			iter = param.find("SkinClient");
			if (iter != param.end()) skinClient = iter->second;
			iter = param.find("OffsetClient");
			if (iter != param.end()) offsetClient = util::parseFloatRect(iter->second);
			iter = param.find("SkinLine");
			if (iter != param.end()) mSkinLine = iter->second;
			iter = param.find("HeightLine");
			if (iter != param.end()) mHeightLine = util::parseInt(iter->second);
		}

		ASSERT(skinScroll.size());
		ASSERT(skinClient.size());
		ASSERT(mSkinLine.size());
		ASSERT(mHeightLine);

		offsetScroll = WidgetManager::convertOffset(offsetScroll, ALIGN_RIGHT | ALIGN_VSTRETCH, _info->getSize(), m_cx, m_cy);
		mWidgetScroll = static_cast<VScrollPtr>(createWidget("VScroll", skinScroll, offsetScroll.left, offsetScroll.top, offsetScroll.right, offsetScroll.bottom, ALIGN_RIGHT | ALIGN_VSTRETCH));
		mWidgetScroll->eventScrollChangePosition = newDelegate(this, &List::notifyScrollChangePosition);
		mWidgetScroll->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);

		offsetClient = WidgetManager::convertOffset(offsetClient, ALIGN_STRETCH, _info->getSize(), m_cx, m_cy);
		mWidgetClient = createWidget("Widget", skinClient, offsetClient.left, offsetClient.top, offsetClient.right, offsetClient.bottom, ALIGN_STRETCH);
		// ������������� �� ����
		mWidgetClient->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);

		mWidgetScroll->setScrollPage((size_t)mHeightLine);

		updateScroll();
		updateLine();

	}

	void List::_onMouseSheel(int _rel)
	{
		Widget::_onMouseSheel(_rel);
		notifyMouseSheel(null, _rel);
	}

	void List::_onKeySetFocus(WidgetPtr _old)
	{
		Widget::_onKeySetFocus(_old);
		mIsFocus = true;
		_updateState();
	}

	void List::_onKeyLostFocus(WidgetPtr _new)
	{
		Widget::_onKeyLostFocus(_new);
		mIsFocus = false;
		_updateState();
	}

	void List::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::_onKeyButtonPressed(_key, _char);

		// ����� ��������� �����, ��������������� �������� �������� �������

		if (getItemCount() == 0) return;
		else if (getItemCount() == 1) {
			if (mIndexSelect == ITEM_NONE) setItemSelect(0);
			return;
		}

		size_t sel = mIndexSelect;

		if (_key == OIS::KC_UP) {

			if (sel == 0) return;

			if (sel == ITEM_NONE) sel = 0;
			else sel --;

		} else if (_key == OIS::KC_DOWN) {

			if (sel == ITEM_NONE) sel = 0;
			else sel ++;

			if (sel >= getItemCount()) return;

		} else if (_key == OIS::KC_HOME) {

			if (sel == 0) return;
			sel = 0;

		} else if (_key == OIS::KC_END) {

			if (sel == (getItemCount() - 1)) return;

			sel = getItemCount() - 1;

		} else if (_key == OIS::KC_PGUP) {

			if (sel == 0) return;

			if (sel == ITEM_NONE) sel = 0;
			else {
				size_t page = mWidgetClient->height() / mHeightLine;
				if (sel <= page) sel = 0;
				else sel -= page;
			}

		} else if (_key == OIS::KC_PGDOWN) {

			if (sel == (getItemCount() - 1)) return;

			if (sel == ITEM_NONE) sel = 0;
			else {
				sel += mWidgetClient->height() / mHeightLine;
				if (sel >= getItemCount()) sel = getItemCount() - 1;
			}

		}

		if ( ! isItemVisible(sel)) beginToIndex(sel);
		setItemSelect(sel);

	}

	void List::notifyMouseSheel(MyGUI::WidgetPtr _sender, int _rel)
	{
		if (mRangeIndex <= 0) return;

		int offset = (int)mWidgetScroll->getScrollPosition();
		if (_rel < 0) offset += mHeightLine;
		else  offset -= mHeightLine;

		if (offset >= mRangeIndex) offset = mRangeIndex;
		else if (offset < 0) offset = 0;

		if (mWidgetScroll->getScrollPosition() == offset) return;

		mWidgetScroll->setScrollPosition(offset);
		notifyScrollChangePosition(null, offset);
	}

	void List::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, int _rel)
	{
		//OUT(mWidgetLines.size());
		mOffsetTop = (_rel % mHeightLine);

		// �������� � ������������� �������
		int offset = 0 - mOffsetTop;

		for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
			mWidgetLines[pos]->move(0, offset);
			offset += mHeightLine;
		}

		// ���� ����� ���������, �� �������������� �����
		int top = (_rel / mHeightLine);
		if (top != mTopIndex) {
			mTopIndex = top;
			_redrawItemRange();
		}

		// ������������� ��� ������ ������, ���� ��� ���������
		_redrawItemRange(mLastRedrawLine);

	}

	void List::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (!_left) return;

		// ������������� ����� �� ����
		if (!mIsFocus) InputManager::getInstance().setKeyFocusWidget(this);

		if (_sender == mWidgetScroll) return;

		// ���� ������� ������, �� ����������
		if (_sender == mWidgetClient) {

			if (mIndexSelect == ITEM_NONE) return;
			_selectIndex(mIndexSelect, false);
			mIndexSelect = ITEM_NONE;
		
		// ���� �� ������, �� �����������
		} else {
			size_t index = (size_t)_sender->getInternalData() + mTopIndex;

			if (mIndexSelect == index) return;

			// ���� ������� �� ��������� �����, �� ���������� ���
			//if ( ! isItemVisible(index)) beginToIndex(index);

			_selectIndex(mIndexSelect, false);
			_selectIndex(index, true);
			mIndexSelect = index;
		}
	
	}

	void List::size(int _cx, int _cy)
	{
		Widget::size(_cx, _cy);
		updateScroll();
		updateLine();
	}

	void List::move(int _x, int _y, int _cx, int _cy)
	{
		Widget::move(_x, _y, _cx, _cy);
		updateScroll();
		updateLine();
	}

	void List::updateScroll()
	{
		mRangeIndex = (mHeightLine * (int)mStringArray.size()) - mWidgetClient->height();
		//OUT(mRangeIndex);

		if ( (mRangeIndex < 1) || (mWidgetScroll->left() <= mWidgetClient->left()) ) {
			if (mWidgetScroll->isShow()) {
				mWidgetScroll->show(false);
				// ����������� ���������� ���� �� ������ ������
				mWidgetClient->size(mWidgetClient->width() + mWidgetScroll->width(), mWidgetClient->height());
			}
			return;
		}
		if (!mWidgetScroll->isShow()) {
			mWidgetClient->size(mWidgetClient->width() - mWidgetScroll->width(), mWidgetClient->height());
			mWidgetScroll->show(true);
		}

		mWidgetScroll->setScrollRange(mRangeIndex + 1);
	}

	void List::updateLine(bool _reset)
	{
		// ����������
		if (_reset) {mOldCx=0;mOldCy=0;mLastRedrawLine=0;}

		// ������� �������
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// ���� ������ ������������� �� ��������� �������
		if (mOldCy < m_cy) {

			int height = (int)mWidgetLines.size() * mHeightLine - mOffsetTop;

			// �� ��� ���, ���� �� ��������� ������������� �����������, � ������ �� ���� ������
			while ( (height <= (mWidgetClient->height() + mHeightLine)) && (mWidgetLines.size() < mStringArray.size()) ) {
				// ������� �����
				WidgetPtr line = mWidgetClient->createWidget("Button", mSkinLine, 0, height, mWidgetClient->width(), mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// ������������� �� ������ ��� �������
				line->eventMouseButtonPressed = newDelegate(this, &List::notifyMousePressed);
				line->eventMouseSheel = newDelegate(this, &List::notifyMouseSheel);
				// ����������� ���������� �����, ��� �������� ��������
				line->setInternalData((int)mWidgetLines.size());
				// � ���������
				mWidgetLines.push_back(line);
				height += mHeightLine;
			};

			// ��������� �� ����������� �� ������ ��������� ������
			if (position >= mRangeIndex) {

				//OUT(mRangeIndex);
				// ������ ���� ���������� � ������
				if (mRangeIndex <= 0) {//((int)mStringArray.size()*mHeightLine) <= mWidgetClient->height()) {

					// ��������, ���� ����
					if (position || mOffsetTop || mTopIndex) {

						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;
						mLastRedrawLine = 0; // ����� ��� ��������������

						// �����������
						int offset = 0;
						for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
							mWidgetLines[pos]->move(0, offset);
							offset += mHeightLine;
						}
					}

				} else {

					// ��������� ������ � ������ �������
					int count = mWidgetClient->height() / mHeightLine;
					mOffsetTop = mHeightLine - (mWidgetClient->height() % mHeightLine);

					if (mOffsetTop == mHeightLine) {
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mStringArray.size() - count - 1;

					// �����������
					int offset = 0 - mOffsetTop;
					for (size_t pos=0; pos<mWidgetLines.size(); pos++) {
						mWidgetLines[pos]->move(0, offset);
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
			_redrawItemRange(mLastRedrawLine);//???

		} // if (old_cy < m_cy)

		//_redrawItemRange();//???

		// ������������ ��������� �������
		mWidgetScroll->setScrollPosition(position);

		mOldCx = m_cx;
		mOldCy = m_cy;
	}

	void List::_redrawItemRange(size_t _start)
	{
		// �������������� �����, ������ ��, ��� �����
		size_t pos = _start;
		for (; pos<mWidgetLines.size(); pos++) {
			// ������ � ����� �������
			size_t index = pos + (size_t)mTopIndex;

			// �� ����� �������� ������� ������
			if (index >= mStringArray.size()) {
				// ���������� ��������� �������������� �����
				mLastRedrawLine = pos;
				break;
			}
			if (mWidgetLines[pos]->top() > mWidgetClient->height()) {
				// ���������� ��������� �������������� �����
				mLastRedrawLine = pos;
				break;
			}

			// ���� ��� �����, �� �������
			if (!mWidgetLines[pos]->isShow()) mWidgetLines[pos]->show(true);
			// ��������� �����
			mWidgetLines[pos]->setCaption(mStringArray[index]);

			// ���� ����� �������� ,�� �������
			if (index == mIndexSelect) {
				if (!static_cast<ButtonPtr>(mWidgetLines[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mWidgetLines[pos])->setButtonPressed(true);
			} else {
				if (static_cast<ButtonPtr>(mWidgetLines[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mWidgetLines[pos])->setButtonPressed(false);
			}
		}

		//OUT(m_name, "  start : ", _start, "   count : ", pos);

		// ���� ���� ���� ������, �� ������ ������������ �����
		if (pos >= mWidgetLines.size()) mLastRedrawLine = pos;
	}

	// �������������� ������
	void List::_redrawItem(size_t _index)
	{
		// �������
		if (_index < (size_t)mTopIndex) return;
		_index -= (size_t)mTopIndex;
		// ���� �������
		if (_index > mLastRedrawLine) return;
		// ��������������
		mWidgetLines[_index]->setCaption(mStringArray[_index + mTopIndex]);
	}

	void List::insertItemString(size_t _index, const Ogre::DisplayString & _item)
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
			if (mWidgetClient->height() < (offset - mHeightLine)) {
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

	void List::deleteItemString(size_t _index)
	{
		// �������, �� ��������
		assert(_index < mStringArray.size());

		// ������� ��������� ������
		_deleteString(_index);

		// ���� ����, �� ������ ���������� �������
		if (mStringArray.empty()) mIndexSelect = ITEM_NONE;
		else if (mIndexSelect != ITEM_NONE) {
			if (_index < mIndexSelect) mIndexSelect--;
			else if ( (_index == mIndexSelect) && (mIndexSelect == (mStringArray.size())) ) mIndexSelect--;
		}

		// ���� �������� ����� ������ , �� �������� �������
		if (mWidgetLines.size() > mStringArray.size()) {
			mWidgetLines[mStringArray.size()]->show(false);
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
			if (mWidgetClient->height() < offset) {
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

	void List::_deleteString(size_t _index)
	{
		for (size_t pos=_index+1; pos<mStringArray.size(); pos++) {
			mStringArray[pos-1] = mStringArray[pos];
		}
		mStringArray.pop_back();
	}

	void List::_insertString(size_t _index, const Ogre::DisplayString & _item)
	{
		mStringArray.push_back("");
		for (size_t pos=mStringArray.size()-1; pos > _index; pos--) {
			mStringArray[pos] = mStringArray[pos-1];
		}
		mStringArray[_index] = _item;
	}

	void List::setItemSelect(size_t _index)
	{
		if (mIndexSelect == _index) return;
		_selectIndex(mIndexSelect, false);
		_selectIndex(_index, true);
		mIndexSelect = _index;
	}

	void List::_selectIndex(size_t _index, bool _select)
	{
		if (_index >= mStringArray.size()) return;
		// �� ����� ������
		if (_index < (size_t)mTopIndex) return;
		// ���������� ��������� ������
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;
		// ������, ����� ���������� �������� ��������
		if (mWidgetClient->height() < offset) return;

		static_cast<ButtonPtr>(mWidgetLines[_index-mTopIndex])->setButtonPressed(_select);
	}

	void List::beginToIndex(size_t _index)
	{
		if (_index >= mStringArray.size()) return;
		if (mRangeIndex <= 0) return;

		int offset = (int)_index * mHeightLine;
		if (offset >= mRangeIndex) offset = mRangeIndex;

		if (mWidgetScroll->getScrollPosition() == offset) return;

		mWidgetScroll->setScrollPosition(offset);
		notifyScrollChangePosition(null, offset);
	}

	// ����� �� �� �������, ��������� ��� ���
	bool List::isItemVisible(size_t _index, bool _fill)
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
		if (mWidgetClient->height() < offset) return false;

		// ���� �� ����� � ��� ����� �����
		if ((mWidgetClient->height() < (offset + mHeightLine)) && (_fill) ) return false;

		return true;
	}

} // namespace MyGUI