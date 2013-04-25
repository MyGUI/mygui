/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TextureUtility.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_Bitwise.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	namespace texture_utility
	{

		const IntSize& getTextureSize(const std::string& _texture, bool _cache)
		{
			static std::string prevTexture;
			static IntSize prevSize;

			if (prevTexture == _texture && _cache)
				return prevSize;

			prevTexture.clear();
			prevSize.clear();

			if (_texture.empty())
				return Constants::getZeroIntSize();

			RenderManager& render = RenderManager::getInstance();

			ITexture* texture = render.getTexture(_texture);
			if (texture == nullptr)
			{
				if (!DataManager::getInstance().isDataExist(_texture))
				{
					MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
					return Constants::getZeroIntSize();
				}
				else
				{
					texture = render.createTexture(_texture);
					if (texture == nullptr)
					{
						MYGUI_LOG(Error, "Texture '" + _texture + "' not found");
						return Constants::getZeroIntSize();
					}
					texture->loadFromFile(_texture);
				}
			}

			prevSize = IntSize(texture->getWidth(), texture->getHeight());
			prevTexture = _texture;

#if MYGUI_DEBUG_MODE == 1
			if (!Bitwise::isPO2(prevSize.width) || !Bitwise::isPO2(prevSize.height))
			{
				MYGUI_LOG(Warning, "Texture '" + _texture + "' have non power of two size");
			}
#endif

			return prevSize;
		}

		uint32 toColourARGB(const Colour& _colour)
		{
			uint32 val32 = uint8(_colour.alpha * 255);
			val32 <<= 8;
			val32 += uint8(_colour.red * 255);
			val32 <<= 8;
			val32 += uint8(_colour.green * 255);
			val32 <<= 8;
			val32 += uint8(_colour.blue * 255);
			return val32;
		}

	} // namespace texture_utility

} // namespace MyGUI
