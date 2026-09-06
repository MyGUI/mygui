/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextureUtility.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI::texture_utility
{

#if !defined(MYGUI_DONT_USE_OBSOLETE)
	const IntSize& getTextureSize(const std::string& _texture, bool _cache)
	{
		return RenderManager::getInstance().getTextureSize(_texture, _cache);
	}
#endif

	uint32 toNativeColour(const Colour& _colour, VertexColourType _format)
	{
		uint32 val32 = uint8(_colour.alpha * 255);
		val32 <<= 8;
		if (_format == VertexColourType::ColourABGR)
		{
			val32 += uint8(_colour.blue * 255);
			val32 <<= 8;
			val32 += uint8(_colour.green * 255);
			val32 <<= 8;
			val32 += uint8(_colour.red * 255);
		}
		else
		{
			val32 += uint8(_colour.red * 255);
			val32 <<= 8;
			val32 += uint8(_colour.green * 255);
			val32 <<= 8;
			val32 += uint8(_colour.blue * 255);
		}
		return val32;
	}

	void convertColour(uint32& _colour, VertexColourType _format)
	{
		if (_format == VertexColourType::ColourABGR)
			_colour = ((_colour & 0x00FF0000) >> 16) | ((_colour & 0x000000FF) << 16) | (_colour & 0xFF00FF00);
	}

} // namespace MyGUI
