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
#include "MyGUI_Gui.h"
#include "MyGUI_Edit.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.05f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_DEFAULT_MAX_TEXT_LENGTH = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 10.0f; // дополнительное смещение для курсора
	const int EDIT_ACTION_MOUSE_ZONE = 1500; // область для восприятия мыши за пределом эдита
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";
	const int EDIT_MOUSE_WHEEL = 50; // область для восприятия мыши за пределом эдита

	Edit::Edit(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name),
		mIsPressed(false),
		mIsFocus(false),
		mCursorActive(false),
		mCursorTimer(0),
		mActionMouseTimer(0),
		mCursorPosition(0),
		mTextLength(0),
		mStartSelect(ITEM_NONE),
		mEndSelect(0),
		mMouseLeftPressed(false),
		mModeReadOnly(false),
		mModePassword(false),
		mModeMultiline(false),
		mModeStatic(false),
		mModeWordWrap(false),
		mTabPrinting(false),
		mCharPassword('*'),
		mOverflowToTheLeft(false),
		mMaxTextLength(EDIT_DEFAULT_MAX_TEXT_LENGTH),
		mVScroll(nullptr),
		mHScroll(nullptr),
		mShowHScroll(true),
		mShowVScroll(true),
		mVRange(0),
		mHRange(0)
	{
		initialiseWidgetSkin(_info);
	}

	Edit::~Edit()
	{
		shutdownWidgetSkin();
	}

	void Edit::baseChangeWidgetSkin(WidgetSkinInfoPtr _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void Edit::initialiseWidgetSkin(WidgetSkinInfoPtr _info)
	{
		mOriginalPointer = mPointer;

		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChildSkin.begin(); iter!=mWidgetChildSkin.end(); ++iter) {
			if (*(*iter)->_getInternalData<std::string>() == "Client") {
				MYGUI_DEBUG_ASSERT( ! mWidgetClient, "widget already assigned");
				mWidgetClient = (*iter);
				mWidgetClient->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
				mWidgetClient->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
				mWidgetClient->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
				mWidgetClient->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
				mWidgetClient->eventMouseDrag = newDelegate(this, &Edit::notifyMouseDrag);
				mWidgetClient->eventMouseButtonDoubleClick = newDelegate(this, &Edit::notifyMouseButtonDoubleClick);
				mWidgetClient->eventMouseWheel = newDelegate(this, &Edit::notifyMouseWheel);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "VScroll") {
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = (*iter)->castType<VScroll>();
				mVScroll->eventScrollChangePosition = newDelegate(this, &Edit::notifyScrollChangePosition);
			}
			else if (*(*iter)->_getInternalData<std::string>() == "HScroll") {
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = (*iter)->castType<HScroll>();
				mHScroll->eventScrollChangePosition = newDelegate(this, &Edit::notifyScrollChangePosition);
			}
		}

		MYGUI_ASSERT(nullptr != mWidgetClient, "Child Widget Client not found in skin (Edit must have Client)");

		ISubWidgetText* text = mWidgetClient->getSubWidgetText();
		if (text) mText = text;

		MYGUI_ASSERT(nullptr != mText, "TextEdit not found in skin (Edit or Client must have TextEdit)");

		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (!properties.empty()) {
			MapString::const_iterator iter = properties.end();
			if ((iter = properties.find("WordWrap")) != properties.end()) setEditWordWrap(utility::parseBool(iter->second));
		}

		updateScroll();

		// первоначальная инициализация курсора
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::shutdownWidgetSkin()
	{
		mWidgetClient = nullptr;
		mVScroll= nullptr;
		mHScroll = nullptr;
	}

	void Edit::notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old)
	{
		if ( (_old == mWidgetClient) || (mIsFocus) ) return;
		mIsFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new)
	{
		if ( (_new == mWidgetClient) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		// в статике все недоступно
		if (mModeStatic || mModeWordWrap) return;

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = mText->getCursorPosition(point);
		mText->setCursorPosition(mCursorPosition);
		mText->setShowCursor(true);
		mCursorTimer = 0;
		updateSelectText();

		if (_id == MouseButton::Left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		// сбрасываем всегда
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		// в статике все недоступно
		if (mModeStatic || mModeWordWrap) return;

		// останавливаем курсор
		mText->setShowCursor(true);

		// сбрасываем все таймеры
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_left, _top);
		mCursorPosition = mText->getCursorPosition(point);
		if (Old == mCursorPosition) return;

		mText->setCursorPosition(mCursorPosition);

		// если не было выделения
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// меняем выделение
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::notifyMouseButtonDoubleClick(WidgetPtr _sender)
	{
		// в статике все недоступно
		if (mModeStatic || mModeWordWrap) return;

		const IntPoint & lastPressed = InputManager::getInstance().getLastLeftPressed();

		size_t cursorPosition = mText->getCursorPosition(lastPressed);
		mStartSelect = cursorPosition;
		mEndSelect = cursorPosition;

		Ogre::UTFString text = this->getOnlyText();
		Ogre::UTFString::reverse_iterator iterBack = text.rend() - cursorPosition;
		Ogre::UTFString::iterator iterForw = text.begin() + cursorPosition;

		while (iterBack != text.rend())
		{
			if (((*iterBack)<265) && (ispunct(*iterBack) || isspace(*iterBack))) break;
			iterBack++;
			mStartSelect--;
		}
		while (iterForw != text.end())
		{
			if (((*iterForw)<265) && (ispunct(*iterForw) || isspace(*iterForw))) break;
			iterForw++;
			mEndSelect++;
		}

		mText->setCursorPosition(mEndSelect);
		mText->setTextSelect(mStartSelect, mEndSelect);
	}

	void Edit::onMouseDrag(int _left, int _top)
	{
		notifyMouseDrag(nullptr, _left, _top);

		Base::onMouseDrag(_left, _top);
	}

	void Edit::onKeySetFocus(WidgetPtr _old)
	{
		if (false == mIsPressed) {
			mIsPressed = true;
			updateEditState();

			if (!mModeStatic && !mModeWordWrap) {
				mCursorActive = true;
				Gui::getInstance().eventFrameStart += newDelegate(this, &Edit::frameEntered);
				//Gui::getInstance().addFrameListener(newDelegate(this, &Edit::frameEntered), this);
				mText->setShowCursor(true);
				mText->setSelectBackground(true);
				mCursorTimer = 0;
				// для первоначального обновления
				//mText->setCursorPosition(mCursorPosition);
				//updateSelectText();

			}
		}

		Base::onKeySetFocus(_old);
	}

	void Edit::onKeyLostFocus(WidgetPtr _new)
	{
		if (mIsPressed) {
			mIsPressed = false;
			updateEditState();

			mCursorActive = false;
			Gui::getInstance().eventFrameStart -= newDelegate(this, &Edit::frameEntered);
			//Gui::getInstance().removeFrameListener(newDelegate(this, &Edit::frameEntered));
			mText->setShowCursor(false);
			mText->setSelectBackground(false);
		}

		Base::onKeyLostFocus(_new);
	}

	void Edit::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		InputManager & input = InputManager::getInstance();

		// в статическом режиме ничего не доступно
		if (mModeStatic || mModeWordWrap) {
			Base::onKeyButtonPressed(_key, _char);
			return;
		}

		mText->setShowCursor(true);
		mCursorTimer = 0.0f;

		if (_key == KeyCode::Escape) {
			InputManager::getInstance().setKeyFocusWidget(nullptr);
		}
		else if (_key == KeyCode::Backspace) {
			// если нуно то удаляем выделенный текст
			if (false == mModeReadOnly) {
				if (false == deleteTextSelect(true)) {
					// прыгаем на одну назад и удаляем
					if (mCursorPosition != 0) {
						mCursorPosition -- ;
						eraseText(mCursorPosition, 1, true);
					}
				}
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}

		}
		else if (_key == KeyCode::Delete) {
			if (input.isShiftPressed()) commandCut();
			else if (false == mModeReadOnly) {
				// если нуно то удаляем выделенный текст
				if (false == deleteTextSelect(true)) {
					if (mCursorPosition != mTextLength) {
						eraseText(mCursorPosition, 1, true);
					}
				}
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}

		}
		else if (_key == KeyCode::Insert) {
			if (input.isShiftPressed()) commandPast();
			else if (input.isControlPressed()) commandCopy();

		}
		else if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter)) {
			// работаем только в режиме редактирования
			if (false == mModeReadOnly) {
				if ((mModeMultiline) && (false == input.isControlPressed())) {
					// попытка объединения двух комманд
					size_t size = mVectorUndoChangeInfo.size();
					// непосредственно операции
					deleteTextSelect(true);
					insertText(TextIterator::getTextNewLine(), mCursorPosition, true);
					// проверяем на возможность объединения
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
					// отсылаем событие о изменении
					eventEditTextChange(this);
				}
				// при сингл лайн и и мульти+сонтрол шлем эвент
				else {
					eventEditSelectAccept(this);
				}
			}

		}
		else if (_key == KeyCode::ArrowRight) {
			if ((mCursorPosition) < mTextLength) {
				mCursorPosition ++;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// сбрасываем выделение
			else if (isTextSelection() && !input.isShiftPressed()) resetSelect();

		}
		else if (_key == KeyCode::ArrowLeft) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// сбрасываем выделение
			else if (isTextSelection() && !input.isShiftPressed()) resetSelect();

		}
		else if (_key == KeyCode::ArrowUp) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// самая верхняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::ArrowDown) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// самая нижняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::Home) {
			// в начало строки
			if ( false == input.isControlPressed()) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MIN_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			// в начало всего текста
			else {
				if (0 != mCursorPosition) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}

		}
		else if (_key == KeyCode::End) {
			// в конец строки
			if ( false ==   input.isControlPressed()) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MAX_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			// в самый конец
			else {
				if (mTextLength != mCursorPosition) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}

		}
		else if (_key == KeyCode::PageUp) {
			// на размер окна, но не меньше одной строки
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= (mWidgetClient->getHeight() > int(mText->getFontHeight())) ? mWidgetClient->getHeight() : int(mText->getFontHeight());
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// самая верхняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::PageDown) {
			// на размер окна, но не меньше одной строки
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += (mWidgetClient->getHeight() > int(mText->getFontHeight())) ? mWidgetClient->getHeight() : int(mText->getFontHeight());
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// самая нижняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if ( (_key == KeyCode::LeftShift) || (_key == KeyCode::RightShift) ) {
			// для правильно выделения
			if (mStartSelect == ITEM_NONE) {
				mStartSelect = mEndSelect = mCursorPosition;
			}
		}
		else if (_char != 0) {

			// если не нажат контрл, то обрабатываем как текст
			if ( false == input.isControlPressed() ) {
				if (false == mModeReadOnly) {
					// таб только если нужно
					if (_char != '\t' || mTabPrinting) {
						// попытка объединения двух комманд
						size_t size = mVectorUndoChangeInfo.size();
						// непосредственно операции
						deleteTextSelect(true);
						insertText(TextIterator::getTextCharInfo(_char), mCursorPosition, true);
						// проверяем на возможность объединения
						if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
						// отсылаем событие о изменении
						eventEditTextChange(this);
					}
				}
			}
			else if (_key == KeyCode::C) {
				commandCopy();

			}
			else if (_key == KeyCode::X) {
				commandCut();

			}
			else if (_key == KeyCode::V) {
				commandPast();

			}
			else if (_key == KeyCode::A) {
				// выделяем весь текст
				setTextSelection(0, mTextLength);

			}
			else if (_key == KeyCode::Z) {
				// отмена
				commandUndo();

			}
			else if (_key == KeyCode::Y) {
				// повтор
				commandRedo();

			}
		}

		Base::onKeyButtonPressed(_key, _char);
	}

	void Edit::frameEntered(float _frame)
	{
		// в статике все недоступно
		if (mModeStatic || mModeWordWrap) return;

		if (mCursorActive) {
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER) {
				mText->setShowCursor(!mText->isCursorShow());
				while (mCursorTimer > EDIT_CURSOR_TIMER) mCursorTimer -= EDIT_CURSOR_TIMER;
			}
		}

		// сдвигаем курсор по положению мыши
		if (mMouseLeftPressed) {
			mActionMouseTimer += _frame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER) {

				IntPoint mouse = InputManager::getInstance().getMousePosition();
				const IntRect& view = mWidgetClient->getAbsoluteRect();
				mouse.left -= view.left;
				mouse.top -= view.top;
				IntPoint point;

				bool action = false;

				// вверх на одну строчку
				if ( (mouse.top < 0) && (mouse.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetClient->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top -= mText->getFontHeight();
						action = true;
					}
				}
				// вниз на одну строчку
				else if ( (mouse.top > mWidgetClient->getHeight()) && (mouse.top < (mWidgetClient->getHeight() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetClient->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top += mText->getFontHeight();
						action = true;
					}
				}

				// влево на небольшое расстояние
				if ( (mouse.left < 0) && (mouse.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					//if ( (mouse.top > 0) && (mouse.top <= mWidgetClient->getHeight()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
						action = true;
					//}
				}
				// вправо на небольшое расстояние
				else if ( (mouse.left > mWidgetClient->getWidth()) && (mouse.left < (mWidgetClient->getWidth() + EDIT_ACTION_MOUSE_ZONE)) ) {
					//if ( (mouse.top > 0) && (mouse.top <= mWidgetClient->getHeight()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.left += (int)EDIT_OFFSET_HORZ_CURSOR;
						action = true;
					//}
				}

				if (action) {
					size_t old = mCursorPosition;
					mCursorPosition = mText->getCursorPosition(point);
					//MYGUI_OUT(mCursorPosition);

					if ( old != mCursorPosition ) {

						mText->setCursorPosition(mCursorPosition);

						mEndSelect = (size_t)mCursorPosition;
						if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
						else mText->setTextSelect(mStartSelect, mEndSelect);

						// пытаемся показать курсор
						updateView(true);
					}

				}
				// если в зону не попадает то сбрасываем
				else mActionMouseTimer = 0;

				while (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER) mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER;
			}

		} // if (mMouseLeftPressed)
  }

	void Edit::setTextCursor(size_t _index)
	{
		// сбрасываем выделение
		resetSelect();

		// новая позиция
		if (_index > mTextLength) _index = mTextLength;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::setTextSelection(size_t _start, size_t _end)
	{
		if (_start > mTextLength) _start = mTextLength;
		if (_end > mTextLength) _end = mTextLength;

		mStartSelect = _start;
		mEndSelect = _end;

		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

		if (mCursorPosition == mEndSelect) return;
		// курсор на конец выделения
		mCursorPosition = mEndSelect;

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
	}

	bool Edit::deleteTextSelect(bool _history)
	{
		if ( ! isTextSelection()) return false;

		// начало и конец выделения
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();

		eraseText(start, end - start, _history);

		return true;
	}

	void Edit::resetSelect()
	{
		if (mStartSelect != ITEM_NONE) {
			mStartSelect = ITEM_NONE;
			mText->setTextSelect(0, 0);
		}
	}

	void Edit::commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo * _info)
	{
		if (_info != nullptr) 	_info->push_back(TextCommandInfo(_undo, _redo, _length));
	}

	void Edit::commandMerge()
	{
		if (mVectorUndoChangeInfo.size() < 2) return; // на всякий
		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		mVectorUndoChangeInfo.pop_back();

		// объединяем последовательности
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {
			mVectorUndoChangeInfo.back().push_back((*iter));
		}
	}

	bool Edit::commandUndo()
	{
		if (mVectorUndoChangeInfo.empty()) return false;

		// сбрасываем выделение
		resetSelect();

		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		// перекидываем последний набор отмен
		mVectorUndoChangeInfo.pop_back();
		mVectorRedoChangeInfo.push_back(info);

		// берем текст для издевательств
		Ogre::UTFString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::reverse_iterator iter=info.rbegin(); iter!=info.rend(); iter++) {

			if ((*iter).type == TextCommandInfo::COMMAND_INSERT) text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == TextCommandInfo::COMMAND_ERASE) text.insert((*iter).start, (*iter).text);
			else {
				mCursorPosition = (*iter).undo;
				mTextLength = (*iter).length;
			}
		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// отсылаем событие о изменении
		eventEditTextChange(this);

		return true;
	}

	bool Edit::commandRedo()
	{
		if (mVectorRedoChangeInfo.empty()) return false;

		// сбрасываем выделение
		resetSelect();

		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorRedoChangeInfo.back();
		// перекидываем последний набор отмен
		mVectorRedoChangeInfo.pop_back();
		mVectorUndoChangeInfo.push_back(info);

		// берем текст для издевательств
		Ogre::UTFString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {

			if ((*iter).type == TextCommandInfo::COMMAND_INSERT) text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == TextCommandInfo::COMMAND_ERASE) text.erase((*iter).start, (*iter).text.size());
			else {
				mCursorPosition = (*iter).redo;
				mTextLength = (*iter).length;
			}

		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// отсылаем событие о изменении
		eventEditTextChange(this);

		return true;
	}

	void Edit::saveInHistory(VectorChangeInfo * _info)
	{
		if (_info == nullptr) return;
		// если нет информации об изменении
		if ( _info->empty() ) return;
		if ( (_info->size() == 1) && (_info->back().type == TextCommandInfo::COMMAND_POSITION)) return;

		mVectorUndoChangeInfo.push_back(*_info);
		// проверяем на максимальный размер
		if (mVectorUndoChangeInfo.size() > EDIT_MAX_UNDO)
			mVectorUndoChangeInfo.pop_front();
	}

	// возвращает текст
	Ogre::UTFString Edit::getTextInterval(size_t _start, size_t _count)
	{
		// подстраховка
		if (_start > mTextLength) _start = mTextLength;
		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString());

		// дефолтный цвет
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getTextColour());

		// нужно ли вставлять цвет
		bool need_colour = true;

		// цикл прохода по строке
		while (iterator.moveNext()) {

			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// еще рано
			if (pos < _start) {
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);

				continue;
			}

			// проверяем на надобность начального тега
			else if (pos == _start) {
				need_colour = ! iterator.getTagColour(colour);
				// сохраняем место откуда начинается
				iterator.saveStartPoint();

			}

			// а теперь просто до конца диапазона
			else if (pos == end) break;

		};

		// возвращаем строку
		if (need_colour) return colour + iterator.getFromStart();
		return iterator.getFromStart();
	}

	// выделяет цветом диапазон
	void Edit::_setTextColour(size_t _start, size_t _count, const Colour& _colour, bool _history)
	{
		// при изменениях сразу сбрасываем повтор
		commandResetRedo();

		// история изменений
		VectorChangeInfo * history = nullptr;
		if (_history) history = new VectorChangeInfo();

		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getTextColour());

		// цикл прохода по строке
		while (iterator.moveNext()) {

			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// берем цвет из позиции и запоминаем
			iterator.getTagColour(colour);

			// еще рано
			if (pos < _start) continue;

			// ставим начальный тег
			else if (pos == _start)
				iterator.setTagColour(_colour);

			// внутри диапазона очищаем все
			else if (pos < end)
				iterator.clearTagColour();

			// на конец ставим последний найденный или дефолтный
			else if (pos == end) {
				iterator.setTagColour(colour);
				// и выходим из цикла
				break;
			}

		};

		// сохраняем позицию для восстановления курсора
		commandPosition(_start, _start+_count, mTextLength, history);

		// запоминаем в историю
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

	}

	void Edit::setTextSelectColour(const Colour& _colour, bool _history)
	{
		// нужно выделение
		if ( false == isTextSelection()) return;
		// начало и конец выделения
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();
		_setTextColour(start, end-start, _colour, _history);
	}

	Ogre::UTFString Edit::getTextSelection()
	{
		if ( false == isTextSelection()) return "";
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();
		return getTextInterval(start, end-start);
	}

	void Edit::setEditPassword(bool _password)
	{
		if (mModePassword == _password) return;
		mModePassword = _password;
		if (mModePassword) {
			mPasswordText = mText->getCaption();
			mText->setCaption(Ogre::UTFString(mTextLength, '*'));
		}
		else {
			mText->setCaption(mPasswordText);
			mPasswordText.clear();
		}
		// обновляем по размерам
		updateView(false);
		// сбрасываем историю
		commandResetHistory();
	}

	void Edit::setText(const Ogre::UTFString & _caption, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo * history = nullptr;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// вставляем текст
		iterator.setText(_caption, mModeMultiline || mModeWordWrap);

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// обрезаем по максимальной длинне
			iterator.cutMaxLength(mMaxTextLength);
		}

		// запоминаем размер строки
		size_t old = mTextLength;
		// новая позиция и положение на конец вставки
		mCursorPosition = mTextLength = iterator.getSize();

		// сохраняем позицию для восстановления курсора
		commandPosition(0, mTextLength, old, history);

		// запоминаем в историю
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::insertText(const Ogre::UTFString & _text, size_t _start, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// если строка пустая, или размер максимален
		if (_text.empty()) return;

		if ((mOverflowToTheLeft == false) && (mTextLength == mMaxTextLength)) return;

		// история изменений
		VectorChangeInfo * history = nullptr;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getTextColour());
		// нужен ли тег текста
		// потом переделать через TextIterator чтобы отвязать понятие тег от эдита
		bool need_colour = ( (_text.size() > 6) && (_text[0] == L'#') && (_text[1] != L'#') );

		// цикл прохода по строке
		while (iterator.moveNext()) {

			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// текущий цвет
			if (need_colour) iterator.getTagColour(colour);

			// если дошли то выходим
			if (pos == _start) break;

		};

		// если нужен цвет то вставляем
		if (need_colour) iterator.setTagColour(colour);

		// а теперь вставляем строку
		iterator.insertText(_text, mModeMultiline || mModeWordWrap);

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// обрезаем по максимальной длинне
			iterator.cutMaxLength(mMaxTextLength);
		}

		// запоминаем размер строки
		size_t old = mTextLength;
		// новая позиция и положение на конец вставки
		mTextLength = iterator.getSize();
		mCursorPosition += mTextLength - old;

		// сохраняем позицию для восстановления курсора
		commandPosition(_start, _start + mTextLength - old, old, history);

		// запоминаем в историю
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::eraseText(size_t _start, size_t _count, bool _history)
	{
		// чета маловато
		if (_count == 0) return;

		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo * history = nullptr;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::UTFString colour;
		// конец диапазона
		size_t end = _start + _count;
		bool need_colour = false;

		// цикл прохода по строке
		while (iterator.moveNext()) {

			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// еще рано
			if (pos < _start) {
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
				continue;
			}

			// сохраняем место откуда начинается
			else if (pos == _start) {
				// если до диапазона был цвет, то нужно закрыть тег
				if ( ! colour.empty()) {
					need_colour = true;
					colour.clear();
				}
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
				iterator.saveStartPoint();
			}

			// внутри диапазона
			else if (pos < end) {
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
			}

			// окончание диапазона
			else if (pos == end) {
				// нужно ставить тег или нет
				if ( ! colour.empty()) need_colour = true;
				if ( iterator.getTagColour(colour)) need_colour = false;

				break;
			}

		};

		// удаляем диапазон
		iterator.eraseFromStart();
		// и вставляем последний цвет
		if (need_colour) iterator.setTagColour(colour);

		// сохраняем позицию для восстановления курсора
		commandPosition(_start + _count, _start, mTextLength, history);

		// на месте удаленного
		mCursorPosition = _start;
		mTextLength -= _count;

		// запоминаем в историю
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::commandCut()
	{
		// вырезаем в буфер обмена
		if ( isTextSelection() && (false == mModePassword) ) {
			ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getTextSelection());
			if (false == mModeReadOnly) {
				deleteTextSelect(true);
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}
		}
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandCopy()
	{
		// копируем в буфер обмена
		if ( isTextSelection() && (false == mModePassword) ) ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getTextSelection());
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandPast()
	{
		// копируем из буфера обмена
		std::string clipboard = ClipboardManager::getInstance().GetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
		if ( (false == mModeReadOnly) && ( false == clipboard.empty()) ) {
			// попытка объединения двух комманд
			size_t size = mVectorUndoChangeInfo.size();
			// непосредственно операции
			deleteTextSelect(true);
			insertText(clipboard, mCursorPosition, true);
			// проверяем на возможность объединения
			if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
			// отсылаем событие о изменении
			eventEditTextChange(this);
		}
	}

	const Ogre::UTFString & Edit::getRealString()
	{
		if (mModePassword) return mPasswordText;
		return mText->getCaption();
	}

	void Edit::setRealString(const Ogre::UTFString & _caption)
	{
		if (mModePassword) {
			mPasswordText = _caption;
			mText->setCaption(Ogre::UTFString(mTextLength, mCharPassword));
		}
		else {
			mText->setCaption(_caption);
		}
	}

	void Edit::setPasswordChar(Char _char)
	{
		mCharPassword = _char;
		if (mModePassword) {
			mText->setCaption(Ogre::UTFString(mTextLength, mCharPassword));
		}
	}

	void Edit::updateEditState()
	{
		if (!mEnabled) setState("disabled");
		else if (mIsPressed) {
			if (mIsFocus) setState("pushed");
			else setState("normal_checked");
		}
		else if (mIsFocus) setState("highlighted");
		else setState("normal");
	}

	void Edit::setPosition(const IntPoint & _point)
	{
		Base::setPosition(_point);
	}

	void Edit::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		// если перенос, то сбрасываем размер текста
		if (mModeWordWrap) mText->setBreakLine(true);
		updateView(false);
	}

	void Edit::setCoord(const IntCoord & _coord)
	{
		Base::setCoord(_coord);

		// если перенос, то сбрасываем размер текста
		if ((mModeWordWrap) && ((mCoord.width != _coord.width) || (mCoord.height != _coord.height))) mText->setBreakLine(true);
		updateView(false);
	}

	void Edit::setCaption(const Ogre::UTFString & _caption)
	{
		setText(_caption, false);
	}

	const Ogre::UTFString& Edit::getCaption()
	{
		return getRealString();
	}

	void Edit::updateSelectText()
	{
		if ( !mModeStatic && !mModeWordWrap ) {

			InputManager & input = InputManager::getInstance();
			if ( (input.isShiftPressed()) && (mStartSelect != ITEM_NONE) )
			{
				// меняем выделение
				mEndSelect = (size_t)mCursorPosition;
				if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
				else mText->setTextSelect(mStartSelect, mEndSelect);

			}
			else if (mStartSelect != ITEM_NONE)
			{
				// сбрасываем шифт
				mStartSelect = ITEM_NONE;
				mText->setTextSelect(0, 0);
			}
		}

		// пытаемся показать курсор
		updateView(true);
	}

	void Edit::setTextAlign(Align _align)
	{
		Base::setTextAlign(_align);

		// так как мы сами рулим смещениями
		updateView(false);
	}

	void Edit::updateView(bool _showCursor)
	{

		// проверяем скролы
		updateScroll();

		// размер контекста текста
		IntSize textSize = mText->getTextSize();
		// текущее смещение контекста текста
		IntPoint point = mText->getViewOffset();
		// расчетное смещение
		IntPoint offset = point;

		// абсолютные координаты курсора
		IntRect cursor = mText->getCursorRect(mCursorPosition);
		cursor.right ++;
		// абсолютные координаты вью
		const IntRect& view = mWidgetClient->getAbsoluteRect();

		// тестируем видимость курсора
		bool inside = view.inside(cursor);

		// проверяем и показываем курсор
		if (_showCursor && ( false == inside)) {

			// горизонтальное смещение
			// FIXME проверить, помоему просто >
			if (textSize.width >= view.width()) {
				if (cursor.left < view.left) {
					offset.left = point.left - (view.left - cursor.left);
					// добавляем смещение, только если курсор не перепрыгнет
					if ((float(view.width()) - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left -= int(EDIT_OFFSET_HORZ_CURSOR);
				}
				else if (cursor.right > view.right) {
					offset.left = point.left + (cursor.right - view.right);
					// добавляем смещение, только если курсор не перепрыгнет
					if ((float(view.width()) - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left += int(EDIT_OFFSET_HORZ_CURSOR);
				}
			}

			// вертикальное смещение
			// FIXME проверить, помоему просто >
			if (textSize.height >= view.height()) {
				if (cursor.top < view.top) {
					offset.top = point.top - (view.top - cursor.top);
				}
				else if (cursor.bottom > view.bottom) {
					offset.top = point.top + (cursor.bottom - view.bottom);
				}
			}

		}

		// выравнивание текста
		Align align = mText->getTextAlign();

		if (textSize.width >= view.width()) {
			// максимальный выход влево
			if ((offset.left + view.width()) > textSize.width) {
				offset.left = textSize.width - view.width();
			}
			// максимальный выход вправо
			else if (offset.left < 0) {
				offset.left = 0;
			}
		}
		else {
			if (align.isLeft()) {
				offset.left = 0;
			}
			else if (align.isRight()) {
				offset.left = textSize.width - view.width();
			}
			else {
				offset.left = (textSize.width - view.width()) / 2;
			}
		}

		if (textSize.height > view.height()) {
			// максимальный выход вверх
			if ((offset.top + view.height()) > textSize.height) {
				offset.top = textSize.height - view.height();
			}
			// максимальный выход вниз
			else if (offset.top < 0) {
				offset.top = 0;
			}
		}
		else {
			if (align.isTop()) {
				offset.top = 0;
			}
			else if (align.isBottom()) {
				offset.top = textSize.height - view.height();
			}
			else {
				offset.top = (textSize.height - view.height()) / 2;
			}
		}

		if (offset != point) {
			mText->setViewOffset(offset);
			if (nullptr != mVScroll) mVScroll->setScrollPosition(offset.top);
			if (nullptr != mHScroll) mHScroll->setScrollPosition(offset.left);
		}
	}

	void Edit::updateScroll()
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

	}

	void Edit::notifyScrollChangePosition(VScrollPtr _sender, size_t _position)
	{
		if (_sender == mVScroll) {
			IntPoint point = mText->getViewOffset();
			point.top = _position;
			mText->setViewOffset(point);
		}
		else if (_sender == mHScroll) {
			IntPoint point = mText->getViewOffset();
			point.left = _position;
			mText->setViewOffset(point);
		}
	}

	void Edit::notifyMouseWheel(WidgetPtr _sender, int _rel)
	{
		if (mVRange != 0) {
			IntPoint point = mText->getViewOffset();
			int offset = point.top;
			if (_rel < 0) offset += EDIT_MOUSE_WHEEL;
			else  offset -= EDIT_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mVRange) offset = mVRange;

			if (offset != point.top) {
				point.top = offset;
				if (mVScroll != nullptr) {
					mVScroll->setScrollPosition(offset);
				}
				mText->setViewOffset(point);
			}
		}
		else if (mHRange != 0) {
			IntPoint point = mText->getViewOffset();
			int offset = point.left;
			if (_rel < 0) offset += EDIT_MOUSE_WHEEL;
			else  offset -= EDIT_MOUSE_WHEEL;

			if (offset < 0) offset = 0;
			else if (offset > (int)mHRange) offset = mHRange;

			if (offset != point.left) {
				point.left = offset;
				if (mHScroll != nullptr) {
					mHScroll->setScrollPosition(offset);
				}
				mText->setViewOffset(point);
			}
		}
	}

	void Edit::setEditWordWrap(bool _wordwrap)
	{
		mModeWordWrap = _wordwrap;
		mText->setBreakLine(mModeWordWrap);
		setCoord(mCoord);
	}

	void Edit::setFontName(const std::string & _font)
	{
		Base::setFontName(_font);

		setCoord(mCoord);
	}

	void Edit::setFontHeight(uint _height)
	{
		Base::setFontHeight(_height);

		setCoord(mCoord);
	}

} // namespace MyGUI
