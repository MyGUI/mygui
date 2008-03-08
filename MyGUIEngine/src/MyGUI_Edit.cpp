/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Edit.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_SubWidgetTextInterface.h"

namespace MyGUI
{

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.20f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_MAX_LENGHT = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 50.0f; // дополнительное смещение для курсора
	const int EDIT_ACTION_MOUSE_ZONE = 50; // область для восприятия мыши за пределом эдита
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";

	Edit::Edit(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mIsPressed(false),
		mIsFocus(false),
		mWidgetUpper(null),
		mWidgetCursor(null),
		mCursorActive(false),
		mCursorTimer(0),
		mActionMouseTimer(0),
		mCursorPosition(0),
		mTextLength(0),
		mShiftPressed(false),
		mCtrlPressed(false),
		mStartSelect(ITEM_NONE),
		mEndSelect(0),
		mHalfCursor(1, 1),
		mMouseLeftPressed(false),
		mModeReadOnly(false),
		mModePassword(false),
		mModeMultiline(false),
		mModeStatic(false)
	{

		MYGUI_ASSERT(null != mText, "TextEdit not found in skin (Edit must have TextEdit)");

		mOriginalPointer = mPointer;

		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
				mWidgetUpper = (*iter);
				mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
				mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
				mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
				mWidgetUpper->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
				mWidgetUpper->eventMouseDrag = newDelegate(this, &Edit::notifyMouseDrag);
			}
		}
		MYGUI_ASSERT(null != mWidgetUpper, "Child Widget Client not found in skin (Edit must have Client)");
		// курсор принадлежит клиенту, пытаемся его найти
		const VectorWidgetPtr& childs = mWidgetUpper->getChilds();
		for (VectorWidgetPtr::const_iterator iter=childs.begin(); iter!=childs.end(); ++iter) {
			if ("Cursor" == (*iter)->_getInternalString()) {
				mWidgetCursor = (*iter);
				mWidgetCursor->hide();
				mWidgetCursor->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
				mWidgetCursor->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
				mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
				mWidgetCursor->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
				mWidgetCursor->eventMouseDrag = newDelegate(this, &Edit::notifyMouseDrag);
				mWidgetCursor->setSize(IntSize(mWidgetCursor->getWidth(), getFontHeight())); // высоту под шрифт
				break;
			}
		}
		MYGUI_ASSERT(null != mWidgetCursor, "Child Widget Cursor not found in skin in Client (Edit must have Cursor)");

		// высчитываем половинки
		mHalfCursor.set(mWidgetCursor->getWidth()/2, mWidgetCursor->getHeight()/2);
		if (mHalfCursor.width < 1) mHalfCursor.width = 1;
		if (mHalfCursor.height < 1) mHalfCursor.height = 1;

		// первоначальная инициализация курсора
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

	}

	Edit::~Edit()
	{
		// на всякий отписываем
		Gui::getInstance().removeFrameListener(this);
	}

	void Edit::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if ( (_old == mWidgetUpper) || (_old == mWidgetCursor) || (mIsFocus) ) return;
		mIsFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( (_new == mWidgetUpper) || (_new == mWidgetCursor) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// в статике все недоступно
		if (mModeStatic) return;

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = mText->getTextCursorFromPoint(point);
		updateCursor(point);

		if (_left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		// сбрасываем всегда
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// в статике все недоступно
		if (mModeStatic) return;

		// останавливаем курсор
		if ( false == mWidgetCursor->isShow()) mWidgetCursor->show();

		// сбрасываем все таймеры
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_left, _top);
		mCursorPosition = mText->getTextCursorFromPoint(point);
		if (Old == mCursorPosition) return;

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->getLeft();
			point.top -= parent->getTop();
			parent = parent->getParent();
		}
		mWidgetCursor->setPosition(point);

		// если не было выделения
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// меняем выделение
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseDrag(int _left, int _top)
	{
		notifyMouseDrag(null, _left, _top);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onMouseDrag(_left, _top);
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		if (false == mIsPressed) {
			mIsPressed = true;
			updateEditState();

			if (false == mModeStatic) {
				mCursorActive = true;
				Gui::getInstance().addFrameListener(this);
				mWidgetCursor->show();
				mText->setSelectBackground(true);
				mCursorTimer = 0;
				// для первоначального обновления
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);

			}
		}
		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeySetFocus(_old);
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		if (mIsPressed) {
			mIsPressed = false;
			updateEditState();

			mCursorActive = false;
			Gui::getInstance().removeFrameListener(this);
			mWidgetCursor->hide();
			mText->setSelectBackground(false);
		}

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeyLostFocus(_new);
	}

	void Edit::_onKeyButtonPressed(int _key, Char _char)
	{
		// в статическом режиме ничего не доступно
		if (mModeStatic) {
			Widget::_onKeyButtonPressed(_key, _char);
			return;
		}

		if ( false == mWidgetCursor->isShow()) mWidgetCursor->show();
		mCursorTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
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

		} else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) commandCut();
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

		} else if (_key == OIS::KC_INSERT) {
			if (mShiftPressed) commandPast();
			else if (mCtrlPressed) commandCopy();

		} else if (_key == OIS::KC_RETURN) {
			// работаем только в режиме редактирования
			if (false == mModeReadOnly) {
				if ((mModeMultiline) && (false == mCtrlPressed)) {
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
				else eventEditSelectAccept(this);
			}

		} else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLength) {
				mCursorPosition ++;
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_UP) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= mHalfCursor.height;
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			// самая верхняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					point.left = EDIT_CURSOR_MIN_POSITION;
					mCursorPosition = mText->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// сбрасываем выделение
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_DOWN) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += getFontHeight() + mHalfCursor.height;
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			// самая нижняя строчка
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					point.left = EDIT_CURSOR_MAX_POSITION;
					point.top += mHalfCursor.height;
					mCursorPosition = mText->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// сбрасываем выделение
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_HOME) {
			IntPoint point(EDIT_CURSOR_MIN_POSITION, EDIT_CURSOR_MIN_POSITION);
			if ( false == mCtrlPressed) {
				// в начало строки
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MIN_POSITION;
				point.top += mHalfCursor.height;
			}
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_END) {
			IntPoint point(EDIT_CURSOR_MAX_POSITION, EDIT_CURSOR_MAX_POSITION);
			if ( !  mCtrlPressed) {
				// в конец строки
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MAX_POSITION;
				point.top += mHalfCursor.height;
			}
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGUP) {
			// на размер окна, но не меньше одной строки
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= (mHalfCursor.height > mWidgetUpper->getHeight()) ? mHalfCursor.height : mWidgetUpper->getHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGDOWN) {
			// на размер окна, но не меньше одной строки
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += (mHalfCursor.height > mWidgetUpper->getHeight()) ? (mHalfCursor.height + getFontHeight()) : mWidgetUpper->getHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// сбрасываем выделение
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) {
			if ( ! mShiftPressed) {
				mShiftPressed = true;
				if (mStartSelect == ITEM_NONE) {
					mStartSelect = mEndSelect = mCursorPosition;
				}
			}
		} else if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) {
			mCtrlPressed = true;

		} else if (_char != 0) {

			// если не нажат контрл, то обрабатываем как текст
			if ( false == mCtrlPressed ) {
				if (false == mModeReadOnly) {
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
			} else if (_key == OIS::KC_C) {
				commandCopy();

			} else if (_key == OIS::KC_X) {
				commandCut();

			} else if (_key == OIS::KC_V) {
				commandPast();

			} else if (_key == OIS::KC_A) {
				// выделяем весь текст
				setTextSelect(0, mTextLength);

			} else if (_key == OIS::KC_Z) {
				// отмена
				commandUndo();

			} else if (_key == OIS::KC_Y) {
				// повтор
				commandRedo();

			}
		}

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) mShiftPressed = false;
		if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) mCtrlPressed = false;

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onKeyButtonReleased(_key);
	}

	void Edit::_frameEntered(float _frame)
	{
		// в статике все недоступно
		if (mModeStatic) return;

		if (mCursorActive) {
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER ) {
				if (mWidgetCursor->isShow()) mWidgetCursor->hide();
				else mWidgetCursor->show();
				mCursorTimer -= EDIT_CURSOR_TIMER ;
			}
		}

		// сдвигаем курсор по положению мыши
		if (mMouseLeftPressed) {
			mActionMouseTimer += _frame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER ) {

				IntPoint point = InputManager::getInstance().getMousePosition();

				WidgetPtr parent = mWidgetUpper;
				while (parent != null) {
					point.left -= parent->getLeft();
					point.top -= parent->getTop();
					parent = parent->getParent();
				}

				bool action = false;

				// вверх на одну строчку
				if ( (point.top < 0) && (point.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->getWidth()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top -= mHalfCursor.height;
						action = true;
					}
				}
				// вниз на одну строчку
				else if ( (point.top > mWidgetUpper->getHeight()) && (point.top < (mWidgetUpper->getHeight() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->getWidth()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top += getFontHeight() + mHalfCursor.height;
						action = true;
					}
				}
				// влево на небольшое расстояние
				else if ( (point.left < 0) && (point.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->getHeight()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfCursor.height;
						action = true;
					}
				}
				// вправо на небольшое расстояние
				else if ( (point.left > mWidgetUpper->getWidth()) && (point.left < (mWidgetUpper->getWidth() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->getHeight()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left += (int)EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfCursor.height;
						action = true;
					}
				}

				if (action) {
					size_t old = mCursorPosition;
					mCursorPosition = mText->getTextCursorFromPoint(point);
					if ( old != mCursorPosition ) {

						WidgetPtr parent = mWidgetUpper;
						while (parent != null) {
							point.left -= parent->getLeft();
							point.top -= parent->getTop();
							parent = parent->getParent();
						}
						mWidgetCursor->setPosition(point);

						mEndSelect = (size_t)mCursorPosition;
						if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
						else mText->setTextSelect(mStartSelect, mEndSelect);

					}
					// пытаемся показать курсор
					updateView(true);

				}
				// если в зону не попадает то сбрасываем
				else mActionMouseTimer = 0;

				mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER ;
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
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::setTextSelect(size_t _start, size_t _end)
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
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->getLeft();
			point.top -= parent->getTop();
			parent = parent->getParent();
		}
		mWidgetCursor->setPosition(point);
	}

	bool Edit::deleteTextSelect(bool _history)
	{
		if ( ! isTextSelect()) return false;

		// начало и конец выделения
		size_t start, end;
		getTextSelect(start, end);

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
		if (_info != null) 	_info->push_back(tagChangeInfo(_undo, _redo, _length));
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
		Ogre::DisplayString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::reverse_iterator iter=info.rbegin(); iter!=info.rend(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == COMMAND_ERASE) text.insert((*iter).start, (*iter).text);
			else {
				mCursorPosition = (*iter).undo;
				mTextLength = (*iter).length;
			}
		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

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
		Ogre::DisplayString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == COMMAND_ERASE) text.erase((*iter).start, (*iter).text.size());
			else {
				mCursorPosition = (*iter).redo;
				mTextLength = (*iter).length;
			}

		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

		// отсылаем событие о изменении
		eventEditTextChange(this);

		return true;
	}

	void Edit::saveInHistory(VectorChangeInfo * _info)
	{
		if (_info == null) return;
		// если нет информации об изменении
		if ( _info->empty() ) return;
		if ( (_info->size() == 1) && (_info->back().type == COMMAND_POSITION)) return;

		mVectorUndoChangeInfo.push_back(*_info);
		// проверяем на максимальный размер
		if (mVectorUndoChangeInfo.size() > EDIT_MAX_UNDO)
			mVectorUndoChangeInfo.pop_front();
	}

	// возвращает текст
	Ogre::DisplayString Edit::getText(size_t _start, size_t _count)
	{
		// подстраховка
		if (_start > mTextLength) _start = mTextLength;
		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString());

		// дефолтный цвет
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

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
	void Edit::setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history)
	{
		// при изменениях сразу сбрасываем повтор
		commandResetRedo();

		// история изменений
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

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

	void Edit::updateCursor(IntPoint _point)
	{
		if (mModeStatic) return;

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			_point.left -= parent->getLeft();
			_point.top -= parent->getTop();
			parent = parent->getParent();
		}

		// если по горизонтали курсор наполовину виден, то нужно показать весь
		if (_point.left == -mHalfCursor.width) _point.left = 0;
		else if (_point.left == mWidgetUpper->getWidth()-mHalfCursor.width) _point.left = mWidgetUpper->getWidth() - mWidgetCursor->getWidth();
		mWidgetCursor->setPosition(_point);

		if ( (mShiftPressed) && (mStartSelect != ITEM_NONE) ) {
			// меняем выделение
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
			else mText->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != ITEM_NONE) {
			// сбрасываем шифт
			mStartSelect = ITEM_NONE;
			mText->setTextSelect(0, 0);
		}

		// пытаемся показать курсор
		updateView(true);
	}

	bool Edit::isShowCursorInView()
	{
		return ( (mWidgetCursor->getLeft() >= 0)
			&& (mWidgetCursor->getTop() >= 0)
			&& (mWidgetCursor->getRight() <= mWidgetUpper->getWidth())
			&& (mWidgetCursor->getBottom() <= mWidgetUpper->getHeight()) );
	}

	void Edit::setTextSelectColour(const Ogre::ColourValue & _colour, bool _history)
	{
		// нужно выделение
		if ( false == isTextSelect()) return;
		// начало и конец выделения
		size_t start, end;
		getTextSelect(start, end);
		setTextColour(start, end-start, _colour, _history);
	}

	Ogre::DisplayString Edit::getSelectedText()
	{
		if ( false == isTextSelect()) return "";
		size_t start, end;
		getTextSelect(start, end);
		return getText(start, end-start);
	}

	void Edit::getTextSelect(size_t & _start, size_t & _end)
	{
		if (mStartSelect == ITEM_NONE) {_start=ITEM_NONE; _end=ITEM_NONE;}
		else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
		else {_start = mStartSelect; _end = mEndSelect;}
	}

	void Edit::setEditPassword(bool _password)
	{
		if (mModePassword == _password) return;
		mModePassword = _password;
		if (mModePassword) {
			mPasswordText = mText->getCaption();
			mText->setCaption(Ogre::DisplayString(mTextLength, '*'));
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

	void Edit::setText(const Ogre::DisplayString & _caption, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// вставляем текст
		iterator.setText(_caption, mModeMultiline);

		// обрезаем по максимальной длинне
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

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
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::insertText(const Ogre::DisplayString & _text, size_t _start, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// если строка пустая, или размер максимален
		if ((_text.empty()) || (mTextLength == EDIT_MAX_LENGHT) ) return;

		// история изменений
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());
		// нужен ли тег текста
		// потом переделать через TextIterator чтобы отвязать понятие тег от эдита
		bool need_colour = ( (_text.size() > 6) && (_text[0] == _T('#')) && (_text[1] != _T('#')) );

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
		iterator.insertText(_text, mModeMultiline);

		// обрезаем по максимальной длинне
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

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
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::eraseText(size_t _start, size_t _count, bool _history)
	{
		// чета маловато
		if (_count == 0) return;

		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::DisplayString colour;
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
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::commandCut()
	{
		// вырезаем в буфер обмена
		if ( isTextSelect() && (false == mModePassword) ) {
			ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
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
		if ( isTextSelect() && (false == mModePassword) ) ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
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

	const Ogre::DisplayString & Edit::getRealString()
	{
		if (mModePassword) return mPasswordText;
		return mText->getCaption();
	}

	void Edit::setRealString(const Ogre::DisplayString & _caption)
	{
		if (mModePassword) {
			mPasswordText = _caption;
			mText->setCaption(Ogre::DisplayString(mTextLength, '*'));
		}
		else {
			mText->setCaption(_caption);
		}
	}

	void Edit::updateEditState()
	{
		if (mIsFocus) {
			if (mIsPressed) setState("select");
			else setState("active");
		} else {
			if (mIsPressed) setState("pressed");
			else setState("normal");
		}
	}

	void Edit::setPosition(const IntCoord& _coord)
	{
		Widget::setPosition(_coord);
		updateView(false);
	}

	void Edit::setSize(const IntSize& _size)
	{
		Widget::setSize(_size);
		updateView(false);
	}

	void Edit::updateView(bool _showCursor)
	{
		// размер контекста
		mSizeView = mText->getTextSize();
		// текущая позиция
		IntPoint point = mText->getTextShift();
		// расчетное смещение
		IntPoint offset;

		int add = _showCursor ? mHalfCursor.width : 0;

		// проверяем и показываем курсор
		if (_showCursor && ( false == isShowCursorInView())) {

			// горизонтальное смещение
			if (mSizeView.width > mWidgetUpper->getWidth()) {
				if (mWidgetCursor->getLeft() < 0) {
					offset.left = mWidgetCursor->getLeft() - (int)EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (mWidgetCursor->getRight() > mWidgetUpper->getWidth()) {
					offset.left = mWidgetCursor->getRight() - mWidgetUpper->getWidth() + (int)EDIT_OFFSET_HORZ_CURSOR;
				}
			}

			// вертикальное смещение
			if (mSizeView.height > mWidgetUpper->getHeight()) {
				if (mWidgetCursor->getTop() < 0) {
					offset.top = mWidgetCursor->getTop();
				}
				else if (mWidgetCursor->getBottom() > mWidgetUpper->getHeight()) {
					offset.top = mWidgetCursor->getBottom() - mWidgetUpper->getHeight();
				}
			}

		}

		if (mSizeView.width > mWidgetUpper->getWidth()) {

			// максимальный выход влево
			if ((point.left + offset.left) <= add) {
				offset.left = - (add + point.left);
			}
			// максимальный выход вправо
			else if ( (point.left + offset.left) > (mSizeView.width - mWidgetUpper->getWidth() + add) ) {
				offset.left = (mSizeView.width-mWidgetUpper->getWidth() + add) - point.left;
			}
		}

		if (mSizeView.height > mWidgetUpper->getHeight()) {

			// максимальный выход вверх
			if ((point.top + offset.top) < 0) {
				offset.top = - point.top;
			}
			// максимальный выход вниз
			else if ( (point.top + offset.top) > (mSizeView.height-mWidgetUpper->getHeight()) ) {
				offset.top = (mSizeView.height-mWidgetUpper->getHeight()) - point.top;
			}
		}

		// ничего не изменилось
		//if (offset.empty()) return; // hook for update не использовать !!!, т.к. код далее вызывает апдейт, хотя координаты на 1-й взгляд не изменились

		mWidgetCursor->setPosition(mWidgetCursor->getPosition() - offset);

		mText->setTextShift(point + offset);

	}

	void Edit::setCaption(const Ogre::DisplayString & _caption)
	{
		setText(_caption, false);
	}

	const Ogre::DisplayString& Edit::getCaption()
	{
		return getRealString();
	}

} // namespace MyGUI
