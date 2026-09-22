/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanTexture.h"
#include "MyGUI_FileSystemUtility.h"
#include "MyGUI_VulkanRTTexture.h"
#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanDiagnostic.h"

#include <vk_mem_alloc.h>

#include <cstring>

namespace MyGUI
{

	struct VulkanTexture::Storage
	{
		VkDevice device{VK_NULL_HANDLE};
		VmaAllocator allocator{};
		VkDescriptorPool descriptorPool{VK_NULL_HANDLE};
		VkImage image{VK_NULL_HANDLE};
		VkImageView imageView{VK_NULL_HANDLE};
		VkDescriptorSet descriptorSet{VK_NULL_HANDLE};
		VkDescriptorSet pointDescriptorSet{VK_NULL_HANDLE};
		VmaAllocation allocation{};

		~Storage()
		{
			if (descriptorSet != VK_NULL_HANDLE)
				vkFreeDescriptorSets(device, descriptorPool, 1, &descriptorSet);
			if (pointDescriptorSet != VK_NULL_HANDLE)
				vkFreeDescriptorSets(device, descriptorPool, 1, &pointDescriptorSet);
			if (imageView != VK_NULL_HANDLE)
				vkDestroyImageView(device, imageView, nullptr);
			if (image != VK_NULL_HANDLE)
				vmaDestroyImage(allocator, image, allocation);
		}
	};

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
		MYGUI_PLATFORM_ASSERT(!mStorage, "Texture already exist");

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

		mStorage = createImage();

		if (_data)
		{
			uploadData(_data);
		}
	}

	std::shared_ptr<VulkanTexture::Storage> VulkanTexture::createImage()
	{
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();
		auto storage = std::make_shared<Storage>();
		storage->device = manager.getDevice();
		storage->allocator = static_cast<VmaAllocator>(manager.getAllocator());
		storage->descriptorPool = manager.getDescriptorPool();

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
				&storage->image,
				&storage->allocation,
				nullptr) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create texture image");

		storage->imageView = manager.createImageView(storage->image, VK_FORMAT_B8G8R8A8_UNORM);

		VkDescriptorSetLayout layout = manager.getDescriptorSetLayout();
		VkDescriptorSetAllocateInfo allocSetInfo{};
		allocSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocSetInfo.descriptorPool = manager.getDescriptorPool();
		allocSetInfo.descriptorSetCount = 1;
		allocSetInfo.pSetLayouts = &layout;
		if (vkAllocateDescriptorSets(manager.getDevice(), &allocSetInfo, &storage->descriptorSet) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate descriptor set");

		VkDescriptorImageInfo imageDescInfo{};
		imageDescInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageDescInfo.imageView = storage->imageView;
		imageDescInfo.sampler = manager.getSampler();

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = storage->descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageDescInfo;

		vkUpdateDescriptorSets(manager.getDevice(), 1, &descriptorWrite, 0, nullptr);

		allocSetInfo.descriptorSetCount = 1;
		allocSetInfo.pSetLayouts = &layout;
		if (vkAllocateDescriptorSets(manager.getDevice(), &allocSetInfo, &storage->pointDescriptorSet) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate descriptor set");

		imageDescInfo.sampler = manager.getPointSampler();
		descriptorWrite.dstSet = storage->pointDescriptorSet;

		vkUpdateDescriptorSets(manager.getDevice(), 1, &descriptorWrite, 0, nullptr);
		return storage;
	}

	void VulkanTexture::uploadData(const void* _data)
	{
		// Recorded draws must keep their image contents and descriptors unchanged.
		// A full upload can populate a fresh version without copying the old image.
		auto storage = mStorage.use_count() > 1 ? createImage() : mStorage;
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();
		manager.uploadTextureData(
			storage->image,
			static_cast<uint32_t>(mWidth),
			static_cast<uint32_t>(mHeight),
			mNumElemBytes,
			_data);
		if (storage != mStorage)
		{
			// Keep the IRenderTarget pointer stable for callers that cache it.
			if (mRenderTarget)
				mRenderTarget->setImageView(storage->imageView);
			mStorage = std::move(storage);
		}
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
			delete[] static_cast<unsigned char*>(mBuffer);
			mBuffer = nullptr;
		}

		mStorage.reset();

		mWidth = 0;
		mHeight = 0;
		mDataSize = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
		mLock = false;
		mWriteLock = false;
		mShaderName = "Default";
	}

	void* VulkanTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mStorage, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLock, "Texture is already locked");
		const bool read = _access.isValue(TextureUsage::Read);
		const bool write = _access.isValue(TextureUsage::Write);
		MYGUI_PLATFORM_ASSERT(read || write, "Texture lock requires read or write access");

		std::unique_ptr<unsigned char[]> bytes(new unsigned char[mDataSize]);

		if (read)
		{
			VulkanRenderManager::getInstance().readbackImage(
				mStorage->image,
				static_cast<uint32_t>(mWidth),
				static_cast<uint32_t>(mHeight),
				mNumElemBytes,
				bytes.get());
		}

		mBuffer = bytes.release();
		mWriteLock = write;
		mLock = true;
		return mBuffer;
	}

	void VulkanTexture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		MYGUI_PLATFORM_ASSERT(mStorage, "Texture is not created");

		if (mWriteLock)
			uploadData(mBuffer);
		delete[] static_cast<unsigned char*>(mBuffer);
		mBuffer = nullptr;
		mLock = false;
		mWriteLock = false;
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
			const auto path = MyGUI::utility::toPath(_filename);
			void* data = lock(TextureUsage::Read);
			mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, path);
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
	VkImage VulkanTexture::getImage() const
	{
		return mStorage ? mStorage->image : VK_NULL_HANDLE;
	}

	VkImageView VulkanTexture::getImageView() const
	{
		return mStorage ? mStorage->imageView : VK_NULL_HANDLE;
	}

	VkDescriptorSet VulkanTexture::getDescriptorSet() const
	{
		return mStorage ? mStorage->descriptorSet : VK_NULL_HANDLE;
	}

	VkDescriptorSet VulkanTexture::getPointDescriptorSet() const
	{
		return mStorage ? mStorage->pointDescriptorSet : VK_NULL_HANDLE;
	}

	std::shared_ptr<void> VulkanTexture::retainStorage() const
	{
		return mStorage;
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
				mStorage->imageView,
				static_cast<uint32_t>(mWidth),
				static_cast<uint32_t>(mHeight));
		}

		return mRenderTarget;
	}

} // namespace MyGUI
