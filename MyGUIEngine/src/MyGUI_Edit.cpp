/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_Edit.h"
#include "MyGUI_TextIterator.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_PointerManager.h"

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
	const std::string EDIT_CURSOR_POINTER = "beam";

	Edit::Edit(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _name),
		mIsPressed(false),
		mIsFocus(false),
		mCursorTimer(0),
		mCursorActive(false),
		mCursorPosition(0),
		mTextLenght(0),
		mShiftPressed(false),
		mCtrlPressed(false),
		mStartSelect(SIZE_MAX),
		mHalfCursor(1, 1),
		mMouseLeftPressed(false),
		mActionMouseTimer(0),
		mModeReadOnly(false),
		mModePassword(false),
		mModeMultiline(false),
		mModeStatic(false)
	{

		MYGUI_ASSERT(null != mText);

		// нам нужен фокус клавы
		mNeedKeyFocus = true;

		// создаем прослойку, чтобы курсор был над текстом
		mWidgetUpper = createWidgetT("Widget", "Empty", mText->getCoord(), mText->getAlign());
		mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetUpper->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
		mWidgetUpper->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

		// парсим свойства
		const MapString & param = _info->getParams();
		MapString::const_iterator iter = param.find("CursorSkin");
		Ogre::String skin = "Default";
		if (iter != param.end()) skin = iter->second;

		// создаем курсор
		WidgetSkinInfo * info = SkinManager::getInstance().getSkin(skin);
		mWidgetCursor = mWidgetUpper->createWidgetT("Widget", skin, 0, 0, info->getSize().width, getFontHeight(), ALIGN_LEFT | ALIGN_TOP);
		mWidgetCursor->hide();
		mWidgetCursor->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetCursor->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetCursor->eventMouseButtonReleased = newDelegate(this, &Edit::notifyMouseReleased);
		mWidgetCursor->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

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
		if ( /*(_old == this) ||*/ (_old == mWidgetUpper) || (_old == mWidgetCursor) || (mIsFocus) ) return;
		mIsFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( /*(_new == this) ||*/ (_new == mWidgetUpper) || (_new == mWidgetCursor) || (false == mIsFocus) ) return;
		mIsFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		// в статике все недоступно
		if (mModeStatic) return;

		// до нажатия мы были неактивны
		//if (false == mCursorActive) {
			//InputManager::getInstance().setKeyFocusWidget(this);
			//IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
			//updateCursor(point);

		//}
		// повторное нажатие
		//else {
			//InputManager::getInstance().setKeyFocusWidget(this);

			IntPoint point = InputManager::getInstance().getLastLeftPressed();
			mCursorPosition = mText->getTextCursorFromPoint(point);
			updateCursor(point);

			if (_left) mMouseLeftPressed = true;
		//}
	}

	void Edit::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		// сбрасываем всегда
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
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
		if (mStartSelect == SIZE_MAX) mStartSelect = Old;

		// меняем выделение
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseSetFocus(WidgetPtr _old)
	{
		/*if (false == mIsFocus) {
			mIsFocus = true;
			updateEditState();
			mText->setSelectBackground(true);
		}*/

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onMouseSetFocus(_old);
	}

	void Edit::_onMouseLostFocus(WidgetPtr _new)
	{
		/*if (! ((false == mIsFocus) || (_new == mWidgetUpper) || (_new == mWidgetCursor)) ) {
			mIsFocus = false;
			updateEditState();
			mText->setSelectBackground(false);
		}*/

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onMouseLostFocus(_new);
	}

	void Edit::_onMouseMove(int _left, int _top)
	{
		notifyMouseMove(null, _left, _top);

		// !!! ОБЯЗАТЕЛЬНО вызывать в конце метода
		Widget::_onMouseMove(_left, _top);
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

	void Edit::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		MYGUI_ASSERT(null != mText);

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
			if ( (false == mModeReadOnly) && (false == deleteTextSelect(true)) ) {
				// прыгаем на одну назад и удаляем
				if (mCursorPosition != 0) {
					mCursorPosition -- ;
					eraseText(mCursorPosition, 1, true);
				}
			}

		} else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) commandCut();
			else if (false == mModeReadOnly) {
				// если нуно то удаляем выделенный текст
				if (false == deleteTextSelect(true)) {
					if (mCursorPosition != mTextLenght) {
						eraseText(mCursorPosition, 1, true); 
					}
				}
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
				}
				// при сингл лайн и и мульти+сонтрол шлем эвент
				else eventEditSelectAccept(this);
			}

		} else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLenght) {
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
				if (mCursorPosition != mTextLenght) {
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

		/*} else if (_key == OIS::KC_F1) {
			setTextSelectColour(Ogre::ColourValue::Black, true);

		} else if (_key == OIS::KC_F2) {
			setTextSelectColour(Ogre::ColourValue::Red, true);

		} else if (_key == OIS::KC_F3) {
			setTextSelectColour(Ogre::ColourValue::Blue, true);

		} else if (_key == OIS::KC_F4) {
			setTextSelectColour(Ogre::ColourValue::Green, true);

		} else if (_key == OIS::KC_F5) {
			setTextSelectColour(Ogre::ColourValue::White, true);

		} else if (_key == OIS::KC_F6) {
			mText->setTextAlign(ALIGN_LEFT | ALIGN_TOP);
			IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);

		} else if (_key == OIS::KC_F7) {
			mText->setTextAlign(ALIGN_CENTER);
			IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);

		} else if (_key == OIS::KC_F8) {
			mText->setTextAlign(ALIGN_RIGHT | ALIGN_BOTTOM);
			IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
			updateCursor(point);*/

		} else if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) {
			if ( ! mShiftPressed) {
				mShiftPressed = true;
				if (mStartSelect == SIZE_MAX) {
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
				}
			} else if (_key == OIS::KC_C) {
				commandCopy();

			} else if (_key == OIS::KC_X) {
				commandCut();

			} else if (_key == OIS::KC_V) {
				commandPast();

			} else if (_key == OIS::KC_A) {
				// выделяем весь текст
				setTextSelect(0, mTextLenght);

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

	void Edit::_frameEntered(float _frame, float _event)
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
						point.left -= EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfCursor.height;
						action = true;
					}
				}
				// вправо на небольшое расстояние
				else if ( (point.left > mWidgetUpper->getWidth()) && (point.left < (mWidgetUpper->getWidth() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->getHeight()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left += EDIT_OFFSET_HORZ_CURSOR;
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
		if (_index > mTextLenght) _index = mTextLenght;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// обновляем по позиции
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::setTextSelect(size_t _start, size_t _end)
	{
		if (_start > mTextLenght) _start = mTextLenght;
		if (_end > mTextLenght) _end = mTextLenght;
		
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

	void Edit::commandPosition(size_t _undo, size_t _redo, size_t _lenght, VectorChangeInfo * _info)
	{
		if (_info != null) 	_info->push_back(tagChangeInfo(_undo, _redo, _lenght));
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
				mTextLenght = (*iter).lenght;
			}
		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

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
				mTextLenght = (*iter).lenght;
			}

		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

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
		if (_start > mTextLenght) _start = mTextLenght;
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
		commandPosition(_start, _start+_count, mTextLenght, history);

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
		//OUT(mCursorPosition);

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

		if ( (mShiftPressed) && (mStartSelect != SIZE_MAX) ) {
			// меняем выделение
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
			else mText->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != SIZE_MAX) {
			// сбрасываем шифт
			mStartSelect = SIZE_MAX;
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
		if (mStartSelect == SIZE_MAX) {_start=SIZE_MAX; _end=SIZE_MAX;}
		else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
		else {_start = mStartSelect; _end = mEndSelect;}
	}

	void Edit::setCaption(const Ogre::DisplayString & _caption)
	{
		setText(_caption, false);
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
		iterator.cutMaxLenght(EDIT_MAX_LENGHT);

		// запоминаем размер строки
		size_t old = mTextLenght;
		// новая позиция и положение на конец вставки
		mCursorPosition = mTextLenght = iterator.getSize();

		// сохраняем позицию для восстановления курсора
		commandPosition(0, mTextLenght, old, history);

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
		if ((_text.empty()) || (mTextLenght == EDIT_MAX_LENGHT) ) return;

		// история изменений
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());
		// нужен ли тег текста
		bool need_colour = ( (_text.size() > 6) && (_text[0] == '#') && (_text[1] != '#') );

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
		iterator.cutMaxLenght(EDIT_MAX_LENGHT);

		// запоминаем размер строки
		size_t old = mTextLenght;
		// новая позиция и положение на конец вставки
		mTextLenght = iterator.getSize();
		mCursorPosition += mTextLenght - old;

		// сохраняем позицию для восстановления курсора
		commandPosition(_start, _start + mTextLenght - old, old, history);

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
		commandPosition(_start + _count, _start, mTextLenght, history);

		// на месте удаленного
		mCursorPosition = _start;
		mTextLenght -= _count;

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
			if (false == mModeReadOnly) deleteTextSelect(true);
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
		}
	}

	void Edit::updateEditState()
	{
		if (mIsFocus) {
			if (mIsPressed) setState("select");
			else setState("active");
			if (false == mModeStatic) PointerManager::getInstance().setPointer(EDIT_CURSOR_POINTER, this);
		} else {
			if (mIsPressed) setState("pressed");
			else setState("normal");
			if (false == mModeStatic) PointerManager::getInstance().defaultPointer();
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
					offset.left = mWidgetCursor->getLeft() - EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (mWidgetCursor->getRight() > mWidgetUpper->getWidth()) {
					offset.left = mWidgetCursor->getRight() - mWidgetUpper->getWidth() + EDIT_OFFSET_HORZ_CURSOR;
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
		if (offset.empty()) return;

		mWidgetCursor->setPosition(mWidgetCursor->getPosition() - offset);

		mText->setTextShift(point + offset);

	}

} // namespace MyGUI
