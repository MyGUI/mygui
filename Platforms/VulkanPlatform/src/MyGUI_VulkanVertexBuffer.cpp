/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanVertexBuffer.h"
#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanDiagnostic.h"
#include "MyGUI_VertexData.h"

#include <vk_mem_alloc.h>
#include <algorithm>
#include <limits>

namespace MyGUI
{
	namespace
	{

		const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	} // namespace

	struct VulkanVertexBuffer::Storage
	{
		VmaAllocator allocator{};
		VkBuffer buffer{VK_NULL_HANDLE};
		VmaAllocation allocation{};
		size_t capacity{0};
		bool mapped{false};

		~Storage()
		{
			if (mapped)
				vmaUnmapMemory(allocator, allocation);
			if (buffer != VK_NULL_HANDLE)
				vmaDestroyBuffer(allocator, buffer, allocation);
		}
	};

	VulkanVertexBuffer::VulkanVertexBuffer() = default;

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		destroy();
	}

	void VulkanVertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t VulkanVertexBuffer::getVertexCount() const
	{
		return mNeedVertexCount;
	}

	VkBuffer VulkanVertexBuffer::getBuffer() const
	{
		return mStorage ? mStorage->buffer : VK_NULL_HANDLE;
	}

	std::shared_ptr<void> VulkanVertexBuffer::retainStorage() const
	{
		MYGUI_PLATFORM_ASSERT(mStorage && !mStorage->mapped, "Vertex buffer is unavailable for rendering");
		return mStorage;
	}

	Vertex* VulkanVertexBuffer::lock()
	{
		MYGUI_PLATFORM_ASSERT(!mStorage || !mStorage->mapped, "Vertex buffer is already locked");
		if (!mStorage || mNeedVertexCount > mStorage->capacity || mStorage.use_count() > 1)
			resize();

		void* data = nullptr;
		const VkResult result = vmaMapMemory(mStorage->allocator, mStorage->allocation, &data);
		MYGUI_PLATFORM_ASSERT(result == VK_SUCCESS, "Failed to map vertex buffer, VkResult=" << int(result));
		mStorage->mapped = true;
		return static_cast<Vertex*>(data);
	}

	void VulkanVertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mStorage && mStorage->mapped, "Vertex buffer is not locked");
		vmaUnmapMemory(mStorage->allocator, mStorage->allocation);
		mStorage->mapped = false;
	}

	void VulkanVertexBuffer::resize()
	{
		MYGUI_PLATFORM_ASSERT(
			mNeedVertexCount <= (std::numeric_limits<size_t>::max)() / sizeof(Vertex) - VERTEX_BUFFER_REALLOCK_STEP,
			"Vertex buffer is too large");
		const size_t capacity = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		// Recorded draws retain the old allocation, even before their queue submission.
		// Reuse retired storage only after the backend observes GPU completion.
		if (mStorage)
			mAvailable.push_back(std::move(mStorage));
		auto reusable = std::find_if(
			mAvailable.begin(),
			mAvailable.end(),
			[this](const auto& storage) { return storage.use_count() == 1 && storage->capacity >= mNeedVertexCount; });
		if (reusable != mAvailable.end())
		{
			mStorage = std::move(*reusable);
			mAvailable.erase(reusable);
			return;
		}
		// Release undersized retired allocations; live recordings retain theirs independently.
		mAvailable.erase(
			std::remove_if(
				mAvailable.begin(),
				mAvailable.end(),
				[](const auto& storage) { return storage.use_count() == 1; }),
			mAvailable.end());
		auto storage = std::make_shared<Storage>();
		storage->allocator = static_cast<VmaAllocator>(VulkanRenderManager::getInstance().getAllocator());
		storage->capacity = capacity;
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = capacity * sizeof(Vertex);
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		const VkResult result = vmaCreateBuffer(
			storage->allocator,
			&bufferInfo,
			&allocInfo,
			&storage->buffer,
			&storage->allocation,
			nullptr);
		MYGUI_PLATFORM_ASSERT(
			result == VK_SUCCESS,
			"Failed to create vertex buffer, VkResult=" << int(result) << ", size=" << bufferInfo.size);
		mStorage = std::move(storage);
	}

	void VulkanVertexBuffer::destroy()
	{
		mStorage.reset();
		mAvailable.clear();
	}

} // namespace MyGUI
