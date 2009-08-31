/*!
	@file
	@author		Albert Semenov
	@date		07/2009
	@module
*/
#ifndef __MYGUI_TEXT_VIEW_H__
#define __MYGUI_TEXT_VIEW_H__

#include <MyGUI.h>

namespace demo
{

	class CharInfo
	{
	public:
		CharInfo() : width(0) { }
		CharInfo(const MyGUI::FloatRect& _rect, int _width) : rect(_rect), width(_width) { }
		CharInfo(MyGUI::uint32 _colour) : rect(-1, 0, 0, 0), width((int)_colour) { }

		bool isColour() const { return rect.left == -1; }
		int getWidth() const { return width; }
		const MyGUI::FloatRect& getUVRect() const { return rect; }
		MyGUI::uint32 getColour() const { return (MyGUI::uint32)width; }

	private:
		MyGUI::FloatRect rect;
		int width;
	};

	typedef std::vector<CharInfo> VectorCharInfo;

	struct LineInfo
	{
		LineInfo() : width(0), count(0), offset(0) { }
		void clear() { width = 0; count = 0; simbols.clear(); offset = 0; }
		int width;
		int offset;
		size_t count;
		VectorCharInfo simbols;
	};

	typedef std::vector<LineInfo> VectorLineInfo;

	class RollBackPoint
	{
	public:
		RollBackPoint() : rollback(false) { }

		void set(
			VectorCharInfo::iterator& _space_rollback,
			MyGUI::UString::const_iterator& _space_point,
			size_t _count,
			int _length
		)
		{
			space_rollback = _space_rollback;
			space_point = _space_point;
			count = _count;
			lenght = _length;
			rollback = true;
		}

		void clear() { rollback = false; }
		bool empty() { MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid"); return !rollback; }
		int getLenght() { MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid"); return lenght; }
		size_t getCount() { MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid"); return count; }
		VectorCharInfo::iterator getCharInfoIter() { MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid"); return space_rollback; }
		MyGUI::UString::const_iterator getTextIter() { MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid"); return space_point; }

	private:
		VectorCharInfo::iterator space_rollback;
		MyGUI::UString::const_iterator space_point;
		size_t count;
		int lenght;
		bool rollback;
	};

	MYGUI_FORCEINLINE void ConvertColour(MyGUI::uint32& _colour, MyGUI::VertexColourType _format)
	{
		if (_format == MyGUI::VertexColourType::ColourABGR)
			_colour = ((_colour & 0x00FF0000) >> 16) | ((_colour & 0x000000FF) << 16) | (_colour & 0xFF00FF00);
	}

	class TextView
	{
	public:
		TextView() :
			mLength(0),
			mFontHeight(0)
		{
		}

		void update(const MyGUI::UString& _text, MyGUI::IFont* _font, int _height, MyGUI::Align _align, MyGUI::VertexColourType _format, int _maxheight = -1)
		{
			mFontHeight = _height;

			// ������ ��� ������� ����������� ������
			static const char convert_colour[64] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0 };

			mViewSize.clear();

			RollBackPoint roll_back;
			MyGUI::IntSize result;
			int width = 0;
			size_t count = 0;
			mLength = 0;
			mLineInfo.clear();
			LineInfo line_info;
			int font_height = _font->getDefaultHeight();

			MyGUI::UString::const_iterator end = _text.end();
			MyGUI::UString::const_iterator index = _text.begin();

			if (index == end)
				return;

			result.height += _height;

			for (; index!=end; ++index)
			{
				MyGUI::Char character = *index;

				// ����� ������
				if (character == MyGUI::FontCodeType::CR
					|| character == MyGUI::FontCodeType::NEL
					|| character == MyGUI::FontCodeType::LF)
				{
					if (character == MyGUI::FontCodeType::CR)
					{
						MyGUI::UString::const_iterator peeki = index;
						peeki ++;
						if ((peeki != end) && (*peeki == MyGUI::FontCodeType::LF))
							index = peeki; // skip both as one newline
					}

					line_info.width = width;
					line_info.count = count;
					mLength += line_info.count + 1;

					result.height += _height;
					if (result.width < width)
						result.width = width;
					width = 0;
					count = 0;

					mLineInfo.push_back(line_info);
					line_info.clear();

					// �������� �����
					roll_back.clear();

					continue;
				}
				// ���
				else if (character == L'#')
				{
					// ����� ��������� ������
					++ index;
					if (index == end) { --index ;continue; } // ��� ������

					character = *index;
					// ���� ��� ������, �� ������ ���� ����, ���� ��� �� ������ ����
					if (character != L'#')
					{
						// ������ ������ ������
						MyGUI::uint32 colour = convert_colour[(character-48) & 0x3F];

						// � ��� ���� �������� ����� �����
						for (char i=0; i<5; i++)
						{
							++ index;
							if (index == end) { --index ;continue; } // ��� ������
							colour <<= 4;
							colour += convert_colour[ ((*index) - 48) & 0x3F ];
						}

						// ���� �����, �� ������ ������� � ����� ����������
						ConvertColour(colour, _format);

						line_info.simbols.push_back( CharInfo(colour) );

						continue;
					}
				}

				MyGUI::GlyphInfo* info = 0;
				if (MyGUI::FontCodeType::Space == character)
				{
					roll_back.set(line_info.simbols.end(), index, count, width);
					info = _font->getSpaceGlyphInfo();
				}
				else if (MyGUI::FontCodeType::Tab == character)
				{
					roll_back.set(line_info.simbols.end(), index, count, width);
					info = _font->getTabGlyphInfo();
				}
				else
				{
					info = _font->getGlyphInfo(character);
				}

				int char_width = info->width;
				if (font_height != _height)
				{
					char_width = char_width * _height / font_height;
					if (!char_width) char_width = 1;
				}

				// ������� ����
				if (_maxheight != -1
					&& (width + char_width) > _maxheight
					&& !roll_back.empty())
				{
					// ���������� �� ���������� �������
					width = roll_back.getLenght();
					count = roll_back.getCount();
					index = roll_back.getTextIter();
					line_info.simbols.erase(roll_back.getCharInfoIter(), line_info.simbols.end());

					// ���������� ����� ������, ��� ������ ������
					line_info.width = width;
					line_info.count = count;
					mLength += line_info.count + 1;

					result.height += _height;
					if (result.width < width)
						result.width = width;
					width = 0;
					count = 0;

					mLineInfo.push_back(line_info);
					line_info.clear();

					// �������� �����
					roll_back.clear();

					continue;
				}

				line_info.simbols.push_back(CharInfo(info->uvRect, char_width));
				width += char_width;
				count ++;
			}

			line_info.width = width;
			line_info.count = count;
			mLength += line_info.count;

			mLineInfo.push_back(line_info);

			if (result.width < width)
				result.width = width;

			// ������ ������������� ������
			for (VectorLineInfo::iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
			{
				if (_align.isRight())
					line->offset = result.width - line->width;
				else if (_align.isHCenter())
					line->offset = (result.width - line->width) / 2;
			}

			mViewSize = result;
		}

		size_t getCursorPosition(const MyGUI::IntPoint& _value)
		{
			const int height = mFontHeight;
			size_t result = 0;
			int top = 0;

			for (VectorLineInfo::const_iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
			{
				// ��� ��������� ������
				bool lastline = !(line + 1 != mLineInfo.end());

				// ���� �������
				if (top + height > _value.top || lastline)
				{
					top += height;
					int left = line->offset;
					int count = 0;

					// ���� ������
					for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
					{
						if (sim->isColour())
							continue;

						if ((left + (sim->getWidth() / 2)) > _value.left)
						{
							break;
						}
						left += sim->getWidth();
						count ++;
					}

					result += count;
					break;
				}

				if (!lastline)
				{
					top += height;
					result += line->count + 1;
				}
			}

			return result;
		}
	
		MyGUI::IntPoint getCursorPoint(size_t _position)
		{
			if (_position >= mLength + 1) _position = mLength;

			size_t position = 0;
			int top = 0;
			int left = 0;
			for (VectorLineInfo::const_iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
			{
				left = line->offset;
				if (position + line->count >= _position)
				{
					for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
					{
						if (sim->isColour())
							continue;

						if (position == _position)
							break;

						position ++;
						left += sim->getWidth();
					}
					break;
				}
				position += line->count + 1;
				top += mFontHeight;
			}

			return MyGUI::IntPoint(left, top);
		}

		const MyGUI::IntSize& getViewSize() { return mViewSize; }
		size_t getTextLength() { return mLength; }
		const VectorLineInfo& getData() { return mLineInfo; }

	private:
		MyGUI::IntSize mViewSize;
		size_t mLength;
		VectorLineInfo mLineInfo;
		int mFontHeight;
	};


} // namespace demo

#endif // __MYGUI_TEXT_VIEW_H__
