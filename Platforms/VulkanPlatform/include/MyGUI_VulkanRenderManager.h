/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_RENDER_MANAGER_H_
#define MYGUI_VULKAN_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_VulkanImageLoader.h"

#include <vulkan/vulkan.h>

namespace MyGUI
{

	class VulkanRenderManager : public RenderManager, public IRenderTarget
	{
	public:
		void initialise(
			VkInstance _instance,
			VkPhysicalDevice _physicalDevice,
			VkDevice _device,
			uint32_t _queueFamily,
			VkQueue _queue,
			VkFormat _colourFormat,
			VkImageLayout _colourImageLayout,
			VulkanImageLoader* _loader);
		void shutdown();

		static VulkanRenderManager& getInstance();
		static VulkanRenderManager* getInstancePtr();

		/** @see RenderManager::getViewSize */
		const IntSize& getViewSize() const override;

		/** @see RenderManager::getVertexFormat */
		VertexColourType getVertexFormat() const override;

		/** @see RenderManager::isFormatSupported */
		bool isFormatSupported(PixelFormat _format, TextureUsage _usage) override;

		/** @see RenderManager::createVertexBuffer */
		IVertexBuffer* createVertexBuffer() override;
		/** @see RenderManager::destroyVertexBuffer */
		void destroyVertexBuffer(IVertexBuffer* _buffer) override;

		/** @see RenderManager::createTexture */
		ITexture* createTexture(const std::string& _name) override;
		/** @see RenderManager::destroyTexture */
		void destroyTexture(ITexture* _texture) override;
		/** @see RenderManager::getTexture */
		ITexture* getTexture(const std::string& _name) override;

		/** @see IRenderTarget::begin */
		void begin() override;
		/** @see IRenderTarget::end */
		void end() override;
		/** @see IRenderTarget::doRender */
		void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count) override;
		/** @see IRenderTarget::getInfo */
		const RenderTargetInfo& getInfo() const override;

		/** Record a MyGUI draw into the given command buffer using the given viewport size */
		void renderGeometry(
			VkCommandBuffer _commandBuffer,
			IVertexBuffer* _buffer,
			ITexture* _texture,
			size_t _count,
			uint32_t _width,
			uint32_t _height);

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

		/** @see RenderManager::registerShader */
		void registerShader(
			const std::string& _shaderName,
			const std::string& _vertexProgramFile,
			const std::string& _fragmentProgramFile) override;

		/*internal:*/
		/** Render MyGUI into the currently recording command buffer.
			The render pass with the framebuffer must already be begun by the caller.
		*/
		void drawOneFrame(VkCommandBuffer _commandBuffer);

		/** Render pass to use when rendering MyGUI into a framebuffer */
		VkRenderPass getRenderPass() const;
		/** Render pass to use when rendering MyGUI into an offscreen render target texture */
		VkRenderPass getRenderTargetRenderPass() const;
		/** Colour attachment format used by the render pass */
		VkFormat getColourFormat() const;
		/** Final layout of the colour attachment after the render pass */
		VkImageLayout getColourImageLayout() const;

		void* getAllocator() const;
		VkDevice getDevice() const;
		VkCommandPool getCommandPool() const;
		VkQueue getQueue() const;
		VkDescriptorPool getDescriptorPool() const;
		VkDescriptorSetLayout getDescriptorSetLayout() const;
		VkSampler getSampler() const;

		/** Upload image data to a texture image using a staging buffer.
			The image is always stored as 4 bytes per pixel; _bytesPerPixel (3 or 4) is the size of a single
			pixel in _data.
		*/
		void uploadTextureData(
			VkImage _image,
			uint32_t _width,
			uint32_t _height,
			uint32_t _bytesPerPixel,
			const void* _data);
		/** Read image data from a texture image using a staging buffer.
			_bytesPerPixel (3 or 4) is the desired size of a single pixel in _data.
		*/
		void readbackImage(VkImage _image, uint32_t _width, uint32_t _height, uint32_t _bytesPerPixel, void* _data);

		VkImageView createImageView(VkImage _image, VkFormat _format);

	private:
		std::vector<uint8_t> loadFileContent(const std::string& _file);
		VkShaderModule createShaderModule(const std::string& _file);
		VkPipeline createShaderPipeline(const std::string& _vertexProgramFile, const std::string& _fragmentProgramFile);
		void destroyAllResources();
		VkPipeline getPipeline(const std::string& _shaderName) const;

		void createRenderPass();
		void createRenderTargetRenderPass();
		void createPipelineLayout();
		void createDescriptorPool();
		void createSampler();
		void createWhiteTexture();

	private:
		VkInstance mInstance = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		VkDevice mDevice = VK_NULL_HANDLE;
		uint32_t mQueueFamily = 0;
		VkQueue mQueue = VK_NULL_HANDLE;
		VkCommandPool mCommandPool = VK_NULL_HANDLE;
		void* mAllocator{nullptr};

		VkFormat mColourFormat = VK_FORMAT_UNDEFINED;
		VkImageLayout mColourImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VkRenderPass mRenderPass = VK_NULL_HANDLE;
		VkRenderPass mRenderTargetRenderPass = VK_NULL_HANDLE;
		VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
		VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
		VkSampler mSampler = VK_NULL_HANDLE;
		VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
		VkPipeline mDefaultPipeline = VK_NULL_HANDLE;
		std::map<std::string, VkPipeline> mRegisteredShaders;

		VkCommandBuffer mCurrentCommandBuffer = VK_NULL_HANDLE;

		// 1x1 white texture used when no texture is bound
		VkImage mWhiteImage = VK_NULL_HANDLE;
		void* mWhiteImageAllocation{nullptr};
		VkImageView mWhiteImageView = VK_NULL_HANDLE;
		VkDescriptorSet mWhiteDescriptorSet = VK_NULL_HANDLE;

		IntSize mViewSize;
		bool mUpdate{false};
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		uint32_t mReferenceCount{0}; // for nested rendering

		using MapTexture = std::map<std::string, ITexture*>;
		MapTexture mTextures;
		VulkanImageLoader* mImageLoader{nullptr};

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_RENDER_MANAGER_H_
