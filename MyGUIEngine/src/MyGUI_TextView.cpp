/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextView.h"
#include "MyGUI_TextTag.h"

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
		void set(size_t _position, const UString::const_iterator& _space_point, size_t _count, float _width)
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
		size_t position{0};
		UString::const_iterator space_point;
		size_t count{0};
		float width{0};
		bool rollback{false};
	};

	void TextView::update(
		const UString& _text,
		IFont* _font,
		int _height,
		Align _align,
		VertexColourType _format,
		int _maxWidth)
	{
		mFontHeight = _height;

		// array for fast colour conversion
		static const char convert_colour[64] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		mViewSize.clear();

		RollBackPoint roll_back;
		IntSize result;
		float width = 0.0f;
		size_t count = 0;
		mLength = 0;
		// Clear line contents while retaining symbol capacity for reuse
		for (auto& line : mLineInfo)
			line.clear();
		if (mLineInfo.empty())
			mLineInfo.emplace_back();
		size_t current_line = 0;
		int font_height = _font->getDefaultHeight();

		UString::const_iterator end = _text.end();
		UString::const_iterator index = _text.begin();

		result.height += _height;

		Char previousChar = 0;

		const auto nextLine = [&](size_t _breakLength)
		{
			auto& line = mLineInfo[current_line];
			line.width = (int)std::ceil(width);
			line.count = count;
			mLength += count + _breakLength;
			setMax(result.width, line.width);
			result.height += _height;
			width = 0;
			count = 0;
			previousChar = 0;
			roll_back.clear();

			++current_line;
			if (current_line == mLineInfo.size())
				mLineInfo.emplace_back();
			mLineInfo[current_line].startPosition = mLength;
		};

		for (; index != end; ++index)
		{
			auto& line_info = mLineInfo[current_line];
			const auto glyphStart = index;
			Char character = *index;

			// new line
			if (character == FontCodeType::CR || character == FontCodeType::NEL || character == FontCodeType::LF)
			{
				if (character == FontCodeType::CR)
				{
					UString::const_iterator peeki = index;
					++peeki;
					if ((peeki != end) && (*peeki == FontCodeType::LF))
						index = peeki; // skip both as one newline
				}

				nextLine(1);
				continue;
			}
			if (text_detail::advanceColourTag(index, end))
			{
				uint32 colour = 0;
				for (auto digit = glyphStart + 1; digit != index + 1; ++digit)
					colour = (colour << 4) + convert_colour[(*digit - 48) & 0x3F];
				texture_utility::convertColour(colour, _format);
				line_info.symbols.emplace_back(colour);
				continue;
			}
			if (index == end)
				break;
			character = *index;

			const GlyphInfo* info = _font->getGlyphInfo(character);

			if (info == nullptr)
			{
				// Invisible glyphs still occupy a logical position in the editor.
				line_info.symbols.emplace_back();
				++count;
				previousChar = 0;
				continue;
			}

			if (FontCodeType::Space == character || FontCodeType::Tab == character)
			{
				roll_back.set(line_info.symbols.size(), index, count, width);
			}

			float char_width = info->width;
			float char_height = info->height;
			float char_advance = info->advance;
			float char_bearingX = info->bearingX;
			float char_bearingY = info->bearingY;
			float kerning = (previousChar != 0) ? _font->getKerning(previousChar, character) : 0.0f;

			if (_height != font_height)
			{
				float scale = (float)_height / font_height;

				char_width *= scale;
				char_height *= scale;
				char_advance *= scale;
				char_bearingX *= scale;
				char_bearingY *= scale;
				kerning *= scale;
			}

			float char_fullAdvance = char_advance + kerning;

			// word wrap
			if (_maxWidth != -1 && (width + char_fullAdvance) > _maxWidth && (count != 0 || !roll_back.empty()))
			{
				if (!roll_back.empty())
				{
					// roll back to last space
					width = roll_back.getWidth();
					count = roll_back.getCount();
					index = roll_back.getTextIter();
					line_info.symbols.erase(
						line_info.symbols.begin() + roll_back.getPosition(),
						line_info.symbols.end());
				}
				else
				{
					// Retry the complete glyph (including an escaped hash) on the next line.
					index = glyphStart - 1;
				}

				// A word break consumes its space; a soft break consumes no character.
				nextLine(roll_back.empty() ? 0 : 1);

				continue;
			}

			line_info.symbols.emplace_back(
				info->uvRect,
				char_width,
				char_height,
				char_advance,
				char_bearingX,
				char_bearingY,
				kerning);
			width += char_fullAdvance;
			previousChar = character;
			count++;
		}

		mLineInfo.resize(current_line + 1);
		auto& line_info = mLineInfo[current_line];
		line_info.width = (int)std::ceil(width);
		line_info.count = count;
		mLength += line_info.count;

		setMax(result.width, line_info.width);

		// now align lines
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

		for (size_t lineIndex = 0; lineIndex < mLineInfo.size(); ++lineIndex)
		{
			const auto& line = mLineInfo[lineIndex];
			result = line.startPosition;
			bool lastline = lineIndex + 1 == mLineInfo.size();

			if (top + mFontHeight <= _value.top && !lastline)
			{
				top += mFontHeight;
			}
			else
			{
				float left = (float)line.offset;
				int count = 0;

				for (const auto& sym : line.symbols)
				{
					if (sym.isColour())
						continue;

					float fullAdvance = sym.getAdvance() + sym.getKerning();
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

		int top = 0;
		float left = 0.0f;
		for (size_t lineIndex = 0; lineIndex < mLineInfo.size(); ++lineIndex)
		{
			const auto& line = mLineInfo[lineIndex];
			size_t position = line.startPosition;
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
					left += sym.getAdvance() + sym.getKerning();
				}
				break;
			}
			top += mFontHeight;
		}

		return {(int)left, top};
	}

	size_t TextView::getLineStartPosition(size_t _line) const
	{
		return mLineInfo.at(_line).startPosition;
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
