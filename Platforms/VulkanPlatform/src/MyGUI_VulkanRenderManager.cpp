/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanTexture.h"
#include "MyGUI_VulkanVertexBuffer.h"
#include "MyGUI_VulkanDiagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_IDataStream.h"

#include <array>
#include <cstring>

namespace MyGUI
{

	namespace
	{
		VmaAllocator getVmaAllocator(void* _allocator)
		{
			return static_cast<VmaAllocator>(_allocator);
		}

		void transitionImageLayout(
			VkCommandBuffer _commandBuffer,
			VkImage _image,
			VkImageLayout _oldLayout,
			VkImageLayout _newLayout)
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = _oldLayout;
			barrier.newLayout = _newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = _image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;

			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			if (_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}

			vkCmdPipelineBarrier(_commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		}
	} // namespace

	VulkanRenderManager& VulkanRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	VulkanRenderManager* VulkanRenderManager::getInstancePtr()
	{
		return static_cast<VulkanRenderManager*>(RenderManager::getInstancePtr());
	}

	void VulkanRenderManager::initialise(
		VkInstance _instance,
		VkPhysicalDevice _physicalDevice,
		VkDevice _device,
		uint32_t _queueFamily,
		VkQueue _queue,
		VkFormat _colourFormat,
		VkImageLayout _colourImageLayout,
		VulkanImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mInstance = _instance;
		mPhysicalDevice = _physicalDevice;
		mDevice = _device;
		mQueueFamily = _queueFamily;
		mQueue = _queue;
		mColourFormat = _colourFormat;
		mColourImageLayout = _colourImageLayout;
		mImageLoader = _loader;

		mVertexFormat = VertexColourType::ColourABGR;
		mUpdate = false;

		VmaAllocatorCreateInfo allocatorInfo{};
		allocatorInfo.physicalDevice = mPhysicalDevice;
		allocatorInfo.device = mDevice;
		allocatorInfo.instance = mInstance;
		if (vmaCreateAllocator(&allocatorInfo, reinterpret_cast<VmaAllocator*>(&mAllocator)) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create Vulkan memory allocator");

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = mQueueFamily;
		if (vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create Vulkan command pool");

		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		if (vkCreatePipelineCache(mDevice, &cacheInfo, nullptr, &mPipelineCache) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create pipeline cache");

		createRenderPass();
		createRenderTargetRenderPass();
		createDescriptorPool();
		createSampler();

		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = 0;
		layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		layoutBinding.descriptorCount = 1;
		layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &layoutBinding;
		if (vkCreateDescriptorSetLayout(mDevice, &layoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create descriptor set layout");

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;
		if (vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create pipeline layout");

		createWhiteTexture();

		registerShader("Default", "MyGUI_Vulkan_VP.spv", "MyGUI_Vulkan_FP.spv");

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void VulkanRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		vkDeviceWaitIdle(mDevice);

		destroyAllResources();

		if (mWhiteDescriptorSet != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(mDevice, mDescriptorPool, 1, &mWhiteDescriptorSet);
			mWhiteDescriptorSet = VK_NULL_HANDLE;
		}
		if (mWhitePointDescriptorSet != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(mDevice, mDescriptorPool, 1, &mWhitePointDescriptorSet);
			mWhitePointDescriptorSet = VK_NULL_HANDLE;
		}
		if (mWhiteImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(mDevice, mWhiteImageView, nullptr);
			mWhiteImageView = VK_NULL_HANDLE;
		}
		if (mWhiteImage != VK_NULL_HANDLE)
		{
			vmaDestroyImage(
				getVmaAllocator(mAllocator),
				mWhiteImage,
				static_cast<VmaAllocation>(mWhiteImageAllocation));
			mWhiteImage = VK_NULL_HANDLE;
		}
		if (mSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(mDevice, mSampler, nullptr);
			mSampler = VK_NULL_HANDLE;
		}
		if (mPointSampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(mDevice, mPointSampler, nullptr);
			mPointSampler = VK_NULL_HANDLE;
		}
		if (mDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);
			mDescriptorPool = VK_NULL_HANDLE;
		}
		if (mPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
			mPipelineLayout = VK_NULL_HANDLE;
		}
		if (mDescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(mDevice, mDescriptorSetLayout, nullptr);
			mDescriptorSetLayout = VK_NULL_HANDLE;
		}
		if (mRenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
			mRenderPass = VK_NULL_HANDLE;
		}
		if (mRenderTargetRenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(mDevice, mRenderTargetRenderPass, nullptr);
			mRenderTargetRenderPass = VK_NULL_HANDLE;
		}
		if (mPipelineCache != VK_NULL_HANDLE)
		{
			vkDestroyPipelineCache(mDevice, mPipelineCache, nullptr);
			mPipelineCache = VK_NULL_HANDLE;
		}
		if (mCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(mDevice, mCommandPool, nullptr);
			mCommandPool = VK_NULL_HANDLE;
		}
		if (mAllocator != VK_NULL_HANDLE)
		{
			vmaDestroyAllocator(getVmaAllocator(mAllocator));
			mAllocator = VK_NULL_HANDLE;
		}

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* VulkanRenderManager::createVertexBuffer()
	{
		return new VulkanVertexBuffer();
	}

	void VulkanRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void VulkanRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		MYGUI_PLATFORM_ASSERT(mCurrentCommandBuffer, "Vertex buffer is not created");
		renderGeometry(mCurrentCommandBuffer, _buffer, _texture, _count);
	}

	void VulkanRenderManager::renderGeometry(
		VkCommandBuffer _commandBuffer,
		IVertexBuffer* _buffer,
		ITexture* _texture,
		size_t _count)
	{
		const auto* buffer = static_cast<VulkanVertexBuffer*>(_buffer);
		MYGUI_PLATFORM_ASSERT(_commandBuffer, "Command buffer is not created");

		VkDescriptorSet descriptorSet = mNearestSampling ? mWhitePointDescriptorSet : mWhiteDescriptorSet;
		VkPipeline pipeline = mDefaultPipeline;

		if (_texture)
		{
			const auto* texture = static_cast<VulkanTexture*>(_texture);
			if (texture->getImageView() != VK_NULL_HANDLE)
			{
				descriptorSet = mNearestSampling ? texture->getPointDescriptorSet() : texture->getDescriptorSet();
				if (texture->getShaderName() != "Default")
					pipeline = getPipeline(texture->getShaderName());
			}
		}

		VkBuffer vertexBuffer = buffer->getBuffer();
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &vertexBuffer, &offset);
		vkCmdBindPipeline(_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindDescriptorSets(
			_commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			mPipelineLayout,
			0,
			1,
			&descriptorSet,
			0,
			nullptr);

		vkCmdDraw(_commandBuffer, _count, 1, 0, 0);
	}

	void VulkanRenderManager::begin()
	{
		++mReferenceCount;
	}

	void VulkanRenderManager::end()
	{
		--mReferenceCount;
	}

	const RenderTargetInfo& VulkanRenderManager::getInfo() const
	{
		return mInfo;
	}

	const IntSize& VulkanRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType VulkanRenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	bool VulkanRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if (_format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8)
			return true;

		return false;
	}

	void VulkanRenderManager::drawOneFrame(VkCommandBuffer _commandBuffer)
	{
		if (!Gui::getInstancePtr())
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent(time / 1000.0f);

		last_time = now_time;

		mCurrentCommandBuffer = _commandBuffer;
		begin();
		onRenderToTarget(this, mUpdate);
		end();
		mCurrentCommandBuffer = VK_NULL_HANDLE;

		mUpdate = false;
	}

	void VulkanRenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);
		mUpdate = true;
	}

	void VulkanRenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
		{
			vkDestroyPipeline(mDevice, iter->second, nullptr);
			mRegisteredShaders.erase(iter);
		}
		VkPipeline pipeline = createShaderPipeline(_vertexProgramFile, _fragmentProgramFile);
		mRegisteredShaders[_shaderName] = pipeline;
		if (_shaderName == "Default")
			mDefaultPipeline = pipeline;
	}

	VkPipeline VulkanRenderManager::getPipeline(const std::string& _shaderName) const
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			return iter->second;
		return mDefaultPipeline;
	}

	ITexture* VulkanRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		VulkanTexture* texture = new VulkanTexture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
	}

	void VulkanRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* VulkanRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	void VulkanRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();

		for (const auto& pipeline : mRegisteredShaders)
		{
			vkDestroyPipeline(mDevice, pipeline.second, nullptr);
		}
		mRegisteredShaders.clear();
		mDefaultPipeline = VK_NULL_HANDLE;
	}

	VkRenderPass VulkanRenderManager::getRenderPass() const
	{
		return mRenderPass;
	}

	VkRenderPass VulkanRenderManager::getRenderTargetRenderPass() const
	{
		return mRenderTargetRenderPass;
	}

	VkFormat VulkanRenderManager::getColourFormat() const
	{
		return mColourFormat;
	}

	VkImageLayout VulkanRenderManager::getColourImageLayout() const
	{
		return mColourImageLayout;
	}

	void* VulkanRenderManager::getAllocator() const
	{
		return mAllocator;
	}

	VkDevice VulkanRenderManager::getDevice() const
	{
		return mDevice;
	}

	VkCommandPool VulkanRenderManager::getCommandPool() const
	{
		return mCommandPool;
	}

	VkQueue VulkanRenderManager::getQueue() const
	{
		return mQueue;
	}

	VkDescriptorPool VulkanRenderManager::getDescriptorPool() const
	{
		return mDescriptorPool;
	}

	VkDescriptorSetLayout VulkanRenderManager::getDescriptorSetLayout() const
	{
		return mDescriptorSetLayout;
	}

	VkSampler VulkanRenderManager::getSampler() const
	{
		return mSampler;
	}

	VkSampler VulkanRenderManager::getPointSampler() const
	{
		return mPointSampler;
	}

	void VulkanRenderManager::setNearestSampling(bool _value)
	{
		mNearestSampling = _value;
	}

	VkImageView VulkanRenderManager::createImageView(VkImage _image, VkFormat _format)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = _format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView view = VK_NULL_HANDLE;
		if (vkCreateImageView(mDevice, &viewInfo, nullptr, &view) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create image view");

		return view;
	}

	void VulkanRenderManager::uploadTextureData(
		VkImage _image,
		uint32_t _width,
		uint32_t _height,
		uint32_t _bytesPerPixel,
		const void* _data)
	{
		const VkDeviceSize size = static_cast<VkDeviceSize>(_width) * _height * 4;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocCreateInfo{};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VmaAllocation stagingAllocation{};
		if (vmaCreateBuffer(
				getVmaAllocator(mAllocator),
				&bufferInfo,
				&allocCreateInfo,
				&stagingBuffer,
				&stagingAllocation,
				nullptr) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create staging buffer");

		void* mapped = nullptr;
		vmaMapMemory(getVmaAllocator(mAllocator), stagingAllocation, &mapped);

		const auto* src = static_cast<const uint8_t*>(_data);
		auto* dst = static_cast<uint8_t*>(mapped);
		if (_bytesPerPixel == 4)
		{
			memcpy(dst, src, size);
		}
		else
		{
			const size_t pixelCount = static_cast<size_t>(_width) * _height;
			for (size_t i = 0; i < pixelCount; ++i)
			{
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst[3] = 255;
				dst += 4;
				src += 3;
			}
		}
		vmaUnmapMemory(getVmaAllocator(mAllocator), stagingAllocation);

		VkCommandBufferAllocateInfo cmdAllocInfo{};
		cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdAllocInfo.commandPool = mCommandPool;
		cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdAllocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		if (vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &commandBuffer) != VK_SUCCESS)
		{
			vmaDestroyBuffer(getVmaAllocator(mAllocator), stagingBuffer, stagingAllocation);
			MYGUI_PLATFORM_EXCEPT("Failed to allocate command buffer");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		transitionImageLayout(commandBuffer, _image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		VkBufferImageCopy region{};
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.layerCount = 1;
		region.imageExtent = {_width, _height, 1};
		vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		transitionImageLayout(
			commandBuffer,
			_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(mQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mQueue);

		vkFreeCommandBuffers(mDevice, mCommandPool, 1, &commandBuffer);
		vmaDestroyBuffer(getVmaAllocator(mAllocator), stagingBuffer, stagingAllocation);
	}

	void VulkanRenderManager::readbackImage(
		VkImage _image,
		uint32_t _width,
		uint32_t _height,
		uint32_t _bytesPerPixel,
		void* _data)
	{
		const VkDeviceSize size = static_cast<VkDeviceSize>(_width) * _height * 4;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocCreateInfo{};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocCreateInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VmaAllocation stagingAllocation{};
		if (vmaCreateBuffer(
				getVmaAllocator(mAllocator),
				&bufferInfo,
				&allocCreateInfo,
				&stagingBuffer,
				&stagingAllocation,
				nullptr) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create readback buffer");

		VkCommandBufferAllocateInfo cmdAllocInfo{};
		cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdAllocInfo.commandPool = mCommandPool;
		cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdAllocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		if (vkAllocateCommandBuffers(mDevice, &cmdAllocInfo, &commandBuffer) != VK_SUCCESS)
		{
			vmaDestroyBuffer(getVmaAllocator(mAllocator), stagingBuffer, stagingAllocation);
			MYGUI_PLATFORM_EXCEPT("Failed to allocate command buffer");
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		transitionImageLayout(commandBuffer, _image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		VkBufferImageCopy region{};
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.layerCount = 1;
		region.imageExtent = {_width, _height, 1};
		vkCmdCopyImageToBuffer(commandBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stagingBuffer, 1, &region);

		transitionImageLayout(
			commandBuffer,
			_image,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(mQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mQueue);

		void* mapped = nullptr;
		vmaMapMemory(getVmaAllocator(mAllocator), stagingAllocation, &mapped);

		const auto* src = static_cast<const uint8_t*>(mapped);
		auto* dst = static_cast<uint8_t*>(_data);
		if (_bytesPerPixel == 4)
		{
			memcpy(dst, src, size);
		}
		else
		{
			const size_t pixelCount = static_cast<size_t>(_width) * _height;
			for (size_t i = 0; i < pixelCount; ++i)
			{
				dst[0] = src[0];
				dst[1] = src[1];
				dst[2] = src[2];
				dst += 3;
				src += 4;
			}
		}
		vmaUnmapMemory(getVmaAllocator(mAllocator), stagingAllocation);

		vkFreeCommandBuffers(mDevice, mCommandPool, 1, &commandBuffer);
		vmaDestroyBuffer(getVmaAllocator(mAllocator), stagingBuffer, stagingAllocation);
	}

	std::vector<uint8_t> VulkanRenderManager::loadFileContent(const std::string& _file)
	{
		IDataStream* stream = DataManager::getInstance().getData(_file);
		if (stream == nullptr || stream->size() == 0)
		{
			MYGUI_PLATFORM_LOG(Error, "Failed to load file content '" << _file << "'.");
			delete stream;
			return {};
		}

		std::vector<uint8_t> content(stream->size());
		stream->read(content.data(), content.size());
		DataManager::getInstance().freeData(stream);
		return content;
	}

	VkShaderModule VulkanRenderManager::createShaderModule(const std::string& _file)
	{
		std::vector<uint8_t> code = loadFileContent(_file);
		if (code.empty())
			MYGUI_PLATFORM_EXCEPT("Failed to load shader file '" << _file << "'");

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule module = VK_NULL_HANDLE;
		if (vkCreateShaderModule(mDevice, &createInfo, nullptr, &module) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create shader module for '" << _file << "'");

		return module;
	}

	VkPipeline VulkanRenderManager::createShaderPipeline(
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		VkShaderModule vertexModule = createShaderModule(_vertexProgramFile);
		VkShaderModule fragmentModule = createShaderModule(_fragmentProgramFile);

		VkPipelineShaderStageCreateInfo vertexStage{};
		vertexStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexStage.module = vertexModule;
		vertexStage.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentStage{};
		fragmentStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentStage.module = fragmentModule;
		fragmentStage.pName = "main";

		std::array<VkPipelineShaderStageCreateInfo, 2> stages = {vertexStage, fragmentStage};

		VkVertexInputBindingDescription binding{};
		binding.binding = 0;
		binding.stride = sizeof(Vertex);
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::array<VkVertexInputAttributeDescription, 3> attributes{};
		attributes[0].location = 0;
		attributes[0].binding = 0;
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[0].offset = offsetof(Vertex, x);
		attributes[1].location = 1;
		attributes[1].binding = 0;
		attributes[1].format = VK_FORMAT_R8G8B8A8_UNORM;
		attributes[1].offset = offsetof(Vertex, colour);
		attributes[2].location = 2;
		attributes[2].binding = 0;
		attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributes[2].offset = offsetof(Vertex, u);

		VkPipelineVertexInputStateCreateInfo vertexInput{};
		vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInput.vertexBindingDescriptionCount = 1;
		vertexInput.pVertexBindingDescriptions = &binding;
		vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
		vertexInput.pVertexAttributeDescriptions = attributes.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_FALSE;
		depthStencil.depthWriteEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		std::array<VkDynamicState, 2> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(stages.size());
		pipelineInfo.pStages = stages.data();
		pipelineInfo.pVertexInputState = &vertexInput;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = mRenderPass;
		pipelineInfo.subpass = 0;

		VkPipeline pipeline = VK_NULL_HANDLE;
		if (vkCreateGraphicsPipelines(mDevice, mPipelineCache, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			vkDestroyShaderModule(mDevice, vertexModule, nullptr);
			vkDestroyShaderModule(mDevice, fragmentModule, nullptr);
			MYGUI_PLATFORM_EXCEPT("Failed to create graphics pipeline");
		}

		vkDestroyShaderModule(mDevice, vertexModule, nullptr);
		vkDestroyShaderModule(mDevice, fragmentModule, nullptr);

		return pipeline;
	}

	void VulkanRenderManager::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = mColourFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = mColourImageLayout;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create render pass");
	}

	void VulkanRenderManager::createRenderTargetRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderTargetRenderPass) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create render target render pass");
	}

	void VulkanRenderManager::createDescriptorPool()
	{
		const uint32_t poolSize = 16384;
		VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, poolSize}};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = poolSize;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = poolSizes;

		if (vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create descriptor pool");
	}

	void VulkanRenderManager::createSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(mDevice, &samplerInfo, nullptr, &mSampler) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create sampler");

		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;

		if (vkCreateSampler(mDevice, &samplerInfo, nullptr, &mPointSampler) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create point sampler");
	}

	void VulkanRenderManager::createWhiteTexture()
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = 1;
		imageInfo.extent.height = 1;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		if (vmaCreateImage(
				getVmaAllocator(mAllocator),
				&imageInfo,
				&allocInfo,
				&mWhiteImage,
				reinterpret_cast<VmaAllocation*>(&mWhiteImageAllocation),
				nullptr) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create white texture image");

		const uint8_t white[4] = {255, 255, 255, 255};
		uploadTextureData(mWhiteImage, 1, 1, 4, white);

		mWhiteImageView = createImageView(mWhiteImage, VK_FORMAT_R8G8B8A8_UNORM);

		VkDescriptorSetAllocateInfo allocSetInfo{};
		allocSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocSetInfo.descriptorPool = mDescriptorPool;
		allocSetInfo.descriptorSetCount = 1;
		allocSetInfo.pSetLayouts = &mDescriptorSetLayout;
		if (vkAllocateDescriptorSets(mDevice, &allocSetInfo, &mWhiteDescriptorSet) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate descriptor set");

		VkDescriptorImageInfo imageDescInfo{};
		imageDescInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageDescInfo.imageView = mWhiteImageView;
		imageDescInfo.sampler = mSampler;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = mWhiteDescriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageDescInfo;

		vkUpdateDescriptorSets(mDevice, 1, &descriptorWrite, 0, nullptr);

		allocSetInfo.descriptorSetCount = 1;
		allocSetInfo.pSetLayouts = &mDescriptorSetLayout;
		if (vkAllocateDescriptorSets(mDevice, &allocSetInfo, &mWhitePointDescriptorSet) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate descriptor set");

		imageDescInfo.sampler = mPointSampler;
		descriptorWrite.dstSet = mWhitePointDescriptorSet;

		vkUpdateDescriptorSets(mDevice, 1, &descriptorWrite, 0, nullptr);
	}

} // namespace MyGUI
