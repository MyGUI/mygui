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
#include "MyGUI_VScroll.h"
#include "MyGUI_HScroll.h"

namespace MyGUI
{

	Ogre::String Edit::WidgetTypeName = "Edit";

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.05f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_DEFAULT_MAX_TEXT_LENGTH = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 10.0f; // �������������� �������� ��� �������
	const int EDIT_ACTION_MOUSE_ZONE = 1500; // ������� ��� ���������� ���� �� �������� �����
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";
	const int EDIT_MOUSE_WHEEL = 50; // ������� ��� ���������� ���� �� �������� �����

	Edit::Edit(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name) :
		Widget(_coord, _align, _info, _parent, _creator, _name),
		mIsPressed(false),
		mIsFocus(false),
		//mWidgetClient(null),
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
		mModeBreak(false),
		mCharPassword('*'),
		mOverflowToTheLeft(false),
		mMaxTextLength(EDIT_DEFAULT_MAX_TEXT_LENGTH),
		mVScroll(null),
		mHScroll(null),
		mShowHScroll(true),
		mShowVScroll(true),
		mVRange(0),
		mHRange(0)
	{

		mOriginalPointer = mPointer;

		// ��� ����� ����� �����
		mNeedKeyFocus = true;

		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Client") {
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
			else if ((*iter)->_getInternalString() == "VScroll") {
				MYGUI_DEBUG_ASSERT( ! mVScroll, "widget already assigned");
				mVScroll = castWidget<VScroll>(*iter);
				mVScroll->eventScrollChangePosition = newDelegate(this, &Edit::notifyScrollChangePosition);
			}
			else if ((*iter)->_getInternalString() == "HScroll") {
				MYGUI_DEBUG_ASSERT( ! mHScroll, "widget already assigned");
				mHScroll = castWidget<HScroll>(*iter);
				mHScroll->eventScrollChangePosition = newDelegate(this, &Edit::notifyScrollChangePosition);
			}
		}

		MYGUI_ASSERT(null != mWidgetClient, "Child Widget Client not found in skin (Edit must have Client)");

		if (null != mWidgetClient->_getSubWidgetText()) {
			mText = mWidgetClient->_getSubWidgetText();
		}

		MYGUI_ASSERT(null != mText, "TextEdit not found in skin (Edit or Client must have TextEdit)");

		updateScroll();

		// �������������� ������������� �������
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

	}

	Edit::~Edit()
	{
		// �� ������ ����������
		//Gui::getInstance().removeFrameListener(newDelegate(this, &Edit::frameEntered));
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
		// � ������� ��� ����������
		if (mModeStatic) return;

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		mCursorPosition = mText->getCursorPosition(point);
		mText->setCursorPosition(mCursorPosition);
		mText->setShowCursor(true);
		mCursorTimer = 0;
		updateSelectText();

		if (_id == MB_Left) mMouseLeftPressed = true;
	}

	void Edit::notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id)
	{
		// ���������� ������
		mMouseLeftPressed = false;
	}

	void Edit::notifyMouseDrag(WidgetPtr _sender, int _left, int _top)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		// ������������� ������
		mText->setShowCursor(true);

		// ���������� ��� �������
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_left, _top);
		mCursorPosition = mText->getCursorPosition(point);
		if (Old == mCursorPosition) return;

		mText->setCursorPosition(mCursorPosition);

		// ���� �� ���� ���������
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// ������ ���������
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
		else mText->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::notifyMouseButtonDoubleClick(WidgetPtr _sender)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

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
				Gui::getInstance().eventFrameStart += newDelegate(this, &Edit::frameEntered);
				//Gui::getInstance().addFrameListener(newDelegate(this, &Edit::frameEntered), this);
				mText->setShowCursor(true);
				mText->setSelectBackground(true);
				mCursorTimer = 0;
				// ��� ��������������� ����������
				//mText->setCursorPosition(mCursorPosition);
				//updateSelectText();

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
			Gui::getInstance().eventFrameStart -= newDelegate(this, &Edit::frameEntered);
			//Gui::getInstance().removeFrameListener(newDelegate(this, &Edit::frameEntered));
			mText->setShowCursor(false);
			mText->setSelectBackground(false);
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyLostFocus(_new);
	}

	void Edit::_onKeyButtonPressed(KeyCode _key, Char _char)
	{
		InputManager & input = InputManager::getInstance();

		// � ����������� ������ ������ �� ��������
		if (mModeStatic) {
			Widget::_onKeyButtonPressed(_key, _char);
			return;
		}

		mText->setShowCursor(true);
		mCursorTimer = 0.0f;

		if (_key == KC_ESCAPE) {
			InputManager::getInstance().setKeyFocusWidget(null);
		}
		else if (_key == KC_BACK) {
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

		}
		else if (_key == KC_DELETE) {
			if (input.isShiftPressed()) commandCut();
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

		}
		else if (_key == KC_INSERT) {
			if (input.isShiftPressed()) commandPast();
			else if (input.isControlPressed()) commandCopy();

		}
		else if ((_key == KC_RETURN) || (_key == KC_NUMPADENTER)) {
			// �������� ������ � ������ ��������������
			if (false == mModeReadOnly) {
				if ((mModeMultiline) && (false == input.isControlPressed())) {
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

		}
		else if (_key == KC_RIGHT) {
			if ((mCursorPosition) < mTextLength) {
				mCursorPosition ++;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// ���������� ���������
			else if (isTextSelect() && !input.isShiftPressed()) resetSelect();

		}
		else if (_key == KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// ���������� ���������
			else if (isTextSelect() && !input.isShiftPressed()) resetSelect();

		}
		else if (_key == KC_UP) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KC_DOWN) {
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KC_HOME) {
			// � ������ ������
			if ( false == input.isControlPressed()) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MIN_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			// � ������ ����� ������
			else {
				if (0 != mCursorPosition) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}

		}
		else if (_key == KC_END) {
			// � ����� ������
			if ( false ==   input.isControlPressed()) {
				IntPoint point = mText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MAX_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mText->getCursorPosition(point);
				if ( old != mCursorPosition ) {
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			// � ����� �����
			else {
				if (mTextLength != mCursorPosition) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}

		}
		else if (_key == KC_PGUP) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top -= (mWidgetClient->getHeight() > mText->getFontHeight()) ? mWidgetClient->getHeight() : mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					mCursorPosition = 0;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KC_PGDOWN) {
			// �� ������ ����, �� �� ������ ����� ������
			IntPoint point = mText->getCursorPoint(mCursorPosition);
			point.top += (mWidgetClient->getHeight() > mText->getFontHeight()) ? mWidgetClient->getHeight() : mText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mText->getCursorPosition(point);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLength) {
					mCursorPosition = mTextLength;
					mText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// ���������� ���������
				else if (isTextSelect() && !input.isShiftPressed()) resetSelect();
			}
			else {
				mText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if ( (_key == KC_LSHIFT) || (_key == KC_RSHIFT) ) {
			// ��� ��������� ���������
			if (mStartSelect == ITEM_NONE) {
				mStartSelect = mEndSelect = mCursorPosition;
			}
		}
		else if (_char != 0) {

			// ���� �� ����� ������, �� ������������ ��� �����
			if ( false == input.isControlPressed() ) {
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
			}
			else if (_key == KC_C) {
				commandCopy();

			}
			else if (_key == KC_X) {
				commandCut();

			}
			else if (_key == KC_V) {
				commandPast();

			}
			else if (_key == KC_A) {
				// �������� ���� �����
				setTextSelect(0, mTextLength);

			}
			else if (_key == KC_Z) {
				// ������
				commandUndo();

			}
			else if (_key == KC_Y) {
				// ������
				commandRedo();

			}
		}

		// !!! ����������� �������� � ����� ������
		Widget::_onKeyButtonPressed(_key, _char);
	}

	void Edit::frameEntered(float _frame)
	{
		// � ������� ��� ����������
		if (mModeStatic) return;

		if (mCursorActive) {
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER ) {
				mText->setShowCursor(!mText->isCursorShow());
				mCursorTimer -= EDIT_CURSOR_TIMER ;
			}
		}

		// �������� ������ �� ��������� ����
		if (mMouseLeftPressed) {
			mActionMouseTimer += _frame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER ) {

				IntPoint mouse = InputManager::getInstance().getMousePosition();
				const IntRect& view = mWidgetClient->getAbsoluteRect();
				mouse.left -= view.left;
				mouse.top -= view.top;
				IntPoint point;

				bool action = false;

				// ����� �� ���� �������
				if ( (mouse.top < 0) && (mouse.top > -EDIT_ACTION_MOUSE_ZONE) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetClient->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top -= mText->getFontHeight();
						action = true;
					}
				}
				// ���� �� ���� �������
				else if ( (mouse.top > mWidgetClient->getHeight()) && (mouse.top < (mWidgetClient->getHeight() + EDIT_ACTION_MOUSE_ZONE)) ) {
					if ( (mouse.left > 0) && (mouse.left <= mWidgetClient->getWidth()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.top += mText->getFontHeight();
						action = true;
					}
				}

				// ����� �� ��������� ����������
				if ( (mouse.left < 0) && (mouse.left > -EDIT_ACTION_MOUSE_ZONE) ) {
					//if ( (mouse.top > 0) && (mouse.top <= mWidgetClient->getHeight()) ) {
						point = mText->getCursorPoint(mCursorPosition);
						point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
						action = true;
					//}
				}
				// ������ �� ��������� ����������
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

						// �������� �������� ������
						updateView(true);
					}

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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
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
		mText->setCursorPosition(mCursorPosition);
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
		Ogre::UTFString text = getRealString();

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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

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
		Ogre::UTFString text = getRealString();

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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();

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
	Ogre::UTFString Edit::getText(size_t _start, size_t _count)
	{
		// ������������
		if (_start > mTextLength) _start = mTextLength;
		// ����� ���������
		size_t end = _start + _count;

		// �������� ����� ������
		TextIterator iterator(getRealString());

		// ��������� ����
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getColour());

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
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getColour());

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

	void Edit::setTextSelectColour(const Ogre::ColourValue & _colour, bool _history)
	{
		// ����� ���������
		if ( false == isTextSelect()) return;
		// ������ � ����� ���������
		size_t start, end;
		getTextSelect(start, end);
		setTextColour(start, end-start, _colour, _history);
	}

	Ogre::UTFString Edit::getSelectedText()
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
			mText->setCaption(Ogre::UTFString(mTextLength, '*'));
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

	void Edit::setText(const Ogre::UTFString & _caption, bool _history)
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

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// �������� �� ������������ ������
			iterator.cutMaxLength(mMaxTextLength);
		}

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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void Edit::insertText(const Ogre::UTFString & _text, size_t _start, bool _history)
	{
		// ���������� ���������
		resetSelect();

		// ���� ������ ������, ��� ������ ����������
		if (_text.empty()) return;

		if ((mOverflowToTheLeft == false) && (mTextLength == mMaxTextLength)) return;

		// ������� ���������
		VectorChangeInfo * history = null;
		if (_history) history = new VectorChangeInfo();

		// �������� ����� ������
		TextIterator iterator(getRealString(), history);

		// ��������� ����
		Ogre::UTFString colour = TextIterator::convertTagColour(mText->getColour());
		// ����� �� ��� ������
		// ����� ���������� ����� TextIterator ����� �������� ������� ��� �� �����
		bool need_colour = ( (_text.size() > 6) && (_text[0] == L'#') && (_text[1] != L'#') );

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

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// �������� �� ������������ ������
			iterator.cutMaxLength(mMaxTextLength);
		}

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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
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
		Ogre::UTFString colour;
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
		mText->setCursorPosition(mCursorPosition);
		updateSelectText();
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
		if ( ! mModeStatic ) {

			InputManager & input = InputManager::getInstance();
			if ( (input.isShiftPressed()) && (mStartSelect != ITEM_NONE) ) {
				// ������ ���������
				mEndSelect = (size_t)mCursorPosition;
				if (mStartSelect > mEndSelect) mText->setTextSelect(mEndSelect, mStartSelect);
				else mText->setTextSelect(mStartSelect, mEndSelect);

			} else if (mStartSelect != ITEM_NONE) {
				// ���������� ����
				mStartSelect = ITEM_NONE;
				mText->setTextSelect(0, 0);
			}
		}

		// �������� �������� ������
		updateView(true);
	}

	void Edit::setTextAlign(Align _align)
	{
		Widget::setTextAlign(_align);
		// ��� ��� �� ���� ����� ����������
		updateView(false);
	}

	void Edit::updateView(bool _showCursor)
	{

		// ��������� ������
		updateScroll();

		// ������ ��������� ������
		IntSize textSize = mText->getTextSize();
		// ������� �������� ��������� ������
		IntPoint point = mText->getViewOffset();
		// ��������� ��������
		IntPoint offset = point;

		// ���������� ���������� �������
		IntRect cursor = mText->getCursorRect(mCursorPosition);
		cursor.right ++;
		// ���������� ���������� ���
		const IntRect& view = mWidgetClient->getAbsoluteRect();

		// ��������� ��������� �������
		bool inside = view.inside(cursor);

		// ��������� � ���������� ������
		if (_showCursor && ( false == inside)) {

			// �������������� ��������
			if (textSize.width >= view.width()) {
				if (cursor.left < view.left) {
					offset.left = point.left - (view.left - cursor.left);
					// ��������� ��������, ������ ���� ������ �� �����������
					if ((view.width() - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left -= EDIT_OFFSET_HORZ_CURSOR;
				}
				else if (cursor.right > view.right) {
					offset.left = point.left + (cursor.right - view.right);
					// ��������� ��������, ������ ���� ������ �� �����������
					if ((view.width() - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR) offset.left += EDIT_OFFSET_HORZ_CURSOR;
				}
			}

			// ������������ ��������
			if (textSize.height >= view.height()) {
				if (cursor.top < view.top) {
					offset.top = point.top - (view.top - cursor.top);
				}
				else if (cursor.bottom > view.bottom) {
					offset.top = point.top + (cursor.bottom - view.bottom);
				}
			}

		}

		// ������������ ������
		Align align = mText->getTextAlign();

		if (textSize.width >= view.width()) {
			// ������������ ����� �����
			if ((offset.left + view.width()) > textSize.width) {
				offset.left = textSize.width - view.width();
			}
			// ������������ ����� ������
			else if (offset.left < 0) {
				offset.left = 0;
			}
		}
		else {
			if (IS_ALIGN_LEFT(align)) offset.left = 0;
			else if (IS_ALIGN_RIGHT(align)) offset.left = textSize.width - view.width();
			else if (IS_ALIGN_HCENTER(align)) offset.left = (textSize.width - view.width()) / 2;
		}

		if (textSize.height > view.height()) {
			// ������������ ����� �����
			if ((offset.top + view.height()) > textSize.height) {
				offset.top = textSize.height - view.height();
			}
			// ������������ ����� ����
			else if (offset.top < 0) {
				offset.top = 0;
			}
		}
		else {
			if (IS_ALIGN_TOP(align)) offset.top = 0;
			else if (IS_ALIGN_BOTTOM(align)) offset.top = textSize.height - view.height();
			else if (IS_ALIGN_VCENTER(align)) offset.top = (textSize.height - view.height()) / 2;
		}
		
		if (offset != point) {
			mText->setViewOffset(offset);
			if (null != mVScroll) mVScroll->setScrollPosition(offset.top);
			if (null != mHScroll) mHScroll->setScrollPosition(offset.left);
		}
	}

	void Edit::updateScroll()
	{
		IntSize textSize = mText->getTextSize();

		// ������������ ����� �� ����������
		if (textSize.height > mText->getHeight()) {
			if (mVScroll != null) {
				if (( ! mVScroll->isShow()) && (mShowVScroll)) {
					mVScroll->show();
					mWidgetClient->setSize(mWidgetClient->getWidth() - mVScroll->getWidth(), mWidgetClient->getHeight());

					// ������ ������ ����� ����������
					textSize = mText->getTextSize();

					if (mHScroll != null) {
						mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

						// ���� �������� ������������ ����� ���, ����������� ��� �� �����������,
						// ������������� �������������� ����� �� ������� ������
						if ((textSize.width > mText->getWidth()) && ( ! mHScroll->isShow()) && (mShowHScroll)) {
							mHScroll->show();
							mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() - mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

							// ������ ������ ����� ����������
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}
		// ������������ ����� ����������
		else {
			if (mVScroll != null) {
				if (mVScroll->isShow()) {
					mVScroll->hide();
					mWidgetClient->setSize(mWidgetClient->getWidth() + mVScroll->getWidth(), mWidgetClient->getHeight());

					// ������ ������ ����� ����������
					textSize = mText->getTextSize();

					if (mHScroll != null) {
						mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

						// ���� ������ ������������ ����� ���, ����������� ��� �� �����������,
						// ������������� �������������� ����� �� ������� �������
						if ((textSize.width <= mText->getWidth()) && (mHScroll->isShow())) {
							mHScroll->hide();
							mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() + mHScroll->getHeight());
							mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

							// ������ ������ ����� ����������
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}


		// �������������� ����� �� ����������
		if (textSize.width > mText->getWidth()) {
			if (mHScroll != null) {
				if (( ! mHScroll->isShow()) && (mShowHScroll)) {
					mHScroll->show();
					mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() - mHScroll->getHeight());

					// ������ ������ ����� ����������
					textSize = mText->getTextSize();

					if (mVScroll != null) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() - mHScroll->getHeight());

						// ���� �������� �������������� ����� ���, ����������� ��� �� ���������,
						// ������������� ������������ ����� �� ������� ������
						if ((textSize.height > mText->getHeight()) && ( ! mVScroll->isShow()) && (mShowVScroll)) {
							mVScroll->show();
							mWidgetClient->setSize(mWidgetClient->getWidth() - mVScroll->getWidth(), mWidgetClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() - mVScroll->getWidth(), mHScroll->getHeight());

							// ������ ������ ����� ����������
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}
		// �������������� ����� ����������
		else {
			if (mHScroll != null) {
				if (mHScroll->isShow()) {
					mHScroll->hide();
					mWidgetClient->setSize(mWidgetClient->getWidth(), mWidgetClient->getHeight() + mHScroll->getHeight());

					// ������ ������ ����� ����������
					textSize = mText->getTextSize();

					if (mVScroll != null) {
						mVScroll->setSize(mVScroll->getWidth(), mVScroll->getHeight() + mHScroll->getHeight());

						// ���� ������ �������������� ����� ���, ����������� ��� �� ���������,
						// ������������� ������������ ����� �� ������� �������
						if ((textSize.height <= mText->getHeight()) && (mVScroll->isShow())) {
							mVScroll->hide();
							mWidgetClient->setSize(mWidgetClient->getWidth() + mVScroll->getWidth(), mWidgetClient->getHeight());
							mHScroll->setSize(mHScroll->getWidth() + mVScroll->getWidth(), mHScroll->getHeight());

							// ������ ������ ����� ����������
							textSize = mText->getTextSize();
						}
					}
				}
			}
		}

		mVRange = (mText->getHeight() >= textSize.height) ? 0 : textSize.height - mText->getHeight();
		mHRange = (mText->getWidth() >= textSize.width) ? 0 : textSize.width - mText->getWidth();

		size_t page = getFontHeight();
		if (mVScroll != null) {
			mVScroll->setScrollPage(page);
			mVScroll->setScrollViewPage(mCoord.width > (int)page ? mCoord.width : page);
			mVScroll->setScrollRange(mVRange + 1);
			if (textSize.height) mVScroll->setTrackSize(1. * mVScroll->getLineSize() * mText->getHeight() / textSize.height);
		}
		if (mHScroll != null) {
			mHScroll->setScrollPage(page);
			mHScroll->setScrollViewPage(mCoord.height > (int)page ? mCoord.height : page);
			mHScroll->setScrollRange(mHRange + 1);
			if (textSize.width) mHScroll->setTrackSize(1. * mHScroll->getLineSize() * mText->getWidth() / textSize.width);
		}

	}

	void Edit::notifyScrollChangePosition(WidgetPtr _sender, size_t _position)
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
				if (mVScroll != null) {
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
				if (mHScroll != null) {
					mHScroll->setScrollPosition(offset);
				}
				mText->setViewOffset(point);
			}
		}
	}

	void Edit::setEditBreakLine(bool _break)
	{
		mModeBreak = _break;
		mText->setBreakLine(mModeBreak);
		mWidgetClient->setSize(mWidgetClient->getSize());
	}

} // namespace MyGUI
