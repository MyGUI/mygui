/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
	public:
		TextIterator(const Ogre::DisplayString & _text, VectorChangeInfo * _history = null) :
			mText(_text),
			mHistory(_history),
			mCurrent(mText.begin()),
			mEnd(mText.end()),
			mSave(mEnd),
			mPosition(0),
			mSize(SIZE_MAX),
			mFirst(true)
		{
		}

		bool moveNext()
		{
			if (mCurrent == mEnd) return false;
			else if (mFirst) {mFirst=false;return true;}

			// ставим на следующий символ проскакивая все тэги
			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {

					// следующий символ
					++ iter;
					if (iter == mEnd) {
						mCurrent = mEnd;
						return false;
					}

					// две решетки подряд
					if ((*iter) == '#') {

						// следующий символ
						mPosition ++;
						iter++;
						if (iter == mEnd) {
							mCurrent = mEnd;
							return false;
						}

						// указатель на следующий символ
						mCurrent = iter;
						return true;
					}

					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++) {
						// следующий символ
						++ iter;
						if (iter == mEnd) {
							mCurrent = mEnd;
							return false;
						}
					}

				} else {

					// обыкновенный символ
					mPosition ++;
					iter++;
					if (iter == mEnd) {
						mCurrent = mEnd;
						return false;
					}

					// указатель на следующий символ
					mCurrent = iter;
					return true;
				}
			}

			return false;
		}

		// возвращает цвет
		inline Ogre::DisplayString getTagColour(bool _clear = false)
		{
			if (mCurrent == mEnd) return L"";

			Ogre::DisplayString::iterator iter = mCurrent;
			Ogre::DisplayString colour;
			// нам нужен последний цвет
			while (getTagColour(colour, iter)) {
				if (_clear) {
					// обязательно обновляем итераторы
					iter = mCurrent = erase(mCurrent, iter);
					mEnd = mText.end();
				}
			};
			return colour;
		}

		// возвращает цвет
		inline bool getTagColour(Ogre::DisplayString & _colour)
		{
			if (mCurrent == mEnd) return false;

			Ogre::DisplayString::iterator iter = mCurrent;

			// нам нужен последний цвет
			bool ret = false;
			while (true) {
				if (!getTagColour(_colour, iter)) break;
				ret = true;
			};

			return ret;
		}

		// удаляет цвет
		inline void clearTagColour() {getTagColour(true);}

		bool setTagColour(const Ogre::ColourValue & _colour)
		{
			if (mCurrent == mEnd) return false;
			// очищаем все цвета
			clearTagColour();
			// на всякий
			if (mCurrent == mEnd) return false;

			wchar_t buff[16];
			::wsprintfW(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));

			// непосредственная вставка
			Ogre::DisplayString tmpStr = Ogre::DisplayString(buff);
			insert(mCurrent, tmpStr);

			return true;
		}

		bool setTagColour(Ogre::DisplayString _colour)
		{
			if (mCurrent == mEnd) return false;
			// очищаем все цвета
			clearTagColour();
			// на всякий
			if (mCurrent == mEnd) return false;

			// проверяем на цвет хоть чуть чуть
			if ( (_colour.size() != 7) || (_colour.find('#', 1) != _colour.npos) ) return false;

			// непосредственная вставка
			insert(mCurrent, _colour);

			return true;
		}

		// просто конвертируем цвет в строку
		inline static Ogre::DisplayString convertTagColour(const Ogre::ColourValue & _colour)
		{
			wchar_t buff[16];
			::wsprintfW(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
			return buff;
		}

		// сохраняет текущий итератор
		bool saveStartPoint()
		{
			if (mCurrent == mEnd) return false;
			mSave = mCurrent;
			return true;
		}

		// возвращает строку от сохраненного итератора до текущего
		Ogre::DisplayString getFromStart()
		{
			if (mSave == mEnd) return "";
			size_t start = mSave-mText.begin();
			return mText.substr(start, mCurrent-mText.begin()-start);
		}

		// удаляет от запомненной точки до текущей
		bool eraseFromStart()
		{
			if (mSave == mEnd) return false;
			mCurrent = erase(mSave, mCurrent);
			mSave = mEnd = mText.end();
			return true;
		}

		// возвращает текущую псевдо позицию
		inline size_t getPosition() {return mPosition;}

		inline const Ogre::DisplayString & getText() {return mText;}

		void insertText(const Ogre::DisplayString & _insert, bool _multiLine)
		{
			Ogre::DisplayString text = _insert;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		void clearNewLine(Ogre::DisplayString & _text)
		{
			for (Ogre::DisplayString::iterator iter=_text.begin(); iter!=_text.end(); ++iter) {
				if ( ((*iter) == Font::FONT_CODE_NEL) || ((*iter) == Font::FONT_CODE_CR) || ((*iter) == Font::FONT_CODE_LF) )
					(*iter) = Font::FONT_CODE_SPACE;
			}
		}

		//очищает весь текст
		void clearText() {clear();}

		// возвращает размер строки
		size_t getSize()
		{
			if (mSize != SIZE_MAX) return mSize;
			mSize = mPosition;

			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {
					// следующий символ
					++ iter;
					if (iter == mEnd) break;

					// тэг цвета
					if ((*iter) != '#') {
						// остальные 5 символов цвета
						for (size_t pos=0; pos<5; pos++) {
							++ iter;
							if (iter == mEnd) break;
						}
						continue;
					}
				}

				// обыкновенный символ
				mSize ++;
			}

			return mSize;
		}

		inline static Ogre::DisplayString getTextNewLine() {return "\n";}
		inline static Ogre::DisplayString getTextCharInfo(wchar_t _char)
		{
			if (_char == L'#') return L"##";
			wchar_t buff[16] = L"_\0";
			buff[0] = _char;
			return buff;
		}

		void setText(const Ogre::DisplayString & _text, bool _multiLine)
		{
			// сначала все очищаем
			clear();
			// а теперь вставляем
			Ogre::DisplayString text = _text;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		void cutMaxLenght(size_t _max)
		{
			if ( (mSize != SIZE_MAX) && (mSize <= _max) ) return;
			if (mPosition > _max) {
				// придется считать сначала
				mSize = mPosition = 0;
				mCurrent = mText.begin();
				mEnd = mSave = mText.end();
			}

			mSize = mPosition;

			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {
					// следующий символ
					++ iter;
					if (iter == mEnd) break;

					// тэг цвета
					if ((*iter) != '#') {
						// остальные 5 символов цвета
						for (size_t pos=0; pos<5; pos++) {
							++ iter;
							if (iter == mEnd) break;
						}
						continue;
					}
				}

				// проверяем и обрезаем
				if (mSize == _max) {
					mPosition = mSize; // сохраняем
					mCurrent = erase(iter, mEnd);
					mSave = mEnd = mText.end();
					mSize = mPosition; // восстанавливаем
					return;
				}

				// увеличиваем
				mSize ++;
			}
		}

	private:

		// возвращает цвет
		bool getTagColour(Ogre::DisplayString & _colour, Ogre::DisplayString::iterator & _iter)
		{
			if ( (_iter == mEnd) || ((*_iter) != '#') ) return false;

			// следующий символ
			++_iter;
			if ( (_iter == mEnd) || ((*_iter) == '#') ) return false;

			// берем цвет
			wchar_t buff[16] = L"#FFFFFF\0";
			buff[1] = (wchar_t)(*_iter);
			for (size_t pos=2; pos<7; pos++) {
				++_iter;
				if ( _iter == mEnd ) return false;
				buff[pos] = (wchar_t)(*_iter);
			}

			// ставим на следующий тег или символ
			++_iter;

			// возвращаем цвет
			_colour = buff;
			return true;
		}

		inline void insert(Ogre::DisplayString::iterator & _start, Ogre::DisplayString & _insert)
		{
			// сбрасываем размер
			mSize = SIZE_MAX;
			// записываем в историю
			if (mHistory) mHistory->push_back(tagChangeInfo(_insert, _start-mText.begin(), COMMAND_INSERT));
			// запоминаем позицию итератора
			size_t pos = _start - mText.begin();
			size_t pos_save = (mSave==mEnd) ? SIZE_MAX : _start - mText.begin();
			// непосредственно вставляем
			mText.insert(_start, _insert.begin(), _insert.end());
			// возвращаем итераторы
			_start = mText.begin() + pos;
			mEnd = mText.end();
			(pos_save==SIZE_MAX) ? mSave = mEnd : mSave = mText.begin() + pos_save;
		}

		inline Ogre::DisplayString::iterator erase(Ogre::DisplayString::iterator _start, Ogre::DisplayString::iterator _end)
		{
			// сбрасываем размер
			mSize = SIZE_MAX;
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
			mSize = SIZE_MAX;
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