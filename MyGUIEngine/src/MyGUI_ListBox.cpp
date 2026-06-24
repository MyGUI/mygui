/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_Button.h"
#include "MyGUI_ScrollBar.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LanguageManager.h"

namespace MyGUI
{

	void ListBox::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME we need key focus
		setNeedKeyFocus(true);

		if (isUserString("SkinLine"))
			mSkinLine = getUserString("SkinLine");

		if (isUserString("HeightLine"))
			mHeightLine = utility::parseInt(getUserString("HeightLine"));

		if (mHeightLine < 1)
			mHeightLine = 1;

		if (getClientWidget() != nullptr)
		{
			getClientWidget()->eventMouseButtonPressed += newDelegate(this, &ListBox::notifyMousePressed);
			getClientWidget()->eventMouseButtonReleased += newDelegate(this, &ListBox::notifyMouseButtonReleased);
			getClientWidget()->eventKeyButtonPressed += newDelegate(this, &ListBox::notifyKeyButtonPressed);
			getClientWidget()->eventKeyButtonReleased += newDelegate(this, &ListBox::notifyKeyButtonReleased);
		}

		///@wskin_child{ListBox, ScrollBar, VScroll} Vertical scroll bar.
		assignWidget(mWidgetScroll, "VScroll");
		if (mWidgetScroll != nullptr)
		{
			mWidgetScroll->eventScrollChangePosition += newDelegate(this, &ListBox::notifyScrollChangePosition);
			mWidgetScroll->setScrollPage((size_t)mHeightLine);
		}

		updateScroll();
		updateLine();
	}

	void ListBox::shutdownOverride()
	{
		mWidgetScroll = nullptr;

		Base::shutdownOverride();
	}

	void ListBox::onMouseWheel(int _rel)
	{
		notifyMouseWheel(nullptr, _rel);

		Base::onMouseWheel(_rel);
	}

	void ListBox::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		if (getItemCount() == 0)
		{
			Base::onKeyButtonPressed(_key, _char);
			eventNotifyItem(this, IBNotifyItemData(ITEM_NONE, IBNotifyItemData::KeyPressed, _key, _char));
			return;
		}

		// very secret patented cursor movement mechanism
		size_t sel = mIndexSelect;

		if (_key == KeyCode::ArrowUp)
		{
			if (sel != 0)
			{
				if (sel == ITEM_NONE)
					sel = 0;
				else
					sel--;
			}
		}
		else if (_key == KeyCode::ArrowDown)
		{
			if (sel == ITEM_NONE)
				sel = 0;
			else
				sel++;

			if (sel >= getItemCount())
			{
				sel = mIndexSelect;
			}
		}
		else if (_key == KeyCode::Home)
		{
			sel = 0;
		}
		else if (_key == KeyCode::End)
		{
			sel = getItemCount() - 1;
		}
		else if (_key == KeyCode::PageUp)
		{
			if (sel != 0)
			{
				if (sel == ITEM_NONE)
				{
					sel = 0;
				}
				else
				{
					size_t page = _getClientWidget()->getHeight() / mHeightLine;
					if (sel <= page)
						sel = 0;
					else
						sel -= page;
				}
			}
		}
		else if (_key == KeyCode::PageDown)
		{
			if (sel != (getItemCount() - 1))
			{
				if (sel == ITEM_NONE)
				{
					sel = 0;
				}
				else
				{
					sel += _getClientWidget()->getHeight() / mHeightLine;
					if (sel >= getItemCount())
						sel = getItemCount() - 1;
				}
			}
		}
		else if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter))
		{
			if (sel != ITEM_NONE)
			{
				//FIXME we may be deleted
				eventListSelectAccept(this, sel);

				Base::onKeyButtonPressed(_key, _char);

				eventNotifyItem(this, IBNotifyItemData(ITEM_NONE, IBNotifyItemData::KeyPressed, _key, _char));
				// exit because number of lines changed
				return;
			}
		}

		if (sel != mIndexSelect)
		{
			_resetContainer(true);

			if (!isItemVisibleAt(sel))
			{
				beginToItemAt(sel);
				if (mWidgetScroll != nullptr)
					_sendEventChangeScroll(mWidgetScroll->getScrollPosition());
			}
			setIndexSelected(sel);

			// position changed
			// FIXME we may be deleted
			eventListChangePosition(this, mIndexSelect);
		}

		Base::onKeyButtonPressed(_key, _char);
		eventNotifyItem(this, IBNotifyItemData(ITEM_NONE, IBNotifyItemData::KeyPressed, _key, _char));
	}

	void ListBox::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mRangeIndex <= 0)
			return;

		if (mWidgetScroll == nullptr)
			return;

		int offset = (int)mWidgetScroll->getScrollPosition();
		if (_rel < 0)
			offset += mHeightLine;
		else
			offset -= mHeightLine;

		if (offset >= mRangeIndex)
			offset = mRangeIndex;
		else if (offset < 0)
			offset = 0;

		if ((int)mWidgetScroll->getScrollPosition() == offset)
			return;

		mWidgetScroll->setScrollPosition(offset);
		_setScrollView(offset);
		_sendEventChangeScroll(offset);

		_resetContainer(true);
	}

	void ListBox::notifyScrollChangePosition(ScrollBar* _sender, size_t _position)
	{
		_setScrollView(_position);
		_sendEventChangeScroll(_position);
	}

	void ListBox::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left == _id && !mActivateOnClick)
			_activateItem(_sender);

		eventNotifyItem(
			this,
			IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::MousePressed, _left, _top, _id));
	}

	void ListBox::notifyMouseClick(MyGUI::Widget* _sender)
	{
		if (mActivateOnClick)
			_activateItem(_sender);
	}

	void ListBox::notifyMouseDoubleClick(Widget* _sender)
	{
		if (mIndexSelect != ITEM_NONE)
			eventListSelectAccept(this, mIndexSelect);
	}

	void ListBox::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ListBox::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateScroll();
		updateLine();
	}

	void ListBox::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateScroll();
		updateLine();
	}

	void ListBox::updateScroll()
	{
		mRangeIndex = (mHeightLine * (int)mItemsInfo.size()) - _getClientWidget()->getHeight();

		if (mWidgetScroll == nullptr)
			return;

		if ((!mNeedVisibleScroll) || (mRangeIndex < 1) || (mWidgetScroll->getLeft() <= _getClientWidget()->getLeft()))
		{
			if (mWidgetScroll->getVisible())
			{
				mWidgetScroll->setVisible(false);
				// increase client area by scroll width
				if (getClientWidget() != nullptr)
					getClientWidget()->setSize(
						getClientWidget()->getWidth() + mWidgetScroll->getWidth(),
						getClientWidget()->getHeight());
			}
		}
		else if (!mWidgetScroll->getVisible())
		{
			if (getClientWidget() != nullptr)
				getClientWidget()->setSize(
					getClientWidget()->getWidth() - mWidgetScroll->getWidth(),
					getClientWidget()->getHeight());
			mWidgetScroll->setVisible(true);
		}

		mWidgetScroll->setScrollRange(mRangeIndex + 1);
		mWidgetScroll->setScrollViewPage(_getClientWidget()->getHeight());
		if (!mItemsInfo.empty())
			mWidgetScroll->setTrackSize(
				mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size());
	}

	void ListBox::updateLine(bool _reset)
	{
		if (_reset)
		{
			mOldSize.clear();
			mLastRedrawLine = 0;
			_resetContainer(false);
		}

		int position = mTopIndex * mHeightLine + mOffsetTop;

		if (mOldSize.height < mCoord.height)
		{
			int height = (int)mWidgetLines.size() * mHeightLine - mOffsetTop;

			// until max count reached, always one more
			while ((height <= (_getClientWidget()->getHeight() + mHeightLine)) &&
				   (mWidgetLines.size() < mItemsInfo.size()))
			{
				Widget* widget = _getClientWidget()->createWidgetT(
					"Button",
					mSkinLine,
					0,
					height,
					_getClientWidget()->getWidth(),
					mHeightLine,
					Align::Top | Align::HStretch);
				Button* line = widget->castType<Button>();
				line->eventMouseButtonPressed += newDelegate(this, &ListBox::notifyMousePressed);
				line->eventMouseButtonReleased += newDelegate(this, &ListBox::notifyMouseButtonReleased);
				line->eventMouseButtonClick += newDelegate(this, &ListBox::notifyMouseClick);
				line->eventMouseButtonDoubleClick += newDelegate(this, &ListBox::notifyMouseDoubleClick);
				line->eventMouseWheel += newDelegate(this, &ListBox::notifyMouseWheel);
				line->eventKeyButtonPressed += newDelegate(this, &ListBox::notifyKeyButtonPressed);
				line->eventKeyButtonReleased += newDelegate(this, &ListBox::notifyKeyButtonReleased);
				line->eventMouseSetFocus += newDelegate(this, &ListBox::notifyMouseSetFocus);
				line->eventMouseLostFocus += newDelegate(this, &ListBox::notifyMouseLostFocus);
				line->_setContainer(this);
				line->_setInternalData((size_t)mWidgetLines.size());
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}

			// check if list position should stay unchanged
			if (position >= mRangeIndex)
			{
				if (mRangeIndex <= 0)
				{
					if (position || mOffsetTop || mTopIndex)
					{
						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;
						mLastRedrawLine = 0;

						int offset = 0;
						for (auto& widgetLine : mWidgetLines)
						{
							widgetLine->setPosition(0, offset);
							offset += mHeightLine;
						}
					}
				}
				else
				{
					// push list to bottom border
					int count = _getClientWidget()->getHeight() / mHeightLine;
					mOffsetTop = mHeightLine - (_getClientWidget()->getHeight() % mHeightLine);

					if (mOffsetTop == mHeightLine)
					{
						mOffsetTop = 0;
						count--;
					}

					int top = (int)mItemsInfo.size() - count - 1;

					int offset = 0 - mOffsetTop;
					for (auto& widgetLine : mWidgetLines)
					{
						widgetLine->setPosition(0, offset);
						offset += mHeightLine;
					}

					// calculate position, should be maximum
					position = top * mHeightLine + mOffsetTop;

					if (top != mTopIndex)
					{
						mTopIndex = top;
						_redrawItemRange();
					}
				}
			}

			// size increased but no scroll down, update lines below
			_redrawItemRange(mLastRedrawLine);

		} // if (old_cy < mCoord.height)

		if (mWidgetScroll != nullptr)
			mWidgetScroll->setScrollPosition(position);

		mOldSize.width = mCoord.width;
		mOldSize.height = mCoord.height;
	}

	void ListBox::_redrawItemRange(size_t _start)
	{
		// redraw only visible lines
		size_t pos = _start;
		for (; pos < mWidgetLines.size(); pos++)
		{
			size_t index = pos + (size_t)mTopIndex;

			if (index >= mItemsInfo.size())
			{
				mLastRedrawLine = pos;
				break;
			}
			if (mWidgetLines[pos]->getTop() > _getClientWidget()->getHeight())
			{
				mLastRedrawLine = pos;
				break;
			}

			mWidgetLines[pos]->setVisible(true);
			mWidgetLines[pos]->setCaption(mItemsInfo[index].first);

			static_cast<Button*>(mWidgetLines[pos])->setStateSelected(index == mIndexSelect);
		}

		if (pos >= mWidgetLines.size())
		{
			mLastRedrawLine = pos;
		}
		else
		{
			//Widget* focus = InputManager::getInstance().getMouseFocusWidget();
			for (; pos < mWidgetLines.size(); pos++)
			{
				static_cast<Button*>(mWidgetLines[pos])->setStateSelected(false);
				static_cast<Button*>(mWidgetLines[pos])->setVisible(false);
				//if (focus == mWidgetLines[pos]) InputManager::getInstance()._unlinkWidget(focus);
			}
		}
	}

	void ListBox::_redrawItem(size_t _index)
	{
		if (_index < (size_t)mTopIndex)
			return;
		_index -= (size_t)mTopIndex;
		if (_index >= mLastRedrawLine)
			return;

		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::_redrawItem");
		mWidgetLines[_index]->setCaption(mItemsInfo[_index + mTopIndex].first);
	}

	void ListBox::insertItemAt(size_t _index, const UString& _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "ListBox::insertItemAt");
		if (_index == ITEM_NONE)
			_index = mItemsInfo.size();

		mItemsInfo.insert(mItemsInfo.begin() + _index, PairItem(_name, _data));

		if ((mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect))
			mIndexSelect++;

		if ((_index <= (size_t)mTopIndex) && (mRangeIndex > 0))
		{
			mTopIndex++;
			if (mWidgetScroll != nullptr)
			{
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
				if (!mItemsInfo.empty())
					mWidgetScroll->setTrackSize(
						mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine /
						(int)mItemsInfo.size());
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			}
			mRangeIndex += mHeightLine;
		}
		else
		{
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// line after last visible element, plus one (scroll needs one more)
			if (_getClientWidget()->getHeight() < (offset - mHeightLine))
			{
				if (mWidgetScroll != nullptr)
				{
					mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
					if (!mItemsInfo.empty())
						mWidgetScroll->setTrackSize(
							mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine /
							(int)mItemsInfo.size());
					mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				}
				mRangeIndex += mHeightLine;
			}
			else
			{
				updateScroll();
				updateLine(true);

				// later add optimization for number of redraws
			}
		}
	}

	void ListBox::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::removeItemAt");

		mItemsInfo.erase(mItemsInfo.begin() + _index);

		if (mItemsInfo.empty())
			mIndexSelect = ITEM_NONE;
		else if (mIndexSelect != ITEM_NONE)
		{
			if (_index < mIndexSelect)
				mIndexSelect--;
			else if ((_index == mIndexSelect) && (mIndexSelect == (mItemsInfo.size())))
				mIndexSelect--;
		}

		// if more widgets than items, hide the last one
		if (mWidgetLines.size() > mItemsInfo.size())
		{
			mWidgetLines[mItemsInfo.size()]->setVisible(false);
		}

		if (_index < (size_t)mTopIndex)
		{
			mTopIndex--;
			if (mWidgetScroll != nullptr)
			{
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
				if (!mItemsInfo.empty())
					mWidgetScroll->setTrackSize(
						mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine /
						(int)mItemsInfo.size());
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			}
			mRangeIndex -= mHeightLine;
		}
		else
		{
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			if (_getClientWidget()->getHeight() < offset)
			{
				if (mWidgetScroll != nullptr)
				{
					mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
					if (!mItemsInfo.empty())
						mWidgetScroll->setTrackSize(
							mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine /
							(int)mItemsInfo.size());
					mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				}
				mRangeIndex -= mHeightLine;
			}
			else
			{
				updateScroll();
				updateLine(true);

				// later add optimization for number of redraws
			}
		}
	}

	void ListBox::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "ListBox::setIndexSelected");
		if (mIndexSelect != _index)
		{
			_selectIndex(mIndexSelect, false);
			_selectIndex(_index, true);
			mIndexSelect = _index;
		}
	}

	void ListBox::_selectIndex(size_t _index, bool _select)
	{
		if (_index == ITEM_NONE)
			return;
		if (_index < (size_t)mTopIndex)
			return;
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;
		if (_getClientWidget()->getHeight() < offset)
			return;

		size_t index = _index - mTopIndex;
		if (index < mWidgetLines.size())
			static_cast<Button*>(mWidgetLines[index])->setStateSelected(_select);
	}

	void ListBox::beginToItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::beginToItemAt");
		if (mRangeIndex <= 0)
			return;

		int offset = (int)_index * mHeightLine;
		if (offset >= mRangeIndex)
			offset = mRangeIndex;

		if (mWidgetScroll != nullptr)
		{
			if ((int)mWidgetScroll->getScrollPosition() == offset)
				return;
			mWidgetScroll->setScrollPosition(offset);
		}
		notifyScrollChangePosition(nullptr, offset);
	}

	// whether item is fully or partially visible
	bool ListBox::isItemVisibleAt(size_t _index, bool _fill)
	{
		if (_index >= mItemsInfo.size())
			return false;
		if (mRangeIndex <= 0)
			return true;

		if (_index < (size_t)mTopIndex)
			return false;

		if (_index == (size_t)mTopIndex)
		{
			return mOffsetTop == 0 || !_fill;
		}

		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

		if (_getClientWidget()->getHeight() < offset)
			return false;

		if (_getClientWidget()->getHeight() < (offset + mHeightLine) && _fill)
			return false;

		return true;
	}

	void ListBox::removeAllItems()
	{
		mTopIndex = 0;
		mIndexSelect = ITEM_NONE;
		mOffsetTop = 0;

		mItemsInfo.clear();

		int offset = 0;
		for (auto& widgetLine : mWidgetLines)
		{
			widgetLine->setVisible(false);
			widgetLine->setPosition(0, offset);
			offset += mHeightLine;
		}

		updateScroll();
		updateLine(true);
	}

	void ListBox::setItemNameAt(size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::setItemNameAt");
		mItemsInfo[_index].first = _name;
		_redrawItem(_index);
	}

	void ListBox::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::setItemDataAt");
		mItemsInfo[_index].second = std::move(_data);
		_redrawItem(_index);
	}

	const UString& ListBox::getItemNameAt(size_t _index) const
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListBox::getItemNameAt");
		return mItemsInfo[_index].first;
	}

	void ListBox::notifyMouseSetFocus(Widget* _sender, Widget* _old)
	{
#if MYGUI_DEBUG_MODE == 1
		MYGUI_ASSERT_RANGE(*_sender->_getInternalData<size_t>(), mWidgetLines.size(), "ListBox::notifyMouseSetFocus");
#endif

		mLineActive = *_sender->_getInternalData<size_t>();
		eventListMouseItemFocus(this, mLineActive);
	}

	void ListBox::notifyMouseLostFocus(Widget* _sender, Widget* _new)
	{
		if ((nullptr == _new) || (_new->getParent() != _getClientWidget()))
		{
			mLineActive = ITEM_NONE;
			eventListMouseItemFocus(this, ITEM_NONE);
		}
	}

	void ListBox::_setItemFocus(size_t _index, bool _focus)
	{
		MYGUI_ASSERT_RANGE(_index, mWidgetLines.size(), "ListBox::_setItemFocus");
		static_cast<Button*>(mWidgetLines[_index])->_setMouseFocus(_focus);
	}

	void ListBox::setScrollVisible(bool _visible)
	{
		if (mNeedVisibleScroll == _visible)
			return;
		mNeedVisibleScroll = _visible;
		updateScroll();
	}

	void ListBox::setScrollPosition(size_t _position)
	{
		if (mWidgetScroll != nullptr)
		{
			if (mWidgetScroll->getScrollRange() > _position)
			{
				mWidgetScroll->setScrollPosition(_position);
				_setScrollView(_position);
			}
		}
	}

	void ListBox::_setScrollView(size_t _position)
	{
		mOffsetTop = ((int)_position % mHeightLine);

		// offset from negative side
		int offset = 0 - mOffsetTop;

		for (auto& widgetLine : mWidgetLines)
		{
			widgetLine->setPosition(IntPoint(0, offset));
			offset += mHeightLine;
		}

		int top = ((int)_position / mHeightLine);
		if (top != mTopIndex)
		{
			mTopIndex = top;
			_redrawItemRange();
		}

		// render all bottom lines if they appeared
		_redrawItemRange(mLastRedrawLine);
	}

	void ListBox::_sendEventChangeScroll(size_t _position)
	{
		eventListChangeScroll(this, _position);
		if (ITEM_NONE != mLineActive)
			eventListMouseItemFocus(this, mLineActive);
	}

	void ListBox::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "ListBox::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "ListBox::swapItemsAt");

		if (_index1 == _index2)
			return;

		std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);

		_redrawItem(_index1);
		_redrawItem(_index2);
	}

	void ListBox::_checkAlign()
	{
		int max_height = mItemsInfo.size() * mHeightLine;
		int visible_height = _getClientWidget()->getHeight();

		if (visible_height >= max_height)
		{
			MYGUI_ASSERT(mTopIndex == 0, "mTopIndex == 0");
			MYGUI_ASSERT(mOffsetTop == 0, "mOffsetTop == 0");
			int height = 0;
			for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
			{
				if (pos >= mItemsInfo.size())
					break;
				MYGUI_ASSERT(mWidgetLines[pos]->getTop() == height, "mWidgetLines[pos]->getTop() == height");
				height += mWidgetLines[pos]->getHeight();
			}
		}
	}

	size_t ListBox::findItemIndexWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].first == _name)
				return pos;
		}
		return ITEM_NONE;
	}

	int ListBox::getOptimalHeight() const
	{
		return (int)((mCoord.height - _getClientWidget()->getHeight()) + (mItemsInfo.size() * mHeightLine));
	}

	size_t ListBox::getItemCount() const
	{
		return mItemsInfo.size();
	}

	void ListBox::addItem(const UString& _name, Any _data)
	{
		insertItemAt(ITEM_NONE, _name, _data);
	}

	size_t ListBox::getIndexSelected() const
	{
		return mIndexSelect;
	}

	void ListBox::clearIndexSelected()
	{
		setIndexSelected(ITEM_NONE);
	}

	void ListBox::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	void ListBox::beginToItemFirst()
	{
		if (getItemCount())
			beginToItemAt(0);
	}

	void ListBox::beginToItemLast()
	{
		if (getItemCount())
			beginToItemAt(getItemCount() - 1);
	}

	void ListBox::beginToItemSelected()
	{
		if (getIndexSelected() != ITEM_NONE)
			beginToItemAt(getIndexSelected());
	}

	bool ListBox::isItemSelectedVisible(bool _fill)
	{
		return isItemVisibleAt(mIndexSelect, _fill);
	}

	size_t ListBox::_getItemIndex(Widget* _item) const
	{
		for (const auto& line : mWidgetLines)
		{
			if (line == _item)
				return *line->_getInternalData<size_t>() + mTopIndex;
		}
		return ITEM_NONE;
	}

	void ListBox::_resetContainer(bool _update)
	{
		Base::_resetContainer(_update);

		if (!_update)
		{
			WidgetManager& instance = WidgetManager::getInstance();
			for (const auto& line : mWidgetLines)
				instance.unlinkFromUnlinkers(line);
		}
	}

	void ListBox::setPropertyOverride(std::string_view _key, std::string_view _value)
	{
		if (_key == "AddItem")
			addItem(LanguageManager::getInstance().replaceTags(UString(_value)));
		else if (_key == "ActivateOnClick")
			mActivateOnClick = utility::parseBool(_value);
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	void ListBox::_activateItem(MyGUI::Widget* _sender)
	{
		if (_sender == _getClientWidget())
		{
			if (mIndexSelect != ITEM_NONE)
			{
				_selectIndex(mIndexSelect, false);
				mIndexSelect = ITEM_NONE;
				eventListChangePosition(this, mIndexSelect);
			}
			eventListMouseItemActivate(this, mIndexSelect);
		}
		// cell may be hidden
		else if (_sender->getVisible())
		{
#if MYGUI_DEBUG_MODE == 1
			MYGUI_ASSERT_RANGE(
				*_sender->_getInternalData<size_t>(),
				mWidgetLines.size(),
				"ListBox::notifyMousePressed");
			MYGUI_ASSERT_RANGE(
				*_sender->_getInternalData<size_t>() + mTopIndex,
				mItemsInfo.size(),
				"ListBox::notifyMousePressed");
#endif

			size_t index = *_sender->_getInternalData<size_t>() + mTopIndex;

			if (mIndexSelect != index)
			{
				_selectIndex(mIndexSelect, false);
				_selectIndex(index, true);
				mIndexSelect = index;
				eventListChangePosition(this, mIndexSelect);
			}
			eventListMouseItemActivate(this, mIndexSelect);
		}

		_resetContainer(true);
	}

	size_t ListBox::_getItemCount() const
	{
		return getItemCount();
	}

	void ListBox::_addItem(const MyGUI::UString& _name)
	{
		addItem(_name);
	}

	void ListBox::_removeItemAt(size_t _index)
	{
		removeItemAt(_index);
	}

	void ListBox::_setItemNameAt(size_t _index, const UString& _name)
	{
		setItemNameAt(_index, _name);
	}

	const UString& ListBox::_getItemNameAt(size_t _index) const
	{
		return getItemNameAt(_index);
	}

	size_t ListBox::getIndexByWidget(Widget* _widget) const
	{
		if (_widget == getClientWidget())
			return ITEM_NONE;
		return *_widget->_getInternalData<size_t>() + mTopIndex;
	}

	void ListBox::notifyKeyButtonPressed(Widget* _sender, KeyCode _key, Char _char)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyPressed, _key, _char));
	}

	void ListBox::notifyKeyButtonReleased(Widget* _sender, KeyCode _key)
	{
		eventNotifyItem(this, IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::KeyReleased, _key));
	}

	void ListBox::notifyMouseButtonReleased(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		eventNotifyItem(
			this,
			IBNotifyItemData(getIndexByWidget(_sender), IBNotifyItemData::MouseReleased, _left, _top, _id));
	}

	void ListBox::onKeyButtonReleased(KeyCode _key)
	{
		Base::onKeyButtonReleased(_key);

		eventNotifyItem(this, IBNotifyItemData(ITEM_NONE, IBNotifyItemData::KeyReleased, _key));
	}

	void ListBox::setActivateOnClick(bool activateOnClick)
	{
		mActivateOnClick = activateOnClick;
	}

	Widget* ListBox::getWidgetByIndex(size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
			return nullptr;

		size_t index = _index - (size_t)mTopIndex;

		if (index < mWidgetLines.size())
			return mWidgetLines[index];
		return nullptr;
	}

} // namespace MyGUI
