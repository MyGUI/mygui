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
#include "MyGUI_FrameListener.h"

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget, public FrameListener
	{
		// для вызова закрытого конструктора
		friend class factory::EditFactory;

	protected:
		Edit(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Edit();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		/** Set edit text applying tags */
		virtual void setCaption(const Ogre::UTFString & _caption);
		/** Get edit text with tags */
		virtual const Ogre::UTFString& getCaption();

		/** Set edit text without tags */
		inline void setOnlyText(const Ogre::UTFString & _text) {setText(TextIterator::toTagsString(_text), false);}
		/** Get edit text without tags */
		inline Ogre::UTFString getOnlyText() {return TextIterator::getOnlyText(getRealString());}

		// возвращает выделение
		/** Get selected text interval
			@param _start of interval will be written here
			@param _end of interval will be written here
		*/
		void getTextSelect(size_t & _start, size_t & _end);
		// устанавливает выделение
		/** Set selected text interval
			@param _start of interval
			@param _end of interval
		*/
		void setTextSelect(size_t _start, size_t _end);
		// выделен ли текст
		/** Is any text selected */
		inline bool isTextSelect() {return ( (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect) );}
		// возвращает выделенный текст
		/** Get selected text */
		Ogre::UTFString getSelectedText();

		/** Get text length excluding tags
			@example "#00FF00Hello" length is 5
		*/
		inline size_t getTextLength() {return mTextLength;}
		/** Get text cursor position */
		inline size_t getTextCursor() {return mCursorPosition;}

		/** Set text cursor position */
		void setTextCursor(size_t _index);

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

		//---------------------------------------------------------------//
		// наружу выставляем инлайны со сбросом истории
		//---------------------------------------------------------------//

		// возвращает текст с тегами
		/** Get _count characters with tags from _start position */
		Ogre::UTFString getText(size_t _start, size_t _count);
		// удаляет все что выделенно
		/** Delete selected text */
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// вставляет текст в указанную позицию
		/** Inser text at _index position (text end by default) */
		inline void insertText(const Ogre::UTFString & _text, size_t _index = ITEM_NONE) {insertText(_text, _index, false);}
		// добавляет текст в конец
		/** Add text */
		inline void addText(const Ogre::UTFString & _text) {insertText(_text, ITEM_NONE, false);}
		/** Erase _count characters from _start position */
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// выделяет цветом выделение
		/** Colour selected text */
		inline void setTextSelectColour(const Ogre::ColourValue & _colour)
		{
			setTextSelectColour(_colour, false);
		}
		// выделяет цветом диапазон
		/** Colour interval */
		inline void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour)
		{
			setTextColour(_start, _count, _colour, false);
		}

		/** Get edit read only mode flag */
		inline bool getEditReadOnly() {return mModeReadOnly;}
		/** Enable or disable edit read only mode */
		inline void setEditReadOnly(bool _read)
		{
			mModeReadOnly = _read;
			// сбрасываем историю
			commandResetHistory();
		}

		/** Get edit password mode flag */
		inline bool getEditPassword() {return mModePassword;}
		/** Enable or disable edit password mode */
		void setEditPassword(bool _password);

		/** Get edit multiline mode flag */
		inline bool getEditMultiLine() {return mModeMultiline;}
		/** Enable or disable edit multiline mode */
		inline void setEditMultiLine(bool _multi)
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
		inline void setEditStatic(bool _static)
		{
			mModeStatic = _static;
			resetSelect();
			if (mModeStatic) mWidgetUpper->setPointer("");
			else mWidgetUpper->setPointer(mOriginalPointer);
		}

		/** Get edit static mode flag */
		inline bool getEditStatic() {return mModeStatic;}

		/** Set edit password character ('*' by default) */
		void setPasswordChar(Char _char);
		/** Set edit password character ('*' by default) */
		inline void setPasswordChar(const Ogre::UTFString & _char)
		{
			if (false == _char.empty()) setPasswordChar(_char[0]);
		}
		/** Get edit password character */
		inline Char getPasswordChar() {return mCharPassword;}

		//! @copydoc Widget::setPosition(const IntCoord& _coord)
		void setPosition(const IntCoord& _coord);
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size);
		//! @copydoc Widget::setPosition(int _left, int _top)
		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		//! @copydoc Widget::setPosition(int _left, int _top, int _width, int _height)
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		//! @copydoc Widget::setSize(int _width, int _height)
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		/** Event : Enter pressed (Ctrl+entre in multiline mode).\n
			signature : void method(WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventEditSelectAccept;

		/** Event : Text changed.\n
			signature : void method(WidgetPtr _sender)
		*/
		EventInfo_WidgetVoid eventEditTextChange;

	protected:

		// устанавливает текст
		void setText(const Ogre::UTFString & _text, bool _history);
		// удаляет все что выделенно
		bool deleteTextSelect(bool _history);
		// вставляет текст в указанную позицию
		void insertText(const Ogre::UTFString & _text, size_t _index, bool _history);
		// удаляет текст
		void eraseText(size_t _start, size_t _count, bool _history);
		// выделяет цветом выделение
		void setTextSelectColour(const Ogre::ColourValue & _colour, bool _history);
		// выделяет цветом диапазон
		void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history);

	protected:
		virtual void _frameEntered(float _frame);

		// потом убрать все нотифи в сраку
		void notifyMouseSetFocus(WidgetPtr _sender, WidgetPtr _old);
		void notifyMouseLostFocus(WidgetPtr _sender, WidgetPtr _new);
		void notifyMousePressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);

		virtual void _onMouseDrag(int _left, int _top);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(KeyCode _key, Char _char);
		virtual void _onKeyButtonReleased(KeyCode _key);

		void updateEditState();

		// обновляет курсор по координате
		void updateSelectText();
		// обновление представления
		void updateView(bool _showCursor);

		// переделать, так как теперь нам все известно о глобальных координатах
		/*inline IntPoint getWorldPostion(WidgetPtr _widget)
		{
			IntPoint point(_widget->getLeft(), _widget->getTop());
			WidgetPtr parent = _widget->getParent();
			while (parent != null) {
				point.left += parent->getLeft();
				point.top += parent->getTop();
				parent = parent->getParent();
			};
			return point;
		}*/

		void resetSelect();

		// запись в историю данных о позиции
		void commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo * _info = null);

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


		const Ogre::UTFString & getRealString();

		void setRealString(const Ogre::UTFString & _caption);


	protected:
		// нажата ли кнопка
		bool mIsPressed;
		// в фокусе ли кнопка
		bool mIsFocus;

		WidgetPtr mWidgetUpper;

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

		// настоящий текст, закрытый за звездочками
		Ogre::UTFString mPasswordText;

		// для поддержки режима статик, где курсор не нужен
		std::string mOriginalPointer;

		Char mCharPassword;

		bool mOverflowToTheLeft;
		size_t mMaxTextLength;

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
