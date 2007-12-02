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
		// для вызова закрытого конструктора
		friend factory::EditFactory;

	protected:
		Edit(int _left, int _top, int _width, int _height, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name);
		~Edit();

	public:

		// устанавливает текст
		void setCaption(const Ogre::DisplayString & _caption);
		// возвращает выделение
		void getTextSelect(size_t & _start, size_t & _end);
		// выделен ли текст
		inline bool isTextSelect() {return ( (mStartSelect != SIZE_MAX) && (mStartSelect != mEndSelect) );}
		// возвращает выделенный текст
		Ogre::DisplayString getSelectedText();

		inline size_t getTextLenght() {return mTextLenght;}
		inline size_t getTextCursor() {return mCursorPosition;}

		void setTextCursor(size_t _index);
		// устанавливает выделение
		void setTextSelect(size_t _start, size_t _end);
		// возвращает текст
		Ogre::DisplayString getText(size_t _start, size_t _count);

		//---------------------------------------------------------------//
		// наружу выставляем инлайны со сбросом истории
		//---------------------------------------------------------------//

		// удаляет все что выделенно
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// вставляет текст в указанную позицию
		inline void insertText(const Ogre::DisplayString & _text, size_t _index = SIZE_MAX) {insertText(_text, _index, false);}
		// добавляет текст в конец
		inline void addText(const Ogre::DisplayString & _text) {addText(_text, false);}
		// удаляет текст
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// выделяет цветом выделение
		inline void setTextSelectColor(const Ogre::ColourValue & _colour) {setTextSelectColor(_colour, false);}
		// выделяет цветом диапазон
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

		// устанавливает текст
		void setText(const Ogre::DisplayString & _text, bool _history);
		// удаляет все что выделенно
		bool deleteTextSelect(bool _history);
		// вставляет текст в указанную позицию
		void insertText(const Ogre::DisplayString & _text, size_t _index, bool _history);
		// добавляет текст в конец
		inline void addText(const Ogre::DisplayString & _text, bool _history) {insertText(_text, SIZE_MAX, _history);}
		// удаляет текст
		void eraseText(size_t _start, size_t _count, bool _history);
		// выделяет цветом выделение
		void setTextSelectColor(const Ogre::ColourValue & _colour, bool _history);
		// выделяет цветом диапазон
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

		// обновляет курсор по координате
		void updateCursor(IntPoint _point);
		// виден ли курсор в окне просмотра
		bool isShowCursorInView();
		// обновление представления
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

		// запись в историю данных о позиции
		void commandPosition(size_t _undo, size_t _redo, size_t _lenght, VectorChangeInfo * _info = null);

		// команнды отмена и повтор
		bool commandRedo();
		bool commandUndo();
		// объединяет последние две комманды
		void commandMerge();
		// очистка
		inline void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		inline void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = null);

		// работа с буфером обмена
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
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;

		WidgetPtr mWidgetUpper;
		WidgetPtr mWidgetCursor;

		bool mCursorActive;
		float mCursorTimer, mActionMouseTimer;

		// позиция курсора
		size_t mCursorPosition;
		// максимаотное колличество
		size_t mTextLenght;
		// выделение
		bool mShiftPressed;
		size_t mStartSelect, mEndSelect;

		bool mCtrlPressed;

		// списоки изменений для отмены и повтора
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		// размер
		IntSize mSizeView;
		int mHalfWidthCursor;
		int mHalfHeightCursor;

		bool mMouseLeftPressed;

		bool mReadOnly;
		bool mPassword;
		bool mMultiLine;

		// настоящий текст, закрытый за звездочками
		Ogre::DisplayString mPasswordText;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
