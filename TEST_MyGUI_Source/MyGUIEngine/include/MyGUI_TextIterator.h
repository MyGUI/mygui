/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_TEXT_ITERATOR_H__
#define __MYGUI_TEXT_ITERATOR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_Font.h"

namespace MyGUI
{

	class _MyGUIExport TextIterator
	{
	private:
		TextIterator();

	public:
		TextIterator(const Ogre::DisplayString & _text, VectorChangeInfo * _history = null);

		bool moveNext();

		// возвращает цвет
		Ogre::DisplayString getTagColour(bool _clear = false);

		// возвращает цвет
		bool getTagColour(Ogre::DisplayString & _colour);

		// удаляет цвет
		inline void clearTagColour() {getTagColour(true);}

		bool setTagColour(const Ogre::ColourValue & _colour);

		bool setTagColour(Ogre::DisplayString _colour);

		// сохраняет текущий итератор
		inline bool saveStartPoint()
		{
			if (mCurrent == mEnd) return false;
			mSave = mCurrent;
			return true;
		}

		// возвращает строку от сохраненного итератора до текущего
		inline Ogre::DisplayString getFromStart()
		{
			if (mSave == mEnd) return _T("");
			size_t start = mSave-mText.begin();
			return mText.substr(start, mCurrent-mText.begin()-start);
		}

		// удаляет от запомненной точки до текущей
		inline bool eraseFromStart()
		{
			if (mSave == mEnd) return false;
			mCurrent = erase(mSave, mCurrent);
			mSave = mEnd = mText.end();
			return true;
		}

		// возвращает текущую псевдо позицию
		inline size_t getPosition() {return mPosition;}

		inline const Ogre::DisplayString & getText() {return mText;}

		inline void insertText(const Ogre::DisplayString & _insert, bool _multiLine)
		{
			Ogre::DisplayString text = _insert;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		inline void clearNewLine(Ogre::DisplayString & _text)
		{
			for (Ogre::DisplayString::iterator iter=_text.begin(); iter!=_text.end(); ++iter) {
				if ( ((*iter) == Font::FONT_CODE_NEL) || ((*iter) == Font::FONT_CODE_CR) || ((*iter) == Font::FONT_CODE_LF) )
					(*iter) = Font::FONT_CODE_SPACE;
			}
		}

		//очищает весь текст
		inline void clearText() {clear();}

		// возвращает размер строки
		size_t getSize();

		inline void setText(const Ogre::DisplayString & _text, bool _multiLine)
		{
			// сначала все очищаем
			clear();
			// а теперь вставляем
			Ogre::DisplayString text = _text;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		void cutMaxLength(size_t _max);

		// возвращает текст без тегов
		static Ogre::DisplayString getOnlyText(const Ogre::DisplayString& _text);

		inline static Ogre::DisplayString getTextNewLine()
		{
			return _T("\n");
		}

		inline static Ogre::DisplayString getTextCharInfo(Char _char)
		{
			if (_char == _T('#')) return _T("##");
			Char buff[16] = _T("_\0");
			buff[0] = _char;
			return buff;
		}

		// просто конвертируем цвет в строку
		inline static Ogre::DisplayString convertTagColour(const Ogre::ColourValue & _colour)
		{
			const size_t SIZE = 16;
			Char buff[SIZE];

			#if MYGUI_UNICODE_SUPPORT
				swprintf(buff, SIZE, _T("#%.2X%.2X%.2X\0"), (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
			#else
				sprintf(buff, _T("#%.2X%.2X%.2X\0"), (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
			#endif

			return buff;
		}

		inline static Ogre::DisplayString toTagsString(const Ogre::DisplayString& _text)
		{
			// преобразуем в строку с тегами
			Ogre::DisplayString text(_text);
			for (Ogre::DisplayString::iterator iter=text.begin(); iter!=text.end(); ++iter) {
				// потом переделать через TextIterator чтобы отвязать понятие тег от эдита
				if (_T('#') == (*iter)) iter = text.insert(++iter, _T('#'));
			}
			return text;
		}


	private:

		// возвращает цвет
		bool getTagColour(Ogre::DisplayString & _colour, Ogre::DisplayString::iterator & _iter);

		inline void insert(Ogre::DisplayString::iterator & _start, Ogre::DisplayString & _insert)
		{
			// сбрасываем размер
			mSize = ITEM_NONE;
			// записываем в историю
			if (mHistory) mHistory->push_back(tagChangeInfo(_insert, _start-mText.begin(), COMMAND_INSERT));
			// запоминаем позицию итератора
			size_t pos = _start - mText.begin();
			size_t pos_save = (mSave==mEnd) ? ITEM_NONE : _start - mText.begin();
			// непосредственно вставляем
			mText.insert(_start, _insert.begin(), _insert.end());
			// возвращаем итераторы
			_start = mText.begin() + pos;
			mEnd = mText.end();
			(pos_save==ITEM_NONE) ? mSave = mEnd : mSave = mText.begin() + pos_save;
		}

		inline Ogre::DisplayString::iterator erase(Ogre::DisplayString::iterator _start, Ogre::DisplayString::iterator _end)
		{
			// сбрасываем размер
			mSize = ITEM_NONE;
			// сохраняем в историю
			size_t start = _start-mText.begin();
			if (mHistory) mHistory->push_back(tagChangeInfo(mText.substr(start, _end-_start), start, COMMAND_ERASE));
			// возвращаем итератор
			return mText.erase(_start, _end);
		}

		inline void clear()
		{
			if (mText.empty()) return;

			// записываем в историю
			if (mHistory) mHistory->push_back(tagChangeInfo(mText, 0, COMMAND_ERASE));

			// все сбрасываем
			mText.clear();
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
			mSize = ITEM_NONE;
		}

	private:
		// текси и два итератора
		Ogre::DisplayString mText;
		Ogre::DisplayString::iterator mCurrent, mEnd, mSave;

		// позиция и размер
		size_t mPosition, mSize;
		bool mFirst;

		VectorChangeInfo * mHistory;

	}; // class _MyGUIExport TextIterator

} // namespace MyGUI

#endif // __MYGUI_TEXT_ITERATOR_H__
