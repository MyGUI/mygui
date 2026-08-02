#include "Precompiled.h"
#include "BaseManager.h"

#include <SDL_image.h>
#include <SDL_vulkan.h>

#include <cstring>
#include <iostream>
#include <vector>

namespace base
{

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		(void)_windowed;

		uint32_t extensionCount = 0;
		if (SDL_Vulkan_GetInstanceExtensions(mSdlWindow, &extensionCount, nullptr) != SDL_TRUE)
		{
			std::cerr << "Failed to get Vulkan instance extensions: " << SDL_GetError() << std::endl;
			exit(1);
		}
		std::vector<const char*> extensions(extensionCount);
		if (SDL_Vulkan_GetInstanceExtensions(mSdlWindow, &extensionCount, extensions.data()) != SDL_TRUE)
		{
			std::cerr << "Failed to get Vulkan instance extensions: " << SDL_GetError() << std::endl;
			exit(1);
		}

		vkb::InstanceBuilder instanceBuilder;
		auto instanceResult =
			instanceBuilder.set_app_name("MyGUI").require_api_version(1, 1, 0).enable_extensions(extensions).build();
		if (!instanceResult)
		{
			std::cerr << "Failed to create Vulkan instance: " << instanceResult.error().message() << std::endl;
			exit(1);
		}
		mInstance = instanceResult.value();

		if (SDL_Vulkan_CreateSurface(mSdlWindow, mInstance.instance, &mSurface) != SDL_TRUE)
		{
			std::cerr << "Failed to create Vulkan surface: " << SDL_GetError() << std::endl;
			exit(1);
		}

		vkb::PhysicalDeviceSelector deviceSelector(mInstance);
		auto physicalResult =
			deviceSelector.set_surface(mSurface)
				.set_minimum_version(1, 1)
				.add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
				.select();
		if (!physicalResult)
		{
			std::cerr << "Failed to select Vulkan physical device: " << physicalResult.error().message() << std::endl;
			exit(1);
		}
		auto physicalDevice = physicalResult.value();

		vkb::DeviceBuilder deviceBuilder(physicalDevice);
		auto deviceResult = deviceBuilder.build();
		if (!deviceResult)
		{
			std::cerr << "Failed to create Vulkan device: " << deviceResult.error().message() << std::endl;
			exit(1);
		}
		mDevice = deviceResult.value();

		mQueueFamily = mDevice.get_queue_index(vkb::QueueType::graphics).value();
		mQueue = mDevice.get_queue(vkb::QueueType::graphics).value();

		createSwapchain();
		createSyncObjects();
		createScreenShotBuffer();

		return true;
	}

	void BaseManager::destroyRender()
	{
		destroyScreenShotBuffer();
		destroySyncObjects();
		destroySwapchain();
		vkb::destroy_device(mDevice);
		vkb::destroy_surface(mInstance, mSurface);
		vkb::destroy_instance(mInstance);
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::VulkanPlatform();
		setupResources();
		mPlatform->initialise(
			mInstance.instance,
			mDevice.physical_device.physical_device,
			mDevice.device,
			mQueueFamily,
			mQueue,
			mSwapchain.image_format,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			this);

		mRenderPass = mPlatform->getRenderManagerPtr()->getRenderPass();
		createFramebuffers();

		mPlatform->getRenderManagerPtr()->setViewSize((int)mExtent.width, (int)mExtent.height);
	}

	void BaseManager::destroyGuiPlatform()
	{
		destroyFramebuffers();

		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::createSwapchain()
	{
		vkb::SwapchainBuilder swapchainBuilder(mDevice);
		auto swapResult =
			swapchainBuilder.set_desired_format({VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
				.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
				.build();
		if (!swapResult)
		{
			std::cerr << "Failed to create swapchain: " << swapResult.error().message() << std::endl;
			exit(1);
		}
		mSwapchain = swapResult.value();
		mExtent = mSwapchain.extent;
		mImageViews = mSwapchain.get_image_views().value();

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = mQueueFamily;
		if (vkCreateCommandPool(mDevice.device, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
			exit(1);

		mCommandBuffers.resize(mSwapchain.image_count);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());
		if (vkAllocateCommandBuffers(mDevice.device, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
			exit(1);

		mImagesInFlight.resize(mSwapchain.image_count, VK_NULL_HANDLE);
	}

	void BaseManager::destroySwapchain()
	{
		if (mCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(mDevice.device, mCommandPool, nullptr);
			mCommandPool = VK_NULL_HANDLE;
		}
		mCommandBuffers.clear();
		mImagesInFlight.clear();

		mSwapchain.destroy_image_views(mImageViews);
		mImageViews.clear();
		vkb::destroy_swapchain(mSwapchain);
	}

	void BaseManager::createFramebuffers()
	{
		mFramebuffers.resize(mImageViews.size());
		for (size_t i = 0; i < mImageViews.size(); ++i)
		{
			VkImageView attachments[] = {mImageViews[i]};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = mRenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = mExtent.width;
			framebufferInfo.height = mExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(mDevice.device, &framebufferInfo, nullptr, &mFramebuffers[i]) != VK_SUCCESS)
				exit(1);
		}
	}

	void BaseManager::destroyFramebuffers()
	{
		for (VkFramebuffer framebuffer : mFramebuffers)
		{
			if (framebuffer != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(mDevice.device, framebuffer, nullptr);
			}
		}
		mFramebuffers.clear();
	}

	void BaseManager::createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			if (vkCreateSemaphore(mDevice.device, &semaphoreInfo, nullptr, &mImageAvailableSemaphores[i]) !=
					VK_SUCCESS ||
				vkCreateSemaphore(mDevice.device, &semaphoreInfo, nullptr, &mRenderFinishedSemaphores[i]) !=
					VK_SUCCESS ||
				vkCreateFence(mDevice.device, &fenceInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS)
				exit(1);
		}
	}

	void BaseManager::destroySyncObjects()
	{
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			if (mImageAvailableSemaphores[i] != VK_NULL_HANDLE)
				vkDestroySemaphore(mDevice.device, mImageAvailableSemaphores[i], nullptr);
			if (mRenderFinishedSemaphores[i] != VK_NULL_HANDLE)
				vkDestroySemaphore(mDevice.device, mRenderFinishedSemaphores[i], nullptr);
			if (mInFlightFences[i] != VK_NULL_HANDLE)
				vkDestroyFence(mDevice.device, mInFlightFences[i], nullptr);
			mImageAvailableSemaphores[i] = VK_NULL_HANDLE;
			mRenderFinishedSemaphores[i] = VK_NULL_HANDLE;
			mInFlightFences[i] = VK_NULL_HANDLE;
		}
	}

	void BaseManager::createScreenShotBuffer()
	{
		mScreenShotBufferSize = static_cast<VkDeviceSize>(mExtent.width) * mExtent.height * 4;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = mScreenShotBufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(mDevice.device, &bufferInfo, nullptr, &mScreenShotBuffer) != VK_SUCCESS)
			exit(1);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(mDevice.device, mScreenShotBuffer, &memRequirements);

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(mDevice.physical_device.physical_device, &memProperties);

		uint32_t memoryType = VK_MAX_MEMORY_TYPES;
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
		{
			if ((memRequirements.memoryTypeBits & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
				(memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
			{
				memoryType = i;
				break;
			}
		}

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = memoryType;
		if (vkAllocateMemory(mDevice.device, &allocInfo, nullptr, &mScreenShotBufferMemory) != VK_SUCCESS)
			exit(1);

		vkBindBufferMemory(mDevice.device, mScreenShotBuffer, mScreenShotBufferMemory, 0);
	}

	void BaseManager::destroyScreenShotBuffer()
	{
		if (mScreenShotBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(mDevice.device, mScreenShotBuffer, nullptr);
			mScreenShotBuffer = VK_NULL_HANDLE;
		}
		if (mScreenShotBufferMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(mDevice.device, mScreenShotBufferMemory, nullptr);
			mScreenShotBufferMemory = VK_NULL_HANDLE;
		}
	}

	void BaseManager::drawOneFrame()
	{
		vkWaitForFences(mDevice.device, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(
			mDevice.device,
			mSwapchain.swapchain,
			UINT64_MAX,
			mImageAvailableSemaphores[mCurrentFrame],
			VK_NULL_HANDLE,
			&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			resizeRender((int)mExtent.width, (int)mExtent.height);
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			std::cerr << "Failed to acquire swapchain image" << std::endl;
			exit(1);
		}

		// wait if this image is still in flight
		if (mImagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(mDevice.device, 1, &mImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		mImagesInFlight[imageIndex] = mInFlightFences[mCurrentFrame];

		vkResetFences(mDevice.device, 1, &mInFlightFences[mCurrentFrame]);

		VkCommandBuffer commandBuffer = mCommandBuffers[imageIndex];
		vkResetCommandBuffer(commandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mRenderPass;
		renderPassInfo.framebuffer = mFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = mExtent;
		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		mPlatform->getRenderManagerPtr()->drawOneFrame(commandBuffer);

		vkCmdEndRenderPass(commandBuffer);

		if (mScreenShotRequested)
		{
			mScreenShotRequested = false;
			mScreenShotFileName = mScreenShotFile;

			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = mSwapchain.get_images().value()[imageIndex];
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier);

			VkBufferImageCopy region{};
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.layerCount = 1;
			region.imageExtent = {mExtent.width, mExtent.height, 1};
			vkCmdCopyImageToBuffer(
				commandBuffer,
				mSwapchain.get_images().value()[imageIndex],
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				mScreenShotBuffer,
				1,
				&region);

			// transition the image back to PRESENT_SRC so vkQueuePresentKHR can present it
			VkImageMemoryBarrier toPresent{};
			toPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			toPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			toPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			toPresent.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			toPresent.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			toPresent.image = mSwapchain.get_images().value()[imageIndex];
			toPresent.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			toPresent.subresourceRange.levelCount = 1;
			toPresent.subresourceRange.layerCount = 1;
			toPresent.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			toPresent.dstAccessMask = 0;
			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&toPresent);
		}

		vkEndCommandBuffer(commandBuffer);

		VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &mImageAvailableSemaphores[mCurrentFrame];
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &mRenderFinishedSemaphores[mCurrentFrame];

		if (vkQueueSubmit(mQueue, 1, &submitInfo, mInFlightFences[mCurrentFrame]) != VK_SUCCESS)
		{
			std::cerr << "Failed to submit draw command buffer" << std::endl;
			exit(1);
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &mRenderFinishedSemaphores[mCurrentFrame];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mSwapchain.swapchain;
		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(mQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			resizeRender((int)mExtent.width, (int)mExtent.height);
		}

		if (!mScreenShotFileName.empty())
		{
			vkWaitForFences(mDevice.device, 1, &mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);

			void* data = nullptr;
			vkMapMemory(mDevice.device, mScreenShotBufferMemory, 0, mScreenShotBufferSize, 0, &data);
			saveImage((int)mExtent.width, (int)mExtent.height, MyGUI::PixelFormat::R8G8B8A8, data, mScreenShotFileName);
			vkUnmapMemory(mDevice.device, mScreenShotBufferMemory);
			mScreenShotFileName.clear();
		}

		mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		(void)_width;
		(void)_height;

		vkDeviceWaitIdle(mDevice.device);

		destroyFramebuffers();
		destroyScreenShotBuffer();

		mSwapchain.destroy_image_views(mImageViews);
		mImageViews.clear();
		vkb::Swapchain oldSwapchain = mSwapchain;

		vkb::SwapchainBuilder swapchainBuilder(mDevice);
		auto swapResult =
			swapchainBuilder.set_old_swapchain(oldSwapchain)
				.set_desired_format({VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR})
				.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
				.build();
		if (!swapResult)
		{
			std::cerr << "Failed to recreate swapchain: " << swapResult.error().message() << std::endl;
			exit(1);
		}
		mSwapchain = swapResult.value();
		mExtent = mSwapchain.extent;
		mImageViews = mSwapchain.get_image_views().value();

		vkb::destroy_swapchain(oldSwapchain);

		mImagesInFlight.resize(mSwapchain.image_count, VK_NULL_HANDLE);

		// recreate the command pool and command buffers for the new swapchain image count
		vkDestroyCommandPool(mDevice.device, mCommandPool, nullptr);
		mCommandPool = VK_NULL_HANDLE;
		mCommandBuffers.clear();
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = mQueueFamily;
		if (vkCreateCommandPool(mDevice.device, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
			exit(1);
		mCommandBuffers.resize(mSwapchain.image_count);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(mCommandBuffers.size());
		if (vkAllocateCommandBuffers(mDevice.device, &allocInfo, mCommandBuffers.data()) != VK_SUCCESS)
			exit(1);

		createFramebuffers();
		createScreenShotBuffer();

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->setViewSize((int)mExtent.width, (int)mExtent.height);
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void* BaseManager::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::VulkanDataManager::getInstance().getDataPath(_filename);
		void* result = nullptr;
		SDL_Surface* image = nullptr;
		SDL_Surface* cvtImage = nullptr; // converted surface with RGBA/RGB pixel format
		image = IMG_Load(fullname.c_str());
		MYGUI_ASSERT(image != nullptr, "Failed to load image: " + fullname);

		_width = image->w;
		_height = image->h;

		int bpp = image->format->BytesPerPixel;
		if (bpp < 3)
		{
			result = convertPixelData(image, _format);
		}
		else
		{
			Uint32 pixelFmt = bpp == 3 ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_BGRA32;
			cvtImage = SDL_ConvertSurfaceFormat(image, pixelFmt, 0);
			result = convertPixelData(cvtImage, _format);
			SDL_FreeSurface(cvtImage);
		}
		SDL_FreeSurface(image);

		return result;
	}

	void BaseManager::saveImage(
		int _width,
		int _height,
		MyGUI::PixelFormat _format,
		void* _texture,
		const std::string& _filename)
	{
		int bpp = _format.getBytesPerPixel();
		Uint32 fmt = (bpp == 3) ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_BGRA32;
		SDL_Surface* surface =
			SDL_CreateRGBSurfaceWithFormatFrom(_texture, _width, _height, bpp * 8, _width * bpp, fmt);
		IMG_SavePNG(surface, _filename.c_str());
		SDL_FreeSurface(surface);
	}

}
