/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_MaskPickInfo.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	MaskPickInfo::MaskPickInfo() :
		width(0),
		height(0)
	{
	}

	bool MaskPickInfo::load(const std::string& _file)
	{
		if (!DataManager::getInstance().isDataExist(_file))
			return false;

		RenderManager& render = RenderManager::getInstance();
		ITexture* texture = render.createTexture(_file);
		texture->loadFromFile(_file);

		uint8* buffer = (uint8*)texture->lock(TextureUsage::Read);
		if (buffer == 0)
		{
			render.destroyTexture(texture);
			return false;
		}

		size_t pixel_size = texture->getNumElemBytes();

		width = texture->getWidth();
		height = texture->getHeight();
		size_t size = width * height;
		data.resize(size);

		size_t pos = 0;
		for (size_t pos_pix = 0; pos_pix < size; pos_pix++)
		{
			bool white = true;
			for (size_t in_pix = 0; in_pix < pixel_size; in_pix++)
			{
				if (0xFF != buffer[pos])
				{
					white = false;
				}
				pos++;
			}

			data[pos_pix] = white;
		}

		texture->unlock();
		render.destroyTexture(texture);

		return true;
	}

	bool MaskPickInfo::pick(const IntPoint& _point, const IntCoord& _coord) const
	{
		if ((0 == _coord.width) || (0 == _coord.height)) return false;

		int x = ((_point.left * width) - 1) / _coord.width;
		int y = ((_point.top * height) - 1) / _coord.height;

		return 0 != data[(size_t)(y * width + x)];
	}

	bool MaskPickInfo::empty() const
	{
		return data.empty();
	}

} // namespace MyGUI
