/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_EDIT_BOX_H_
#define MYGUI_EDIT_BOX_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextBox.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_TextIterator.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	using EventHandle_EditPtr = delegates::MultiDelegate<EditBox*>;

	/** \brief @wpage{EditBox}
		EditBox widget description should be here.
	*/
	class MYGUI_EXPORT EditBox : public TextBox, public ScrollViewBase, public MemberObsolete<EditBox>
	{
		MYGUI_RTTI_DERIVED(EditBox)

	public:
		EditBox();

		/** Colour interval */
		void setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour);

		/** Get index of first selected character or ITEM_NONE if nothing selected */
		size_t getTextSelectionStart() const;

		/** Get index of last selected character or ITEM_NONE if nothing selected */
		size_t getTextSelectionEnd() const;

		/** Get length of selected text */
		size_t getTextSelectionLength() const;

		// возвращает текст с тегами
		/** Get _count characters with tags from _start position */
		UString getTextInterval(size_t _start, size_t _count) const;

		/** Set selected text interval
			@param _start of interval
			@param _end of interval
		*/
		void setTextSelection(size_t _start, size_t _end);

		/** Delete selected text */
		void deleteTextSelection();

		/** Get selected text */
		UString getTextSelection() const;

		/** Is any text selected */
		bool isTextSelection() const;

		/** Colour selected text */
		void setTextSelectionColour(const Colour& _colour);

		/** Set text cursor position */
		void setTextCursor(size_t _index);
		/** Get text cursor position */
		size_t getTextCursor() const;


		/** Set edit text applying tags */
		void setCaption(const UString& _value) override;
		/** Get edit text with tags */
		const UString& getCaption() const override;

		/** Set edit text without tags */
		void setOnlyText(const UString& _text);
		/** Get edit text without tags */
		UString getOnlyText() const;

		/** Get text length excluding tags
			For example "Hello" length is 5
			and "#00FF00Hello!" length is 6
		*/
		size_t getTextLength() const;

		//! Sets if surplus characters should push characters off the left side rather than ignored
		void setOverflowToTheLeft(bool _value);
		//! Returns true if surplus characters will be pushed off the left rather than ignored
		bool getOverflowToTheLeft() const;

		//! Sets the max amount of text allowed in the edit field.
		void setMaxTextLength(size_t _value);
		//! Gets the max amount of text allowed in the edit field.
		size_t getMaxTextLength() const;

		/** Inser text at _index position (text end by default) */
		void insertText(const UString& _text, size_t _index = ITEM_NONE);
		/** Add text */
		void addText(const UString& _text);
		/** Erase _count characters from _start position */
		void eraseText(size_t _start, size_t _count = 1);

		/** Enable or disable edit read only mode\n
			Read only mode: you can't edit text, but can select it.\n
			Disabled (false) by default.
		*/
		void setEditReadOnly(bool _value);
		/** Get edit read only mode flag */
		bool getEditReadOnly() const;

		/** Enable or disable edit password mode\n
			Password mode: you see password chars (*** by default) instead text.\n
			Disabled (false) by default.
		*/
		void setEditPassword(bool _password);
		/** Get edit password mode flag */
		bool getEditPassword() const;

		/** Enable or disable edit multiline mode\n
			Multile mode: new line character moves text to new line.\n
			Otherwise new lines replaced with space and all text is in single line.\n
			Disabled (false) by default.
		*/
		void setEditMultiLine(bool _value);
		/** Get edit multiline mode flag */
		bool getEditMultiLine() const;

		/** Enable or disable edit static mode\n
			Static mode is same as read only, but you also can't select text.\n
			Disabled (false) by default.
		*/
		void setEditStatic(bool _value);
		/** Get edit static mode flag */
		bool getEditStatic() const;

		/** Set edit password character ('*' by default) */
		void setPasswordChar(Char _char);
		/** Set edit password character ('*' by default). First character of string used. */
		void setPasswordChar(const UString& _value);
		/** Get edit password character */
		Char getPasswordChar() const;

		/** Enable or disable edit word wrap mode\n
			Word Wrap mode: move words to new line if they goes out of width.
			Also in this mode you can't edit or select text.\n
			Disabled (false) by default.
		*/
		void setEditWordWrap(bool _value);
		/** Get edit word wrap mode flag */
		bool getEditWordWrap() const;

		/** Enable or disable tab printing mode\n
			Tab printing mode: when editing text and pressing Tab key it displayed.
			If this mode disabled Tab key ignored.\n
			Disabled (false) by default.
		*/
		void setTabPrinting(bool _value);
		/** Get edit tab printing wrap mode flag */
		bool getTabPrinting() const;

		/** Get invert selected text color property */
		bool getInvertSelected() const;
		/** Enable or disable inverting color of selected text\n
			Enabled (true) by default
		*/
		void setInvertSelected(bool _value);

		//! @copydoc Widget::setPosition(const IntPoint& _point)
		void setPosition(const IntPoint& _point) override;
		//! @copydoc Widget::setSize(const IntSize& _size)
		void setSize(const IntSize& _size) override;
		//! @copydoc Widget::setCoord(const IntCoord& _coord)
		void setCoord(const IntCoord& _coord) override;

		using Widget::setPosition;
		using Widget::setSize;
		using Widget::setCoord;

		/** Show VScroll when text size larger than EditBox */
		void setVisibleVScroll(bool _value);
		/** Get Show VScroll flag */
		bool isVisibleVScroll() const;
		/** Get range of vertical scroll (or 0 if no scroll).
			Range measured in pixels (full text heiht minus EditBox height).
			For example if EditBox is 200 pixels height and 40 lines of text
			30 pixels height each (i.e. 600 pixels total), then return
			value is 400 ( = 600 - 200 ).
		*/
		size_t getVScrollRange() const;
		/** Get current position of vertical scroll (or 0 if no scroll) */
		size_t getVScrollPosition() const;
		/** Set current position of vertical scroll */
		void setVScrollPosition(size_t _index);

		/** Show HScroll when text size larger than EditBox */
		void setVisibleHScroll(bool _value);
		/** Get Show HScroll flag */
		bool isVisibleHScroll() const;
		/** Get range of horizontal scroll (or 0 if no scroll).
			Range measured in pixels (full text width minus EditBox width).
			For example if EditBox is 200 pixels width and the longest line
			is 600 pixels width, then return value is 400 ( = 600 - 200 ).
		*/
		size_t getHScrollRange() const;
		/** Get current position of horizontal scroll (or 0 if no scroll) */
		size_t getHScrollPosition() const;
		/** Set current position of horizontal scroll */
		void setHScrollPosition(size_t _index);


		//! @copydoc TextBox::setFontName
		void setFontName(std::string_view _value) override;
		//! @copydoc TextBox::setFontHeight
		void setFontHeight(int _value) override;
		//! @copydoc TextBox::getFontHeight
		int getFontHeight() const override;

		//! @copydoc TextBox::setTextAlign
		void setTextAlign(Align _value) override;
		//! @copydoc TextBox::setTextColour
		void setTextColour(const Colour& _value) override;

		//! @copydoc TextBox::getTextRegion
		IntCoord getTextRegion() const override;

		//! @copydoc TextBox::getTextSize
		IntSize getTextSize() const override;

		//! @copydoc TextBox::setTextShadowColour
		void setTextShadowColour(const Colour& _value) override;

		//! @copydoc TextBox::setTextShadow
		void setTextShadow(bool _value) override;

		/*events:*/
		/** Event : Enter pressed (Ctrl+enter in multiline mode).\n
			signature : void method(MyGUI::EditBox* _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_EditPtr> eventEditSelectAccept;

		/** Event : Text changed.\n
			signature : void method(MyGUI::EditBox* _sender)
			@param _sender widget that called this event
		*/
		EventPair<EventHandle_WidgetVoid, EventHandle_EditPtr> eventEditTextChange;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void onMouseDrag(int _left, int _top, MouseButton _id) override;
		void onKeyLostFocus(Widget* _new) override;
		void onKeySetFocus(Widget* _old) override;
		void onKeyButtonPressed(KeyCode _key, Char _char) override;

		// потом убрать все нотифи в сраку
		void notifyMouseSetFocus(Widget* _sender, Widget* _old);
		void notifyMouseLostFocus(Widget* _sender, Widget* _new);
		void notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonDoubleClick(Widget* _sender);

		void notifyScrollChangePosition(ScrollBar* _sender, size_t _position);
		void notifyMouseWheel(Widget* _sender, int _rel);

		// обновление представления
		void updateView();
		void updateViewWithCursor();

		void eraseView();

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

	private:
		// устанавливает текст
		void setText(const UString& _caption, bool _history);
		// удаляет все что выделенно
		bool deleteTextSelect(bool _history);
		// вставляет текст в указанную позицию
		void insertText(const UString& _text, size_t _start, bool _history);
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
		void commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo* _info = nullptr);

		// команнды отмена и повтор
		bool commandRedo();
		bool commandUndo();
		// объединяет последние две комманды
		void commandMerge();
		// очистка
		void commandResetRedo();
		void commandResetHistory();
		void saveInHistory(VectorChangeInfo* _info = nullptr);

		// clipboard commands
		void commandCut();
		void commandCopy() const;
		void commandPaste();

		const UString& getRealString() const;

		void setRealString(const UString& _caption);

		void updateCursorPosition();

		// размер данных
		IntSize getContentSize() const override;
		// смещение данных
		IntPoint getContentPosition() const override;
		void setContentPosition(const IntPoint& _point) override;
		// размер окна, через которые видно данные
		IntSize getViewSize() const override;
		// размер на который прокручиваются данные при щелчке по скролу
		size_t getVScrollPage() const override;
		size_t getHScrollPage() const override;

		Align getContentAlign() const override;

	protected:
		// нажата ли кнопка
		bool mIsPressed{false};
		// в фокусе ли кнопка
		bool mIsFocus{false};

		bool mCursorActive{false};
		float mCursorTimer{0};
		float mActionMouseTimer{0};

		// позиция курсора
		size_t mCursorPosition{0};
		// максимальное колличество
		size_t mTextLength{0};

		// выделение
		size_t mStartSelect{ITEM_NONE};
		size_t mEndSelect{0};

		// списоки изменений для отмены и повтора
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

		bool mMouseLeftPressed{false};

		bool mModeReadOnly{false};
		bool mModePassword{false};
		bool mModeMultiline{false};
		bool mModeStatic{false};
		bool mModeWordWrap{false};

		bool mTabPrinting{false};

		// настоящий текст, закрытый за звездочками
		UString mPasswordText;

		// для поддержки режима статик, где курсор не нужен
		std::string mOriginalPointer;

		Char mCharPassword{'*'};

		bool mOverflowToTheLeft{false};
		size_t mMaxTextLength;

		ISubWidgetText* mClientText{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_EDIT_BOX_H_
