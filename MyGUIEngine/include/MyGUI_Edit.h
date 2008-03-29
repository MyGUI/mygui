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
		Edit(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		virtual ~Edit();

		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//!	@copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// работа с текстом с тегами
		virtual void setCaption(const Ogre::UTFString & _caption);
		virtual const Ogre::UTFString& getCaption();

		// работа с текстом без тегов
		inline void setOnlyText(const Ogre::UTFString & _text) {setText(TextIterator::toTagsString(_text), false);}
		inline Ogre::UTFString getOnlyText() {return TextIterator::getOnlyText(getRealString());}

		// возвращает выделение
		void getTextSelect(size_t & _start, size_t & _end);
		// выделен ли текст
		inline bool isTextSelect() {return ( (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect) );}
		// возвращает выделенный текст
		Ogre::UTFString getSelectedText();

		inline size_t getTextLength() {return mTextLength;}
		inline size_t getTextCursor() {return mCursorPosition;}

		void setTextCursor(size_t _index);
		// устанавливает выделение
		void setTextSelect(size_t _start, size_t _end);

		virtual void setTextAlign(Align _align);


		//---------------------------------------------------------------//
		// наружу выставляем инлайны со сбросом истории
		//---------------------------------------------------------------//

		// возвращает текст с тегами
		Ogre::UTFString getText(size_t _start, size_t _count);
		// удаляет все что выделенно
		inline bool deleteTextSelect() {return deleteTextSelect(false);}
		// вставляет текст в указанную позицию
		inline void insertText(const Ogre::UTFString & _text, size_t _index = ITEM_NONE) {insertText(_text, _index, false);}
		// добавляет текст в конец
		inline void addText(const Ogre::UTFString & _text) {addText(_text, false);}
		// удаляет текст
		inline void eraseText(size_t _start, size_t _count = 1) {eraseText(_start, _count, false);}
		// выделяет цветом выделение
		inline void setTextSelectColour(const Ogre::ColourValue & _colour)
		{
			setTextSelectColour(_colour, false);
		}
		// выделяет цветом диапазон
		inline void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour)
		{
			setTextColour(_start, _count, _colour, false);
		}

		inline bool getEditReadOnly() {return mModeReadOnly;}
		inline void setEditReadOnly(bool _read)
		{
			mModeReadOnly = _read;
			// сбрасываем историю
			commandResetHistory();
		}

		inline bool getEditPassword() {return mModePassword;}
		void setEditPassword(bool _password);

		inline bool getEditMultiLine() {return mModeMultiline;}
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

		inline void setEditStatic(bool _static)
		{
			mModeStatic = _static;
			resetSelect();
			if (mModeStatic) mWidgetUpper->setPointer("");
			else mWidgetUpper->setPointer(mOriginalPointer);
		}

		inline bool getEditStatic()
		{
			return mModeStatic;
		}

		void setPosition(const IntCoord& _coord);
		void setSize(const IntSize& _size);

		inline void setPosition(int _left, int _top) {Widget::setPosition(IntPoint(_left, _top));}
		inline void setPosition(int _left, int _top, int _width, int _height) {setPosition(IntCoord(_left, _top, _width, _height));}
		inline void setSize(int _width, int _height) {setSize(IntSize(_width, _height));}

		// event : нажат ентер, или сонтрол энтер в мультилайн
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventEditSelectAccept;

		// event : изменился текст
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventEditTextChange;

	protected:

		// устанавливает текст
		void setText(const Ogre::UTFString & _text, bool _history);
		// удаляет все что выделенно
		bool deleteTextSelect(bool _history);
		// вставляет текст в указанную позицию
		void insertText(const Ogre::UTFString & _text, size_t _index, bool _history);
		// добавляет текст в конец
		inline void addText(const Ogre::UTFString & _text, bool _history) {insertText(_text, ITEM_NONE, _history);}
		// удаляет текст
		void eraseText(size_t _start, size_t _count, bool _history);
		// выделяет цветом выделение
		void setTextSelectColour(const Ogre::ColourValue & _colour, bool _history);
		// выделяет цветом диапазон
		void setTextColour(size_t _start, size_t _count, const Ogre::ColourValue & _colour, bool _history);

	protected:
		virtual void _frameEntered(float _frame);

		// потом убрать все нотифи в сраку
		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, bool _left);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		virtual void _onMouseDrag(int _left, int _top);
		virtual void _onKeyLostFocus(WidgetPtr _new);
		virtual void _onKeySetFocus(WidgetPtr _old);
		virtual void _onKeyButtonPressed(int _key, Char _char);
		virtual void _onKeyButtonReleased(int _key);

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

	}; // class Edit : public Widget

} // namespace MyGUI

#endif // __MYGUI_EDIT_H__
