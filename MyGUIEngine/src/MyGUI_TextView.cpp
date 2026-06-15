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
		void set(
			size_t _position,
			const UString::utf32string::const_iterator& _space_point,
			size_t _count,
			float _width)
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

		UString::utf32string::const_iterator getTextIter() const
		{
			MYGUI_DEBUG_ASSERT(rollback, "rollback point not valid");
			return space_point;
		}

	private:
		size_t position{0};
		UString::utf32string::const_iterator space_point;
		size_t count{0};
		float width{0};
		bool rollback{false};
	};

	void TextView::update(
		const UString::utf32string& _text,
		IFont* _font,
		int _height,
		Align _align,
		VertexColourType _format,
		int _maxWidth)
	{
		mFontHeight = _height;

		// массив для быстрой конвертации цветов
		static const char convert_colour[64] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		mViewSize.clear();

		RollBackPoint roll_back;
		IntSize result;
		float width = 0.0f;
		size_t count = 0;
		mLength = 0;
		mLineInfo.clear();
		LineInfo line_info;
		int font_height = _font->getDefaultHeight();

		UString::utf32string::const_iterator end = _text.end();
		UString::utf32string::const_iterator index = _text.begin();

		/*if (index == end)
			return;*/

		result.height += _height;

		for (; index != end; ++index)
		{
			Char character = *index;

			// new line
			if (character == FontCodeType::CR || character == FontCodeType::NEL || character == FontCodeType::LF)
			{
				if (character == FontCodeType::CR)
				{
					UString::utf32string::const_iterator peeki = index;
					++peeki;
					if ((peeki != end) && (*peeki == FontCodeType::LF))
						index = peeki; // skip both as one newline
				}

				line_info.width = (int)std::ceil(width);
				line_info.count = count;
				mLength += line_info.count + 1;

				result.height += _height;
				setMax(result.width, line_info.width);
				width = 0;
				count = 0;

				mLineInfo.push_back(line_info);
				line_info.clear();

				roll_back.clear();

				continue;
			}
			// tag
			if (character == L'#')
			{
				// check next character
				++index;
				if (index == end)
				{
					--index;
					continue;
				}

				character = *index;
				// "##" converted to visible '#', change colour otherwise
				if (character != L'#')
				{
					// read first character
					uint32 colour = convert_colour[(character - 48) & 0x3F];

					// and 5 more after '#'
					for (char i = 0; i < 5; i++)
					{
						++index;
						if (index == end)
						{
							--index;
							continue;
						}
						colour <<= 4;
						colour += convert_colour[((*index) - 48) & 0x3F];
					}

					// convert to ABGR if we use that colour format
					texture_utility::convertColour(colour, _format);

					line_info.symbols.emplace_back(colour);

					continue;
				}
			}

			const GlyphInfo* info = _font->getGlyphInfo(character);

			if (info == nullptr)
				continue;

			if (FontCodeType::Space == character || FontCodeType::Tab == character)
			{
				roll_back.set(line_info.symbols.size(), index, count, width);
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
			if (_maxWidth != -1 && (width + char_fullAdvance) > _maxWidth)
			{
				if (!roll_back.empty())
				{
					// откатываем до последнего пробела
					width = roll_back.getWidth();
					count = roll_back.getCount();
					index = roll_back.getTextIter();
					line_info.symbols.erase(line_info.symbols.begin() + roll_back.getPosition(), line_info.symbols.end());
				}

				// запоминаем место отката, как полную строку
				line_info.width = (int)std::ceil(width);
				line_info.count = count;
				mLength += line_info.count + 1;

				result.height += _height;
				setMax(result.width, line_info.width);
				width = 0;
				count = 0;

				mLineInfo.push_back(line_info);
				line_info.clear();

				// отменяем откат
				if (!roll_back.empty())
					roll_back.clear();

				continue;
			}

			line_info.symbols
				.emplace_back(info->uvRect, char_width, char_height, char_advance, char_bearingX, char_bearingY);
			width += char_fullAdvance;
			count++;
		}

		line_info.width = (int)std::ceil(width);
		line_info.count = count;
		mLength += line_info.count;

		mLineInfo.push_back(line_info);

		setMax(result.width, line_info.width);

		// теперь выравниванием строки
		for (auto& line : mLineInfo)
		{
			if (_align.isRight())
				line.offset = result.width - line.width;
			else if (_align.isHCenter())
				line.offset = (result.width - line.width) / 2;
		}

		mViewSize = result;
	}

	size_t TextView::getCursorPosition(const IntPoint& _value) const
	{
		size_t result = 0;
		int top = 0;

		for (VectorLineInfo::const_iterator line = mLineInfo.begin(); line != mLineInfo.end(); ++line)
		{
			bool lastline = line + 1 == mLineInfo.end();

			// наша строчка
			if (top + mFontHeight <= _value.top && !lastline)
			{
				top += mFontHeight;
				result += line->count + 1;
			}
			else
			{
				float left = (float)line->offset;
				int count = 0;

				// ищем символ
				for (const auto& sym : line->symbols)
				{
					if (sym.isColour())
						continue;

					float fullAdvance = sym.getAdvance() + sym.getBearingX();
					if (left + fullAdvance / 2.0f > _value.left)
					{
						break;
					}
					left += fullAdvance;
					count++;
				}

				result += count;
				break;
			}
		}

		return result;
	}

	IntPoint TextView::getCursorPoint(size_t _position) const
	{
		setMin(_position, mLength);

		size_t position = 0;
		int top = 0;
		float left = 0.0f;
		for (const auto& line : mLineInfo)
		{
			left = (float)line.offset;
			if (position + line.count >= _position)
			{
				for (const auto& sym : line.symbols)
				{
					if (sym.isColour())
						continue;

					if (position == _position)
						break;

					position++;
					left += sym.getBearingX() + sym.getAdvance();
				}
				break;
			}
			position += line.count + 1;
			top += mFontHeight;
		}

		return {(int)left, top};
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
