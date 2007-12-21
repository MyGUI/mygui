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

			// ������ �� ��������� ������ ����������� ��� ����
			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {

					// ��������� ������
					++ iter;
					if (iter == mEnd) {
						mCurrent = mEnd;
						return false;
					}

					// ��� ������� ������
					if ((*iter) == '#') {

						// ��������� ������
						mPosition ++;
						iter++;
						if (iter == mEnd) {
							mCurrent = mEnd;
							return false;
						}

						// ��������� �� ��������� ������
						mCurrent = iter;
						return true;
					}

					// ��������� 5 �������� �����
					for (size_t pos=0; pos<5; pos++) {
						// ��������� ������
						++ iter;
						if (iter == mEnd) {
							mCurrent = mEnd;
							return false;
						}
					}

				} else {

					// ������������ ������
					mPosition ++;
					iter++;
					if (iter == mEnd) {
						mCurrent = mEnd;
						return false;
					}

					// ��������� �� ��������� ������
					mCurrent = iter;
					return true;
				}
			}

			return false;
		}

		// ���������� ����
		inline Ogre::DisplayString getTagColour(bool _clear = false)
		{
			if (mCurrent == mEnd) return L"";

			Ogre::DisplayString::iterator iter = mCurrent;
			Ogre::DisplayString colour;
			// ��� ����� ��������� ����
			while (getTagColour(colour, iter)) {
				if (_clear) {
					// ����������� ��������� ���������
					iter = mCurrent = erase(mCurrent, iter);
					mEnd = mText.end();
				}
			};
			return colour;
		}

		// ���������� ����
		inline bool getTagColour(Ogre::DisplayString & _colour)
		{
			if (mCurrent == mEnd) return false;

			Ogre::DisplayString::iterator iter = mCurrent;

			// ��� ����� ��������� ����
			bool ret = false;
			while (true) {
				if (!getTagColour(_colour, iter)) break;
				ret = true;
			};

			return ret;
		}

		// ������� ����
		inline void clearTagColour() {getTagColour(true);}

		bool setTagColour(const Ogre::ColourValue & _colour)
		{
			if (mCurrent == mEnd) return false;
			// ������� ��� �����
			clearTagColour();
			// �� ������
			if (mCurrent == mEnd) return false;

			wchar_t buff[16];
			::wsprintfW(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));

			// ���������������� �������
			Ogre::DisplayString tmpStr = Ogre::DisplayString(buff);
			insert(mCurrent, tmpStr);

			return true;
		}

		bool setTagColour(Ogre::DisplayString _colour)
		{
			if (mCurrent == mEnd) return false;
			// ������� ��� �����
			clearTagColour();
			// �� ������
			if (mCurrent == mEnd) return false;

			// ��������� �� ���� ���� ���� ����
			if ( (_colour.size() != 7) || (_colour.find('#', 1) != _colour.npos) ) return false;

			// ���������������� �������
			insert(mCurrent, _colour);

			return true;
		}

		// ������ ������������ ���� � ������
		inline static Ogre::DisplayString convertTagColour(const Ogre::ColourValue & _colour)
		{
			wchar_t buff[16];
			::wsprintfW(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
			return buff;
		}

		// ��������� ������� ��������
		bool saveStartPoint()
		{
			if (mCurrent == mEnd) return false;
			mSave = mCurrent;
			return true;
		}

		// ���������� ������ �� ������������ ��������� �� ��������
		Ogre::DisplayString getFromStart()
		{
			if (mSave == mEnd) return "";
			size_t start = mSave-mText.begin();
			return mText.substr(start, mCurrent-mText.begin()-start);
		}

		// ������� �� ����������� ����� �� �������
		bool eraseFromStart()
		{
			if (mSave == mEnd) return false;
			mCurrent = erase(mSave, mCurrent);
			mSave = mEnd = mText.end();
			return true;
		}

		// ���������� ������� ������ �������
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

		//������� ���� �����
		void clearText() {clear();}

		// ���������� ������ ������
		size_t getSize()
		{
			if (mSize != SIZE_MAX) return mSize;
			mSize = mPosition;

			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {
					// ��������� ������
					++ iter;
					if (iter == mEnd) break;

					// ��� �����
					if ((*iter) != '#') {
						// ��������� 5 �������� �����
						for (size_t pos=0; pos<5; pos++) {
							++ iter;
							if (iter == mEnd) break;
						}
						continue;
					}
				}

				// ������������ ������
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
			// ������� ��� �������
			clear();
			// � ������ ���������
			Ogre::DisplayString text = _text;
			if (false == _multiLine) clearNewLine(text);
			insert(mCurrent, text);
		}

		void cutMaxLenght(size_t _max)
		{
			if ( (mSize != SIZE_MAX) && (mSize <= _max) ) return;
			if (mPosition > _max) {
				// �������� ������� �������
				mSize = mPosition = 0;
				mCurrent = mText.begin();
				mEnd = mSave = mText.end();
			}

			mSize = mPosition;

			for (Ogre::DisplayString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

				if ((*iter) == '#') {
					// ��������� ������
					++ iter;
					if (iter == mEnd) break;

					// ��� �����
					if ((*iter) != '#') {
						// ��������� 5 �������� �����
						for (size_t pos=0; pos<5; pos++) {
							++ iter;
							if (iter == mEnd) break;
						}
						continue;
					}
				}

				// ��������� � ��������
				if (mSize == _max) {
					mPosition = mSize; // ���������
					mCurrent = erase(iter, mEnd);
					mSave = mEnd = mText.end();
					mSize = mPosition; // ���������������
					return;
				}

				// �����������
				mSize ++;
			}
		}

	private:

		// ���������� ����
		bool getTagColour(Ogre::DisplayString & _colour, Ogre::DisplayString::iterator & _iter)
		{
			if ( (_iter == mEnd) || ((*_iter) != '#') ) return false;

			// ��������� ������
			++_iter;
			if ( (_iter == mEnd) || ((*_iter) == '#') ) return false;

			// ����� ����
			wchar_t buff[16] = L"#FFFFFF\0";
			buff[1] = (wchar_t)(*_iter);
			for (size_t pos=2; pos<7; pos++) {
				++_iter;
				if ( _iter == mEnd ) return false;
				buff[pos] = (wchar_t)(*_iter);
			}

			// ������ �� ��������� ��� ��� ������
			++_iter;

			// ���������� ����
			_colour = buff;
			return true;
		}

		inline void insert(Ogre::DisplayString::iterator & _start, Ogre::DisplayString & _insert)
		{
			// ���������� ������
			mSize = SIZE_MAX;
			// ���������� � �������
			if (mHistory) mHistory->push_back(tagChangeInfo(_insert, _start-mText.begin(), COMMAND_INSERT));
			// ���������� ������� ���������
			size_t pos = _start - mText.begin();
			size_t pos_save = (mSave==mEnd) ? SIZE_MAX : _start - mText.begin();
			// ��������������� ���������
			mText.insert(_start, _insert.begin(), _insert.end());
			// ���������� ���������
			_start = mText.begin() + pos;
			mEnd = mText.end();
			(pos_save==SIZE_MAX) ? mSave = mEnd : mSave = mText.begin() + pos_save;
		}

		inline Ogre::DisplayString::iterator erase(Ogre::DisplayString::iterator _start, Ogre::DisplayString::iterator _end)
		{
			// ���������� ������
			mSize = SIZE_MAX;
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
			mSize = SIZE_MAX;
		}

	private:
		// ����� � ��� ���������
		Ogre::DisplayString mText;
		Ogre::DisplayString::iterator mCurrent, mEnd, mSave;

		// ������� � ������
		size_t mPosition, mSize;
		bool mFirst;

		VectorChangeInfo * mHistory;

	}; // class _MyGUIExport TextIterator

} // namespace MyGUI

#endif // __MYGUI_TEXT_ITERATOR_H__