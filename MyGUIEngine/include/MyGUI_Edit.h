/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_H__
#define __MYGUI_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_EditFactory.h"
#include "MyGUI_Widget.h"
#include "MyGUI_TextChangeHistory.h"

namespace MyGUI
{

	class Edit;
	typedef Edit * EditPtr;

	class _MyGUIExport Edit : public Widget
	{
		// ��� ������ ��������� ������������
		friend factory::EditFactory;

	protected:
		Edit(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		~Edit();

	public:

		// ������������� �����
		void setCaption(const Ogre::DisplayString & _caption);
		// ���������� ���������
		void getTextSelect(size_t & _start, size_t & _end);
		// ������� �� �����
		inline bool isTextSelect() {return ( (mStartSelect != SIZE_MAX) && (mStartSelect != mEndSelect) );}
		// ���������� ���������� �����
		Ogre::DisplayString getSelectedText();

		inline size_t getTextLenght() {return mTextLenght;}
		inline size_t getTextCursor() {return mCursorPosition;}

		void setTextCursor(size_t _index);
		// ������������� ���������
		void setTextSelect(size_t _start, size_t _end);
		// ���������� �����
		Ogre::DisplayString getText(size_t _start, size_t _count);

		//---------------------------------------------------------------//
		// ������ ���������� ������� �� ������� �������
		//---------------------------------------------------------------//

		// ������� ��� ��� ���������
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// ��������� ����� � ��������� �������
		inline void insertText(const Ogre::DisplayString & _text, size_t _index = SIZE_MAX) {insertText(_text, _index, false);}
		// ��������� ����� � �����
		inline void addText(const Ogre::DisplayString & _text) {addText(_text, false);}
		// ������� �����
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// �������� ������ ���������
		inline void setTextSelectColor(const Ogre::ColourValue & _colour) {setTextSelectColor(_colour, false);}
		// �������� ������ ��������
		inline void setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _colour) {setTextColor(_start, _count, _colour, false);}

		inline bool getEditReadOnly() {return mReadOnly;}
		inline void setEditReadOnly(bool _read) {mReadOnly=_read;commandResetHistory();}

		inline bool getEditPassword() {return mPassword;}
		inline void setEditPassword(bool _password)
		{
			if (mPassword == _password) return;
			mPassword = _password;
			if (mPassword) {
				mPasswordText = mText->getCaption();
				mText->setCaption(Ogre::DisplayString(mTextLenght, '*'));
			}
			else {
				mText->setCaption(mPasswordText);
				mPasswordText.clear();
			}
		}

		inline bool getEditMultiLine() {return mMultiLine;}
		inline void setEditMultiLine(bool _multi) {mMultiLine = _multi;}

	protected:

		// ������������� �����
		void setText(const Ogre::DisplayString & _text, bool _history);
		// ������� ��� ��� ���������
		bool deleteTextSelect(bool _history);
		// ��������� ����� � ��������� �������
		void insertText(const Ogre::DisplayString & _text, size_t _index, bool _history);
		// ��������� ����� � �����
		inline void addText(const Ogre::DisplayString & _text, bool _history) {insertText(_text, SIZE_MAX, _history);}
		// ������� �����
		void eraseText(size_t _start, size_t _count, bool _history);
		// �������� ������ ���������
		void setTextSelectColor(const Ogre::ColourValue & _colour, bool _history);
		// �������� ������ ��������
		void setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history);

	protected:
		void _frameStarted(float _frame, float _event);

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top);

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseMove(int _left, int _top);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, wchar_t _char);
		virtual void _onKeyButtonReleased(int _key);

		inline void updateEditState()
		{
			if (mIsFocus) {
				if (mIsPressed) setState("select");
				else setState("active");
			} else {
				if (mIsPressed) setState("pressed");
				else setState("normal");
			}
		}

		// ��������� ������ �� ����������
		void updateCursor(IntPoint _point);
		// ����� �� ������ � ���� ���������
		bool isShowCursorInView();
		// ���������� �������������
		void updateView(bool _showCursor);

		inline IntPoint getWorldPostion(WidgetPtr _widget)
		{
			IntPoint point(_widget->getLeft(), _widget->getTop());
			WidgetPtr parent = _widget->getParent();
			while (parent != null) {
				point.left += parent->getLeft();
				point.top += parent->getTop();
				parent = parent->getParent();
			};
			return point;
		}

		inline void resetSelect()
		{
			if (mStartSelect != SIZE_MAX) {
				mStartSelect = SIZE_MAX;
				mText->setTextSelect(0, 0);
			}
		}

		// ������ � ������� ������ � �������
		void commandPosition(size_t _undo, size_t _redo, size_t _lenght, VectorChangeInfo * _info = null);

		// �������� ������ � ������
		bool commandRedo();
		bool commandUndo();
		// ���������� ��������� ��� ��������
		void commandMerge();
		// �������
		inline void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		inline void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = null);

		// ������ � ������� ������
		void commandCut();
		void commandCopy();
		void commandPast();


		inline const Ogre::DisplayString & getRealString()
		{
			if (mPassword) return mPasswordText;
			return mText->getCaption();
		}

		inline void setRealString(const Ogre::DisplayString & _caption)
		{
			if (mPassword) {
				mPasswordText = _caption;
				mText->setCaption(Ogre::DisplayString(mTextLenght, '*'));
			}
			else {
				mText->setCaption(_caption);
			}
		}


	protected:
		// ������ �� ������
		bool mIsPressed;
		// � ������ �� ������
		bool mIsFocus;

		WidgetPtr mWidgetUpper;
		WidgetPtr mWidgetCursor;

		bool mCursorActive;
		float mCursorTimer, mActionMouseTimer;

		// ������� �������
		size_t mCursorPosition;
		// ������������ �����������
		size_t mTextLenght;
		// ���������
		bool mShiftPressed;
		size_t mStartSelect, mEndSelect;

		bool mCtrlPressed;

		// ������� ��������� ��� ������ � �������
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		// ������
		IntSize mSizeView;
		int mHalfWidthCursor;
		int mHalfHeightCursor;

		bool mMouseLeftPressed;

		bool mReadOnly;
		bool mPassword;
		bool mMultiLine;

		// ��������� �����, �������� �� �����������
		Ogre::DisplayString mPasswordText;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
