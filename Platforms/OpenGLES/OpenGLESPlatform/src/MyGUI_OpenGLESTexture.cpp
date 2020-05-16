#include "MyGUI_OpenGLESTexture.h"
#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESDiagnostic.h"
#include "MyGUI_OpenGLESPlatform.h"
#include "MyGUI_OpenGLESRTTexture.h"

#include "platform.h"

namespace MyGUI
{

	OpenGLESTexture::OpenGLESTexture(const std::string& _name, OpenGLESImageLoader* _loader) :
		mName(_name),
		mTextureID(0),
		mPboID(0),
		mWidth(0),
		mHeight(0),
		mLock(false),
		mPixelFormat(0),
		mDataSize(0),
		mUsage(0),
		mBuffer(0),
		mInternalPixelFormat(0),
		mAccess(0),
		mNumElemBytes(0),
		mImageLoader(_loader),
		mRenderTarget(nullptr)
	{
	}

	OpenGLESTexture::~OpenGLESTexture()
	{
		destroy();
	}

	const std::string& OpenGLESTexture::getName() const
	{
		return mName;
	}

	void OpenGLESTexture::setUsage(TextureUsage _usage)
	{
		mAccess = 0;
		mUsage = 0;
	}

	void OpenGLESTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, 0);
	}

	void OpenGLESTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureID, "Texture already exist");

		//FIXME перенести в метод
		mInternalPixelFormat = 0;
		mPixelFormat = 0;
		mNumElemBytes = 0;
		if (_format == PixelFormat::L8)
		{
			mInternalPixelFormat = GL_LUMINANCE;
			mPixelFormat = GL_LUMINANCE;
			mNumElemBytes = 1;
		}
		else if (_format == PixelFormat::L8A8)
		{
			mInternalPixelFormat = GL_LUMINANCE_ALPHA;
			mPixelFormat = GL_LUMINANCE_ALPHA;
			mNumElemBytes = 2;
		}
		else if (_format == PixelFormat::R8G8B8)
		{
			mInternalPixelFormat = GL_RGB;
			mPixelFormat = GL_RGB;
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::R8G8B8A8)
		{
			mInternalPixelFormat = GL_RGBA;
			mPixelFormat = GL_RGBA;
			mNumElemBytes = 4;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("format not support");
		}

		mWidth = _width;
		mHeight = _height;
		mDataSize = _width * _height * mNumElemBytes;
		setUsage(_usage);
		//MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;

		// Set unpack alignment to one byte
		int alignment = 0;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, (GLint*) &alignment);
		CHECK_GL_ERROR_DEBUG();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		CHECK_GL_ERROR_DEBUG();

		// создаем тукстуру
		glGenTextures(1, (GLuint*) &mTextureID);
		CHECK_GL_ERROR_DEBUG();
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		CHECK_GL_ERROR_DEBUG();
		// Set texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR
		CHECK_GL_ERROR_DEBUG();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		CHECK_GL_ERROR_DEBUG();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		CHECK_GL_ERROR_DEBUG();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		CHECK_GL_ERROR_DEBUG();

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			mInternalPixelFormat,
			mWidth,
			mHeight,
			0,
			mPixelFormat,
			GL_UNSIGNED_BYTE,
			(GLvoid*)_data);
		CHECK_GL_ERROR_DEBUG();
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR_DEBUG();

		// Restore old unpack alignment
		//glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
		//CHECK_GL_ERROR_DEBUG();
#ifdef PixelBufferObjectSupported
		if (!_data && OpenGLESRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//создаем текстурнный буфер
			//glGenBuffersARB(1, (GLuint *)&mPboID);
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
			//glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

			glGenBuffers(1, (GLuint*)&mPboID);
			CHECK_GL_ERROR_DEBUG();
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);
			CHECK_GL_ERROR_DEBUG();
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);
			CHECK_GL_ERROR_DEBUG();
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
			CHECK_GL_ERROR_DEBUG();

		}
#endif
	}

	void OpenGLESTexture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (mTextureID != 0)
		{
			glDeleteTextures(1, (GLuint*)&mTextureID);
			mTextureID = 0;
		}
		if (mPboID != 0)
		{
            glDeleteBuffers(1, (GLuint*)&mPboID);
			mPboID = 0;
		}

		mWidth = 0;
		mHeight = 0;
		mLock = false;
		mPixelFormat = 0;
		mDataSize = 0;
		mUsage = 0;
		mBuffer = nullptr;
		mInternalPixelFormat = 0;
		mAccess = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
	}

	void* OpenGLESTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTextureID, "Texture is not created");

		/*
		if (_access == TextureUsage::Read)
		{
			glBindTexture(GL_TEXTURE_2D, mTextureID);
			CHECK_GL_ERROR_DEBUG();

			mBuffer = new unsigned char[mDataSize];
			//glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);

			mLock = false;

			return mBuffer;
		}*/

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		CHECK_GL_ERROR_DEBUG();
		if (!OpenGLESRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//Fallback if PBO's are not supported
			mBuffer = new unsigned char[mDataSize];
		}
		else
		{
#ifdef PixelBufferObjectSupported
			// bind the PBO
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);
			CHECK_GL_ERROR_DEBUG();

			// Note that glMapBufferARB() causes sync issue.
			// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
			// until GPU to finish its job. To avoid waiting (idle), you can call
			// first glBufferDataARB() with nullptr pointer before glMapBufferARB().
			// If you do that, the previous data in PBO will be discarded and
			// glMapBufferARB() returns a new allocated pointer immediately
			// even if GPU is still working with the previous data.
			//glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);
			CHECK_GL_ERROR_DEBUG();

			// map the buffer object into client's memory
			//mBuffer = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mAccess);
			mBuffer = (GLubyte*)glMapBufferOES(GL_PIXEL_UNPACK_BUFFER_ARB, mAccess);
			CHECK_GL_ERROR_DEBUG();
			if (!mBuffer)
			{
				//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
				CHECK_GL_ERROR_DEBUG();
				glBindTexture(GL_TEXTURE_2D, 0);
				CHECK_GL_ERROR_DEBUG();
				MYGUI_PLATFORM_EXCEPT("Error texture lock");
			}
#endif
		}

		mLock = true;

		return mBuffer;
	}

	void OpenGLESTexture::unlock()
	{
		if (!mLock && mBuffer)
		{
			delete (unsigned char*)mBuffer;
			mBuffer = nullptr;

			glBindTexture(GL_TEXTURE_2D, 0);
			CHECK_GL_ERROR_DEBUG();

			return;
		}

		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");

		if (!OpenGLESRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//Fallback if PBO's are not supported
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);
			CHECK_GL_ERROR_DEBUG();
			delete (unsigned char*)mBuffer;
		}
		else
		{
#ifdef PixelBufferObjectSupported
			// release the mapped buffer
			//glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);
			glUnmapBufferOES(GL_PIXEL_UNPACK_BUFFER_ARB);
			CHECK_GL_ERROR_DEBUG();

			// copy pixels from PBO to texture object
			// Use offset instead of ponter.
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, 0);
			CHECK_GL_ERROR_DEBUG();

			// it is good idea to release PBOs with ID 0 after use.
			// Once bound with 0, all pixel operations are back to normal ways.
			//glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			CHECK_GL_ERROR_DEBUG();
#endif
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR_DEBUG();
		mBuffer = nullptr;
		mLock = false;
	}

	void OpenGLESTexture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			void* data = mImageLoader->loadImage(width, height, format, _filename);
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
				delete (unsigned char*)data;
			}
		}
	}

	void OpenGLESTexture::saveToFile(const std::string& _filename)
	{
		if (mImageLoader)
		{
			void* data = lock(TextureUsage::Read);
			mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, _filename);
			unlock();
		}
	}

	IRenderTarget* OpenGLESTexture::getRenderTarget()
	{
		if (mRenderTarget == nullptr)
			mRenderTarget = new OpenGLESRTTexture(mTextureID);

		return mRenderTarget;
	}

	unsigned int OpenGLESTexture::getTextureID() const
	{
		return mTextureID;
	}

} // namespace MyGUI
