#ifndef MYGUI_OPENGLES_TEXTURE_H__
#define MYGUI_OPENGLES_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_OpenGLESImageLoader.h"

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

namespace MyGUI
{

	class OpenGLESRTTexture;

	class OpenGLESTexture : public ITexture
	{
	public:
		OpenGLESTexture(const std::string& _name, OpenGLESImageLoader* _loader);
		virtual ~OpenGLESTexture();

		virtual const std::string& getName() const;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth()
		{
			return mWidth;
		}
		virtual int getHeight()
		{
			return mHeight;
		}

		virtual void* lock(TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked()
		{
			return mLock;
		}

		virtual PixelFormat getFormat()
		{
			return mOriginalFormat;
		}
		virtual TextureUsage getUsage()
		{
			return mOriginalUsage;
		}
		virtual size_t getNumElemBytes()
		{
			return mNumElemBytes;
		}

		virtual IRenderTarget* getRenderTarget();

	/*internal:*/
		unsigned int getTextureID() const;
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
		unsigned int mTextureID;
		unsigned int mPboID;
		bool mLock;
		void* mBuffer;
		PixelFormat mOriginalFormat;
		TextureUsage mOriginalUsage;
		OpenGLESImageLoader* mImageLoader;
		OpenGLESRTTexture* mRenderTarget;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGLES_TEXTURE_H__
