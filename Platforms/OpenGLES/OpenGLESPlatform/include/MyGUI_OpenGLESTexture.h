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

		int getWidth() override
		{
			return mWidth;
		}
		int getHeight() override
		{
			return mHeight;
		}

		void* lock(TextureUsage _access) override;
		void unlock() override;
		bool isLocked() override
		{
			return mLock;
		}

		PixelFormat getFormat() override
		{
			return mOriginalFormat;
		}
		TextureUsage getUsage() override
		{
			return mOriginalUsage;
		}
		size_t getNumElemBytes() override
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
		int mWidth;
		int mHeight;
		int mPixelFormat;
		int mInternalPixelFormat;
		int mUsage;
		int mAccess;
		size_t mNumElemBytes;
		size_t mDataSize;
		unsigned int mTextureId;
		unsigned int mProgramId;
		unsigned int mPboID;
		bool mLock;
		void* mBuffer;
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		OpenGLESImageLoader* mImageLoader;
		OpenGLESRTTexture* mRenderTarget;
	};

} // namespace MyGUI
