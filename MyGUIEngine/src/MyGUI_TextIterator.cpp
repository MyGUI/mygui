/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextIterator.h"
#include <cstdio>

namespace MyGUI
{

	namespace
	{

		constexpr size_t ColourTagLength = 7; // #RRGGBB
		constexpr int ColourChannelMax = 255;

	}

	TextIterator::TextIterator() :
		mPosition(0),
		mSize(ITEM_NONE),
		mFirst(true),
		mHistory(nullptr)
	{
	}

	TextIterator::TextIterator(const UString& _text, VectorChangeInfo* _history) :
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

	template<typename Iterator, typename EndIterator>
	bool advanceColorTag(Iterator& _iter, const EndIterator& _end)
	{
		if (*_iter != U'#')
			return false;
		++_iter;
		if (_iter == _end)
			return false;
		if (*_iter != U'#')
		{
			for (size_t pos = 2; pos < ColourTagLength; ++pos)
			{
				++_iter;
				if (_iter == _end)
				{
					--_iter;
					break;
				}
			}
			return true;
		}
		return false;
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
		for (UString::iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if ((*iter) == U'#')
			{
				if (advanceColorTag(iter, mEnd))
					continue;

				if (iter == mEnd)
				{
					mCurrent = mEnd;
					return false;
				}
			}

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

		mCurrent = mEnd;
		return false;
	}

	bool TextIterator::getTagColour(UString& _colour) const
	{
		auto iter = mCurrent;
		auto start = mEnd;
		auto end = mEnd;
		while (true)
		{
			auto tagStart = iter;
			if (!skipColourTag(iter))
				break;
			start = tagStart;
			end = iter;
		}
		if (start == mEnd)
			return false;

		_colour = mText.substr(start - mText.begin(), end - start);
		return true;
	}

	bool TextIterator::setTagColour(const Colour& _colour)
	{
		if (mCurrent == mEnd)
			return false;
		clearTagColour();
		if (mCurrent == mEnd)
			return false;

		insert(mCurrent, convertTagColour(_colour));

		return true;
	}

	bool TextIterator::setTagColour(const UString& _colour)
	{
		if (mCurrent == mEnd)
			return false;
		clearTagColour();
		if (mCurrent == mEnd)
			return false;

		// check if it looks like a colour tag
		if ((_colour.size() != ColourTagLength) || (_colour.find(U'#', 1) != MyGUI::UString::npos))
			return false;

		insert(mCurrent, _colour);

		return true;
	}

	size_t TextIterator::getSize() const
	{
		if (mSize != ITEM_NONE)
			return mSize;
		mSize = mPosition;

		for (UString::const_iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if (advanceColorTag(iter, mEnd))
				continue;

			if (iter == mEnd)
				break;

			mSize++;
		}

		return mSize;
	}

	UString TextIterator::getOnlyText(const UString& _text)
	{
		UString ret;
		ret.reserve(_text.size());

		UString::const_iterator end = _text.end();
		for (UString::const_iterator iter = _text.begin(); iter != end; ++iter)
		{
			if (advanceColorTag(iter, end))
				continue;

			if (iter == end)
				break;

			ret.push_back(*iter);
		}

		return ret;
	}

	bool TextIterator::skipColourTag(UString::iterator& _iter) const
	{
		if ((_iter == mEnd) || (*_iter != U'#'))
			return false;

		++_iter;
		if ((_iter == mEnd) || (*_iter == U'#'))
			return false;

		for (size_t pos = 2; pos < ColourTagLength; ++pos)
		{
			++_iter;
			if (_iter == mEnd)
				return false;
		}
		++_iter;
		return true;
	}

	void TextIterator::clearNewLine(UString& _text)
	{
		for (auto& character : _text)
		{
			if (character == FontCodeType::NEL || character == FontCodeType::CR || character == FontCodeType::LF)
			{
				character = FontCodeType::Space;
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
		return mText.substr(start, mCurrent - mText.begin() - start);
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

		insert(mCurrent, text);
	}

	void TextIterator::setText(const UString& _text, bool _multiLine)
	{
		UString text = _text;
		clear();

		normaliseNewLine(text);

		if (!_multiLine)
			clearNewLine(text);

		insert(mCurrent, text);
	}

	UString TextIterator::getTextCharInfo(Char _char)
	{
		if (_char == '#')
			return "##";
		return UString(_char);
	}

	UString TextIterator::convertTagColour(const Colour& _colour)
	{
		char buff[16];
		std::snprintf(
			buff,
			sizeof(buff),
			"#%.2X%.2X%.2X",
			static_cast<unsigned int>(static_cast<int>(_colour.red * ColourChannelMax)),
			static_cast<unsigned int>(static_cast<int>(_colour.green * ColourChannelMax)),
			static_cast<unsigned int>(static_cast<int>(_colour.blue * ColourChannelMax)));
		return {buff};
	}

	UString TextIterator::toTagsString(const UString& _text)
	{
		// convert to string with tags
		UString text;
		for (auto character : _text)
		{
			text.push_back(character);
			if (character == U'#')
				text.push_back(character);
		}
		return text;
	}

	void TextIterator::insert(UString::iterator& _start, const UString& _insert)
	{
		mSize = ITEM_NONE;
		// write to history
		if (mHistory)
			mHistory->push_back(TextCommandInfo(_insert, _start - mText.begin(), TextCommandInfo::COMMAND_INSERT));
		// remember iterator position
		size_t pos = _start - mText.begin();
		size_t pos_save = (mSave == mEnd) ? ITEM_NONE : _start - mText.begin();
		mText.insert(pos, _insert);
		_start = mText.begin() + pos;
		mEnd = mText.end();
		(pos_save == ITEM_NONE) ? mSave = mEnd : mSave = mText.begin() + pos_save;
	}

	UString::iterator TextIterator::erase(UString::iterator _start, UString::iterator _end)
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

		for (UString::iterator iter = mCurrent; iter != mEnd; ++iter)
		{
			if (advanceColorTag(iter, mEnd))
				continue;

			if (iter == mEnd)
				break;

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
		UString::iterator iter_colour = mEnd;

		// now traverse from start to find real difference position
		UString::iterator iter = mText.begin();
		for (; iter != mEnd; ++iter)
		{
			UString::iterator save = iter;
			if (advanceColorTag(iter, mEnd))
			{
				if (static_cast<size_t>(mEnd - save) >= 7)
					iter_colour = save;
				continue;
			}
			if (iter == mEnd)
				break;
			if (diff == 0)
			{
				// advanceColorTag moves an escaped hash iterator to the second '#'.
				// Keep both characters when the escaped hash is the first retained glyph.
				iter = save;
				break;
			}
			--diff;
		}

		UString colour;
		// if there was a colour, insert it back
		if (iter_colour != mEnd)
		{
			colour = mText.substr(iter_colour - mText.begin(), ColourTagLength);
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

		UString::iterator iter = mCurrent;
		while (skipColourTag(iter))
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

	const UString& TextIterator::getText() const
	{
		return mText;
	}

	void TextIterator::clearText()
	{
		clear();
	}

	UString TextIterator::getTextNewLine()
	{
		return UString(U'\n');
	}

	void TextIterator::normaliseNewLine(UString& _text)
	{
		UString text;
		for (size_t index = 0; index < _text.size(); ++index)
		{
			Char character = _text[index];
			if ((character == FontCodeType::CR) && ((index + 1) < _text.size()) &&
				(_text[index + 1] == FontCodeType::LF))
				continue;
			text.push_back(_text[index]);
		}
		_text = std::move(text);
	}

} // namespace MyGUI
