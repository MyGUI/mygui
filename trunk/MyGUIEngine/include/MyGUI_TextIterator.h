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
		TextIterator(const Ogre::UTFString & _text, VectorChangeInfo * _history = null);

		bool moveNext();

		// ���������� ����
		Ogre::UTFString getTagColour(bool _clear = false);

		// ���������� ����
		bool getTagColour(Ogre::UTFString & _colour);

		// ������� ����
		inline void clearTagColour() {getTagColour(true);}

		bool setTagColour(const Ogre::ColourValue & _colour);

		bool setTagColour(Ogre::UTFString _colour);

		// ��������� ������� ��������
		inline bool saveStartPoint()
		{
			if (mCurrent == mEnd) return false;
			mSave = mCurrent;
			return true;
		}

		// ���������� ������ �� ������������ ��������� �� ��������
		inline Ogre::UTFString getFromStart()
		{
			if (mSave == mEnd) return L"";
			size_t start = mSave-mText.begin();
			return mText.substr(start, mCurrent-mText.begin()-start);
		}

		// ������� �� ����������� ����� �� �������
		inline bool eraseFromStart()
		{
			if (mSave == mEnd) return false;
			mCurrent = erase(mSave, mCurrent);
			mSave = mEnd = mText.end();
			return true;
		}

		// ���������� ������� ������ �������
		inline size_t getPosition() {return mPosition;}

		inline const Ogre::UTFString & getText() {return mText;}

		inline void insertText(const Ogre::UTFString & _insert, bool _multiLine)
		{
			Ogre::UTFString text = _insert;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		inline void clearNewLine(Ogre::UTFString & _text)
		{
			for (Ogre::UTFString::iterator iter=_text.begin(); iter!=_text.end(); ++iter) {
				if ( ((*iter) == Font::FONT_CODE_NEL) || ((*iter) == Font::FONT_CODE_CR) || ((*iter) == Font::FONT_CODE_LF) )
					(*iter) = Font::FONT_CODE_SPACE;
			}
		}

		//������� ���� �����
		inline void clearText() {clear();}

		// ���������� ������ ������
		size_t getSize();

		inline void setText(const Ogre::UTFString & _text, bool _multiLine)
		{
			// ������� ��� �������
			clear();
			// � ������ ���������
			Ogre::UTFString text = _text;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		void cutMaxLength(size_t _max);

		void cutMaxLengthFromBeginning(size_t _max);

		// ���������� ����� ��� �����
		static Ogre::UTFString getOnlyText(const Ogre::UTFString& _text);

		inline static Ogre::UTFString getTextNewLine()
		{
			return L"\n";
		}

		inline static Ogre::UTFString getTextCharInfo(Char _char)
		{
			if (_char == L'#') return L"##";
			Char buff[16] = L"_\0";
			buff[0] = _char;
			return buff;
		}

		// ������ ������������ ���� � ������
		inline static Ogre::UTFString convertTagColour(const Ogre::ColourValue & _colour)
		{
			const size_t SIZE = 16;
			Char buff[SIZE];

#ifdef __MINGW32__
            swprintf(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
#else
            swprintf(buff, SIZE, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
#endif
			return buff;
		}

		inline static Ogre::UTFString toTagsString(const Ogre::UTFString& _text)
		{
			// ����������� � ������ � ������
			Ogre::UTFString text(_text);
			for (Ogre::UTFString::iterator iter=text.begin(); iter!=text.end(); ++iter) {
				// ����� ���������� ����� TextIterator ����� �������� ������� ��� �� �����
				if (L'#' == (*iter)) iter = text.insert(++iter, L'#');
			}
			return text;
		}


	private:

		// ���������� ����
		bool getTagColour(Ogre::UTFString & _colour, Ogre::UTFString::iterator & _iter);

		inline void insert(Ogre::UTFString::iterator & _start, Ogre::UTFString & _insert)
		{
			// ���������� ������
			mSize = ITEM_NONE;
			// ���������� � �������
			if (mHistory) mHistory->push_back(tagChangeInfo(_insert, _start-mText.begin(), COMMAND_INSERT));
			// ���������� ������� ���������
			size_t pos = _start - mText.begin();
			size_t pos_save = (mSave==mEnd) ? ITEM_NONE : _start - mText.begin();
			// ��������������� ���������
			mText.insert(_start, _insert.begin(), _insert.end());
			// ���������� ���������
			_start = mText.begin() + pos;
			mEnd = mText.end();
			(pos_save==ITEM_NONE) ? mSave = mEnd : mSave = mText.begin() + pos_save;
		}

		inline Ogre::UTFString::iterator erase(Ogre::UTFString::iterator _start, Ogre::UTFString::iterator _end)
		{
			// ���������� ������
			mSize = ITEM_NONE;
			// ��������� � �������
			size_t start = _start-mText.begin();
			if (mHistory) mHistory->push_back(tagChangeInfo(mText.substr(start, _end-_start), start, COMMAND_ERASE));
			// ���������� ��������
			return mText.erase(_start, _end);
		}

		inline void clear()
		{
			if (mText.empty()) return;

			// ���������� � �������
			if (mHistory) mHistory->push_back(tagChangeInfo(mText, 0, COMMAND_ERASE));

			// ��� ����������
			mText.clear();
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
			mSize = ITEM_NONE;
		}

	private:
		// ����� � ��� ���������
		Ogre::UTFString mText;
		Ogre::UTFString::iterator mCurrent, mEnd, mSave;

		// ������� � ������
		size_t mPosition, mSize;
		bool mFirst;

		VectorChangeInfo * mHistory;

	}; // class _MyGUIExport TextIterator

} // namespace MyGUI

#endif // __MYGUI_TEXT_ITERATOR_H__
