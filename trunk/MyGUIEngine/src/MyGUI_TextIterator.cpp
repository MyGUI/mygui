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
		else if (mFirst) {mFirst=false;return true;}

		// ������ �� ��������� ������ ����������� ��� ����
		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == L'#') {

				// ��������� ������
				++ iter;
				if (iter == mEnd) {
					mCurrent = mEnd;
					return false;
				}

				// ��� ������� ������
				if ((*iter) == L'#') {

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
	Ogre::UTFString TextIterator::getTagColour(bool _clear)
	{
		if (mCurrent == mEnd) return L"";

		Ogre::UTFString::iterator iter = mCurrent;
		Ogre::UTFString colour;
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
	bool TextIterator::getTagColour(Ogre::UTFString & _colour)
	{
		if (mCurrent == mEnd) return false;

		Ogre::UTFString::iterator iter = mCurrent;

		// ��� ����� ��������� ����
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
		// ������� ��� �����
		clearTagColour();
		// �� ������
		if (mCurrent == mEnd) return false;

		const size_t SIZE = 16;
		Char buff[SIZE];

#ifdef __MINGW32__
		swprintf(buff, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
#else
		swprintf(buff, SIZE, L"#%.2X%.2X%.2X\0", (int)(_colour.r*255), (int)(_colour.g*255), (int)(_colour.b*255));
#endif
		// ���������������� �������
		Ogre::UTFString tmpStr = Ogre::UTFString(buff);
		insert(mCurrent, tmpStr);

		return true;
	}

	bool TextIterator::setTagColour(Ogre::UTFString _colour)
	{
		if (mCurrent == mEnd) return false;
		// ������� ��� �����
		clearTagColour();
		// �� ������
		if (mCurrent == mEnd) return false;

		// ��������� �� ���� ���� ���� ����
		if ( (_colour.size() != 7) || (_colour.find(L'#', 1) != _colour.npos) ) return false;

		// ���������������� �������
		insert(mCurrent, _colour);

		return true;
	}

	// ���������� ������ ������
	size_t TextIterator::getSize()
	{
		if (mSize != ITEM_NONE) return mSize;
		mSize = mPosition;

		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == L'#') {
				// ��������� ������
				++ iter;
				if (iter == mEnd) break;

				// ��� �����
				if ((*iter) != L'#') {
					// ��������� 5 �������� �����
					for (size_t pos=0; pos<5; pos++) {
						++ iter;
						if (iter == mEnd) {
							--iter;
							break;
						}
					}
					continue;
				}
			}

			// ������������ ������
			mSize ++;
		}

		return mSize;
	}

	void TextIterator::cutMaxLength(size_t _max)
	{
		if ( (mSize != ITEM_NONE) && (mSize <= _max) ) return;
		if (mPosition > _max) {
			// �������� ������� �������
			mSize = mPosition = 0;
			mCurrent = mText.begin();
			mEnd = mSave = mText.end();
		}

		mSize = mPosition;

		for (Ogre::UTFString::iterator iter=mCurrent; iter!=mEnd; ++iter) {

			if ((*iter) == L'#') {
				// ��������� ������
				++ iter;
				if (iter == mEnd) break;

				// ��� �����
				if ((*iter) != L'#') {
					// ��������� 5 �������� �����
					for (size_t pos=0; pos<5; pos++) {
						++ iter;
						if (iter == mEnd) {
							-- iter;
							break;
						}
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

	// ���������� ����� ��� �����
	Ogre::UTFString TextIterator::getOnlyText(const Ogre::UTFString& _text)
	{
		Ogre::UTFString ret;
		ret.reserve(_text.size());

		Ogre::UTFString::const_iterator end = _text.end();
		for (Ogre::UTFString::const_iterator iter=_text.begin(); iter!=end; ++iter) {

			if ((*iter) == L'#') {
				// ��������� ������
				++ iter;
				if (iter == end) break;

				// ��� �����
				if ((*iter) != L'#') {
					// ��������� 5 �������� �����
					for (size_t pos=0; pos<5; pos++) {
						++ iter;
						if (iter == end) {
							--iter;
							break;
						}
					}
					continue;
				}
			}

			// ������������ ������
			ret.push_back(*iter);
		}

		return ret;
	}

	// ���������� ����
	bool TextIterator::getTagColour(Ogre::UTFString & _colour, Ogre::UTFString::iterator & _iter)
	{
		if ( (_iter == mEnd) || ((*_iter) != L'#') ) return false;

		// ��������� ������
		++_iter;
		if ( (_iter == mEnd) || ((*_iter) == L'#') ) return false;

		// ����� ����
		Char buff[16] = L"#FFFFFF\0";
		buff[1] = (Char)(*_iter);
		for (size_t pos=2; pos<7; pos++) {
			++_iter;
			if ( _iter == mEnd ) return false;
			buff[pos] = (Char)(*_iter);
		}

		// ������ �� ��������� ��� ��� ������
		++_iter;

		// ���������� ����
		_colour = buff;
		return true;
	}

} // namespace MyGUI
