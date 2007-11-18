#ifndef _EDIT_H_
#define _EDIT_H_

#include "Prerequest.h"
#include "Widget.h"
#include "TextChangeHistory.h"

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget, public Ogre::FrameListener
	{
		// ��� ������ ��������� ������������
		friend WidgetFactory<Edit>;

	protected:
		Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Edit"); return type;};

		// ������������� ����
		//void setCaption(const Ogre::DisplayString & _caption);
		// ���������� ���������
		inline void getTextSelect(size_t & _start, size_t & _end)
		{
			if (mStartSelect == SIZE_MAX) {_start=SIZE_MAX; _end=SIZE_MAX;}
			else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
			else {_start = mStartSelect; _end = mEndSelect;}
		};
		// ������� �� �����
		inline bool isTextSelect() {return ( (mStartSelect != SIZE_MAX) && (mStartSelect != mEndSelect) );}
		// ���������� ���������� �����
		Ogre::DisplayString getSelectedText()
		{
			if ( false == isTextSelect()) return "";
			size_t start, end;
			getTextSelect(start, end);
			return getText(start, end-start);
		}
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

		// ������������� �����
		inline void setText(const Ogre::DisplayString & _text) {setText(_text, false);}
		// ������� ��� ��� ���������
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// ��������� ����� � ��������� �������
		inline void insertText(const Ogre::DisplayString & _text, size_t _index = SIZE_MAX) {insertText(_text, _index, false);}
		// ��������� ����� � �����
		inline void addText(const Ogre::DisplayString & _text) {addText(_text, false);}
		// ������� �����
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// �������� ������ ���������
		inline void setTextSelectColor(const Ogre::ColourValue & _color) {setTextSelectColor(_color, false);}
		// �������� ������ ��������
		inline void setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _color) {setTextColor(_start, _count, _color, false);}


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
		void setTextSelectColor(const Ogre::ColourValue & _color, bool _history);
		// �������� ������ ��������
		void setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _color, bool _history);

	protected:

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseMove(MyGUI::WidgetPtr _sender, int _x, int _y);

		virtual void _onMouseLostFocus(WidgetPtr _new);
		virtual void _onMouseSetFocus(WidgetPtr _old);
		virtual void _onMouseMove(int _x, int _y);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, wchar_t _char);
		virtual void _onKeyButtonReleased(int _key);

		inline void updateEditState()
		{
			if (m_isFocus) {
				if (m_isPressed) setState("select");
				else setState("active");
			} else {
				if (m_isPressed) setState("pressed");
				else setState("normal");
			}
		}

		// ��������� ������ �� ����������
		void updateCursor(IntPoint _point);
		// ����� �� ������ � ���� ���������
		inline bool isShowCursorInView();
		// ���������� �������������
		void updateView(bool _showCursor);

		inline IntPoint getWorldPostion(WidgetPtr _widget)
		{
			IntPoint point(_widget->left(), _widget->top());
			WidgetPtr parent = _widget->getParent();
			while (parent != null) {
				point.left += parent->left();
				point.top += parent->top();
				parent = parent->getParent();
			};
			return point;
		}

		inline void resetSelect()
		{
			if (mStartSelect != SIZE_MAX) {
				mStartSelect = SIZE_MAX;
				m_text->setTextSelect(0, 0);
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


	private:
		// ������ �� ������
		bool m_isPressed;
		// � ������ �� ������
		bool m_isFocus;

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
		Ogre::DisplayString mClipboard;

		// ������� ��������� ��� ������ � �������
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		// ������
		IntSize mSizeView;
		int mHalfWidthCursor;
		int mHalfHeightCursor;

		bool mMouseLeftPressed;

	}; // class Edit : public Widget

	typedef Edit * EditPtr;

} // namespace MyGUI

#endif
