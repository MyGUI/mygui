/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_IMAGE_LOADER_H_
#define MYGUI_VULKAN_IMAGE_LOADER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class VulkanImageLoader
	{
	public:
		virtual ~VulkanImageLoader() = default;

		virtual void* loadImage(int& _width, int& _height, PixelFormat& _format, const std::string& _filename) = 0;
		virtual void saveImage(
			int _width,
			int _height,
			MyGUI::PixelFormat _format,
			void* _texture,
			const std::string& _filename) = 0;
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_IMAGE_LOADER_H_
