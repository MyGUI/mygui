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
	const float EDIT_OFFSET_HORZ_CURSOR = 50.0f; // �������������� �������� ��� �������
	const int EDIT_ACTION_MOUSE_ZONE = 50; // ������� ��� ���������� ���� �� �������� �����
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

		// ��� ����� ����� �����
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
		// ������ ����������� �������, �������� ��� �����
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
				mWidgetCursor->setSize(IntSize(mWidgetCursor->getWidth(), getFontHeight())); // ������ ��� �����
				break;
			}
		}
		MYGUI_ASSERT(null != mWidgetCursor, "Child Widget Cursor not found in skin in Client (Edit must have Cursor)");

		// ����������� ���������
		mHalfCursor.set(mWidgetCursor->getWidth()/2, mWidgetCursor->getHeight()/2);
		if (mHalfCursor.width < 1) mHalfCursor.width = 1;
		if (mHalfCursor.height < 1) mHalfCursor.height = 1;

		// �������������� ������������� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

	}

	Edit::~Edit()
	{
		// �� ������ ����������
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
		// � ������� ��� ����������
		if (mModeStatic) return;

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = mText->getTextCursorFromPoint(point);
		updateCursor(point);

		if (_left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left)
	{
		// ���������� ������
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		// ������������� ������
		if ( false == mWidgetCursor->isShow()) mWidgetCursor->show();

		// ���������� ��� �������
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

		// ���� �� ���� ���������
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// ������ ���������
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseDrag(int _left, int _top)
	{
		notifyMouseDrag(null, _left, _top);

		// !!! ����������� �������� � ����� ������
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
				// ��� ��������������� ����������
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);

			}
		}
		// !!! ����������� �������� � ����� ������
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

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyLostFocus(_new);
	}

	void Edit::_onKeyButtonPressed(int _key, Char _char)
	{
		// � ����������� ������ ������ �� ��������
		if (mModeStatic) {
			Widget::_onKeyButtonPressed(_key, _char);
			return;
		}

		if ( false == mWidgetCursor->isShow()) mWidgetCursor->show();
		mCursorTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
			// ���� ���� �� ������� ���������� �����
			if (false == mModeReadOnly) {
				if (false == deleteTextSelect(true)) {
					// ������� �� ���� ����� � �������
					if (mCursorPosition != 0) {
						mCursorPosition -- ;
						eraseText(mCursorPosition, 1, true);
					}
				}
				// �������� ������� � ���������
				eventEditTextChange(this);
			}

		} else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) commandCut();
			else if (false == mModeReadOnly) {
				// ���� ���� �� ������� ���������� �����
				if (false == deleteTextSelect(true)) {
					if (mCursorPosition != mTextLength) {
						eraseText(mCursorPosition, 1, true);
					}
				}
				// �������� ������� � ���������
				eventEditTextChange(this);
			}

		} else if (_key == OIS::KC_INSERT) {
			if (mShiftPressed) commandPast();
			else if (mCtrlPressed) commandCopy();

		} else if (_key == OIS::KC_RETURN) {
			// �������� ������ � ������ ��������������
			if (false == mModeReadOnly) {
				if ((mModeMultiline) && (false == mCtrlPressed)) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect(true);
					insertText(TextIterator::getTextNewLine(), mCursorPosition, true);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
					// �������� ������� � ���������
					eventEditTextChange(this);
				}
				// ��� ����� ���� � � ������+������� ���� �����
				else eventEditSelectAccept(this);
			}

		} else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLength) {
				mCursorPosition ++;
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
				updateCursor(point);
			}
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_UP) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= mHalfCursor.height;
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					point.left = EDIT_CURSOR_MIN_POSITION;
					mCursorPosition = mText->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_DOWN) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += getFontHeight() + mHalfCursor.height;
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					point.left = EDIT_CURSOR_MAX_POSITION;
					point.top += mHalfCursor.height;
					mCursorPosition = mText->getTextCursorFromPoint(point);
					updateCursor(point);
				}
				// ���������� ���������
				else if (isTextSelect() && !mShiftPressed) resetSelect();
			} else updateCursor(point);

		} else if (_key == OIS::KC_HOME) {
			IntPoint point(EDIT_CURSOR_MIN_POSITION, EDIT_CURSOR_MIN_POSITION);
			if ( false == mCtrlPressed) {
				// � ������ ������
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MIN_POSITION;
				point.top += mHalfCursor.height;
			}
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_END) {
			IntPoint point(EDIT_CURSOR_MAX_POSITION, EDIT_CURSOR_MAX_POSITION);
			if ( !  mCtrlPressed) {
				// � ����� ������
				point = getWorldPostion(mWidgetCursor);
				point.left = EDIT_CURSOR_MAX_POSITION;
				point.top += mHalfCursor.height;
			}
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGUP) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= (mHalfCursor.height > mWidgetUpper->getHeight()) ? mHalfCursor.height : mWidgetUpper->getHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
			else if (isTextSelect() && !mShiftPressed) resetSelect();

		} else if (_key == OIS::KC_PGDOWN) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += (mHalfCursor.height > mWidgetUpper->getHeight()) ? (mHalfCursor.height + getFontHeight()) : mWidgetUpper->getHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getTextCursorFromPoint(point);
			if ( old != mCursorPosition ) updateCursor(point);
			// ���������� ���������
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

			// ���� �� ����� ������, �� ������������ ��� �����
			if ( false == mCtrlPressed ) {
				if (false == mModeReadOnly) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect(true);
					insertText(TextIterator::getTextCharInfo(_char), mCursorPosition, true);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
					// �������� ������� � ���������
					eventEditTextChange(this);
				}
			} else if (_key == OIS::KC_C) {
				commandCopy();

			} else if (_key == OIS::KC_X) {
				commandCut();

			} else if (_key == OIS::KC_V) {
				commandPast();

			} else if (_key == OIS::KC_A) {
				// �������� ���� �����
				setTextSelect(0, mTextLength);

			} else if (_key == OIS::KC_Z) {
				// ������
				commandUndo();

			} else if (_key == OIS::KC_Y) {
				// ������
				commandRedo();

			}
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) mShiftPressed = false;
		if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) mCtrlPressed = false;

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonReleased(_key);
	}

	void Edit::_frameEntered(float _frame)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		if (mCursorActive) {
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER ) {
				if (mWidgetCursor->isShow()) mWidgetCursor->hide();
				else mWidgetCursor->show();
				mCursorTimer -= EDIT_CURSOR_TIMER ;
			}
		}

		// �������� ������ �� ��������� ����
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

				// ����� �� ���� �������
				if ( (point.top < 0) && (point.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->getWidth()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top -= mHalfCursor.height;
						action = true;
					}
				}
				// ���� �� ���� �������
				else if ( (point.top > mWidgetUpper->getHeight()) && (point.top < (mWidgetUpper->getHeight() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (point.left > 0) && (point.left <= mWidgetUpper->getWidth()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.top += getFontHeight() + mHalfCursor.height;
						action = true;
					}
				}
				// ����� �� ��������� ����������
				else if ( (point.left < 0) && (point.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (point.top > 0) && (point.top <= mWidgetUpper->getHeight()) ) {
						point = getWorldPostion(mWidgetCursor);
						point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
						point.top += mHalfCursor.height;
						action = true;
					}
				}
				// ������ �� ��������� ����������
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
					// �������� �������� ������
					updateView(true);

				}
				// ���� � ���� �� �������� �� ����������
				else mActionMouseTimer = 0;

				mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER ;
			}

		} // if (mMouseLeftPressed)
  }

	void Edit::setTextCursor(size_t _index)
	{
		// ���������� ���������
		resetSelect();

		// ����� �������
		if (_index > mTextLength) _index = mTextLength;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// ��������� �� �������
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
		// ������ �� ����� ���������
		mCursorPosition = mEndSelect;

		// ��������� �� �������
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

		// ������ � ����� ���������
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
		if (mVectorUndoChangeInfo.size() < 2) return; // �� ������
		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		mVectorUndoChangeInfo.pop_back();

		// ���������� ������������������
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {
			mVectorUndoChangeInfo.back().push_back((*iter));
		}
	}

	bool Edit::commandUndo()
	{
		if (mVectorUndoChangeInfo.empty()) return false;

		// ���������� ���������
		resetSelect();

		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		// ������������ ��������� ����� �����
		mVectorUndoChangeInfo.pop_back();
		mVectorRedoChangeInfo.push_back(info);

		// ����� ����� ��� �������������
		Ogre::DisplayString text = getRealString();

		// ��������������� ������������������
		for (VectorChangeInfo::reverse_iterator iter=info.rbegin(); iter!=info.rend(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == COMMAND_ERASE) text.insert((*iter).start, (*iter).text);
			else {
				mCursorPosition = (*iter).undo;
				mTextLength = (*iter).length;
			}
		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

		// �������� ������� � ���������
		eventEditTextChange(this);

		return true;
	}

	bool Edit::commandRedo()
	{
		if (mVectorRedoChangeInfo.empty()) return false;

		// ���������� ���������
		resetSelect();

		// ��������� ��������� ����� �����
		VectorChangeInfo info = mVectorRedoChangeInfo.back();
		// ������������ ��������� ����� �����
		mVectorRedoChangeInfo.pop_back();
		mVectorUndoChangeInfo.push_back(info);

		// ����� ����� ��� �������������
		Ogre::DisplayString text = getRealString();

		// ��������������� ������������������
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == COMMAND_ERASE) text.erase((*iter).start, (*iter).text.size());
			else {
				mCursorPosition = (*iter).redo;
				mTextLength = (*iter).length;
			}

		}

		// ���������� �����
		setRealString(text);

		// ��������� �� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);

		// �������� ������� � ���������
		eventEditTextChange(this);

		return true;
	}

	void Edit::saveInHistory(VectorChangeInfo * _info)
	{
		if (_info == null) return;
		// ���� ��� ���������� �� ���������
		if ( _info->empty() ) return;
		if ( (_info->size() == 1) && (_info->back().type == COMMAND_POSITION)) return;

		mVectorUndoChangeInfo.push_back(*_info);
		// ��������� �� ������������ ������
		if (mVectorUndoChangeInfo.size() > EDIT_MAX_UNDO)
			mVectorUndoChangeInfo.pop_front();
	}

	// ���������� �����
	Ogre::DisplayString Edit::getText(size_t _start, size_t _count)
	{
		// ������������
		if (_start > mTextLength) _start = mTextLength;
		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString());

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

		// ����� �� ��������� ����
		bool need_colour = true;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);

				continue;
			}

			// ��������� �� ���������� ���������� ����
			else if (pos == _start) {
				need_colour = ! iterator.getTagColour(colour);
				// ��������� ����� ������ ����������
				iterator.saveStartPoint();

			}

			// � ������ ������ �� ����� ���������
			else if (pos == end) break;

		};

		// ���������� ������
		if (need_colour) return colour + iterator.getFromStart();
		return iterator.getFromStart();
	}

	// �������� ������ ��������
	void Edit::setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history)
	{
		// ��� ���������� ����� ���������� ������
		commandResetRedo();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ����� ���� �� ������� � ����������
			iterator.getTagColour(colour);

			// ��� ����
			if (pos < _start) continue;

			// ������ ��������� ���
			else if (pos == _start)
				iterator.setTagColour(_colour);

			// ������ ��������� ������� ���
			else if (pos < end)
				iterator.clearTagColour();

			// �� ����� ������ ��������� ��������� ��� ���������
			else if (pos == end) {
				iterator.setTagColour(colour);
				// � ������� �� �����
				break;
			}

		};

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start+_count, mTextLength, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
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

		// ���� �� ����������� ������ ���������� �����, �� ����� �������� ����
		if (_point.left == -mHalfCursor.width) _point.left = 0;
		else if (_point.left == mWidgetUpper->getWidth()-mHalfCursor.width) _point.left = mWidgetUpper->getWidth() - mWidgetCursor->getWidth();
		mWidgetCursor->setPosition(_point);

		if ( (mShiftPressed) && (mStartSelect != ITEM_NONE) ) {
			// ������ ���������
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
			else mText->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != ITEM_NONE) {
			// ���������� ����
			mStartSelect = ITEM_NONE;
			mText->setTextSelect(0, 0);
		}

		// �������� �������� ������
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
		// ����� ���������
		if ( false == isTextSelect()) return;
		// ������ � ����� ���������
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
		// ��������� �� ��������
		updateView(false);
		// ���������� �������
		commandResetHistory();
	}

	void Edit::setText(const Ogre::DisplayString & _caption, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� �����
		iterator.setText(_caption, mModeMultiline);

		// �������� �� ������������ ������
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLength;
		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = mTextLength = iterator.getSize();

		// ��������� ������� ��� �������������� �������
		commandPosition(0, mTextLength, old, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::insertText(const Ogre::DisplayString & _text, size_t _start, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ���� ������ ������, ��� ������ ����������
		if ((_text.empty()) || (mTextLength == EDIT_MAX_LENGHT) ) return;

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour = TextIterator::convertTagColour(mText->getColour());
		// ����� �� ��� ������
		// ����� ���������� ����� TextIterator ����� �������� ������� ��� �� �����
		bool need_colour = ( (_text.size() > 6) && (_text[0] == _T('#')) && (_text[1] != _T('#')) );

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ������� ����
			if (need_colour) iterator.getTagColour(colour);

			// ���� ����� �� �������
			if (pos == _start) break;

		};

		// ���� ����� ���� �� ���������
		if (need_colour) iterator.setTagColour(colour);

		// � ������ ��������� ������
		iterator.insertText(_text, mModeMultiline);

		// �������� �� ������������ ������
		iterator.cutMaxLength(EDIT_MAX_LENGHT);

		// ���������� ������ ������
		size_t old = mTextLength;
		// ����� ������� � ��������� �� ����� �������
		mTextLength = iterator.getSize();
		mCursorPosition += mTextLength - old;

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start + mTextLength - old, old, history);

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::eraseText(size_t _start, size_t _count, bool _history)
	{
		// ���� ��������
		if (_count == 0) return;

		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::DisplayString colour;
		// ����� ���������
		size_t end = _start + _count;
		bool need_colour = false;

		// ���� ������� �� ������
		while (iterator.moveNext()) {

			// �������� �������
			size_t pos = iterator.getPosition();

			// ��� ����
			if (pos < _start) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
				continue;
			}

			// ��������� ����� ������ ����������
			else if (pos == _start) {
				// ���� �� ��������� ��� ����, �� ����� ������� ���
				if ( ! colour.empty()) {
					need_colour = true;
					colour.clear();
				}
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
				iterator.saveStartPoint();
			}

			// ������ ���������
			else if (pos < end) {
				// ����� ���� �� ������� � ����������
				iterator.getTagColour(colour);
			}

			// ��������� ���������
			else if (pos == end) {
				// ����� ������� ��� ��� ���
				if ( ! colour.empty()) need_colour = true;
				if ( iterator.getTagColour(colour)) need_colour = false;

				break;
			}

		};

		// ������� ��������
		iterator.eraseFromStart();
		// � ��������� ��������� ����
		if (need_colour) iterator.setTagColour(colour);

		// ��������� ������� ��� �������������� �������
		commandPosition(_start + _count, _start, mTextLength, history);

		// �� ����� ����������
		mCursorPosition = _start;
		mTextLength -= _count;

		// ���������� � �������
		if (_history) {
			saveInHistory(history);
			delete history;
		}
		// ���������� �������
		else commandResetHistory();

		// � ���������� ������ �� �����
		setRealString(iterator.getText());

		// ��������� �� �������
		IntPoint point = mText->getTextCursorFromPosition(mCursorPosition);
		updateCursor(point);
	}

	void Edit::commandCut()
	{
		// �������� � ����� ������
		if ( isTextSelect() && (false == mModePassword) ) {
			ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
			if (false == mModeReadOnly) {
				deleteTextSelect(true);
				// �������� ������� � ���������
				eventEditTextChange(this);
			}
		}
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandCopy()
	{
		// �������� � ����� ������
		if ( isTextSelect() && (false == mModePassword) ) ClipboardManager::getInstance().SetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getSelectedText());
		else ClipboardManager::getInstance().ClearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void Edit::commandPast()
	{
		// �������� �� ������ ������
		std::string clipboard = ClipboardManager::getInstance().GetClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
		if ( (false == mModeReadOnly) && ( false == clipboard.empty()) ) {
			// ������� ����������� ���� �������
			size_t size = mVectorUndoChangeInfo.size();
			// ��������������� ��������
			deleteTextSelect(true);
			insertText(clipboard, mCursorPosition, true);
			// ��������� �� ����������� �����������
			if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
			// �������� ������� � ���������
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
		// ������ ���������
		mSizeView = mText->getTextSize();
		// ������� �������
		IntPoint point = mText->getTextShift();
		// ��������� ��������
		IntPoint offset;

		int add = _showCursor ? mHalfCursor.width : 0;

		// ��������� � ���������� ������
		if (_showCursor && ( false == isShowCursorInView())) {

			// �������������� ��������
			if (mSizeView.width > mWidgetUpper->getWidth()) {
				if (mWidgetCursor->getLeft() < 0) {
					offset.left = mWidgetCursor->getLeft() - (int)EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (mWidgetCursor->getRight() > mWidgetUpper->getWidth()) {
					offset.left = mWidgetCursor->getRight() - mWidgetUpper->getWidth() + (int)EDIT_OFFSET_HORZ_CURSOR;
				}
			}

			// ������������ ��������
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

			// ������������ ����� �����
			if ((point.left + offset.left) <= add) {
				offset.left = - (add + point.left);
			}
			// ������������ ����� ������
			else if ( (point.left + offset.left) > (mSizeView.width - mWidgetUpper->getWidth() + add) ) {
				offset.left = (mSizeView.width-mWidgetUpper->getWidth() + add) - point.left;
			}
		}

		if (mSizeView.height > mWidgetUpper->getHeight()) {

			// ������������ ����� �����
			if ((point.top + offset.top) < 0) {
				offset.top = - point.top;
			}
			// ������������ ����� ����
			else if ( (point.top + offset.top) > (mSizeView.height-mWidgetUpper->getHeight()) ) {
				offset.top = (mSizeView.height-mWidgetUpper->getHeight()) - point.top;
			}
		}

		// ������ �� ����������
		//if (offset.empty()) return; // hook for update �� ������������ !!!, �.�. ��� ����� �������� ������, ���� ���������� �� 1-� ������ �� ����������

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
