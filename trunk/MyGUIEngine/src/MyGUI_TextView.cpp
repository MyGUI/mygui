/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextView.h"

namespace MyGUI
{

	namespace
	{

		template<typename T>
		void setMin(T& _var, const T& _newValue)
		{
			if (_newValue < _var)
				_var = _newValue;
		}

		template<typename T>
		void setMax(T& _var, const T& _newValue)
		{
			if (_var < _newValue)
				_var = _newValue;
		}

	}

	class RollBackPoint
	{
	public:
		RollBackPoint() :
			position(0),
			count(0),
			width(0),
			rollback(false)
		{
		}

		void set(size_t _position, UString::const_iterator& _space_point, size_t _count, float _width)
		{
			position = _position;
			space_point = _space_point;
			count = _count;
			width = _width;
			rollback = true;
		}

		void clear()
		{
			rollback = false;
		}

		bool empty() const
		{
			return !rollback;
		}

		float getWidth() const
		{
			MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid");
			return width;
		}

		size_t getCount() const
		{
			MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid");
			return count;
		}

		size_t getPosition() const
		{
			MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid");
			return position;
		}

		UString::const_iterator getTextIter() const
		{
			MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid");
			return space_point;
		}

	private:
		size_t position;
		UString::const_iterator space_point;
		size_t count;
		float width;
		bool rollback;
	};

	TextView::TextView() :
		mLength(0),
		mFontHeight(0)
	{
	}

	void TextView::update(const UString& _text, IFont* _font, int _height, Align _align, VertexColourType _format, int _maxWidth)
	{
		mFontHeight = _height;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] =
		{
			0,  1,  2,  3,  4,  5,  6, 7, 8, 9, 0, 0, 0, 0, 0, 0,
			0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		mViewSize.clear();

		RollBackPoint roll_back;
		IntSize result;
		float width = 0.0f;
		size_t count = 0;
		mLength = 0;
		mLineInfo.clear();
		LineInfo line_info;
		int font_height = _font->getDefaultHeight();

		UString::const_iterator end = _text.end();
		UString::const_iterator index = _text.begin();

		/*if (index == end)
			return;*/

		result.height += _height;

		for (; index != end; ++index)
		{
			Char character = *index;

			// новая строка
			if (character == FontCodeType::CR
				|| character == FontCodeType::NEL
				|| character == FontCodeType::LF)
			{
				if (character == FontCodeType::CR)
				{
					UString::const_iterator peeki = index;
					++peeki;
					if ((peeki != end) && (*peeki == FontCodeType::LF))
						index = peeki; // skip both as one newline
				}

				line_info.width = (int)ceil(width);
				line_info.count = count;
				mLength += line_info.count + 1;

				result.height += _height;
				setMax(result.width, line_info.width);
				width = 0;
				count = 0;

				mLineInfo.push_back(line_info);
				line_info.clear();

				// отменяем откат
				roll_back.clear();

				continue;
			}
			// тег
			else if (character == L'#')
			{
				// берем следующий символ
				++ index;
				if (index == end)
				{
					--index;    // это защита
					continue;
				}

				character = *index;
				// если два подряд, то рисуем один шарп, если нет то меняем цвет
				if (character != L'#')
				{
					// парсим первый символ
					uint32 colour = convert_colour[(character - 48) & 0x3F];

					// и еще пять символов после шарпа
					for (char i = 0; i < 5; i++)
					{
						++ index;
						if (index == end)
						{
							--index;    // это защита
							continue;
						}
						colour <<= 4;
						colour += convert_colour[ ((*index) - 48) & 0x3F ];
					}

					// если нужно, то меняем красный и синий компоненты
					texture_utility::convertColour(colour, _format);

					line_info.simbols.push_back( CharInfo(colour) );

					continue;
				}
			}

			GlyphInfo* info = _font->getGlyphInfo(character);

			if (info == nullptr)
				continue;

			if (FontCodeType::Space == character)
			{
				roll_back.set(line_info.simbols.size(), index, count, width);
			}
			else if (FontCodeType::Tab == character)
			{
				roll_back.set(line_info.simbols.size(), index, count, width);
			}

			float char_width = info->width;
			float char_height = info->height;
			float char_advance = info->advance;
			float char_bearingX = info->bearingX;
			float char_bearingY = info->bearingY;

			if (_height != font_height)
			{
				float scale = (float)_height / font_height;

				char_width *= scale;
				char_height *= scale;
				char_advance *= scale;
				char_bearingX *= scale;
				char_bearingY *= scale;
			}

			float char_fullAdvance = char_bearingX + char_advance;

			// перенос слов
			if (_maxWidth != -1
				&& (width + char_fullAdvance) > _maxWidth
				&& !roll_back.empty())
			{
				// откатываем до последнего пробела
				width = roll_back.getWidth();
				count = roll_back.getCount();
				index = roll_back.getTextIter();
				line_info.simbols.erase(line_info.simbols.begin() + roll_back.getPosition(), line_info.simbols.end());

				// запоминаем место отката, как полную строку
				line_info.width = (int)ceil(width);
				line_info.count = count;
				mLength += line_info.count + 1;

				result.height += _height;
				setMax(result.width, line_info.width);
				width = 0;
				count = 0;

				mLineInfo.push_back(line_info);
				line_info.clear();

				// отменяем откат
				roll_back.clear();

				continue;
			}

			line_info.simbols.push_back(CharInfo(info->uvRect, char_width, char_height, char_advance, char_bearingX, char_bearingY));
			width += char_fullAdvance;
			count ++;
		}

		line_info.width = (int)ceil(width);
		line_info.count = count;
		mLength += line_info.count;

		mLineInfo.push_back(line_info);

		setMax(result.width, line_info.width);

		// теперь выравниванием строки
		for (VectorLineInfo::iterator line = mLineInfo.begin(); line != mLineInfo.end(); ++line)
		{
			if (_align.isRight())
				line->offset = result.width - line->width;
			else if (_align.isHCenter())
				line->offset = (result.width - line->width) / 2;
		}

		mViewSize = result;
	}

	size_t TextView::getCursorPosition(const IntPoint& _value)
	{
		const int height = mFontHeight;
		size_t result = 0;
		int top = 0;

		for (VectorLineInfo::const_iterator line = mLineInfo.begin(); line != mLineInfo.end(); ++line)
		{
			// это последняя строка
			bool lastline = !(line + 1 != mLineInfo.end());

			// наша строчка
			if (top + height > _value.top || lastline)
			{
				top += height;
				float left = (float)line->offset;
				int count = 0;

				// ищем символ
				for (VectorCharInfo::const_iterator sim = line->simbols.begin(); sim != line->simbols.end(); ++sim)
				{
					if (sim->isColour())
						continue;

					float fullAdvance = sim->getAdvance() + sim->getBearingX();
					if (left + fullAdvance / 2.0f > _value.left)
					{
						break;
					}
					left += fullAdvance;
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

	IntPoint TextView::getCursorPoint(size_t _position)
	{
		setMin(_position, mLength);

		size_t position = 0;
		int top = 0;
		float left = 0.0f;
		for (VectorLineInfo::const_iterator line = mLineInfo.begin(); line != mLineInfo.end(); ++line)
		{
			left = (float)line->offset;
			if (position + line->count >= _position)
			{
				for (VectorCharInfo::const_iterator sim = line->simbols.begin(); sim != line->simbols.end(); ++sim)
				{
					if (sim->isColour())
						continue;

					if (position == _position)
						break;

					position ++;
					left += sim->getBearingX() + sim->getAdvance();
				}
				break;
			}
			position += line->count + 1;
			top += mFontHeight;
		}

		return IntPoint((int)left, top);
	}

	const IntSize& TextView::getViewSize() const
	{
		return mViewSize;
	}

	size_t TextView::getTextLength() const
	{
		return mLength;
	}

	const VectorLineInfo& TextView::getData() const
	{
		return mLineInfo;
	}

} // namespace MyGUI
