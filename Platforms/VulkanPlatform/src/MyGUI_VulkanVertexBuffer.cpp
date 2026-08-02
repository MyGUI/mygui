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

namespace MyGUI
{

	namespace
	{
		const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;
	} // namespace

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

	Vertex* VulkanVertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		MYGUI_PLATFORM_ASSERT(mBuffer, "Vertex buffer is not created");

		void* data = nullptr;
		vmaMapMemory(
			static_cast<VmaAllocator>(VulkanRenderManager::getInstance().getAllocator()),
			static_cast<VmaAllocation>(mAllocation),
			&data);

		return reinterpret_cast<Vertex*>(data);
	}

	void VulkanVertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mBuffer, "Vertex buffer is not created");

		vmaUnmapMemory(
			static_cast<VmaAllocator>(VulkanRenderManager::getInstance().getAllocator()),
			static_cast<VmaAllocation>(mAllocation));
	}

	void VulkanVertexBuffer::resize()
	{
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		const size_t bufferSize = mVertexCount * sizeof(Vertex);
		destroy();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkResult result = vmaCreateBuffer(
			static_cast<VmaAllocator>(VulkanRenderManager::getInstance().getAllocator()),
			&bufferInfo,
			&allocInfo,
			&mBuffer,
			reinterpret_cast<VmaAllocation*>(&mAllocation),
			nullptr);
		if (result != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT(
				"Failed to create vertex buffer, VkResult=" << int(result) << ", size=" << bufferSize);
	}

	void VulkanVertexBuffer::destroy()
	{
		if (mBuffer != VK_NULL_HANDLE)
		{
			vmaDestroyBuffer(
				static_cast<VmaAllocator>(VulkanRenderManager::getInstance().getAllocator()),
				mBuffer,
				static_cast<VmaAllocation>(mAllocation));
			mBuffer = VK_NULL_HANDLE;
			mAllocation = nullptr;
		}
		mVertexCount = 0;
	}

} // namespace MyGUI
