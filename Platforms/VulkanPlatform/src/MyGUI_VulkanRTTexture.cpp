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
		{
			vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
			MYGUI_PLATFORM_EXCEPT("Failed to allocate command buffer");
		}

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if (vkCreateFence(mDevice, &fenceInfo, nullptr, &mFence) != VK_SUCCESS)
		{
			vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mCommandBuffer);
			vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
			MYGUI_PLATFORM_EXCEPT("Failed to create fence");
		}

		mRenderTargetInfo.maximumDepth = 1;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);
	}

	VulkanRTTexture::~VulkanRTTexture()
	{
		if (mPending)
		{
			const VkResult result = vkWaitForFences(mDevice, 1, &mFence, VK_TRUE, UINT64_MAX);
			if (result != VK_SUCCESS)
				MYGUI_PLATFORM_LOG(Error, "Render target completion failed, VkResult=" << int(result));
		}
		if (mFence != VK_NULL_HANDLE)
			vkDestroyFence(mDevice, mFence, nullptr);
		if (mCommandBuffer != VK_NULL_HANDLE)
			vkFreeCommandBuffers(mDevice, mCommandPool, 1, &mCommandBuffer);
		VulkanRenderManager::getInstance().releaseCommandBufferResources(mCommandBuffer);
		if (mFramebuffer != VK_NULL_HANDLE)
			vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
	}

	void VulkanRTTexture::begin()
	{
		if (mPending)
		{
			MYGUI_PLATFORM_ASSERT(
				vkWaitForFences(mDevice, 1, &mFence, VK_TRUE, UINT64_MAX) == VK_SUCCESS,
				"Failed to wait for render target reuse");
			mPending = false;
		}
		MYGUI_PLATFORM_ASSERT(vkResetFences(mDevice, 1, &mFence) == VK_SUCCESS, "Failed to reset render target fence");
		MYGUI_PLATFORM_ASSERT(
			vkResetCommandBuffer(mCommandBuffer, 0) == VK_SUCCESS,
			"Failed to reset render target commands");
		VulkanRenderManager::getInstance().releaseCommandBufferResources(mCommandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		MYGUI_PLATFORM_ASSERT(
			vkBeginCommandBuffer(mCommandBuffer, &beginInfo) == VK_SUCCESS,
			"Failed to begin render target commands");

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

		VkViewport viewport{0.0f, 0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight), 0.0f, 1.0f};
		vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{{0, 0}, {mWidth, mHeight}};
		vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
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

		// Queue ordering and render-pass dependencies make the result visible to subsequent draws.
		// Wait only when reusing this command buffer or destroying the target.
		mPending = true;
	}

	void VulkanRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		VulkanRenderManager& manager = VulkanRenderManager::getInstance();
		manager.renderGeometry(mCommandBuffer, _buffer, _texture, _count);
	}

	const RenderTargetInfo& VulkanRTTexture::getInfo() const
	{
		return mRenderTargetInfo;
	}

} // namespace MyGUI
