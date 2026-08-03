/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanTexture.h"
#include "MyGUI_VulkanRTTexture.h"
#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanDiagnostic.h"

#include <vk_mem_alloc.h>

#include <cstring>

namespace MyGUI
{

	VulkanTexture::VulkanTexture(const std::string& _name, VulkanImageLoader* _loader) :
		mName(_name),
		mImageLoader(_loader)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		VulkanTexture::destroy();
	}

	const std::string& VulkanTexture::getName() const
	{
		return mName;
	}

	void VulkanTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, nullptr);
	}

	void VulkanTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(mImage == VK_NULL_HANDLE, "Texture already exist");

		if (_format == PixelFormat::R8G8B8)
		{
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::R8G8B8A8)
		{
			mNumElemBytes = 4;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("format not support");
		}

		mWidth = _width;
		mHeight = _height;
		mDataSize = (size_t)_width * _height * mNumElemBytes;

		mOriginalFormat = _format;
		mOriginalUsage = _usage;

		createImage();

		if (_data)
		{
			uploadData(_data);
		}
	}

	void VulkanTexture::createImage()
	{
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = static_cast<uint32_t>(mWidth);
		imageInfo.extent.height = static_cast<uint32_t>(mHeight);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage =
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		if (mOriginalUsage.isValue(TextureUsage::RenderTarget))
		{
			imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		if (vmaCreateImage(
				static_cast<VmaAllocator>(manager.getAllocator()),
				&imageInfo,
				&allocInfo,
				&mImage,
				reinterpret_cast<VmaAllocation*>(&mAllocation),
				nullptr) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create texture image");

		mImageView = manager.createImageView(mImage, VK_FORMAT_B8G8R8A8_UNORM);

		VkDescriptorSetLayout layout = manager.getDescriptorSetLayout();
		VkDescriptorSetAllocateInfo allocSetInfo{};
		allocSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocSetInfo.descriptorPool = manager.getDescriptorPool();
		allocSetInfo.descriptorSetCount = 1;
		allocSetInfo.pSetLayouts = &layout;
		if (vkAllocateDescriptorSets(manager.getDevice(), &allocSetInfo, &mDescriptorSet) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate descriptor set");

		VkDescriptorImageInfo imageDescInfo{};
		imageDescInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageDescInfo.imageView = mImageView;
		imageDescInfo.sampler = manager.getSampler();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = mDescriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageDescInfo;

		vkUpdateDescriptorSets(manager.getDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanTexture::uploadData(const void* _data)
	{
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();
		manager.uploadTextureData(
			mImage,
			static_cast<uint32_t>(mWidth),
			static_cast<uint32_t>(mHeight),
			mNumElemBytes,
			_data);
	}

	void VulkanTexture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (mBuffer)
		{
			delete[] (char*)mBuffer;
			mBuffer = nullptr;
		}

		if (mImage != VK_NULL_HANDLE)
		{
			VulkanRenderManager& manager = VulkanRenderManager::getInstance();

			if (mDescriptorSet != VK_NULL_HANDLE)
			{
				vkFreeDescriptorSets(manager.getDevice(), manager.getDescriptorPool(), 1, &mDescriptorSet);
				mDescriptorSet = VK_NULL_HANDLE;
			}
			if (mImageView != VK_NULL_HANDLE)
			{
				vkDestroyImageView(manager.getDevice(), mImageView, nullptr);
				mImageView = VK_NULL_HANDLE;
			}
			vmaDestroyImage(
				static_cast<VmaAllocator>(manager.getAllocator()),
				mImage,
				static_cast<VmaAllocation>(mAllocation));
			mImage = VK_NULL_HANDLE;
			mAllocation = nullptr;
		}

		mWidth = 0;
		mHeight = 0;
		mDataSize = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
		mLock = false;
		mShaderName = "Default";
	}

	void* VulkanTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mImage, "Texture is not created");

		mBuffer = new unsigned char[mDataSize];

		if (_access == TextureUsage::Read)
		{
			VulkanRenderManager::getInstance().readbackImage(
				mImage,
				static_cast<uint32_t>(mWidth),
				static_cast<uint32_t>(mHeight),
				mNumElemBytes,
				mBuffer);
			mLock = false;
			return mBuffer;
		}

		mLock = true;
		return mBuffer;
	}

	void VulkanTexture::unlock()
	{
		if (!mLock && mBuffer)
		{
			delete[] (char*)mBuffer;
			mBuffer = nullptr;
			return;
		}

		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		MYGUI_PLATFORM_ASSERT(mImage, "Texture is not created");

		uploadData(mBuffer);
		delete[] (char*)mBuffer;
		mBuffer = nullptr;
		mLock = false;
	}

	void VulkanTexture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			void* data = mImageLoader->loadImage(width, height, format, _filename);
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
				delete[] (unsigned char*)data;
			}
		}
	}

	void VulkanTexture::saveToFile(const std::string& _filename)
	{
		if (mImageLoader)
		{
			void* data = lock(TextureUsage::Read);
			mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, _filename);
			unlock();
		}
	}

	void VulkanTexture::setShader(const std::string& _shaderName)
	{
		mShaderName = _shaderName;
	}

	int VulkanTexture::getWidth() const
	{
		return mWidth;
	}

	int VulkanTexture::getHeight() const
	{
		return mHeight;
	}

	bool VulkanTexture::isLocked() const
	{
		return mLock;
	}

	PixelFormat VulkanTexture::getFormat() const
	{
		return mOriginalFormat;
	}

	TextureUsage VulkanTexture::getUsage() const
	{
		return mOriginalUsage;
	}

	size_t VulkanTexture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

	IRenderTarget* VulkanTexture::getRenderTarget()
	{
		if (mRenderTarget == nullptr && mOriginalUsage.isValue(TextureUsage::RenderTarget))
		{
			VulkanRenderManager& manager = VulkanRenderManager::getInstance();
			mRenderTarget = new VulkanRTTexture(
				manager.getDevice(),
				manager.getCommandPool(),
				manager.getQueue(),
				manager.getRenderTargetRenderPass(),
				mImageView,
				static_cast<uint32_t>(mWidth),
				static_cast<uint32_t>(mHeight));
		}

		return mRenderTarget;
	}

} // namespace MyGUI
