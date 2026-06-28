#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_OpenGLESImageLoader.h"

namespace MyGUI
{

	class OpenGLESRTTexture;

	class OpenGLESTexture : public ITexture
	{
	public:
		OpenGLESTexture(const std::string& _name, OpenGLESImageLoader* _loader);
		~OpenGLESTexture() override;

		const std::string& getName() const override;

		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) override;
		void loadFromFile(const std::string& _filename) override;
		void saveToFile(const std::string& _filename) override;
		void setShader(const std::string& _shaderName) override;

		void destroy() override;

		int getWidth() const override
		{
			return mWidth;
		}
		int getHeight() const override
		{
			return mHeight;
		}

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() const override
		{
			return mLock;
		}

		PixelFormat getFormat() const override
		{
			return mOriginalFormat;
		}
		TextureUsage getUsage() const override
		{
			return mOriginalUsage;
		}
		size_t getNumElemBytes() const override
		{
			return mNumElemBytes;
		}

		IRenderTarget* getRenderTarget() override;

		/*internal:*/
		unsigned int getTextureId() const;
		unsigned int getShaderId() const;
		void setUsage(TextureUsage _usage);
		void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data);

	private:
		void _create();

	private:
		std::string mName;
		int mWidth{0};
		int mHeight{0};
		int mPixelFormat{0};
		int mInternalPixelFormat{0};
		int mUsage{0};
		int mAccess{0};
		size_t mNumElemBytes{0};
		size_t mDataSize{0};
		unsigned int mTextureId{0};
		unsigned int mProgramId{0};
		unsigned int mPboID{0};
		bool mLock{false};
		void* mBuffer{nullptr};
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		OpenGLESImageLoader* mImageLoader;
		OpenGLESRTTexture* mRenderTarget{nullptr};
	};

} // namespace MyGUI
