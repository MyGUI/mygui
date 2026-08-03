/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanRTTexture.h"
#include "MyGUI_VulkanRenderManager.h"
#include "MyGUI_VulkanDiagnostic.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_ITexture.h"

namespace MyGUI
{

	VulkanRTTexture::VulkanRTTexture(
		VkDevice _device,
		VkCommandPool _commandPool,
		VkQueue _queue,
		VkRenderPass _renderPass,
		VkImageView _imageView,
		uint32_t _width,
		uint32_t _height) :
		mDevice(_device),
		mCommandPool(_commandPool),
		mQueue(_queue),
		mRenderPass(_renderPass),
		mImageView(_imageView),
		mWidth(_width),
		mHeight(_height)
	{
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = mRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = &mImageView;
		framebufferInfo.width = mWidth;
		framebufferInfo.height = mHeight;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr, &mFramebuffer) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create framebuffer");

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;
		if (vkAllocateCommandBuffers(mDevice, &allocInfo, &mCommandBuffer) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to allocate command buffer");

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if (vkCreateFence(mDevice, &fenceInfo, nullptr, &mFence) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to create fence");

		mRenderTargetInfo.maximumDepth = 1;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);
	}

	VulkanRTTexture::~VulkanRTTexture()
	{
		if (mFence != VK_NULL_HANDLE)
			vkDestroyFence(mDevice, mFence, nullptr);
		if (mCommandBuffer != VK_NULL_HANDLE)
			vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mCommandBuffer);
		if (mFramebuffer != VK_NULL_HANDLE)
			vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
	}

	void VulkanRTTexture::begin()
	{
		vkResetCommandBuffer(mCommandBuffer, 0);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(mCommandBuffer, &beginInfo);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = mRenderPass;
		renderPassInfo.framebuffer = mFramebuffer;
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = {mWidth, mHeight};
		VkClearValue clearValue = {{{0.0f, 0.0f, 0.0f, 0.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearValue;
		vkCmdBeginRenderPass(mCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanRTTexture::end()
	{
		vkCmdEndRenderPass(mCommandBuffer);

		if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to end command buffer");

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffer;

		if (vkQueueSubmit(mQueue, 1, &submitInfo, mFence) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to submit render target command buffer");

		vkWaitForFences(mDevice, 1, &mFence, VK_TRUE, UINT64_MAX);
		vkResetFences(mDevice, 1, &mFence);
	}

	void VulkanRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();
		manager.renderGeometry(mCommandBuffer, _buffer, _texture, _count, mWidth, mHeight);
	}

	const RenderTargetInfo& VulkanRTTexture::getInfo() const
	{
		return mRenderTargetInfo;
	}

} // namespace MyGUI
