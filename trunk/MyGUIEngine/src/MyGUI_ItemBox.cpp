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
				//mWidgetClient->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMousePressed);
				mWidgetClient->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
			}
		}
		MYGUI_ASSERT(null != mWidgetScroll, "Child VScroll not found in skin (ItemBox must have VScroll)");//???
		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (ItemBox must have Client)");

		// парсим свойства
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

		// если колличество айтемов в строке изменилось, то перерисовываем все
		if (old_count == mCountItemInLine) {
			// если строк стало меньшн то ничего не нужно
			//if (_size.height >= mCoord.height) return;
		}

		notifyScrollChangePosition(null, mWidgetScroll->getScrollPosition());
		_redrawAllVisible();
	}

	void ItemBox::notifyScrollChangePosition(MyGUI::WidgetPtr _sender, size_t _rel)
	{
		mOffsetTop = ((int)_rel % mSizeItem.height);

		// смещение с отрицательной стороны
		int offsetVert = 0 - mOffsetTop;
		int countInLine = 0;
		int offsetHorz = 0;

		// переставляем тупо виджеты
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

		// если индекс изменился, то перерисовываем линии
		int top = ((int)_rel / mSizeItem.height) * mCountItemInLine;
		if (top != mTopIndex) {
			mTopIndex = top;
			_redrawAllVisible();
		}

		// прорисовываем все нижние строки, если они появились
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
		mRangeIndex = (mSizeItem.height * mCountLines) - mWidgetClient->getHeight();

		if ( (mRangeIndex < 1) || (mWidgetScroll->getLeft() <= mWidgetClient->getLeft()) ) {
			if (mWidgetScroll->isShow()) {
				mWidgetScroll->hide();
				// увеличиваем клиентскую зону на ширину скрола
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

		// корректируем позицию скролла
		size_t pos = mWidgetScroll->getScrollPosition();
		mWidgetScroll->setScrollRange(mRangeIndex + 1);
		// если позиция стала недопустима, то отодвигаем ее в самый конец
		if (pos >= mRangeIndex + 1) mWidgetScroll->setScrollPosition(mRangeIndex);
	}

	void ItemBox::_redrawAllVisible()
	{

		int count = mCountLineVisible * mCountItemInLine + mTopIndex;
		size_t iwid = 0; // индекс виджета
		for (int pos = mTopIndex; pos<count; ++pos, ++iwid) {
			// дальше нет айтемов
			if (pos >= mCountItems) break;

			WidgetPtr widget = getItemWidget(iwid);

			widget->show();
			redrawItem(widget, (size_t)pos);

		}

		// все виджеты еще есть, то их надо бы скрыть
		while (iwid < mVectorItems.size()) {
			mVectorItems[iwid]->hide();
			iwid ++;
		}

		//MYGUI_OUT(mVectorItems.size());

	}

	// отрисовка конкретного айтема
	void ItemBox::redrawItem(WidgetPtr _widget, size_t _index)
	{
		_widget->setCaption(utility::toString(_index));
	}

	WidgetPtr ItemBox::getItemWidget(size_t _index)
	{
		// еще нет такого виджета, нуно создать
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

	/*void ItemBox::_onKeyButtonPressed(int _key, Char _char)
	{
		// очень секретный метод, запатентованный механизм движения курсора
		if (getItemCount() == 0) {

			// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
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
				// старое значение
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
				// выходим, так как изменили колличество строк
				return;
			}

		}

		if (sel != mIndexSelect) {
			if ( false == isItemVisible(sel)) beginToIndex(sel);
			setItemSelect(sel);
			// изменилась позиция
			eventItemBoxChangePosition(this, mIndexSelect);
		}

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void ItemBox::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		if (false == _left) return;

		if (_sender == mWidgetScroll) return;

		// если выделен клиент, то сбрасываем
		if (_sender == mWidgetClient) {

			if (mIndexSelect != ITEM_NONE) {
				_selectIndex(mIndexSelect, false);
				mIndexSelect = ITEM_NONE;
				eventItemBoxChangePosition(this, mIndexSelect);
			}
			eventItemBoxMouseItemActivate(this, mIndexSelect);

		// если не клиент, то просчитывам
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
		// сбрасываем
		if (_reset) {
			mOldSize.clear();
			mLastRedrawLine = 0;
		}

		// позиция скролла
		int position = mTopIndex * mHeightLine + mOffsetTop;

		// если высота увеличивалась то добавляем виджеты
		if (mOldSize.height < mCoord.height) {

			int height = (int)mVectorItems.size() * mHeightLine - mOffsetTop;

			// до тех пор, пока не достигнем максимального колличества, и всегда на одну больше
			while ( (height <= (mWidgetClient->getHeight() + mHeightLine)) && (mVectorItems.size() < mStringArray.size()) ) {
				// создаем линию
				WidgetPtr line = mWidgetClient->createWidgetT("Button", mSkinLine, 0, height, mWidgetClient->getWidth(), mHeightLine, ALIGN_TOP | ALIGN_HSTRETCH);
				// подписываемся на всякие там события
				line->eventMouseButtonPressed = newDelegate(this, &ItemBox::notifyMousePressed);
				line->eventMouseWheel = newDelegate(this, &ItemBox::notifyMouseWheel);
				// присваиваем порядковый номер, длу простоты просчета
				line->_setInternalData((int)mVectorItems.size());
				// и сохраняем
				mVectorItems.push_back(line);
				height += mHeightLine;
			};

			// проверяем на возможность не менять положение списка
			if (position >= mRangeIndex) {

				// размер всех помещается в клиент
				if (mRangeIndex <= 0) {

					// обнуляем, если надо
					if (position || mOffsetTop || mTopIndex) {

						position = 0;
						mTopIndex = 0;
						mOffsetTop = 0;
						mLastRedrawLine = 0; // чтобы все перерисовалось

						// выравниваем
						int offset = 0;
						for (size_t pos=0; pos<mVectorItems.size(); pos++) {
							mVectorItems[pos]->setPosition(0, offset);
							offset += mHeightLine;
						}
					}

				} else {

					// прижимаем список к нижней границе
					int count = mWidgetClient->getHeight() / mHeightLine;
					mOffsetTop = mHeightLine - (mWidgetClient->getHeight() % mHeightLine);

					if (mOffsetTop == mHeightLine) {
						mOffsetTop = 0;
						count --;
					}

					int top = (int)mStringArray.size() - count - 1;

					// выравниваем
					int offset = 0 - mOffsetTop;
					for (size_t pos=0; pos<mVectorItems.size(); pos++) {
						mVectorItems[pos]->setPosition(0, offset);
						offset += mHeightLine;
					}

					// высчитываем положение, должно быть максимальным
					position = top * mHeightLine + mOffsetTop;

					// если индех изменился, то перерисовываем линии
					if (top != mTopIndex) {
						mTopIndex = top;
						_redrawItemRange();
					}

				}
			}

			// увеличился размер, но прокрутки вниз небыло, обновляем линии снизу
			_redrawItemRange(mLastRedrawLine);

		} // if (old_cy < mCoord.height)

		// просчитываем положение скролла
		mWidgetScroll->setScrollPosition(position);

		mOldSize.width = mCoord.width;
		mOldSize.height = mCoord.height;
	}

	void ItemBox::_redrawItemRange(size_t _start)
	{
		// перерисовываем линии, только те, что видны
		size_t pos = _start;
		for (; pos<mVectorItems.size(); pos++) {
			// индекс в нашем массиве
			size_t index = pos + (size_t)mTopIndex;

			// не будем заходить слишком далеко
			if (index >= mStringArray.size()) {
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}
			if (mVectorItems[pos]->getTop() > mWidgetClient->getHeight()) {
				// запоминаем последнюю перерисованную линию
				mLastRedrawLine = pos;
				break;
			}

			// если был скрыт, то покажем
			if (false == mVectorItems[pos]->isShow()) mVectorItems[pos]->show();
			// обновляем текст
			mVectorItems[pos]->setCaption(mStringArray[index]);

			// если нужно выделить ,то выделим
			if (index == mIndexSelect) {
				if (!static_cast<ButtonPtr>(mVectorItems[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mVectorItems[pos])->setButtonPressed(true);
			} else {
				if (static_cast<ButtonPtr>(mVectorItems[pos])->getButtonPressed())
					static_cast<ButtonPtr>(mVectorItems[pos])->setButtonPressed(false);
			}
		}

		// если цикл весь прошли, то ставим максимальную линию
		if (pos >= mVectorItems.size()) mLastRedrawLine = pos;
	}

	// перерисовывает индекс
	void ItemBox::_redrawItem(size_t _index)
	{
		// невидно
		if (_index < (size_t)mTopIndex) return;
		_index -= (size_t)mTopIndex;
		// тоже невидно
		if (_index > mLastRedrawLine) return;
		// перерисовываем
		mVectorItems[_index]->setCaption(mStringArray[_index + mTopIndex]);
	}

	void ItemBox::insertItemString(size_t _index, const Ogre::UTFString & _item)
	{
		if (_index > mStringArray.size()) _index = mStringArray.size();
		// вставляем физически
		_insertString(_index, _item);

		// если надо, то меняем выделенный элемент
		if ( (mIndexSelect != ITEM_NONE) && (_index <= mIndexSelect) ) mIndexSelect++;

		// строка, до первого видимого элемента
		if ( (_index <= (size_t)mTopIndex) && (mRangeIndex > 0) ) {
			mTopIndex ++;
			// просчитываем положение скролла
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			mRangeIndex += mHeightLine;

		} else {

			// высчитывам положение строки
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// строка, после последнего видимого элемента, плюс одна строка (потому что для прокрутки нужно на одну строчку больше)
			if (mWidgetClient->getHeight() < (offset - mHeightLine)) {
				// просчитываем положение скролла
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() + mHeightLine);
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				mRangeIndex += mHeightLine;

			// строка в видимой области
			} else {

				// обновляем все
				updateScroll();
				updateLine(true);

				// позже сюда еще оптимизацию по колличеству перерисовок
			}
		}

	}

	void ItemBox::deleteItemString(size_t _index)
	{
		// доверяй, но проверяй
		MYGUI_ASSERT(_index < mStringArray.size(), "deleteItemString: index '" << _index << "' out of range");

		// удяляем физически строку
		_deleteString(_index);

		// если надо, то меняем выделенный элемент
		if (mStringArray.empty()) mIndexSelect = ITEM_NONE;
		else if (mIndexSelect != ITEM_NONE) {
			if (_index < mIndexSelect) mIndexSelect--;
			else if ( (_index == mIndexSelect) && (mIndexSelect == (mStringArray.size())) ) mIndexSelect--;
		}

		// если виджетов стало больше , то скрываем крайний
		if (mVectorItems.size() > mStringArray.size()) {
			mVectorItems[mStringArray.size()]->hide();
		}

		// строка, до первого видимого элемента
		if (_index < (size_t)mTopIndex) {
			mTopIndex --;
			// просчитываем положение скролла
			mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
			mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
			mRangeIndex -= mHeightLine;

		} else {

			// высчитывам положение удаляемой строки
			int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

			// строка, после последнего видимого элемента
			if (mWidgetClient->getHeight() < offset) {
				// просчитываем положение скролла
				mWidgetScroll->setScrollRange(mWidgetScroll->getScrollRange() - mHeightLine);
				mWidgetScroll->setScrollPosition(mTopIndex * mHeightLine + mOffsetTop);
				mRangeIndex -= mHeightLine;

			// строка в видимой области
			} else {

				// обновляем все
				updateScroll();
				updateLine(true);

				// позже сюда еще оптимизацию по колличеству перерисовок
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
		// не видно строки
		if (_index < (size_t)mTopIndex) return;
		// высчитывам положение строки
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;
		// строка, после последнего видимого элемента
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

	// видим ли мы элемент, полностью или нет
	bool ItemBox::isItemVisible(size_t _index, bool _fill)
	{
		// если элемента нет, то мы его не видим (в том числе когда их вообще нет)
		if (_index >= mStringArray.size()) return false;
		// если скрола нет, то мы палюбак видим
		if (mRangeIndex <= 0) return true;

		// строка, до первого видимого элемента
		if (_index < (size_t)mTopIndex) return false;

		// строка это верхний выделенный
		if (_index == (size_t)mTopIndex) {
			if ( (mOffsetTop != 0) && (_fill) ) return false; // нам нужна полностью видимость
			return true;
		}

		// высчитывам положение строки
		int offset = ((int)_index - mTopIndex) * mHeightLine - mOffsetTop;

		// строка, после последнего видимого элемента
		if (mWidgetClient->getHeight() < offset) return false;

		// если мы внизу и нам нужен целый
		if ((mWidgetClient->getHeight() < (offset + mHeightLine)) && (_fill) ) return false;

		return true;
	}
*/
} // namespace MyGUI
