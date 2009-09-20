/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_OpenGLDiagnostic.h"

#define GLEW_STATIC
#define GL_GLEXT_PROTOTYPES
#include "GL/glew.h"

//FIXME
#include "D:/MyGUI_Source/Dependencies/glfw/include/GL/glfw.h"

namespace MyGUI
{

	OpenGLTexture::OpenGLTexture(const std::string& _name) :
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
		mNumElemBytes(0)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
		destroy();
	}

	const std::string& OpenGLTexture::getName()
	{
		return mName;
	}

	void OpenGLTexture::setUsage(TextureUsage _usage)
	{
		mAccess = 0;
		mUsage = 0;

		if (_usage == TextureUsage::Default)
		{
			mUsage = GL_STATIC_READ_ARB;
			mAccess = GL_READ_ONLY_ARB;
		}
		else if (_usage.isValue(TextureUsage::Static))
		{
			if (_usage.isValue(TextureUsage::Read))
			{
				if (_usage.isValue(TextureUsage::Write))
				{
					mUsage = GL_STATIC_COPY_ARB;
					mAccess = GL_READ_WRITE_ARB;
				}
				else
				{
					mUsage = GL_STATIC_READ_ARB;
					mAccess = GL_READ_ONLY_ARB;
				}
			}
			else if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_STATIC_DRAW_ARB;
				mAccess = GL_WRITE_ONLY_ARB;
			}
		}
		else if (_usage.isValue(TextureUsage::Dynamic))
		{
			if (_usage.isValue(TextureUsage::Read))
			{
				if (_usage.isValue(TextureUsage::Write))
				{
					mUsage = GL_DYNAMIC_COPY_ARB;
					mAccess = GL_READ_WRITE_ARB;
				}
				else
				{
					mUsage = GL_DYNAMIC_READ_ARB;
					mAccess = GL_READ_ONLY_ARB;
				}
			}
			else if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_DYNAMIC_DRAW_ARB;
				mAccess = GL_WRITE_ONLY_ARB;
			}
		}
		else if (_usage.isValue(TextureUsage::Stream))
		{
			if (_usage.isValue(TextureUsage::Read))
			{
				if (_usage.isValue(TextureUsage::Write))
				{
					mUsage = GL_STREAM_COPY_ARB;
					mAccess = GL_READ_WRITE_ARB;
				}
				else
				{
					mUsage = GL_STREAM_READ_ARB;
					mAccess = GL_READ_ONLY_ARB;
				}
			}
			else if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = GL_STREAM_DRAW_ARB;
				mAccess = GL_WRITE_ONLY_ARB;
			}
		}
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, 0);
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureID, "Texture already exist");

		mInternalPixelFormat = 0;
		mPixelFormat = 0;
		mNumElemBytes = 0;
		if (_format == PixelFormat::L8)
		{
			mInternalPixelFormat = GL_LUMINANCE8;
			mPixelFormat = GL_LUMINANCE;
			mNumElemBytes = 1;
		}
		else if (_format == PixelFormat::L8A8)
		{
			mInternalPixelFormat = GL_LUMINANCE8_ALPHA8;
			mPixelFormat = GL_LUMINANCE_ALPHA;
			mNumElemBytes = 2;
		}
		else if (_format == PixelFormat::R8G8B8)
		{
			mInternalPixelFormat = GL_RGB8;
			mPixelFormat = GL_RGB;
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::A8R8G8B8)
		{
			mInternalPixelFormat = GL_RGBA8;
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
		MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;

		// создаем тукстуру
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
        // Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, mInternalPixelFormat, mWidth, mHeight, 0, mPixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)_data);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (true)//!_data)
		{
			//создаем текстурнный буфер
			glGenBuffersARB(2, &mPboID);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
			glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		}
	}

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		destroy();

		std::string name = DataManager::getInstance().getDataPath(_filename);

		GLFWimage image_info;
		if (!glfwReadImage(name.c_str(), &image_info, GLFW_NO_RESCALE_BIT | GLFW_ORIGIN_UL_BIT))
		{
			// error load image
			return;
		}

		PixelFormat format;

		if (image_info.Format == GL_LUMINANCE)
			format = PixelFormat::L8;
		else if (image_info.Format == GL_LUMINANCE_ALPHA)
			format = PixelFormat::L8A8;
		else if (image_info.Format == GL_RGB)
			format = PixelFormat::R8G8B8;
		else if (image_info.Format == GL_RGBA)
			format = PixelFormat::A8R8G8B8;
		else
		{
			glfwFreeImage(&image_info);
			MYGUI_PLATFORM_EXCEPT("format not support");
			return;
		}

		createManual(image_info.Width, image_info.Height, TextureUsage::Static | TextureUsage::Write, format, image_info.Data);

		glfwFreeImage(&image_info);
	}

	void OpenGLTexture::destroy()
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
		if (mPboID != 0)
		{
			glDeleteBuffersARB(1, &mPboID);
			mPboID = 0;
		}

		mWidth = 0;
		mHeight = 0;
		mLock = false;
		mPixelFormat = 0;
		mDataSize = 0;
		mUsage = 0;
		mBuffer = 0;
		mInternalPixelFormat = 0;
		mAccess = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::MAX;
		mOriginalUsage = TextureUsage::Default;
	}

	void OpenGLTexture::saveToFile(const std::string& _filename)
	{
	}

	void* OpenGLTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTextureID, "Texture is not created");

		if (_access == TextureUsage::Read)
		{
			glBindTexture(GL_TEXTURE_2D, mTextureID);

			mBuffer = new unsigned char[mDataSize];
			glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);

			mLock = false;

			return mBuffer;
		}

		// bind the texture and PBO
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);

		// Note that glMapBufferARB() causes sync issue.
		// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
		// until GPU to finish its job. To avoid waiting (idle), you can call
		// first glBufferDataARB() with NULL pointer before glMapBufferARB().
		// If you do that, the previous data in PBO will be discarded and
		// glMapBufferARB() returns a new allocated pointer immediately
		// even if GPU is still working with the previous data.
		glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);

		// map the buffer object into client's memory
		mBuffer = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mAccess);

		if (!mBuffer)
		{
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			MYGUI_PLATFORM_EXCEPT("Error texture lock");
		}

		mLock = true;

		return mBuffer;
	}

	void OpenGLTexture::unlock()
	{
		if (!mLock && mBuffer)
		{
			delete mBuffer;
			mBuffer = 0;

			glBindTexture(GL_TEXTURE_2D, 0);

			return;
		}

		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");

		// release the mapped buffer
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);

		// copy pixels from PBO to texture object
		// Use offset instead of ponter.
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, 0);

		// it is good idea to release PBOs with ID 0 after use.
		// Once bound with 0, all pixel operations are back to normal ways.
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

		glBindTexture(GL_TEXTURE_2D, 0);

		mBuffer = 0;
		mLock = false;
	}

} // namespace MyGUI
