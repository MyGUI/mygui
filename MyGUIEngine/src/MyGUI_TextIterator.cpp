/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	TextIterator::TextIterator() :
		mPosition(0),
		mSize(ITEM_NONE),
		mFirst(true),
		mHistory(nullptr)
	{
	}

	TextIterator::TextIterator(const UString& _text, VectorChangeInfo* _history) :
		mText(_text.asUTF32()),
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
		if (mCurrent == mEnd)
			return false;
		if (mFirst)
		{
			mFirst = false;
			return true;
		}

		// jump to next character, skipping tags (#)
		for (UString::utf32string::iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if ((*iter) == L'#')
			{
				++iter;
				if (iter == mEnd)
				{
					mCurrent = mEnd;
					return false;
				}

				// two hashes in a row
				if ((*iter) == L'#')
				{
					mPosition++;
					++iter;
					if (iter == mEnd)
					{
						mCurrent = mEnd;
						return false;
					}

					mCurrent = iter;
					return true;
				}

				// remaining 5 colour characters
				for (size_t pos = 0; pos < 5; pos++)
				{
					++iter;
					if (iter == mEnd)
					{
						mCurrent = mEnd;
						return false;
					}
				}
			}
			else
			{
				mPosition++;
				++iter;
				if (iter == mEnd)
				{
					mCurrent = mEnd;
					return false;
				}

				mCurrent = iter;
				return true;
			}
		}

		return false;
	}

	bool TextIterator::getTagColour(UString& _colour) const
	{
		if (mCurrent == mEnd)
			return false;

		UString::utf32string::iterator iter = mCurrent;

		// we need the last colour
		bool ret = false;
		while (getTagColour(_colour, iter))
		{
			ret = true;
		}

		return ret;
	}

	bool TextIterator::setTagColour(const Colour& _colour)
	{
		if (mCurrent == mEnd)
			return false;
		clearTagColour();
		if (mCurrent == mEnd)
			return false;

		const size_t SIZE = 16;
		wchar_t buff[SIZE];

#ifdef __MINGW32__
		swprintf(
			buff,
			L"#%.2X%.2X%.2X\0",
			(int)(_colour.red * 255),
			(int)(_colour.green * 255),
			(int)(_colour.blue * 255));
#else
		swprintf(
			buff,
			SIZE,
			L"#%.2X%.2X%.2X\0",
			(int)(_colour.red * 255),
			(int)(_colour.green * 255),
			(int)(_colour.blue * 255));
#endif
		UString tmpStr = UString(buff);
		insert(mCurrent, tmpStr.asUTF32());

		return true;
	}

	bool TextIterator::setTagColour(const UString::utf32string& _colour)
	{
		if (mCurrent == mEnd)
			return false;
		clearTagColour();
		if (mCurrent == mEnd)
			return false;

		// check if it looks like a colour tag
		if ((_colour.size() != 7) || (_colour.find(L'#', 1) != MyGUI::UString::npos))
			return false;

		insert(mCurrent, _colour);

		return true;
	}

	bool TextIterator::setTagColour(const UString& _colour)
	{
		return setTagColour(_colour.asUTF32());
	}

	size_t TextIterator::getSize() const
	{
		if (mSize != ITEM_NONE)
			return mSize;
		mSize = mPosition;

		for (UString::utf32string::const_iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if ((*iter) == L'#')
			{
				++iter;
				if (iter == mEnd)
					break;

				// colour tag
				if ((*iter) != L'#')
				{
					// remaining 5 colour characters
					for (size_t pos = 0; pos < 5; pos++)
					{
						++iter;
						if (iter == mEnd)
						{
							--iter;
							break;
						}
					}
					continue;
				}
			}

			mSize++;
		}

		return mSize;
	}

	UString TextIterator::getOnlyText(const UString& _text)
	{
		UString::utf32string ret;
		UString::utf32string text(_text.asUTF32());
		ret.reserve(text.size());

		UString::utf32string::const_iterator end = text.end();
		for (UString::utf32string::const_iterator iter = text.begin(); iter != end; ++iter)
		{
			if ((*iter) == L'#')
			{
				++iter;
				if (iter == end)
					break;

				// colour tag
				if ((*iter) != L'#')
				{
					// remaining 5 colour characters
					for (size_t pos = 0; pos < 5; pos++)
					{
						++iter;
						if (iter == end)
						{
							--iter;
							break;
						}
					}
					continue;
				}
			}

			ret.push_back(*iter);
		}

		return UString(ret);
	}

	bool TextIterator::getTagColour(UString& _colour, UString::utf32string::iterator& _iter) const
	{
		if ((_iter == mEnd) || ((*_iter) != L'#'))
			return false;

		++_iter;
		if ((_iter == mEnd) || ((*_iter) == L'#'))
			return false;

		// take the colour
		wchar_t buff[16] = L"#FFFFFF\0";
		buff[1] = (wchar_t)(*_iter);
		for (size_t pos = 2; pos < 7; pos++)
		{
			++_iter;
			if (_iter == mEnd)
				return false;
			buff[pos] = (wchar_t)(*_iter);
		}

		// move to next tag or character
		++_iter;

		_colour = buff;
		return true;
	}

	void TextIterator::clearNewLine(UString& _text)
	{
		for (UString::iterator iter = _text.begin(); iter != _text.end(); iter.moveNext())
		{
			auto character = iter.getCharacter();
			if (character == FontCodeType::NEL || character == FontCodeType::CR || character == FontCodeType::LF)
			{
				(*iter) = FontCodeType::Space;
			}
		}
	}

	bool TextIterator::saveStartPoint()
	{
		if (mCurrent == mEnd)
			return false;
		mSave = mCurrent;
		return true;
	}

	UString TextIterator::getFromStart() const
	{
		if (mSave == mEnd)
			return {};
		size_t start = mSave - mText.begin();
		return UString(mText.substr(start, mCurrent - mText.begin() - start));
	}

	bool TextIterator::eraseFromStart()
	{
		if (mSave == mEnd)
			return false;
		mCurrent = erase(mSave, mCurrent);
		mSave = mEnd = mText.end();
		return true;
	}

	void TextIterator::insertText(const UString& _insert, bool _multiLine)
	{
		UString text = _insert;

		normaliseNewLine(text);

		if (!_multiLine)
			clearNewLine(text);

		insert(mCurrent, text.asUTF32());
	}

	void TextIterator::setText(const UString& _text, bool _multiLine)
	{
		clear();

		UString text = _text;

		normaliseNewLine(text);

		if (!_multiLine)
			clearNewLine(text);

		insert(mCurrent, text.asUTF32());
	}

	UString TextIterator::getTextCharInfo(Char _char)
	{
		if (_char == L'#')
			return L"##";
		wchar_t buff[16] = L"_\0";
		buff[0] = (wchar_t)_char;
		return buff;
	}

	UString TextIterator::convertTagColour(const Colour& _colour)
	{
		const size_t SIZE = 16;
		wchar_t buff[SIZE];
//FIXME
#ifdef __MINGW32__
		swprintf(
			buff,
			L"#%.2X%.2X%.2X\0",
			(int)(_colour.red * 255),
			(int)(_colour.green * 255),
			(int)(_colour.blue * 255));
#else
		swprintf(
			buff,
			SIZE,
			L"#%.2X%.2X%.2X\0",
			(int)(_colour.red * 255),
			(int)(_colour.green * 255),
			(int)(_colour.blue * 255));
#endif
		return buff;
	}

	UString TextIterator::toTagsString(const UString& _text)
	{
		// convert to string with tags
		UString text(_text);
		for (UString::iterator iter = text.begin(); iter != text.end(); iter.moveNext())
		{
			// FIXME redo through TextIterator to decouple tag concept from edit
			if (L'#' == (*iter))
				iter = text.insert(iter.moveNext(), L'#');
		}
		return text;
	}

	void TextIterator::insert(UString::utf32string::iterator& _start, const UString::utf32string& _insert)
	{
		mSize = ITEM_NONE;
		// write to history
		if (mHistory)
			mHistory->push_back(TextCommandInfo(_insert, _start - mText.begin(), TextCommandInfo::COMMAND_INSERT));
		// remember iterator position
		size_t pos = _start - mText.begin();
		size_t pos_save = (mSave == mEnd) ? ITEM_NONE : _start - mText.begin();
		mText.insert(_start, _insert.begin(), _insert.end());
		_start = mText.begin() + pos;
		mEnd = mText.end();
		(pos_save == ITEM_NONE) ? mSave = mEnd : mSave = mText.begin() + pos_save;
	}

	UString::utf32string::iterator TextIterator::erase(
		UString::utf32string::iterator _start,
		UString::utf32string::iterator _end)
	{
		mSize = ITEM_NONE;
		// save to history
		size_t start = _start - mText.begin();
		if (mHistory)
			mHistory->push_back(
				TextCommandInfo(mText.substr(start, _end - _start), start, TextCommandInfo::COMMAND_ERASE));
		return mText.erase(_start, _end);
	}

	void TextIterator::clear()
	{
		if (mText.empty())
			return;

		// write to history
		if (mHistory)
			mHistory->push_back(TextCommandInfo(mText, 0, TextCommandInfo::COMMAND_ERASE));

		mText.clear();
		mCurrent = mText.begin();
		mEnd = mSave = mText.end();
		mSize = ITEM_NONE;
	}

	void TextIterator::cutMaxLength(size_t _max)
	{
		if ((mSize != ITEM_NONE) && (mSize <= _max))
			return;
		if (mPosition > _max)
		{
			// will have to count from start
			mSize = mPosition = 0;
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
		}

		mSize = mPosition;

		for (UString::utf32string::iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if ((*iter) == L'#')
			{
				++iter;
				if (iter == mEnd)
					break;

				// colour tag
				if ((*iter) != L'#')
				{
					// remaining 5 colour characters
					for (size_t pos = 0; pos < 5; pos++)
					{
						++iter;
						if (iter == mEnd)
						{
							--iter;
							break;
						}
					}
					continue;
				}
			}

			// check and cut
			if (mSize == _max)
			{
				mPosition = mSize; // save
				mCurrent = erase(iter, mEnd);
				mSave = mEnd = mText.end();
				mSize = mPosition; // restore
				return;
			}

			mSize++;
		}
	}

	void TextIterator::cutMaxLengthFromBeginning(size_t _max)
	{
		// get size without tags
		size_t size = getSize();
		if (size <= _max)
			return;

		size_t diff = size - _max;

		// last colour
		UString::utf32string::iterator iter_colour = mEnd;

		// now traverse from start to find real difference position
		UString::utf32string::iterator iter = mText.begin();
		for (; iter != mEnd; ++iter)
		{
			if ((*iter) == L'#')
			{
				UString::utf32string::iterator save = iter;

				++iter;
				if (iter == mEnd)
					break;

				// colour tag
				if ((*iter) != L'#')
				{
					// remaining 5 colour characters
					for (size_t pos = 0; pos < 5; pos++)
					{
						++iter;
						if (iter == mEnd)
						{
							--iter;
							break;
						}
					}
					// save colour
					iter_colour = save;
				}
				continue;
			}
			if (diff == 0)
				break;
			--diff;
		}

		UString::utf32string colour;
		// if there was a colour, insert it back
		if (iter_colour != mEnd)
		{
			colour.append(iter_colour, iter_colour + size_t(7));
		}

		mCurrent = erase(mText.begin(), iter);
		mEnd = mText.end();
		mSave = mText.end(); //FIXME
		mPosition = 0;
		mSize = _max;

		if (!colour.empty())
			setTagColour(colour);
	}

	void TextIterator::clearTagColour()
	{
		if (mCurrent == mEnd)
			return;

		UString::utf32string::iterator iter = mCurrent;
		UString colour;
		// we need the last colour
		while (getTagColour(colour, iter))
		{
			// must update iterators
			iter = mCurrent = erase(mCurrent, iter);
			mEnd = mText.end();
		}
	}

	size_t TextIterator::getPosition() const
	{
		return mPosition;
	}

	UString TextIterator::getText() const
	{
		return UString(mText);
	}

	void TextIterator::clearText()
	{
		clear();
	}

	UString TextIterator::getTextNewLine()
	{
		return L"\n";
	}

	void TextIterator::normaliseNewLine(UString& _text)
	{
		for (size_t index = 0; index < _text.size(); ++index)
		{
			Char character = _text[index];
			if ((character == FontCodeType::CR) && ((index + 1) < _text.size()) &&
				(_text[index + 1] == FontCodeType::LF))
			{
				_text.erase(index, 1);
			}
		}
	}

} // namespace MyGUI
