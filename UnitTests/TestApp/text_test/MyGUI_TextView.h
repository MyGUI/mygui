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

	struct CharInfo
	{
		CharInfo() : width(0) { }
		CharInfo(const MyGUI::FloatRect& _rect, int _width) : rect(_rect), width(_width) { }

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

	struct RollBackSave
	{
		RollBackSave() : rollback(false) { }

		void set(
			VectorCharInfo::const_iterator& _space_rollback,
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

		void reset() { rollback = false; }

		VectorCharInfo::const_iterator space_rollback;
		MyGUI::UString::const_iterator space_point;
		bool rollback;
		size_t count;
		int lenght;
	};

	class TextView
	{
	public:
		TextView() :
			mLength(0),
			mFontHeight(0)
		{
		}

		void update(const MyGUI::UString& _text, MyGUI::FontPtr _font, int _height, MyGUI::Align _align, int _maxheight = -1)
		{
			mFontHeight = _height;

			// массив дл€ быстрой конвертации цветов
			static const char convert_colour[64] = { 0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0 };

			mViewSize.clear();

			RollBackSave roll_back;
			MyGUI::IntSize result;
			int width = 0;
			size_t count = 0;
			mLength = 0;
			mLineInfo.clear();
			LineInfo line_info;
			int font_height = _font->getHeightPix();

			MyGUI::UString::const_iterator end = _text.end();
			MyGUI::UString::const_iterator index = _text.begin();

			if (index == end)
				return;

			result.height += _height;

			for (; index!=end; ++index)
			{
				MyGUI::Char character = *index;

				// нова€ строка
				if (character == MyGUI::Font::FONT_CODE_CR
					|| character == MyGUI::Font::FONT_CODE_NEL
					|| character == MyGUI::Font::FONT_CODE_LF)
				{
					if (character == MyGUI::Font::FONT_CODE_CR)
					{
						MyGUI::UString::const_iterator peeki = index;
						peeki ++;
						if ((peeki != end) && (*peeki == MyGUI::Font::FONT_CODE_LF))
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

					// отмен€ем откат
					roll_back.reset();

					continue;
				}
				// тег
				else if (character == L'#')
				{
					// берем следующий символ
					++ index;
					if (index == end) { --index ;continue; } // это защита

					character = *index;
					// если два подр€д, то рисуем один шарп, если нет то мен€ем цвет
					if (character != L'#')
					{
						// парсим первый символ
						Ogre::RGBA colour = convert_colour[(character-48) & 0x3F];

						// и еще п€ть символов после шарпа
						for (char i=0; i<5; i++)
						{
							++ index;
							if (index == end) { --index ;continue; } // это защита
							colour <<= 4;
							colour += convert_colour[ ((*index) - 48) & 0x3F];
						}

						// если нужно, то мен€ем красный и синий компоненты
						//MYGUI_CONVERT_COLOUR(colour, mRenderGL);

						// запоминаем цвет, в верхнем байте единицы
						//mLinesInfo.back().second.push_back( EnumCharInfo(colour, true) );

						continue;
					}
				}

				MyGUI::Font::GlyphInfo* info = 0;
				if (MyGUI::Font::FONT_CODE_SPACE == character)
				{
					roll_back.set(line_info.simbols.end(), index, count, width);
					info = _font->getSpaceGlyphInfo();
				}
				else if (MyGUI::Font::FONT_CODE_TAB == character)
				{
					roll_back.set(line_info.simbols.end(), index, count, width);
					info = _font->getTabGlyphInfo();
				}
				else
				{
					info = _font->getGlyphInfo(character);
				}

				int char_width = info->mPixSize.width;
				if (font_height != _height)
				{
					char_width = char_width * _height / font_height;
					if (!char_width) char_width = 1;
				}

				// перенос слов
				if (_maxheight != -1
					&& (width + char_width) > _maxheight
					&& roll_back.rollback)
				{
					// откатываем до последнего пробела
					width = roll_back.lenght;
					count = roll_back.count;
					index = roll_back.space_point;
					line_info.simbols.erase(line_info.simbols.begin() + (count), line_info.simbols.end());

					// запоминаем место отката, как полную строку
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

					// отмен€ем откат
					roll_back.reset();

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

			// теперь выравниванием строки
			for (VectorLineInfo::iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
			{
				if (_align.isRight())
					line->offset = result.width - line->width;
				else if (_align.isHCenter())
					line->offset = (result.width - line->width) / 2;
			}

			mViewSize = result;
		}

		size_t getCursorPosition(int _left, int _top)
		{
			const int height = mFontHeight;
			size_t result = 0;
			int top = 0;

			for (VectorLineInfo::const_iterator line=mLineInfo.begin(); line!=mLineInfo.end(); ++line)
			{
				// это последн€€ строка
				bool lastline = !(line + 1 != mLineInfo.end());

				// наша строчка
				if (top + height > _top || lastline)
				{
					top += height;
					int left = line->offset;
					int count = 0;

					// ищем символ
					for (VectorCharInfo::const_iterator sim=line->simbols.begin(); sim!=line->simbols.end(); ++sim)
					{
						if ((left + (sim->width / 2)) > _left)
						{
							break;
						}
						left += sim->width;
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
						if (position == _position)
						{
							break;
						}

						position ++;
						left += sim->width;
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
