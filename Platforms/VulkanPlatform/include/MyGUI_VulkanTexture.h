/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_TEXTURE_H_
#define MYGUI_VULKAN_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_VulkanImageLoader.h"

#include <vulkan/vulkan.h>

namespace MyGUI
{

	class VulkanTexture : public ITexture
	{
	public:
		VulkanTexture(const std::string& _name, VulkanImageLoader* _loader);
		~VulkanTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void destroy() override;

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override;

		int getWidth() const override;
		int getHeight() const override;

		PixelFormat getFormat() const override;
		TextureUsage getUsage() const override;
		size_t getNumElemBytes() const override;

		/*internal:*/
		VkImage getImage() const
		{
			return mImage;
		}
		VkImageView getImageView() const
		{
			return mImageView;
		}
		VkDescriptorSet getDescriptorSet() const
		{
			return mDescriptorSet;
		}
		const std::string& getShaderName() const
		{
			return mShaderName;
		}

	private:
		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data);
		void createImage();
		void uploadData(const void* _data);

	private:
		std::string mName;
		VulkanImageLoader* mImageLoader{nullptr};

		int mWidth{0};
		int mHeight{0};
		size_t mDataSize{0};
		size_t mNumElemBytes{0};
		PixelFormat mOriginalFormat{PixelFormat::Unknow};
		TextureUsage mOriginalUsage{TextureUsage::Default};

		VkImage mImage{VK_NULL_HANDLE};
		VkImageView mImageView{VK_NULL_HANDLE};
		VkDescriptorSet mDescriptorSet{VK_NULL_HANDLE};
		void* mAllocation{nullptr};

		bool mLock{false};
		void* mBuffer{nullptr};

		std::string mShaderName{"Default"};
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_TEXTURE_H_
