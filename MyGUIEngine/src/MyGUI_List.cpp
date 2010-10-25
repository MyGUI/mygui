/*!
	@file
	@author		Albert Semenov
	@date		11/2007
*/
/*
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
#include "MyGUI_List.h"
#include "MyGUI_Button.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	List::List() :
		mWidgetScroll(nullptr),
		mHeightLine(1),
		mTopIndex(0),
		mOffsetTop(0),
		mRangeIndex(-1),
		mLastRedrawLine(0),
		mIndexSelect(ITEM_NONE),
		mLineActive(ITEM_NONE),
		mNeedVisibleScroll(true),
		mClient(nullptr)
	{
	}

	void List::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		// парсим свойства
		if (isUserString("SkinLine"))
			mSkinLine = getUserString("SkinLine");

		if (isUserString("HeightLine"))
			mHeightLine = utility::parseInt(getUserString("HeightLine"));

		if (mHeightLine < 1)
			mHeightLine = 1;

		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			mClient->eventMouseButtonPressed += newDelegate(this, &List::notifyMousePressed);
			setWidgetClient(mClient);
		}

		assignWidget(mWidgetScroll, "VScroll");
		if (mWidgetScroll != nullptr)
		{
			mWidgetScroll->eventScrollChangePosition += newDelegate(this, &List::notifyScrollChangePosition);
			mWidgetScroll->eventMouseButtonPressed += newDelegate(this, &List::notifyMousePressed);
			mWidgetScroll->setScrollPage((size_t)mHeightLine);
			mWidgetScroll->setScrollViewPage((size_t)mHeightLine);
		}

		updateScroll();
		updateLine();
	}

	void List::shutdownOverride()
	{
		mWidgetScroll = nullptr;
		mClient = nullptr;

		Base::shutdownOverride();
	}

	void List::onMouseWheel(int _rel)
	{
		notifyMouseWheel(nullptr, _rel);

		Base::onMouseWheel(_rel);
	}

	void List::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		if (getItemCount() == 0)
		{
			Base::onKeyButtonPressed(_key, _char);
			return;
		}

		// очень секретный метод, запатентованный механизм движения курсора
		size_t sel = mIndexSelect;

		if (_key == KeyCode::ArrowUp)
		{
			if (sel != 0)
			{
				if (sel == ITEM_NONE)
					sel = 0;
				else
					sel --;
			}
		}
		else if (_key == KeyCode::ArrowDown)
		{
			if (sel == ITEM_NONE)
				sel = 0;
			else
				sel ++;

			if (sel >= getItemCount())
			{
				// старое значение
				sel = mIndexSelect;
			}
		}
		else if (_key == KeyCode::Home)
		{
			if (sel != 0)
				sel = 0;
		}
		else if (_key == KeyCode::End)
		{
			if (sel != (getItemCount() - 1))
			{
				sel = getItemCount() - 1;
			}
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
				//FIXME нас могут удалить
				eventListSelectAccept(this, sel);

				Base::onKeyButtonPressed(_key, _char);
				// выходим, так как изменили колличество строк
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

			// изменилась позиция
			// FIXME нас могут удалить
			eventListChangePosition(this, mIndexSelect);
		}

		Base::onKeyButtonPressed(_key, _char);
	}

	void List::notifyMouseWheel(Widget* _sender, int _rel)
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

	void List::notifyScrollChangePosition(VScroll* _sender, size_t _position)
	{
		_setScrollView(_position);
		_sendEventChangeScroll(_position);
	}

	void List::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (MouseButton::Left != _id)
			return;

		if (_sender == mWidgetScroll)
			return;

		// если выделен клиент, то сбрасываем
		if (_sender == _getClientWidget())
		{
			if (mIndexSelect != ITEM_NONE)
			{
				_selectIndex(mIndexSelect, false);
				mIndexSelect = ITEM_NONE;
				eventListChangePosition(this, mIndexSelect);
			}
			eventListMouseItemActivate(this, mIndexSelect);

			// если не клиент, то просчитывам
		}
		// ячейка может быть скрыта
		else if (_sender->getVisible())
		{

#if MYGUI_DEBUG_MODE == 1
			_checkMapping("List::notifyMousePressed");
			MYGUI_ASSERT_RANGE(*_sender->_getInternalData<size_t>(), mWidgetLines.size(), "List::notifyMousePressed");
			MYGUI_ASSERT_RANGE(*_sender->_getInternalData<size_t>() + mTopIndex, mItemsInfo.size(), "List::notifyMousePressed");
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

	void List::notifyMouseDoubleClick(Widget* _sender)
	{
		if (mIndexSelect != ITEM_NONE)
			eventListSelectAccept(this, mIndexSelect);
	}

	void List::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void List::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateScroll();
		updateLine();
	}

	void List::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateScroll();
		updateLine();
	}

	void List::updateScroll()
	{
		mRangeIndex = (mHeightLine * (int)mItemsInfo.size()) - _getClientWidget()->getHeight();

		if (mWidgetScroll == nullptr)
			return;

		if ((!mNeedVisibleScroll) || (mRangeIndex < 1) || (mWidgetScroll->getLeft() <= _getClientWidget()->getLeft()))
		{
			if (mWidgetScroll->getVisible())
			{
				mWidgetScroll->setVisible(false);
				// увеличиваем клиентскую зону на ширину скрола
				if (mClient != nullptr)
					mClient->setSize(mClient->getWidth() + mWidgetScroll->getWidth(), mClient->getHeight());
			}
		}
		else if (!mWidgetScroll->getVisible())
		{
			if (mClient != nullptr)
				mClient->setSize(mClient->getWidth() - mWidgetScroll->getWidth(), mClient->getHeight());
			mWidgetScroll->setVisible(true);
		}

		mWidgetScroll->setScrollRange(mRangeIndex + 1);
		if ((int)mItemsInfo.size())
			mWidgetScroll->setTrackSize(mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size());
	}

	void List::updateLine(bool _reset)
	{
		// сбрасываем
		if (_reset)
		{
			mOldSize.clear();
			mLastRedrawLine = 0;
			_resetContainer(false);
		}

		// позиция скролла
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// если высота увеличивалась то добавляем виджеты
		if (mOldSize.height < mCoord.height)
		{
			int height = (int)mWidgetLines.size() * mHeightLine - mOffsetTop;

			// до тех пор, пока не достигнем максимального колличества, и всегда на одну больше
			while ( (height <= (_getClientWidget()->getHeight() + mHeightLine)) && (mWidgetLines.size() < mItemsInfo.size()) )
			{
				// создаем линию
				Widget* widget = _getClientWidget()->createWidgetT("Button", mSkinLine, 0, height, _getClientWidget()->getWidth(), mHeightLine, Align::Top | Align::HStretch);
				Button* line = widget->castType<Button>();
				// подписываемся на всякие там события
				line->eventMouseButtonPressed += newDelegate(this, &List::notifyMousePressed);
				line->eventMouseButtonDoubleClick += newDelegate(this, &List::notifyMouseDoubleClick);
				line->eventMouseWheel += newDelegate(this, &List::notifyMouseWheel);
				line->eventMouseSetFocus += newDelegate(this, &List::notifyMouseSetFocus);
				line->eventMouseLostFocus += newDelegate(this, &List::notifyMouseLostFocus);
				line->_setContainer(this);
				// присваиваем порядковый номер, для простоты просчета
				line->_setInternalData((size_t)mWidgetLines.size());
				// и сохраняем
				mWidgetLines.push_back(line);
				height += mHeightLine;
			}

			// проверяем на возможность не менять положение списка
			if (position >= mRangeIndex)
			{
				// размер всех помещается в клиент
				if (mRangeIndex <= 0)
				{
					// обнуляем, если надо
					if (position || mOffsetTop || mTopIndex)
					{
						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;
						mLastRedrawLine = 0; // чтобы все перерисовалось

						// выравниваем
						int offset = 0;
						for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
						{
							mWidgetLines[pos]->setPosition(0, offset);
							offset += mHeightLine;
						}
					}
				}
				else
				{
					// прижимаем список к нижней границе
					int count = _getClientWidget()->getHeight() / mHeightLine;
					mOffsetTop = mHeightLine - (_getClientWidget()->getHeight() % mHeightLine);

					if (mOffsetTop == mHeightLine)
					{
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mItemsInfo.size() - count - 1;

					// выравниваем
					int offset = 0 - mOffsetTop;
					for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
					{
						mWidgetLines[pos]->setPosition(0, offset);
						offset += mHeightLine;
					}

					// высчитываем положение, должно быть максимальным
					position = top * mHeightLine + mOffsetTop;

					// если индех изменился, то перерисовываем линии
					if (top != mTopIndex)
					{
						mTopIndex = top;
						_redrawItemRange();
					}
				}
			}

			// увеличился размер, но прокрутки вниз небыло, обновляем линии снизу
			_redrawItemRange(mLastRedrawLine);

		} // if (old_cy < mCoord.height)

		// просчитываем положение скролла
		if (mWidgetScroll != nullptr)
			mWidgetScroll->setScrollPosition(position);

		mOldSize.width = mCoord.width;
		mOldSize.height = mCoord.height;

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::updateLine");
#endif
	}

	void List::_redrawItemRange(size_t _start)
	{
		// перерисовываем линии, только те, что видны
		size_t pos = _start;
		for (; pos < mWidgetLines.size(); pos++)
		{
			// индекс в нашем массиве
			size_t index = pos + (size_t)mTopIndex;

			// не будем заходить слишком далеко
			if (index >= mItemsInfo.size())
			{
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}
			if (mWidgetLines[pos]->getTop() > _getClientWidget()->getHeight())
			{
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}

			// если был скрыт, то покажем
			mWidgetLines[pos]->setVisible(true);
			// обновляем текст
			mWidgetLines[pos]->setCaption(mItemsInfo[index].first);

			// если нужно выделить ,то выделим
			static_cast<Button*>(mWidgetLines[pos])->setStateSelected(index == mIndexSelect);
		}

		// если цикл весь прошли, то ставим максимальную линию
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

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::_redrawItemRange");
#endif
	}

	// перерисовывает индекс
	void List::_redrawItem(size_t _index)
	{
		// невидно
		if (_index < (size_t)mTopIndex)
			return;
		_index -= (size_t)mTopIndex;
		// тоже невидно
		if (_index >= mLastRedrawLine)
			return;

		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::_redrawItem");
		// перерисовываем
		mWidgetLines[_index]->setCaption(mItemsInfo[_index + mTopIndex].first);

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::_redrawItem");
#endif
	}

	void List::insertItemAt(size_t _index, const UString& _name, Any _data)
	{
		MYGUI_ASSERT_RANGE_INSERT(_index, mItemsInfo.size(), "List::insertItemAt");
		if (_index == ITEM_NONE)
			_index = mItemsInfo.size();

		// вставляем физически
		mItemsInfo.insert(mItemsInfo.begin() + _index, PairItem(_name, _data));

		// если надо, то меняем выделенный элемент
		if ((mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect))
			mIndexSelect++;

		// строка, до первого видимого элемента
		if ((_index <= (size_t)mTopIndex) && (mRangeIndex > 0))
		{
			mTopIndex ++;
			// просчитываем положение скролла
			if (mWidgetScroll != nullptr)
			{
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
				if ((int)mItemsInfo.size())
					mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size() );
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			}
			mRangeIndex += mHeightLine;
		}
		else
		{
			// высчитывам положение строки
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// строка, после последнего видимого элемента, плюс одна строка (потому что для прокрутки нужно на одну строчку больше)
			if (_getClientWidget()->getHeight() < (offset - mHeightLine))
			{
				// просчитываем положение скролла
				if (mWidgetScroll != nullptr)
				{
					mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
					if ((int)mItemsInfo.size())
						mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size() );
					mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				}
				mRangeIndex += mHeightLine;

				// строка в видимой области
			}
			else
			{
				// обновляем все
				updateScroll();
				updateLine(true);

				// позже сюда еще оптимизацию по колличеству перерисовок
			}
		}

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::insertItemAt");
#endif
	}

	void List::removeItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::removeItemAt");

		// удяляем физически строку
		mItemsInfo.erase(mItemsInfo.begin() + _index);

		// если надо, то меняем выделенный элемент
		if (mItemsInfo.empty()) mIndexSelect = ITEM_NONE;
		else if (mIndexSelect != ITEM_NONE)
		{
			if (_index < mIndexSelect)
				mIndexSelect--;
			else if ((_index == mIndexSelect) && (mIndexSelect == (mItemsInfo.size())))
				mIndexSelect--;
		}

		// если виджетов стало больше , то скрываем крайний
		if (mWidgetLines.size() > mItemsInfo.size())
		{
			mWidgetLines[mItemsInfo.size()]->setVisible(false);
		}

		// строка, до первого видимого элемента
		if (_index < (size_t)mTopIndex)
		{
			mTopIndex --;
			// просчитываем положение скролла
			if (mWidgetScroll != nullptr)
			{
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
				if ((int)mItemsInfo.size())
					mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size() );
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			}
			mRangeIndex -= mHeightLine;
		}
		else
		{
			// высчитывам положение удаляемой строки
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// строка, после последнего видимого элемента
			if (_getClientWidget()->getHeight() < offset)
			{
				// просчитываем положение скролла
				if (mWidgetScroll != nullptr)
				{
					mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
					if ((int)mItemsInfo.size())
						mWidgetScroll->setTrackSize( mWidgetScroll->getLineSize() * _getClientWidget()->getHeight() / mHeightLine / (int)mItemsInfo.size() );
					mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				}
				mRangeIndex -= mHeightLine;

				// строка в видимой области
			}
			else
			{
				// обновляем все
				updateScroll();
				updateLine(true);

				// позже сюда еще оптимизацию по колличеству перерисовок
			}
		}

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::removeItemAt");
#endif
	}

	void List::setIndexSelected(size_t _index)
	{
		MYGUI_ASSERT_RANGE_AND_NONE(_index, mItemsInfo.size(), "List::setIndexSelected");
		if (mIndexSelect != _index)
		{
			_selectIndex(mIndexSelect, false);
			_selectIndex(_index, true);
			mIndexSelect = _index;
		}
	}

	void List::_selectIndex(size_t _index, bool _select)
	{
		if (_index == ITEM_NONE)
			return;
		// не видно строки
		if (_index < (size_t)mTopIndex)
			return;
		// высчитывам положение строки
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;
		// строка, после последнего видимого элемента
		if (_getClientWidget()->getHeight() < offset)
			return;

		size_t index = _index - mTopIndex;
		if (index < mWidgetLines.size())
			static_cast<Button*>(mWidgetLines[index])->setStateSelected(_select);

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::_selectIndex");
#endif
	}

	void List::beginToItemAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::beginToItemAt");
		if (mRangeIndex <= 0)
			return;

		int offset = (int)_index * mHeightLine;
		if (offset >= mRangeIndex) offset = mRangeIndex;

		if (mWidgetScroll != nullptr)
		{
			if ((int)mWidgetScroll->getScrollPosition() == offset)
				return;
			mWidgetScroll->setScrollPosition(offset);
		}
		notifyScrollChangePosition(nullptr, offset);

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::beginToItemAt");
#endif
	}

	// видим ли мы элемент, полностью или нет
	bool List::isItemVisibleAt(size_t _index, bool _fill)
	{
		// если элемента нет, то мы его не видим (в том числе когда их вообще нет)
		if (_index >= mItemsInfo.size())
			return false;
		// если скрола нет, то мы палюбак видим
		if (mRangeIndex <= 0)
			return true;

		// строка, до первого видимого элемента
		if (_index < (size_t)mTopIndex)
			return false;

		// строка это верхний выделенный
		if (_index == (size_t)mTopIndex)
		{
			if ((mOffsetTop != 0) && (_fill))
				return false; // нам нужна полностью видимость
			return true;
		}

		// высчитывам положение строки
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

		// строка, после последнего видимого элемента
		if (_getClientWidget()->getHeight() < offset)
			return false;

		// если мы внизу и нам нужен целый
		if ((_getClientWidget()->getHeight() < (offset + mHeightLine)) && (_fill))
			return false;

		return true;
	}

	void List::removeAllItems()
	{
		mTopIndex = 0;
		mIndexSelect = ITEM_NONE;
		mOffsetTop = 0;

		mItemsInfo.clear();

		int offset = 0;
		for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
		{
			mWidgetLines[pos]->setVisible(false);
			mWidgetLines[pos]->setPosition(0, offset);
			offset += mHeightLine;
		}

		// обновляем все
		updateScroll();
		updateLine(true);

#if MYGUI_DEBUG_MODE == 1
		_checkMapping("List::removeAllItems");
#endif
	}

	void List::setItemNameAt(size_t _index, const UString& _name)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::setItemNameAt");
		mItemsInfo[_index].first = _name;
		_redrawItem(_index);
	}

	void List::setItemDataAt(size_t _index, Any _data)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::setItemDataAt");
		mItemsInfo[_index].second = _data;
		_redrawItem(_index);
	}

	const UString& List::getItemNameAt(size_t _index)
	{
		MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "List::getItemNameAt");
		return mItemsInfo[_index].first;
	}

	void List::notifyMouseSetFocus(Widget* _sender, Widget* _old)
	{

#if MYGUI_DEBUG_MODE == 1
		MYGUI_ASSERT_RANGE(*_sender->_getInternalData<size_t>(), mWidgetLines.size(), "List::notifyMouseSetFocus");
#endif

		mLineActive = *_sender->_getInternalData<size_t>();
		eventListMouseItemFocus(this, mLineActive);
	}

	void List::notifyMouseLostFocus(Widget* _sender, Widget* _new)
	{
		if ((nullptr == _new) || (_new->getParent() != _getClientWidget()))
		{
			mLineActive = ITEM_NONE;
			eventListMouseItemFocus(this, ITEM_NONE);
		}
	}

	void List::_setItemFocus(size_t _index, bool _focus)
	{
		MYGUI_ASSERT_RANGE(_index, mWidgetLines.size(), "List::_setItemFocus");
		static_cast<Button*>(mWidgetLines[_index])->_setMouseFocus(_focus);
	}

	void List::setScrollVisible(bool _visible)
	{
		if (mNeedVisibleScroll == _visible)
			return;
		mNeedVisibleScroll = _visible;
		updateScroll();
	}

	void List::setScrollPosition(size_t _position)
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

	void List::_setScrollView(size_t _position)
	{
		mOffsetTop = ((int)_position % mHeightLine);

		// смещение с отрицательной стороны
		int offset = 0 - mOffsetTop;

		for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
		{
			mWidgetLines[pos]->setPosition(IntPoint(0, offset));
			offset += mHeightLine;
		}

		// если индех изменился, то перерисовываем линии
		int top = ((int)_position / mHeightLine);
		if (top != mTopIndex)
		{
			mTopIndex = top;
			_redrawItemRange();
		}

		// прорисовываем все нижние строки, если они появились
		_redrawItemRange(mLastRedrawLine);
	}

	void List::_sendEventChangeScroll(size_t _position)
	{
		eventListChangeScroll(this, _position);
		if (ITEM_NONE != mLineActive)
			eventListMouseItemFocus(this, mLineActive);
	}

	void List::swapItemsAt(size_t _index1, size_t _index2)
	{
		MYGUI_ASSERT_RANGE(_index1, mItemsInfo.size(), "List::swapItemsAt");
		MYGUI_ASSERT_RANGE(_index2, mItemsInfo.size(), "List::swapItemsAt");

		if (_index1 == _index2)
			return;

		std::swap(mItemsInfo[_index1], mItemsInfo[_index2]);

		_redrawItem(_index1);
		_redrawItem(_index2);
	}

	void List::_checkMapping(const std::string& _owner)
	{
		size_t count_pressed = 0;
		size_t count_show = 0;

		for (size_t pos = 0; pos < mWidgetLines.size(); pos++)
		{
			MYGUI_ASSERT(pos == *mWidgetLines[pos]->_getInternalData<size_t>(), _owner);
			static_cast<Button*>(mWidgetLines[pos])->getStateSelected() ? count_pressed ++ : 0;
			static_cast<Button*>(mWidgetLines[pos])->getVisible() ? count_show ++ : 0;
		}
		MYGUI_ASSERT(count_pressed < 2, _owner);
		//MYGUI_ASSERT((count_show + mOffsetTop) <= mItemsInfo.size(), _owner);
	}

	void List::_checkAlign()
	{
		// максимальная высота всех строк
		int max_height = mItemsInfo.size() * mHeightLine;
		// видимая высота
		int visible_height = _getClientWidget()->getHeight();

		// все строки помещаются
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

	size_t List::findItemIndexWith(const UString& _name)
	{
		for (size_t pos = 0; pos < mItemsInfo.size(); pos++)
		{
			if (mItemsInfo[pos].first == _name)
				return pos;
		}
		return ITEM_NONE;
	}

	int List::getOptimalHeight()
	{
		return (int)((mCoord.height - _getClientWidget()->getHeight()) + (mItemsInfo.size() * mHeightLine));
	}

	Widget* List::_getClientWidget()
	{
		return mClient == nullptr ? this : mClient;
	}

	size_t List::getItemCount() const
	{
		return mItemsInfo.size();
	}

	void List::addItem(const UString& _name, Any _data)
	{
		insertItemAt(ITEM_NONE, _name, _data);
	}

	size_t List::getIndexSelected() const
	{
		return mIndexSelect;
	}

	void List::clearIndexSelected()
	{
		setIndexSelected(ITEM_NONE);
	}

	void List::clearItemDataAt(size_t _index)
	{
		setItemDataAt(_index, Any::Null);
	}

	void List::beginToItemFirst()
	{
		if (getItemCount())
			beginToItemAt(0);
	}

	void List::beginToItemLast()
	{
		if (getItemCount())
			beginToItemAt(getItemCount() - 1);
	}

	void List::beginToItemSelected()
	{
		if (getIndexSelected() != ITEM_NONE)
			beginToItemAt(getIndexSelected());
	}

	bool List::isItemSelectedVisible(bool _fill)
	{
		return isItemVisibleAt(mIndexSelect, _fill);
	}

	void List::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void List::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void List::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	size_t List::_getItemIndex(Widget* _item)
	{
		for (VectorButton::iterator iter = mWidgetLines.begin(); iter != mWidgetLines.end(); ++iter)
		{
			if ((*iter) == _item)
				return *(*iter)->_getInternalData<size_t>() + mTopIndex;
		}
		return ITEM_NONE;
	}

	void List::_resetContainer(bool _update)
	{
		// обязательно у базового
		Base::_resetContainer(_update);

		if (!_update)
		{
			WidgetManager& instance = WidgetManager::getInstance();
			for (VectorButton::iterator iter = mWidgetLines.begin(); iter != mWidgetLines.end(); ++iter)
				instance.unlinkFromUnlinkers(*iter);
		}
	}

} // namespace MyGUI
