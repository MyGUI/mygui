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
#include <memory>
#include <vector>

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
		VkBuffer getBuffer() const;
		// Keep the allocation alive until the recording command buffer is retired.
		std::shared_ptr<void> retainStorage() const;

	private:
		struct Storage;
		void resize();
		void destroy();

		size_t mNeedVertexCount{0};
		std::shared_ptr<Storage> mStorage;
		std::vector<std::shared_ptr<Storage>> mAvailable;
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_VERTEX_BUFFER_H_
