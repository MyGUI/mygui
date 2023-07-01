/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_TEXTURE_UTILITY_H_
#define MYGUI_TEXTURE_UTILITY_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Colour.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI::texture_utility
{

	MYGUI_EXPORT const IntSize& getTextureSize(const std::string& _texture, bool _cache = true);
	/// Convert Colour to 32-bit representation
	MYGUI_EXPORT uint32 toNativeColour(const Colour& _colour, VertexColourType _format);
	/// Convert from 32-bit ARGB to native colour (ABGR or ARGB)
	MYGUI_EXPORT void convertColour(uint32& _colour, VertexColourType _format);

#ifndef MYGUI_DONT_USE_OBSOLETE
	MYGUI_OBSOLETE(
		" is deprecated, use : uint32 texture_utility::toNativeColour(const Colour& _colour, VertexColourType _format)")
	inline uint32 toColourARGB(const Colour& _colour)
	{
		return toNativeColour(_colour, VertexColourType::ColourARGB);
	}
#endif

} // namespace MyGUI

#endif // MYGUI_TEXTURE_UTILITY_H_
