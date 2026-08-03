/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_RT_TEXTURE_H_
#define MYGUI_VULKAN_RT_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_RenderFormat.h"

#include <vulkan/vulkan.h>

namespace MyGUI
{

	class VulkanRTTexture : public IRenderTarget
	{
	public:
		VulkanRTTexture(
			VkDevice _device,
			VkCommandPool _commandPool,
			VkQueue _queue,
			VkRenderPass _renderPass,
			VkImageView _imageView,
			uint32_t _width,
			uint32_t _height);
		~VulkanRTTexture() override;

		void begin() override;
		void end() override;
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		const RenderTargetInfo& getInfo() const override;

	private:
		VkDevice mDevice{VK_NULL_HANDLE};
		VkCommandPool mCommandPool{VK_NULL_HANDLE};
		VkQueue mQueue{VK_NULL_HANDLE};
		VkRenderPass mRenderPass{VK_NULL_HANDLE};
		VkImageView mImageView{VK_NULL_HANDLE};
		VkFramebuffer mFramebuffer{VK_NULL_HANDLE};
		VkCommandBuffer mCommandBuffer{VK_NULL_HANDLE};
		VkFence mFence{VK_NULL_HANDLE};
		uint32_t mWidth{0};
		uint32_t mHeight{0};
		RenderTargetInfo mRenderTargetInfo;
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_RT_TEXTURE_H_
