/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FONT_DATA_H_
#define MYGUI_FONT_DATA_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

#include <unordered_map>
#include <utility>

namespace MyGUI
{

	namespace FontCodeType
	{

		enum Enum : MyGUI::Char
		{
			Tab = 0x0009,
			LF = 0x000A,
			CR = 0x000D,
			Space = 0x0020,
			NEL = 0x0085,

			// The following are special code points. These are used represent displayable text elements that do not correspond to
			// any actual Unicode code point. To prevent collisions, they must be defined with values higher than that of the
			// highest valid Unicode code point (0x10FFFF as of Unicode 6.1).
			Selected = 0xFFFFFFFC, // Used for rendering text selections when they have input focus.
			SelectedBack = 0xFFFFFFFD, // Used for rendering text selections when they don't have input focus.
			Cursor = 0xFFFFFFFE, // Used for rendering the blinking text cursor.
			NotDefined =
				0xFFFFFFFF // Used to render substitute glyphs for characters that aren't supported by the current font.
		};

	}

	struct GlyphInfo
	{
		GlyphInfo(
			Char _codePoint = 0U,
			float _width = 0.0f,
			float _height = 0.0f,
			float _advance = 0.0f,
			float _bearingX = 0.0f,
			float _bearingY = 0.0f,
			const FloatRect& _uvRect = FloatRect()) :
			codePoint(_codePoint),
			width(_width),
			height(_height),
			advance(_advance),
			bearingX(_bearingX),
			bearingY(_bearingY),
			uvRect(_uvRect)
		{
		}

		Char codePoint;
		float width;
		float height;
		float advance;
		float bearingX;
		float bearingY;
		FloatRect uvRect;
	};

	using VectorGlyphInfo = std::vector<GlyphInfo>;

	struct KerningPairHash
	{
		size_t operator()(const std::pair<Char, Char>& p) const noexcept
		{
			// 32-bit fallback
			if constexpr (sizeof(size_t) == 4)
				return p.first ^ (p.second + 0x9e3779b9u + (p.first << 6) + (p.first >> 2));
			return (static_cast<uint64_t>(p.first) << 32) | p.second;
		}
	};
	// A map of kerning pairs (left code point, right code point) to kerning values in pixels.
	using KerningMap = std::unordered_map<std::pair<Char, Char>, float, KerningPairHash>;

} // namespace MyGUI

#endif // MYGUI_FONT_DATA_H_
