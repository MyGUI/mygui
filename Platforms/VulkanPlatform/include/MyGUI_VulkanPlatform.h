/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_PLATFORM_H_
#define MYGUI_VULKAN_PLATFORM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_VulkanDiagnostic.h"
#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanDataManager.h"
#include "MyGUI_VulkanImageLoader.h"
#include "MyGUI_LogManager.h"

#include <vulkan/vulkan.h>

namespace MyGUI
{

	class VulkanPlatform
	{
	public:
		VulkanPlatform();
		~VulkanPlatform();

		/** _colourImageLayout - layout the colour attachment should be transitioned to at the end
			of the render pass (use VK_IMAGE_LAYOUT_PRESENT_SRC_KHR for a swapchain image).
		*/
		void initialise(
			VkInstance _instance,
			VkPhysicalDevice _physicalDevice,
			VkDevice _device,
			uint32_t _queueFamily,
			VkQueue _queue,
			VkFormat _colourFormat,
			VkImageLayout _colourImageLayout,
			VulkanImageLoader* _loader,
			std::string_view _logName = MYGUI_PLATFORM_LOG_FILENAME);
		void shutdown();

		VulkanRenderManager* getRenderManagerPtr() const;
		VulkanDataManager* getDataManagerPtr() const;

	private:
		bool mIsInitialise{false};
		VulkanRenderManager* mRenderManager;
		VulkanDataManager* mDataManager;
		LogManager* mLogManager;
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_PLATFORM_H_
