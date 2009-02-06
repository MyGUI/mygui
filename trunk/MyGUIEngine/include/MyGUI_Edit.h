/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_H__
#define __MYGUI_EDIT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_TextIterator.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CDelegate1<EditPtr> EventHandle_EditPtr;

	class MYGUI_EXPORT Edit : public Widget
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<Edit>;

		MYGUI_RTTI_CHILD_HEADER( Edit, Widget );

	public:
		/** Colour interval */
		void setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour) { _setTextColour(_start, _count, _colour, false); }

		MYGUI_OBSOLETE("use : void setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour)")
		void setTextColour(size_t _start, size_t _count, const Colour& _colour) { setTextIntervalColour(_start, _count, _colour); }

		/** Get index of first selected character or ITEM_NONE if nothing selected */
		size_t getTextSelectionStart() { return (mStartSelect == ITEM_NONE) ? ITEM_NONE : (mStartSelect > mEndSelect ? mEndSelect : mStartSelect); }

		/** Get index of last selected character or ITEM_NONE if nothing selected */
		size_t getTextSelectionEnd() { return (mStartSelect == ITEM_NONE) ? ITEM_NONE : (mStartSelect > mEndSelect ? mStartSelect : mEndSelect); }

		MYGUI_OBSOLETE("use : size_t getTextSelectionStart() , size_t getTextSelectionEnd()")
		void getTextSelect(size_t & _start, size_t & _end);

		// возвращает длинну выделения !!! ПРОВЕРИТЬ
		/** Get length of selected text */
		size_t getTextSelectionLength() { return mEndSelect - mStartSelect; }

		// возвращает текст с тегами
		/** Get _count characters with tags from _start position */
		Ogre::UTFString getTextInterval(size_t _start, size_t _count);
		MYGUI_OBSOLETE("use : Ogre::UTFString getTextInterval(size_t _start, size_t _count)")
		Ogre::UTFString getText(size_t _start, size_t _count) { return getTextInterval(_start, _count); }

		/** Set selected text interval
			@param _start of interval
			@param _end of interval
		*/
		void setTextSelection(size_t _start, size_t _end);
		MYGUI_OBSOLETE("use : void setTextSelection(size_t _start, size_t _end)")
		void setTextSelect(size_t _start, size_t _end) { setTextSelection(_start, _end); }

		/** Delete selected text */
		void deleteTextSelection() { deleteTextSelect(false); }
		MYGUI_OBSOLETE("use : void deleteTextSelection()")
		void deleteTextSelect() { deleteTextSelection(); }

		/** Get selected text */
		Ogre::UTFString getTextSelection();
		MYGUI_OBSOLETE("use : Ogre::UTFString getTextSelection()")
		Ogre::UTFString getSelectedText() { return getTextSelection(); }

		/** Is any text selected */
		bool isTextSelection() { return ( (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect) ); }
		MYGUI_OBSOLETE("use : bool isTextSelection()")
		bool isTextSelect() { return isTextSelection(); }


		/** Colour selected text */
		void setTextSelectionColour(const Colour& _colour) { setTextSelectColour(_colour, false); }
		MYGUI_OBSOLETE("use : void setTextSelectionColour(const Colour& _colour)")
		void setTextSelectColour(const Colour& _colour) { setTextSelectionColour(_colour); }


		/** Set text cursor position */
		void setTextCursor(size_t index);
		/** Get text cursor position */
		size_t getTextCursor() { return mCursorPosition; }


		/** Set edit text applying tags */
		virtual void setCaption(const Ogre::UTFString & _caption);
		/** Get edit text with tags */
		virtual const Ogre::UTFString& getCaption();

		/** Set edit text without tags */
		void setOnlyText(const Ogre::UTFString & _text) { setText(TextIterator::toTagsString(_text), false); }
		/** Get edit text without tags */
		Ogre::UTFString getOnlyText() { return TextIterator::getOnlyText(getRealString()); }

		/** Get text length excluding tags
			For example "#00FF00Hello" length is 5
		*/
		size_t getTextLength() { return mTextLength; }

		//! @copydoc Widget::setTextAlign
		virtual void setTextAlign(Align _align);

		//! Sets if surplus characters should push characters off the left side rather than ignored
		void setOverflowToTheLeft(bool _overflowToLeft) { mOverflowToTheLeft = _overflowToLeft; }

		//! Returns true if surplus characters will be pushed off the left rather than ignored
		bool getOverflowToTheLeft() { return mOverflowToTheLeft; }

		//! Sets the max amount of text allowed in the edit field.
		void setMaxTextLength(size_t _maxTextLength) { mMaxTextLength = _maxTextLength; }

		//! Gets the max amount of text allowed in the edit field.
		size_t getMaxTextLength() { return mMaxTextLength; }

		// вставляет текст в указанную позицию
		/** Inser text at _index position (text end by default) */
		void insertText(const Ogre::UTFString & _text, size_t _index = ITEM_NONE) { insertText(_text, _index, false); }
		// добавляет текст в конец
		/** Add text */
		void addText(const Ogre::UTFString & _text) { insertText(_text, ITEM_NONE, false); }
		/** Erase _count characters from _start position */
		void eraseText(size_t _start, size_t _count = 1) { eraseText(_start, _count, false); }

		//! @copydoc Widget::setTextColour(const Colour& _colour)
		void setTextColour(const Colour& _colour) { Widget::setTextColour(_colour); }

		/** Get edit read only mode flag */
		bool getEditReadOnly() { return mModeReadOnly; }
		/** Enable or disable edit read only mode */
		void setEditReadOnly(bool _read)
		{
			mModeReadOnly = _read;
			// сбрасываем историю
			commandResetHistory();
		}

		/** Get edit password mode flag */
		bool getEditPassword() { return mModePassword; }
		/** Enable or disable edit password mode */
		void setEditPassword(bool _password);

		/** Get edit multiline mode flag */
		bool getEditMultiLine() { return mModeMultiline; }
		/** Enable or disable edit multiline mode */
		void setEditMultiLine(bool _multi)
		{
			mModeMultiline = _multi;
			// на всякий, для уберания переносов
			if (false == mModeMultiline) {
				setText(getRealString(), false);
			}
			// обновляем по размерам
			else updateView(false);
			// сбрасываем историю
			commandResetHistory();
		}

		/** Enable or disable edit static mode */
		void setEditStatic(bool _static)
		{
			mModeStatic = _static;
			resetSelect();
			if (mModeStatic) mWidgetClient->setPointer("");
			else mWidgetClient->setPointer(mOriginalPointer);
		}

		/** Get edit static mode flag */
		bool getEditStatic() { return mModeStatic; }

		/** Set edit password character ('*' by default) */
		void setPasswordChar(Char _char);
		/** Set edit password character ('*' by default). First character of string used. */
		void setPasswordChar(const Ogre::UTFString & _char)
		{
			if (false == _char.empty()) setPasswordChar(_char[0]);
		}
		/** Get edit password character */
		Char getPasswordChar() { return mCharPassword; }

		/** Get edit word wrap mode flag */
		bool getEditWordWrap() { return mModeWordWrap; }
		/** Enable or disable edit word wrap mode */
		void setEditWordWrap(bool _wordwrap);

		/** Get edit tab printing wrap mode flag */
		bool getTabPrinting() { return mTabPrinting; }
		/** Enable or disable tab printing mode */
		void setTabPrinting(bool _print) { mTabPrinting = _print; }


		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Show VScroll when text size larger than Edit */
		void setVisibleVScroll(bool _visible) { mShowVScroll = _visible; updateView(false); }
		/** Get Show VScroll flag */
		bool isVisibleVScroll() { return mShowVScroll; }

		/** Show HScroll when text size larger than Edit */
		void setVisibleHScroll(bool _visible) { mShowHScroll = _visible; updateView(false); }
		/** Get Show HScroll flag */
		bool isVisibleHScroll() { return mShowHScroll; }

		/** Set widget text font */
		virtual void setFontName(const std::string & _font);
		/** Set widget text font height */
		virtual void setFontHeight(uint _height);


	/*event:*/
		/** Event : Enter pressed (Ctrl+enter in multiline mode).\n
			signature : void method(MyGUI::EditPtr _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_EditPtr> eventEditSelectAccept;

		/** Event : Text changed.\n
			signature : void method(MyGUI::EditPtr _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_EditPtr> eventEditTextChange;


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Edit::setVisibleVScroll(bool _visible)")
		void showVScroll(bool _visible) { setVisibleVScroll(_visible); }
		MYGUI_OBSOLETE("use : bool Edit::isVisibleVScroll()")
		bool isShowVScroll() { return isVisibleVScroll(); }
		MYGUI_OBSOLETE("use : void Edit::setVisibleHScroll(bool _visible)")
		void showHScroll(bool _visible) { setVisibleHScroll(_visible); }
		MYGUI_OBSOLETE("use : bool Edit::isVisibleHScroll()")
		bool isShowHScroll() { return isVisibleHScroll(); }

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord & _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

#endif // MYGUI_DONT_USE_OBSOLETE


	protected:
		Edit(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~Edit();

		virtual void onMouseDrag(int _left, int _top);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);
		virtual void onKeyButtonPressed(KeyCode _key, Char _char);

		// потом убрать все нотифи в сраку
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);

		void notifyScrollChangePosition(VScrollPtr _sender, size_t _position);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);

		// обновление представления
		void updateView(bool _showCursor);

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

	private:
		// устанавливает текст
		void setText(const Ogre::UTFString & _text, bool _history);
		// удаляет все что выделенно
		bool deleteTextSelect(bool _history);
		// вставляет текст в указанную позицию
		void insertText(const Ogre::UTFString & _text, size_t _index, bool _history);
		// удаляет текст
		void eraseText(size_t _start, size_t _count, bool _history);
		// выделяет цветом выделение
		void setTextSelectColour(const Colour& _colour, bool _history);
		// выделяет цветом диапазон
		void _setTextColour(size_t _start, size_t _count, const Colour& _colour, bool _history);

		void frameEntered(float _frame);

		void updateEditState();

		// обновляет курсор по координате
		void updateSelectText();

		void resetSelect();

		// запись в историю данных о позиции
		void commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo * _info = nullptr);

		// команнды отмена и повтор
		bool commandRedo();
		bool commandUndo();
		// объединяет последние две комманды
		void commandMerge();
		// очистка
		void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = nullptr);

		// работа с буфером обмена
		void commandCut();
		void commandCopy();
		void commandPast();


		const Ogre::UTFString & getRealString();

		void setRealString(const Ogre::UTFString & _caption);

		void updateScroll();


	protected:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;

		//WidgetPtr mWidgetUpper;

		bool mCursorActive;
		float mCursorTimer, mActionMouseTimer;

		// позиция курсора
		size_t mCursorPosition;
		// максимальное колличество
		size_t mTextLength;

		// выделение
		size_t mStartSelect, mEndSelect;

		// списоки изменений для отмены и повтора
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		bool mMouseLeftPressed;

		bool mModeReadOnly;
		bool mModePassword;
		bool mModeMultiline;
		bool mModeStatic;
		bool mModeWordWrap;

		bool mTabPrinting;

		// настоящий текст, закрытый за звездочками
		Ogre::UTFString mPasswordText;

		// для поддержки режима статик, где курсор не нужен
		std::string mOriginalPointer;

		Char mCharPassword;

		bool mOverflowToTheLeft;
		size_t mMaxTextLength;

		VScrollPtr mVScroll;
		HScrollPtr mHScroll;

		bool mShowHScroll;
		bool mShowVScroll;

		size_t mVRange;
		size_t mHRange;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
