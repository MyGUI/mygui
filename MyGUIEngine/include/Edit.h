#ifndef _EDIT_H_
#define _EDIT_H_

#include "Prerequest.h"
#include "Widget.h"
#include <deque>

namespace MyGUI
{

	class _MyGUIExport Edit : public Widget, public Ogre::FrameListener
	{
		// для вызова закрытого конструктора
		friend WidgetFactory<Edit>;

	protected:

		// типы операций
		enum CommandType
		{
			COMMAND_POSITION,
			COMMAND_INSERT,
			COMMAND_ERASE
		};

		// инфо об одной операции
		struct tagChangeInfo
		{
			// для удаления и вставки текста
			tagChangeInfo(const Ogre::DisplayString & _text, size_t _start, CommandType _type)
				: text(_text), start(_start), type(_type), undo(ITEM_NONE), redo(ITEM_NONE) {}
			// для указания позиции
			tagChangeInfo(size_t _undo, size_t _redo)
				: start(ITEM_NONE), type(COMMAND_POSITION), undo(_undo), redo(_redo) {}

			// строка харрактиризуещая изменения
			Ogre::DisplayString text;
			// тип операции
			CommandType type;
			// инфа о начале позиции
			size_t start;
			// инфа о псевдо позиции
			size_t undo, redo;
		};
		typedef std::vector<tagChangeInfo> VectorChangeInfo;
		typedef std::deque<VectorChangeInfo> DequeUndoRedoInfo;

	protected:
		Edit(int _x, int _y, int _cx, int _cy, char _align, const WidgetSkinInfoPtr _info, BasisWidgetPtr _parent, const Ogre::String & _name);

	public:
		inline const static Ogre::String & getType() {static Ogre::String type("Edit"); return type;};

		// устанавливает тект
		void setCaption(const Ogre::DisplayString & _caption);
		// устанавливает тект
		inline void setText(const Ogre::DisplayString & _text) {setCaption(_text);}
		// удаляет все что выделенно
		bool deleteTextSelect();
		// возвращает выделение
		inline void getTextSelect(size_t & _start, size_t & _end)
		{
			if (mStartSelect == ITEM_NONE) {_start=ITEM_NONE; _end=ITEM_NONE;}
			else if (mStartSelect > mEndSelect) {_start = mEndSelect; _end = mStartSelect;}
			else {_start = mStartSelect; _end = mEndSelect;}
		};
		// выделен ли текст
		inline bool isTextSelect() {return ( (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect) );}
		// устанавливает выделение
		void setTextSelect(size_t _start, size_t _end);
		// вставляет текст в указанную позицию
		void insertText(const Ogre::DisplayString & _text, size_t _index = ITEM_NONE);
		// добавляет текст в конец
		inline void addText(const Ogre::DisplayString & _text) {insertText(_text, ITEM_NONE);};
		// удаляет текст
		void eraseText(size_t _start, size_t _count = 1);
		// возвращает текст
		Ogre::DisplayString getText(size_t _start, size_t _count);
		// возвращает текст
		Ogre::DisplayString getSelectedText()
		{
			if ( ! isTextSelect()) return "";
			size_t start, end;
			getTextSelect(start, end);
			return getText(start, end-start);
		}

		inline size_t getTextLenght() {return mTextLenght;}
		inline size_t getTextCursor() {return mCursorPosition;}
		void setTextCursor(size_t _index);

		// выделяет цветом диапазон
		void setTextColor(size_t _start, size_t _count, const Ogre::ColourValue & _color);
		// выделяет цветом выделение
		inline void setTextSelectColor(const Ogre::ColourValue & _color)
		{
			// нужно выделение
			if ( ! isTextSelect()) return;
			// начало и конец выделения
			size_t start, end;
			getTextSelect(start, end);
			setTextColor(start, end-start, _color);
		}

	protected:

		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

		void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old);
		void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new);
		void notifyMousePressed(MyGUI::WidgetPtr _sender, bool _left);
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

		void updateCursor(IntPoint _point);

		inline IntPoint getWorldPostion(WidgetPtr _widget)
		{
			IntPoint point(_widget->left(), _widget->top());
			WidgetPtr parent = _widget->getParent();
			while (parent != null) {
				point.left += parent->left();
				point.top += parent->top();
				parent = parent->getParent();
			}
			return point;
		}

		inline void resetSelect()
		{
			if (mStartSelect != ITEM_NONE) {
				mStartSelect = ITEM_NONE;
				m_text->setTextSelect(0, 0);
			}
		}

		// непосредственное выполнение операций с записью в историю
		bool commandErase(size_t _start, size_t _count, VectorChangeInfo * _info = null);
		bool commandInsert(const Ogre::DisplayString & _insert, size_t _start, VectorChangeInfo * _info = null);
		void commandPosition(size_t _undo, size_t _redo, VectorChangeInfo * _info = null);

		// команнды отмена и повтор
		bool commandRedo();
		bool commandUndo();
		// объединяет последние две комманды
		void commandMerge();
		// очистка
		inline void commandResetRedo() {mVectorRedoChangeInfo.clear();}
		inline void commandResetHistory() {mVectorRedoChangeInfo.clear();mVectorUndoChangeInfo.clear();}
		void saveInHistory(VectorChangeInfo * _info = null);


	private:
		// нажата ли кнопка
		bool m_isPressed;
		// в фокусе ли кнопка
		bool m_isFocus;

		WidgetPtr mWidgetUpper;
		WidgetPtr mWidgetCursor;

		bool mCursorActive;
		float mTimer;

		// позиция курсора
		size_t mCursorPosition;
		// максимаотное колличество
		size_t mTextLenght;
		// выделение
		bool mShiftPressed;
		size_t mStartSelect, mEndSelect;

		bool mCtrlPressed;
		Ogre::DisplayString mClipboard;

		// списоки изменений для отмены и повтора
		DequeUndoRedoInfo mVectorUndoChangeInfo;
		DequeUndoRedoInfo mVectorRedoChangeInfo;

	}; // class Edit : public Widget

	typedef Edit * EditPtr;

} // namespace MyGUI

#endif
