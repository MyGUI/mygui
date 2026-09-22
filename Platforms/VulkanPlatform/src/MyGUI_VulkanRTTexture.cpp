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
		mFramebuffer = createFramebuffer(mImageView);

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
		mResources.clear();
		if (mFramebuffer != VK_NULL_HANDLE)
			vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
	}

	VkFramebuffer VulkanRTTexture::createFramebuffer(VkImageView _imageView)
	{
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = mRenderPass;
		info.pAttachments = &_imageView;
		info.attachmentCount = 1;
		info.width = mWidth;
		info.height = mHeight;
		info.layers = 1;
		VkFramebuffer framebuffer = VK_NULL_HANDLE;
		MYGUI_PLATFORM_ASSERT(
			vkCreateFramebuffer(mDevice, &info, nullptr, &framebuffer) == VK_SUCCESS,
			"Failed to create framebuffer");
		return framebuffer;
	}

	void VulkanRTTexture::setImageView(VkImageView _imageView)
	{
		resetCommands();
		const auto framebuffer = createFramebuffer(_imageView);
		vkDestroyFramebuffer(mDevice, mFramebuffer, nullptr);
		mFramebuffer = framebuffer;
		mImageView = _imageView;
	}

	void VulkanRTTexture::resetCommands()
	{
		MYGUI_PLATFORM_ASSERT(!mRecording, "Render target is already recording");
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
		mResources.clear();
	}

	void VulkanRTTexture::begin()
	{
		resetCommands();

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
		mRecording = true;
	}

	void VulkanRTTexture::end()
	{
		vkCmdEndRenderPass(mCommandBuffer);

		if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
			MYGUI_PLATFORM_EXCEPT("Failed to end command buffer");
		mRecording = false;

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
		manager.renderGeometry(mCommandBuffer, _buffer, _texture, _count, mResources, true);
	}

	const RenderTargetInfo& VulkanRTTexture::getInfo() const
	{
		return mRenderTargetInfo;
	}

} // namespace MyGUI
