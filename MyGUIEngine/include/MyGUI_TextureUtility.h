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

namespace MyGUI
{

	namespace texture_utility
	{

		MYGUI_EXPORT const IntSize& getTextureSize(const std::string& _texture, bool _cache = true);
		MYGUI_EXPORT uint32 toColourARGB(const Colour& _colour);

		MYGUI_FORCEINLINE void convertColour(uint32& _colour, VertexColourType _format)
		{
			if (_format == VertexColourType::ColourABGR)
				_colour = ((_colour & 0x00FF0000) >> 16) | ((_colour & 0x000000FF) << 16) | (_colour & 0xFF00FF00);
		}


	} // namespace texture_utility

} // namespace MyGUI

#endif // MYGUI_TEXTURE_UTILITY_H_
