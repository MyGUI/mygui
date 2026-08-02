/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_VERTEX_BUFFER_H_
#define MYGUI_VULKAN_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <vulkan/vulkan.h>

namespace MyGUI
{

	class VulkanVertexBuffer : public IVertexBuffer
	{
	public:
		VulkanVertexBuffer();
		~VulkanVertexBuffer() override;

		void setVertexCount(size_t _value) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		/*internal:*/
		VkBuffer getBuffer() const
		{
			return mBuffer;
		}

	private:
		void resize();
		void destroy();

	private:
		size_t mVertexCount{0};
		size_t mNeedVertexCount{0};
		VkBuffer mBuffer{VK_NULL_HANDLE};
		void* mAllocation{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_VERTEX_BUFFER_H_
