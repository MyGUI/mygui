/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	TextIterator::TextIterator() {}

	TextIterator::TextIterator(const Ogre::DisplayString & _text, VectorChangeInfo * _history) :
		mText(_text),
		mCurrent(mText.begin()),
		mEnd(mText.end()),
		mSave(mEnd),
		mPosition(0),
		mSize(ITEM_NONE),
		mFirst(true),
		mHistory(_history)
	{
	}

	bool TextIterator::moveNext()
	{
		if (mCurrent == mEnd) return false;
		else if (mFirst) {mFirst=false;return true;}

		// ставим на следующий символ проскакивая все тэги
		for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == _T('#')) {

				// следующий символ
				++ iter;
				if (iter == mEnd) {
					mCurrent = mEnd;
					return false;
				}

				// две решетки подряд
				if ((*iter) == _T('#')) {

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
	Ogre::DisplayString TextIterator::getTagColour(bool _clear)
	{
		if (mCurrent == mEnd) return _T("");

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
	bool TextIterator::getTagColour(Ogre::DisplayString & _colour)
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

	bool TextIterator::setTagColour(const Ogre::ColourValue & _colour)
	{
		if (mCurrent == mEnd) return false;
		// очищаем все цвета
		clearTagColour();
		// на всякий
		if (mCurrent == mEnd) return false;

		const size_t SIZE = 16;
		Char buff[SIZE];

		#if MYGUI_UNICODE_SUPPORT
			swprintf(buff, SIZE, _T("#%.2X%.2X%.2X\0"), (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
		#else
			sprintf(buff, _T("#%.2X%.2X%.2X\0"), (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
		#endif

		// непосредственная вставка
		Ogre::DisplayString tmpStr = Ogre::DisplayString(buff);
		insert(mCurrent, tmpStr);

		return true;
	}

	bool TextIterator::setTagColour(Ogre::DisplayString _colour)
	{
		if (mCurrent == mEnd) return false;
		// очищаем все цвета
		clearTagColour();
		// на всякий
		if (mCurrent == mEnd) return false;

		// проверяем на цвет хоть чуть чуть
		if ( (_colour.size() != 7) || (_colour.find(_T('#'), 1) != _colour.npos) ) return false;

		// непосредственная вставка
		insert(mCurrent, _colour);

		return true;
	}

	// возвращает размер строки
	size_t TextIterator::getSize()
	{
		if (mSize != ITEM_NONE) return mSize;
		mSize = mPosition;

		for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == _T('#')) {
				// следующий символ
				++ iter;
				if (iter == mEnd) break;

				// тэг цвета
				if ((*iter) != _T('#')) {
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

	void TextIterator::cutMaxLength(size_t _max)
	{
		if ( (mSize != ITEM_NONE) && (mSize <= _max) ) return;
		if (mPosition > _max) {
			// придется считать сначала
			mSize = mPosition = 0;
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
		}

		mSize = mPosition;

		for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == _T('#')) {
				// следующий символ
				++ iter;
				if (iter == mEnd) break;

				// тэг цвета
				if ((*iter) != _T('#')) {
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

	// возвращает текст без тегов
	Ogre::DisplayString TextIterator::getOnlyText(const Ogre::DisplayString& _text)
	{
		Ogre::DisplayString ret;
		ret.reserve(_text.size());

		Ogre::DisplayString::const_iterator end = _text.end();
		for (Ogre::DisplayString::const_iterator iter=_text.begin(); iter!=end; ++iter) {

			if ((*iter) == _T('#')) {
				// следующий символ
				++ iter;
				if (iter == end) break;

				// тэг цвета
				if ((*iter) != _T('#')) {
					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++) {
						++ iter;
						if (iter == end) break;
					}
					continue;
				}
			}

			// обыкновенный символ
			ret.push_back(*iter);
		}

		return ret;
	}

	// возвращает цвет
	bool TextIterator::getTagColour(Ogre::DisplayString & _colour, Ogre::DisplayString::iterator & _iter)
	{
		if ( (_iter == mEnd) || ((*_iter) != _T('#')) ) return false;

		// следующий символ
		++_iter;
		if ( (_iter == mEnd) || ((*_iter) == _T('#')) ) return false;

		// берем цвет
		Char buff[16] = _T("#FFFFFF\0");
		buff[1] = (Char)(*_iter);
		for (size_t pos=2; pos<7; pos++) {
			++_iter;
			if ( _iter == mEnd ) return false;
			buff[pos] = (Char)(*_iter);
		}

		// ставим на следующий тег или символ
		++_iter;

		// возвращаем цвет
		_colour = buff;
		return true;
	}

} // namespace MyGUI
