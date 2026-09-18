/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXT_ITERATOR_H_
#define MYGUI_TEXT_ITERATOR_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Colour.h"
#include "MyGUI_TextChangeHistory.h"
#include "MyGUI_IFont.h"

namespace MyGUI
{

	class MYGUI_EXPORT TextIterator
	{
	private:
		TextIterator();

	public:
		TextIterator(const UString& _text, VectorChangeInfo* _history = nullptr);

		bool moveNext();

		bool getTagColour(UString& _colour) const;

		void clearTagColour();

		bool setTagColour(const Colour& _colour);

		bool setTagColour(const UString& _colour);

		bool saveStartPoint();

		UString getFromStart() const;

		// Delete from saved position to current
		bool eraseFromStart();

		size_t getPosition() const;

		const UString& getText() const;

		void insertText(const UString& _insert, bool _multiLine);

		void clearNewLine(UString& _text);

		void clearText();

		size_t getSize() const;

		void setText(const UString& _text, bool _multiLine);

		void cutMaxLength(size_t _max);

		void cutMaxLengthFromBeginning(size_t _max);

		static UString getOnlyText(const UString& _text);

		static UString getTextNewLine();

		static UString getTextCharInfo(Char _char);

		static UString convertTagColour(const Colour& _colour);

		static UString toTagsString(const UString& _text);

	private:
		bool skipColourTag(UString::iterator& _iter) const;

		void insert(UString::iterator& _start, const UString& _insert);

		UString::iterator erase(UString::iterator _start, UString::iterator _end);

		void clear();

		void normaliseNewLine(UString& _text);

	private:
		UString mText;
		UString::iterator mCurrent, mEnd, mSave;

		size_t mPosition;
		mutable size_t mSize;
		bool mFirst;

		VectorChangeInfo* mHistory;
	};

} // namespace MyGUI

#endif // MYGUI_TEXT_ITERATOR_H_
