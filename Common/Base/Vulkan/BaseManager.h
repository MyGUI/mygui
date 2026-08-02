#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>

#include <MyGUI_VulkanPlatform.h>

#include <array>

namespace base
{

	class BaseManager : public SdlBaseManager, public MyGUI::VulkanImageLoader
	{
	public:
		BaseManager() :
			SdlBaseManager(SDL_WINDOW_VULKAN)
		{
		}
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

		/*internal:*/
		void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename) override;
		void saveImage(
			int _width,
			int _height,
			MyGUI::PixelFormat _format,
			void* _texture,
			const std::string& _filename) override;

	private:
		void createSwapchain();
		void destroySwapchain();
		void createFramebuffers();
		void destroyFramebuffers();
		void createSyncObjects();
		void destroySyncObjects();
		void createScreenShotBuffer();
		void destroyScreenShotBuffer();

	private:
		static constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

		MyGUI::VulkanPlatform* mPlatform = nullptr;

		vkb::Instance mInstance;
		vkb::Device mDevice;
		vkb::Swapchain mSwapchain;
		VkSurfaceKHR mSurface = VK_NULL_HANDLE;
		uint32_t mQueueFamily = 0;
		VkQueue mQueue = VK_NULL_HANDLE;
		VkRenderPass mRenderPass = VK_NULL_HANDLE;
		VkExtent2D mExtent = {0, 0};

		std::vector<VkImageView> mImageViews;
		std::vector<VkFramebuffer> mFramebuffers;

		VkCommandPool mCommandPool = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> mCommandBuffers;

		std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> mImageAvailableSemaphores{};
		std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> mRenderFinishedSemaphores{};
		std::array<VkFence, MAX_FRAMES_IN_FLIGHT> mInFlightFences{};
		std::vector<VkFence> mImagesInFlight;
		uint32_t mCurrentFrame = 0;

		VkBuffer mScreenShotBuffer = VK_NULL_HANDLE;
		VkDeviceMemory mScreenShotBufferMemory = VK_NULL_HANDLE;
		VkDeviceSize mScreenShotBufferSize = 0;
		std::string mScreenShotFileName;
	};

}
