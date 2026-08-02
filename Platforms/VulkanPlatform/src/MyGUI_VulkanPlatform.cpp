/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanPlatform.h"
#include <cassert>

namespace MyGUI
{

	VulkanPlatform::VulkanPlatform()
	{
		mRenderManager = new VulkanRenderManager();
		mDataManager = new VulkanDataManager();
		mLogManager = new LogManager();
	}

	VulkanPlatform::~VulkanPlatform()
	{
		assert(!mIsInitialise);
		delete mRenderManager;
		delete mDataManager;
		delete mLogManager;
	}

	void VulkanPlatform::initialise(
		VkInstance _instance,
		VkPhysicalDevice _physicalDevice,
		VkDevice _device,
		uint32_t _queueFamily,
		VkQueue _queue,
		VkFormat _colourFormat,
		VkImageLayout _colourImageLayout,
		VulkanImageLoader* _loader,
		std::string_view _logName)
	{
		assert(!mIsInitialise);
		mIsInitialise = true;

		if (!_logName.empty())
			LogManager::getInstance().createDefaultSource(_logName);

		mRenderManager->initialise(
			_instance,
			_physicalDevice,
			_device,
			_queueFamily,
			_queue,
			_colourFormat,
			_colourImageLayout,
			_loader);
		mDataManager->initialise();
	}

	void VulkanPlatform::shutdown()
	{
		assert(mIsInitialise);
		mIsInitialise = false;

		mRenderManager->shutdown();
		mDataManager->shutdown();
	}

	VulkanRenderManager* VulkanPlatform::getRenderManagerPtr() const
	{
		return mRenderManager;
	}

	VulkanDataManager* VulkanPlatform::getDataManagerPtr() const
	{
		return mDataManager;
	}

} // namespace MyGUI
