
#include "Edit.h"

namespace MyGUI
{
	// ������� ������� ��� ����� �������
	namespace factory { WidgetFactory<Edit> EditFactoryInstance("Edit"); }

	const float CURSOR_TIMER = 0.7f;
	const int CURSOR_MAX_POSITION = 100000;
	const int CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_MAX_LENGHT = 24;

	Edit::Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name) :
		Widget(_x, _y, _cx, _cy, _align, _info, _parent, _name),
		m_isPressed(false),
		m_isFocus(false),
		mTimer(0),
		mCursorActive(false),
		mCursorPosition(0),
		mTextLenght(0),
		mShiftPressed(false),
		mCtrlPressed(false),
		mStartSelect(ITEM_NONE)
	{

		ASSERT(m_text);

		// ������� ���������, ����� ������ ��� ��� �������
		mWidgetUpper = createWidget("Widget", "Empty", m_text->left(), m_text->top(), m_text->width(), m_text->height(), m_text->getAlign());
		mWidgetUpper->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetUpper->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetUpper->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetUpper->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

		// ������ ��������
		const SkinParam & param = _info->getParams();
		SkinParam::const_iterator iter = param.find("CursorSkin");
		String skin = "Default";
		if (iter != param.end()) skin = iter->second;

		// ������� ������
		WidgetSkinInfo * info = SkinManager::getInstance().getSkin(skin);
		mWidgetCursor = mWidgetUpper->createWidget("Widget", skin, 0, 0, info->getSize().width, getFontHeight(), ALIGN_LEFT | ALIGN_TOP);
		mWidgetCursor->show(false);
		mWidgetCursor->eventMouseSetFocus = newDelegate(this, &Edit::notifyMouseSetFocus);
		mWidgetCursor->eventMouseLostFocus = newDelegate(this, &Edit::notifyMouseLostFocus);
		mWidgetCursor->eventMouseButtonPressed = newDelegate(this, &Edit::notifyMousePressed);
		mWidgetCursor->eventMouseMove = newDelegate(this, &Edit::notifyMouseMove);

		//m_text->setTextSelect(10, 20);
		//setText("iuyerituwyeriutweirunwgrrg");
	}

	void Edit::notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		if ( (_old == this) || (_old == mWidgetUpper) || (_old == mWidgetCursor) || (m_isFocus) ) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		if ( (_new == this) || (_new == mWidgetUpper) || (_new == mWidgetCursor) || (!m_isFocus) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left)
	{
		InputManager::getInstance().setKeyFocusWidget(this);

		IntPoint point = InputManager::getInstance().getLastLeftPressed();
		m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
		updateCursor(point);
	}

	void Edit::notifyMouseMove(MyGUI::WidgetPtr _sender, int _x, int _y)
	{
		// ������������� ������
		if ( ! mWidgetCursor->isShow()) mWidgetCursor->show(true);
		mTimer = 0.0f;

		size_t Old = mCursorPosition;
		IntPoint point(_x, _y);
		m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
		if (Old == mCursorPosition) return;

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->left();
			point.top -= parent->top();
			parent = parent->getParent();
		}
		mWidgetCursor->move(point.left, point.top);

		// ���� �� ���� ���������
		if (mStartSelect == ITEM_NONE) mStartSelect = Old;

		// ������ ���������
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
		else m_text->setTextSelect(mStartSelect, mEndSelect);

	}

	void Edit::_onMouseSetFocus(WidgetPtr _old)
	{
		Widget::_onMouseSetFocus(_old);
		if (m_isFocus) return;
		m_isFocus = true;
		updateEditState();
	}

	void Edit::_onMouseLostFocus(WidgetPtr _new)
	{
		Widget::_onMouseLostFocus(_new);
		if ( (!m_isFocus) || (_new == mWidgetUpper) || (_new == mWidgetCursor) ) return;
		m_isFocus = false;
		updateEditState();
	}

	void Edit::_onMouseMove(int _x, int _y)
	{
		Widget::_onMouseMove(_x, _y);
		notifyMouseMove(null, _x, _y);
	}

	void Edit::_onKeySetFocus(WidgetPtr _old)
	{
		Widget::_onKeySetFocus(_old);
		if (m_isPressed) return;
		m_isPressed = true;
		updateEditState();

		mCursorActive = true;
		Ogre::Root::getSingleton().addFrameListener(this);
		mWidgetCursor->show(true);
		mTimer = 0;
	}

	void Edit::_onKeyLostFocus(WidgetPtr _new)
	{
		Widget::_onKeyLostFocus(_new);
		if ( ! m_isPressed) return;
		m_isPressed = false;
		updateEditState();

		mCursorActive = false;
		Ogre::Root::getSingleton().removeFrameListener(this);
		mWidgetCursor->show(false);
	}

	void Edit::_onKeyButtonPressed(int _key, wchar_t _char)
	{
		Widget::_onKeyButtonPressed(_key, _char);
		ASSERT(m_text);

		if ( ! mWidgetCursor->isShow()) mWidgetCursor->show(true);
		mTimer = 0.0f;

		if (_key == OIS::KC_ESCAPE) InputManager::getInstance().setKeyFocusWidget(null);
		else if (_key == OIS::KC_BACK) {
			// ���� ���� �� ������� ���������� �����
			if (!deleteTextSelect()) {
				// ������� �� ���� ����� � �������
				if (mCursorPosition != 0) {
					mCursorPosition -- ;
					eraseText(mCursorPosition, 1);
				}
			}

		} else if (_key == OIS::KC_DELETE) {
			if (mShiftPressed) {
				// �������� � ����� ������
				if ( isTextSelect() ) {
					mClipboard = getSelectedText();
					deleteTextSelect();
				} else mClipboard = "";

			} else {
				// ���� ���� �� ������� ���������� �����
				if (!deleteTextSelect()) {
					if (mCursorPosition != mTextLenght) {
						eraseText(mCursorPosition, 1); 
					}
				}
			}

		} else if (_key == OIS::KC_INSERT) {
			if (mShiftPressed) {
				// �������� �� ������ ������
				if ( ! mClipboard.empty()) {
					deleteTextSelect();
					insertText(mClipboard, mCursorPosition);
				}

			} else if (mCtrlPressed) {
				// �������� � ����� ������
				if ( isTextSelect() ) mClipboard = getSelectedText();
				else mClipboard = "";
			}

		} else if (_key == OIS::KC_RETURN) {
			// ������� ����������� ���� �������
			size_t size = mVectorUndoChangeInfo.size();
			// ��������������� ��������
			deleteTextSelect();
			insertText(m_text->getTextNewLine(), mCursorPosition);
			// ��������� �� ����������� �����������
			if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();

		} else if (_key == OIS::KC_RIGHT) {
			if ((mCursorPosition) < mTextLenght) {
				mCursorPosition ++;
				IntPoint point;
				m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
				updateCursor(point);
			}

		} else if (_key == OIS::KC_LEFT) {
			if (mCursorPosition != 0) {
				mCursorPosition --;
				IntPoint point;
				m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
				updateCursor(point);
			}

		} else if (_key == OIS::KC_UP) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top -= (getFontHeight() / 2);
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			// ����� ������� �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != 0) {
					point.left = CURSOR_MIN_POSITION;
					m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
					updateCursor(point);
				}
			} else updateCursor(point);

		} else if (_key == OIS::KC_DOWN) {
			IntPoint point = getWorldPostion(mWidgetCursor);
			point.top += getFontHeight() + (getFontHeight() / 2);
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			// ����� ������ �������
			if ( old == mCursorPosition ) {
				if (mCursorPosition != mTextLenght) {
					point.left = CURSOR_MAX_POSITION;
					point.top += (getFontHeight() / 2);
					m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
					updateCursor(point);
				}
			} else updateCursor(point);

		} else if (_key == OIS::KC_HOME) {
			IntPoint point(CURSOR_MIN_POSITION, CURSOR_MIN_POSITION);
			if ( !  mCtrlPressed) {
				// � ������ ������
				point = getWorldPostion(mWidgetCursor);
				point.left = CURSOR_MIN_POSITION;
				point.top += (getFontHeight() / 2);
			}
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			if ( old != mCursorPosition ) updateCursor(point);

		} else if (_key == OIS::KC_END) {
			IntPoint point(CURSOR_MAX_POSITION, CURSOR_MAX_POSITION);
			if ( !  mCtrlPressed) {
				// � ����� ������
				point = getWorldPostion(mWidgetCursor);
				point.left = CURSOR_MAX_POSITION;
				point.top += (getFontHeight() / 2);
			}
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			if ( old != mCursorPosition ) updateCursor(point);

		} else if (_key == OIS::KC_PGUP) {
			IntPoint point(CURSOR_MIN_POSITION, CURSOR_MIN_POSITION);
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			if ( old != mCursorPosition ) updateCursor(point);

		} else if (_key == OIS::KC_PGDOWN) {
			IntPoint point(CURSOR_MAX_POSITION, CURSOR_MAX_POSITION);
			size_t old = mCursorPosition;
			m_text->getTextCursorFromPoint(point.left, point.top, mCursorPosition);
			if ( old != mCursorPosition ) updateCursor(point);

		} else if (_key == OIS::KC_F1) {
			setTextSelectColor(Ogre::ColourValue::Black);

		} else if (_key == OIS::KC_F2) {
			setTextSelectColor(Ogre::ColourValue::Red);

		} else if (_key == OIS::KC_F3) {
			setTextSelectColor(Ogre::ColourValue::Blue);

		} else if (_key == OIS::KC_F4) {
			setTextSelectColor(Ogre::ColourValue::Green);

		} else if (_key == OIS::KC_F5) {
			setTextSelectColor(Ogre::ColourValue::White);

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
			if ( ! mCtrlPressed ) {
				// ������� ����������� ���� �������
				size_t size = mVectorUndoChangeInfo.size();
				// ��������������� ��������
				deleteTextSelect();
				insertText(m_text->getTextCharInfo(_char), mCursorPosition);
				// ��������� �� ����������� �����������
				if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();

			} else if (_key == OIS::KC_C) {
				// �������� � ����� ������
				if ( isTextSelect() ) mClipboard = getSelectedText();
				else mClipboard = "";

			} else if (_key == OIS::KC_X) {
				// �������� � ����� ������
				if ( isTextSelect() ) {
					mClipboard = getSelectedText();
					deleteTextSelect();
				} else mClipboard = "";

			} else if (_key == OIS::KC_V) {
				// �������� �� ������ ������
				if ( ! mClipboard.empty()) {
					// ������� ����������� ���� �������
					size_t size = mVectorUndoChangeInfo.size();
					// ��������������� ��������
					deleteTextSelect();
					insertText(mClipboard, mCursorPosition);
					// ��������� �� ����������� �����������
					if ((size+2) == mVectorUndoChangeInfo.size()) commandMerge();
				}

			} else if (_key == OIS::KC_A) {
				// �������� ���� �����
				setTextSelect(0, mTextLenght);

			} else if (_key == OIS::KC_Z) {
				// ������
				commandUndo();

			} else if (_key == OIS::KC_Y) {
				// ������
				commandRedo();

			}

		}
	}

	void Edit::_onKeyButtonReleased(int _key)
	{
		Widget::_onKeyButtonReleased(_key);
		if ( (_key == OIS::KC_LSHIFT) || (_key == OIS::KC_RSHIFT) ) mShiftPressed = false;
		if ( (_key == OIS::KC_LCONTROL) || (_key == OIS::KC_RCONTROL) ) mCtrlPressed = false;
	}

	bool Edit::frameStarted(const Ogre::FrameEvent& evt)
	{
		if (!mCursorActive) return true;
		mTimer += evt.timeSinceLastFrame;

		if (mTimer <= CURSOR_TIMER) return true;

		mWidgetCursor->show( ! mWidgetCursor->isShow());
		mTimer -= CURSOR_TIMER;
		return true;
	}

	bool Edit::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}

	void Edit::updateCursor(IntPoint _point)
	{
		OUT(mCursorPosition);

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			_point.left -= parent->left();
			_point.top -= parent->top();
			parent = parent->getParent();
		}
		mWidgetCursor->move(_point.left, _point.top);

		// ��������� ������ �� ����� �� ������� ���������
		if ( (_point.left < 0) || (_point.top < 0) || ((_point.left+mWidgetCursor->width()) > mWidgetUpper->width()) || ((_point.top+mWidgetCursor->height()) > mWidgetUpper->height()) ) {
		//	OUT("exit");
		}

		if ( (mShiftPressed) && (mStartSelect != ITEM_NONE) ) {
			// ������ ���������
			mEndSelect = (size_t)mCursorPosition;
			if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
			else m_text->setTextSelect(mStartSelect, mEndSelect);

		} else if (mStartSelect != ITEM_NONE) {
			// ���������� ����
			mStartSelect = ITEM_NONE;
			m_text->setTextSelect(0, 0);
		}
	}

	void Edit::setTextCursor(size_t _index)
	{
		// ���������� ���������
		resetSelect();

		// ����� �������
		if (_index > mTextLenght) _index = mTextLenght;
		if (mCursorPosition == _index) return;
		mCursorPosition = _index;

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);
	}

	void Edit::setTextSelect(size_t _start, size_t _end)
	{
		if (_start > mTextLenght) _start = mTextLenght;
		if (_end > mTextLenght) _end = mTextLenght;
		
		mStartSelect = _start;
		mEndSelect = _end;

		if (mStartSelect > mEndSelect) m_text->setTextSelect(mEndSelect, mStartSelect);
		else m_text->setTextSelect(mStartSelect, mEndSelect);

		if (mCursorPosition == mEndSelect) return;
		// ������ �� ����� ���������
		mCursorPosition = mEndSelect;

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);

		WidgetPtr parent = mWidgetUpper;
		while (parent != null) {
			point.left -= parent->left();
			point.top -= parent->top();
			parent = parent->getParent();
		}
		mWidgetCursor->move(point.left, point.top);
	}

	bool Edit::deleteTextSelect()
	{
		if ( ! isTextSelect()) return false;

		// ������ � ����� ���������
		size_t start, end;
		getTextSelect(start, end);

		eraseText(start, end - start);

		return true;
	}

	// ���������� �����
	Ogre::DisplayString Edit::getText(size_t _start, size_t _count)
	{
		// ����� ������� ������ ��� ����, ������� ��� �� ������
		size_t start = _start, count = 0;
		m_text->getTagColor(start, count);
		// ���� ���� ���� ����� �������, �� ������ �� ����
		if (count != 0 ) {
			m_text->convertTextRange(_start, _count);
			return m_text->getCaption().substr(_start, _count);
		}

		// �������� �� ������ � ������� ��������� ��� ����� �����
		Ogre::DisplayString color = m_text->getTagColor();
		for (size_t pos=0; pos<_start; pos++) {

			size_t start = pos, count = 0;
			m_text->getTagColor(start, count);

			// ���� ����, ����������
			if (count != 0) color = m_text->getCaption().substr(start, count);
		}

		// � ���������� ������ � ������ �������
		m_text->convertTextRange(_start, _count);
		return color + m_text->getCaption().substr(_start, _count);
	}

	// �������� ������ ��������
	void Edit::setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _color)
	{
		// ������� ���������
		VectorChangeInfo history;

		// �������� �� ������ � ������� ��������� ��� ����� �����
		size_t end = _start + _count;
		Ogre::DisplayString oldColor;
		for (size_t pos=0; pos<=end; pos++) {

			size_t start = pos, count = 0;
			m_text->getTagColor(start, count);

			// ���� ����, ���������� � �������
			if (count != 0) {
				const Ogre::DisplayString & text = m_text->getCaption();
				oldColor = text.substr(start, count);
				// ������� ������ ���, ���� �� ������
				if (pos >= _start) commandErase(start, count, &history);
			}

		}

		// ���� � ��������� ������ �����, �� ������
		if (oldColor.empty()) oldColor = m_text->getTagColor();

		// ������ ��� � ������
		size_t start = _start, count = 0;
		m_text->convertTextRange(start, count);
		commandInsert(m_text->getTagColor(_color), start, &history);

		// ������ ��� � �����
		start = _start + _count;
		count = 0;
		m_text->convertTextRange(start, count);
		commandInsert(oldColor, start, &history);

		// ��������� ������� ��� �������������� �������
		commandPosition(_start, _start+_count, &history);

		// ���������� � �������
		saveInHistory(&history);

		OUT(m_text->getCaption());
	}

	void Edit::insertText(const Ogre::DisplayString & _text, size_t _index)
	{
		// ���������� ���������
		resetSelect();

		// ���� ������ ������, �� �� ����
		if (_text.empty()) return;

		// ������� ���������
		VectorChangeInfo history;

		Ogre::DisplayString color;

		// � ������� ������ ���� ����������� ����� ���� � ������
		if ( (_text.size() > 6) && (_text[0] == '#') && (_text[1] != '#') ) {
			size_t start = _index, count = 0;
			m_text->getTagColor(start, count);
			if (count == 0) {
				// �������� �� ������ � ������� ��������� ��� ����� �����
				color = m_text->getTagColor();
				for (size_t pos=0; pos<_index; pos++) {

					start = pos;
					count = 0;
					m_text->getTagColor(start, count);

					// ���� ����, ����������
					if (count != 0) color = m_text->getCaption().substr(start, count);
				}
			}
		} // if ( (_text.size() > 6) && (_text[0] == '#') && (_text[1] != '#') ) {

		// ������ ������
		size_t start = _index, count = 0;
		m_text->convertTextRange(start, count);
		// ���������� ������� ������ ������
		count = mTextLenght;

		// ���� ���� �� ����
		if ( ! color.empty()) commandInsert(color, start, &history);
		// ���������� ������
		commandInsert(_text, start, &history);

		// ����� ������� � ��������� �� ����� �������
		mTextLenght = m_text->getTextLenght();
		mCursorPosition += mTextLenght - count;

		// ��������� ������� ��� �������������� �������
		commandPosition(_index, _index + mTextLenght - count, &history);

		// ���������� � �������
		saveInHistory(&history);

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);
	}

	void Edit::eraseText(size_t _start, size_t _count)
	{

		// ���� ��������
		if (_count == 0) return;
		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo history;

		// ���� � ����� ��� �����, �� ���� ��� ����� � ��������
		Ogre::DisplayString color;
		size_t start = _start + _count, count = 0;
		m_text->getTagColor(start, count);
		if (count == 0) {
			// �������� �� ������ � ������� ��������� ��� ����� �����
			size_t end = _start + _count;
			for (size_t pos=0; pos<end; pos++) {

				start = pos;
				count = 0;
				m_text->getTagColor(start, count);

				// ���� ����, ����������
				if (count != 0) color = m_text->getCaption().substr(start, count);
			}
		} else {
			// �����������, ���� ���� ���� � �� ����� �� ��� �� ��������, �� ���� � ��� �����
			size_t save_start = start, save_count = count;
			Ogre::DisplayString find_color = m_text->getTagColor();
			for (size_t pos=0; pos<_start; pos++) {

				start = pos;
				count = 0;
				m_text->getTagColor(start, count);

				// ���� ����, ����������
				if (count != 0) find_color = m_text->getCaption().substr(start, count);
			}
			// ���� ����� ����������, �� �������
			if (find_color == m_text->getCaption().substr(save_start, save_count)) {
				commandErase(save_start, save_count, &history);
			}
		}

		// ������ ������
		start = _start;
		count = _count;
		m_text->convertTextRange(start, count);

		// �������� ������
		commandErase(start, count, &history);

		// ���� ���� ���
		if ( ! color.empty()) commandInsert(color, start, &history);

		// �� ����� ����������
		mCursorPosition = _start;
		mTextLenght = m_text->getTextLenght();

		// ��������� ������� ��� �������������� �������
		commandPosition(_start + _count, _start, &history);

		// ���������� � �������
		saveInHistory(&history);

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);
	}

	void Edit::setCaption(const Ogre::DisplayString & _caption)
	{
		// ���������� ���������
		resetSelect();

		// ������� ���������
		VectorChangeInfo history;

		// �������� ������
		commandErase(0, m_text->getCaption().size(), &history);

		// ���������� ������
		commandInsert(_caption, 0, &history);

		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = mTextLenght = m_text->getTextLenght();

		// ��������� ������� ��� �������������� �������
		commandPosition(0, mTextLenght, &history);

		// ���������� � �������
		saveInHistory(&history);

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);
	}

	bool Edit::commandErase(size_t _start, size_t _count, VectorChangeInfo * _info)
	{
		if (_count == 0) return false;

		// ��� ���������� ����� ���������� ������
		commandResetRedo();

		Ogre::DisplayString text = m_text->getCaption();

		// ��������� � �������
		if (_info != null) 	_info->push_back(tagChangeInfo(text.substr(_start, _count), _start, COMMAND_ERASE));

		text.erase(_start, _count);
		m_text->setCaption(text);
		return true;
	}

	bool Edit::commandInsert(const Ogre::DisplayString & _insert, size_t _start, VectorChangeInfo * _info)
	{
		if (_insert.size() == 0) return false;
		// ����� ������� �� ��������
		if (EDIT_MAX_LENGHT == m_text->getTextLenght()) {
			return false;
		}

		// ��� ���������� ����� ���������� ������
		commandResetRedo();

		Ogre::DisplayString text = m_text->getCaption();

		// ��������� � �������
		if (_info != null) 	_info->push_back(tagChangeInfo(_insert, _start, COMMAND_INSERT));

		text.insert(_start, _insert);
		m_text->setCaption(text);

		// ��������� �� ������������ ������
		size_t lenght = m_text->getTextLenght();
		if (lenght > EDIT_MAX_LENGHT) {
			// ��������
			size_t start = EDIT_MAX_LENGHT, count = lenght - EDIT_MAX_LENGHT;
			m_text->convertTextRange(start, count);
			commandErase(start, count, _info);
		}

		return true;
	}

	void Edit::commandPosition(size_t _undo, size_t _redo, VectorChangeInfo * _info)
	{
		if (_info != null) 	_info->push_back(tagChangeInfo(_undo, _redo));
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

		Ogre::DisplayString text = m_text->getCaption();
		size_t position = ITEM_NONE;

		// ��������������� ������������������
		for (VectorChangeInfo::reverse_iterator iter=info.rbegin(); iter!=info.rend(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == COMMAND_ERASE) text.insert((*iter).start, (*iter).text);
			else position = (*iter).undo;

		}

		m_text->setCaption(text);

		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = (position != ITEM_NONE) ? position : 0;
		mTextLenght = m_text->getTextLenght();

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);

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

		Ogre::DisplayString text = m_text->getCaption();
		size_t position = ITEM_NONE;

		// ��������������� ������������������
		for (VectorChangeInfo::iterator iter=info.begin(); iter!=info.end(); iter++) {

			if ((*iter).type == COMMAND_INSERT) text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == COMMAND_ERASE) text.erase((*iter).start, (*iter).text.size());
			else position = (*iter).redo;

		}

		m_text->setCaption(text);

		// ����� ������� � ��������� �� ����� �������
		mCursorPosition = (position != ITEM_NONE) ? position : 0;
		mTextLenght = m_text->getTextLenght();

		// ��������� �� �������
		IntPoint point;
		m_text->getTextCursorFromPosition(point.left, point.top, mCursorPosition);
		updateCursor(point);

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

} // namespace MyGUI