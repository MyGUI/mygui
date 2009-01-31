/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_TEXT_ITERATOR_H__
#define __MYGUI_TEXT_ITERATOR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Colour.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_Font.h"

namespace MyGUI
{

	class MYGUI_EXPORT TextIterator
	{
	private:
		TextIterator();

	public:
		TextIterator(const Ogre::UTFString & _text, VectorChangeInfo * _history = nullptr);

		bool moveNext();

		// возвращает цвет
		Ogre::UTFString getTagColour(bool _clear = false);

		// возвращает цвет
		bool getTagColour(Ogre::UTFString & _colour);

		// удаляет цвет
		void clearTagColour() { getTagColour(true); }

		bool setTagColour(const Colour& _colour);

		bool setTagColour(Ogre::UTFString _colour);

		// сохраняет текущий итератор
		bool saveStartPoint();

		// возвращает строку от сохраненного итератора до текущего
		Ogre::UTFString getFromStart();

		// удаляет от запомненной точки до текущей
		bool eraseFromStart();

		// возвращает текущую псевдо позицию
		size_t getPosition() {return mPosition;}

		const Ogre::UTFString & getText() {return mText;}

		void insertText(const Ogre::UTFString & _insert, bool _multiLine);

		void clearNewLine(Ogre::UTFString & _text);

		//очищает весь текст
		void clearText() { clear(); }

		// возвращает размер строки
		size_t getSize();

		void setText(const Ogre::UTFString & _text, bool _multiLine);

		void cutMaxLength(size_t _max);

		void cutMaxLengthFromBeginning(size_t _max);

		// возвращает текст без тегов
		static Ogre::UTFString getOnlyText(const Ogre::UTFString& _text);

		static Ogre::UTFString getTextNewLine() { return L"\n"; }

		static Ogre::UTFString getTextCharInfo(Char _char);

		// просто конвертируем цвет в строку
		static Ogre::UTFString convertTagColour(const Colour& _colour);

		static Ogre::UTFString toTagsString(const Ogre::UTFString& _text);

	private:

		// возвращает цвет
		bool getTagColour(Ogre::UTFString & _colour, Ogre::UTFString::iterator & _iter);

		void insert(Ogre::UTFString::iterator & _start, Ogre::UTFString & _insert);

		Ogre::UTFString::iterator erase(Ogre::UTFString::iterator _start, Ogre::UTFString::iterator _end);

		void clear();

	private:
		Ogre::UTFString mText;
		Ogre::UTFString::iterator mCurrent, mEnd, mSave;

		// позиция и размер
		size_t mPosition, mSize;
		bool mFirst;

		VectorChangeInfo * mHistory;

	}; // class MYGUI_EXPORT TextIterator

} // namespace MyGUI

#endif // __MYGUI_TEXT_ITERATOR_H__
