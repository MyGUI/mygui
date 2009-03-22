/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	TextIterator::TextIterator()
	{
	}

	TextIterator::TextIterator(const Ogre::UTFString & _text, VectorChangeInfo * _history) :
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
		else if (mFirst)
		{
			mFirst = false;
			return true;
		}

		// ставим на следующий символ проскакивая все тэги
		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter)
		{

			if ((*iter) == L'#')
			{

				// следующий символ
				++ iter;
				if (iter == mEnd)
				{
					mCurrent = mEnd;
					return false;
				}

				// две решетки подряд
				if ((*iter) == L'#')
				{

					// следующий символ
					mPosition ++;
					iter++;
					if (iter == mEnd)
					{
						mCurrent = mEnd;
						return false;
					}

					// указатель на следующий символ
					mCurrent = iter;
					return true;
				}

				// остальные 5 символов цвета
				for (size_t pos=0; pos<5; pos++)
				{
					// следующий символ
					++ iter;
					if (iter == mEnd)
					{
						mCurrent = mEnd;
						return false;
					}
				}

			}
			else
			{

				// обыкновенный символ
				mPosition ++;
				iter++;
				if (iter == mEnd)
				{
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
	Ogre::UTFString TextIterator::getTagColour(bool _clear)
	{
		if (mCurrent == mEnd) return L"";

		Ogre::UTFString::iterator iter = mCurrent;
		Ogre::UTFString colour;
		// нам нужен последний цвет
		while (getTagColour(colour, iter))
		{
			if (_clear)
			{
				// обязательно обновляем итераторы
				iter = mCurrent = erase(mCurrent, iter);
				mEnd = mText.end();
			}
		}
		return colour;
	}

	// возвращает цвет
	bool TextIterator::getTagColour(Ogre::UTFString & _colour)
	{
		if (mCurrent == mEnd) return false;

		Ogre::UTFString::iterator iter = mCurrent;

		// нам нужен последний цвет
		bool ret = false;
		while (true)
		{
			if (!getTagColour(_colour, iter)) break;
			ret = true;
		};

		return ret;
	}

	bool TextIterator::setTagColour(const Colour& _colour)
	{
		if (mCurrent == mEnd) return false;
		// очищаем все цвета
		clearTagColour();
		// на всякий
		if (mCurrent == mEnd) return false;

		const size_t SIZE = 16;
		wchar_t buff[SIZE];

#ifdef __MINGW32__
		swprintf(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.red*255), (int)(_colour.green*255), (int)(_colour.blue*255));
#else
		swprintf(buff, SIZE, L"#%.2X%.2X%.2X\0", (int)(_colour.red*255), (int)(_colour.green*255), (int)(_colour.blue*255));
#endif
		// непосредственная вставка
		Ogre::UTFString tmpStr = Ogre::UTFString(buff);
		insert(mCurrent, tmpStr);

		return true;
	}

	bool TextIterator::setTagColour(Ogre::UTFString _colour)
	{
		if (mCurrent == mEnd) return false;
		// очищаем все цвета
		clearTagColour();
		// на всякий
		if (mCurrent == mEnd) return false;

		// проверяем на цвет хоть чуть чуть
		if ( (_colour.size() != 7) || (_colour.find(L'#', 1) != _colour.npos) ) return false;

		// непосредственная вставка
		insert(mCurrent, _colour);

		return true;
	}

	// возвращает размер строки
	size_t TextIterator::getSize()
	{
		if (mSize != ITEM_NONE) return mSize;
		mSize = mPosition;

		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter)
		{

			if ((*iter) == L'#')
			{
				// следующий символ
				++ iter;
				if (iter == mEnd) break;

				// тэг цвета
				if ((*iter) != L'#')
				{
					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++)
					{
						++ iter;
						if (iter == mEnd)
						{
							--iter;
							break;
						}
					}
					continue;
				}
			}

			// обыкновенный символ
			mSize ++;
		}

		return mSize;
	}

	// возвращает текст без тегов
	Ogre::UTFString TextIterator::getOnlyText(const Ogre::UTFString& _text)
	{
		Ogre::UTFString ret;
		ret.reserve(_text.size());

		Ogre::UTFString::const_iterator end = _text.end();
		for (Ogre::UTFString::const_iterator iter=_text.begin(); iter!=end; ++iter)
		{

			if ((*iter) == L'#')
			{
				// следующий символ
				++ iter;
				if (iter == end) break;

				// тэг цвета
				if ((*iter) != L'#') {
					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++)
					{
						++ iter;
						if (iter == end)
						{
							--iter;
							break;
						}
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
	bool TextIterator::getTagColour(Ogre::UTFString & _colour, Ogre::UTFString::iterator & _iter)
	{
		if ( (_iter == mEnd) || ((*_iter) != L'#') ) return false;

		// следующий символ
		++_iter;
		if ( (_iter == mEnd) || ((*_iter) == L'#') ) return false;

		// берем цвет
		wchar_t buff[16] = L"#FFFFFF\0";
		buff[1] = (wchar_t)(*_iter);
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

	void TextIterator::clearNewLine(Ogre::UTFString & _text)
	{
		for (Ogre::UTFString::iterator iter=_text.begin(); iter!=_text.end(); ++iter) {
			if ( ((*iter) == Font::FONT_CODE_NEL) ||
				((*iter) == Font::FONT_CODE_CR) ||
				((*iter) == Font::FONT_CODE_LF) )
			{
				(*iter) = Font::FONT_CODE_SPACE;
			}
		}
	}

	bool TextIterator::saveStartPoint()
	{
		if (mCurrent == mEnd) return false;
		mSave = mCurrent;
		return true;
	}

	Ogre::UTFString TextIterator::getFromStart()
	{
		if (mSave == mEnd) return L"";
		size_t start = mSave-mText.begin();
		return mText.substr(start, mCurrent-mText.begin()-start);
	}

	bool TextIterator::eraseFromStart()
	{
		if (mSave == mEnd) return false;
		mCurrent = erase(mSave, mCurrent);
		mSave = mEnd = mText.end();
		return true;
	}

	void TextIterator::insertText(const Ogre::UTFString & _insert, bool _multiLine)
	{
		Ogre::UTFString text = _insert;
		if (false == _multiLine) clearNewLine(text);
		insert(mCurrent, text);
	}

	void TextIterator::setText(const Ogre::UTFString & _text, bool _multiLine)
	{
		// сначала все очищаем
		clear();
		// а теперь вставляем
		Ogre::UTFString text = _text;
		if (false == _multiLine) clearNewLine(text);
		insert(mCurrent, text);
	}

	Ogre::UTFString TextIterator::getTextCharInfo(Char _char)
	{
		if (_char == L'#') return L"##";
		wchar_t buff[16] = L"_\0";
		buff[0] = _char;
		return buff;
	}

	Ogre::UTFString TextIterator::convertTagColour(const Colour& _colour)
	{
		const size_t SIZE = 16;
		wchar_t buff[SIZE];
//FIXME
#ifdef __MINGW32__
        swprintf(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.red*255), (int)(_colour.green*255), (int)(_colour.blue*255));
#else
        swprintf(buff, SIZE, L"#%.2X%.2X%.2X\0", (int)(_colour.red*255), (int)(_colour.green*255), (int)(_colour.blue*255));
#endif
		return buff;
	}

	Ogre::UTFString TextIterator::toTagsString(const Ogre::UTFString& _text)
	{
		// преобразуем в строку с тегами
		Ogre::UTFString text(_text);
		for (Ogre::UTFString::iterator iter=text.begin(); iter!=text.end(); ++iter) {
			// потом переделать через TextIterator чтобы отвязать понятие тег от эдита
			if (L'#' == (*iter)) iter = text.insert(++iter, L'#');
		}
		return text;
	}

	void TextIterator::insert(Ogre::UTFString::iterator & _start, Ogre::UTFString & _insert)
	{
		// сбрасываем размер
		mSize = ITEM_NONE;
		// записываем в историю
		if (mHistory) mHistory->push_back(TextCommandInfo(_insert, _start-mText.begin(), TextCommandInfo::COMMAND_INSERT));
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

	Ogre::UTFString::iterator TextIterator::erase(Ogre::UTFString::iterator _start, Ogre::UTFString::iterator _end)
	{
		// сбрасываем размер
		mSize = ITEM_NONE;
		// сохраняем в историю
		size_t start = _start-mText.begin();
		if (mHistory) mHistory->push_back(TextCommandInfo(mText.substr(start, _end-_start), start, TextCommandInfo::COMMAND_ERASE));
		// возвращаем итератор
		return mText.erase(_start, _end);
	}

	void TextIterator::clear()
	{
		if (mText.empty()) return;

		// записываем в историю
		if (mHistory) mHistory->push_back(TextCommandInfo(mText, 0, TextCommandInfo::COMMAND_ERASE));

		// все сбрасываем
		mText.clear();
		mCurrent = mText.begin();
		mEnd = mSave = mText.end();
		mSize = ITEM_NONE;
	}

	void TextIterator::cutMaxLength(size_t _max)
	{
		if ( (mSize != ITEM_NONE) && (mSize <= _max) ) return;
		if (mPosition > _max)
		{
			// придется считать сначала
			mSize = mPosition = 0;
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
		}

		mSize = mPosition;

		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter)
		{

			if ((*iter) == L'#')
			{
				// следующий символ
				++ iter;
				if (iter == mEnd) break;

				// тэг цвета
				if ((*iter) != L'#')
				{
					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++)
					{
						++ iter;
						if (iter == mEnd)
						{
							-- iter;
							break;
						}
					}
					continue;
				}
			}

			// проверяем и обрезаем
			if (mSize == _max)
			{
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

	void TextIterator::cutMaxLengthFromBeginning(size_t _max)
	{
		// узнаем размер без тегов
		size_t size = getSize();
		if (size <= _max) return;

		// разница 
		size_t diff = size - _max;

		// последний цвет
		Ogre::UTFString::iterator iter_colour = mEnd;

		// теперь пройдем от начала и узнаем реальную позицию разницы
		Ogre::UTFString::iterator iter=mText.begin();
		for (; iter!=mEnd; ++iter)
		{
			if ((*iter) == L'#')
			{
				Ogre::UTFString::iterator save = iter;

				// следующий символ
				++ iter;
				if (iter == mEnd) break;

				// тэг цвета
				if ((*iter) != L'#')
				{
					// остальные 5 символов цвета
					for (size_t pos=0; pos<5; pos++)
					{
						++ iter;
						if (iter == mEnd)
						{
							-- iter;
							break;
						}
					}
					// сохраняем цвет
					iter_colour = save;
				}
				continue;
			}
			// обычный символ был
			if (diff == 0) break;
			-- diff;
		}

		Ogre::UTFString colour;
		// если бы цвет, то вставляем назад
		if (iter_colour != mEnd)
		{
			colour.append(iter_colour, iter_colour + size_t(7));
		}

		mCurrent = erase(mText.begin(), iter);
		mEnd = mText.end();
		mSave = mText.end(); //FIXME
		mPosition = 0;
		mSize = _max;

		if ( ! colour.empty() ) setTagColour(colour);

	}

} // namespace MyGUI
