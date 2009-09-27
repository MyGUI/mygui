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
#include "MyGUI_OpenGLPlatform.h"

#define GLEW_STATIC
#define GL_GLEXT_PROTOTYPES
#include "GL/glew.h"

//#include <png.h>

namespace MyGUI
{

	// ������� ��� ������ ������ �� �����. �� ����� ������������ ���������� PNG.
	// �� ���� �������� ��������� �� ��������� ���������� ���������� � ����������
	// (png_structp png_ptr) � ����� ����, ������� ����� ��������. �����������
	// ������ ��������� � data.
	/*void PNGReadFunction(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		IDataStream* data_stream = (IDataStream*)png_get_io_ptr(png_ptr);
		data_stream->read(data, length);
	}*/

	OpenGLTexture::OpenGLTexture(const std::string& _name, OpenGLImageLoader* _loader) :
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
		mImageLoader(_loader)
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

		//FIXME ��������� � �����
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

		// Set unpack alignment to one byte
		int alignment = 0;
		glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );
	    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		// ������� ��������
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
        // Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, mInternalPixelFormat, mWidth, mHeight, 0, mPixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)_data);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Restore old unpack alignment
	    glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

		if (!_data)
		{
			//������� ����������� �����
			glGenBuffersARB(1, &mPboID);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, mPboID);
			glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, mDataSize, 0, mUsage);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		}
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
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
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

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			void* data = mImageLoader->LoadImage(&width, &height, &format, _filename);
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
				delete data;
			}

			return;
		}

		//----------------------------
		/*const int number = 8;	// ����� ���� � ���������

		// ��������� ���� ��� ������
		IDataStream* data_stream = DataManager::getInstance().getData(_filename);
		if (data_stream == nullptr)
		{
			MYGUI_PLATFORM_LOG(Warning, "Texture '" << _filename << "' format not suported");
			return;
		}

		// ��������� ��������� ����� (������ number ����)
		png_byte sig[number] = {0};
		data_stream->read(sig, sizeof(png_byte) * number);
		if (!png_check_sig(sig, number))
		{ 
			delete data_stream;
			return;
		}
		// �������� ������ ������� - ��� png-����

		// ������� ���������� ��������� png ��� ������ � ������
		// ��������� ��������� - ���������, ��� ������� ��������� ������ � �������� (�������. 2 ���������)
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if ( !png_ptr )
		{ 
			delete data_stream;
			return;
		}

		// ������� ��������� � ����������� � �����
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if ( !info_ptr )
		{
			png_destroy_read_struct(&png_ptr, 0, 0);	// ������� ���������� ���������
			delete data_stream;
			return;
		}

		// ����������� ���������� �� ��� ������ ������ �����
		// ��������� �� file ����� ����� �������� ����� � PNGReadFunction
		png_set_read_fn(png_ptr, (void*)data_stream, PNGReadFunction);

		// ������� ����������, ��� �� ��� ������ number ����, ����� ��������� ���������
		png_set_sig_bytes(png_ptr, number);

		// ������ ��� ���������� � �����
		png_read_info(png_ptr, info_ptr);

		// ��� ������� ���������� ���� �� info_ptr
		png_uint_32 width = 0, height = 0;	// ������ �������� � ��������
		int bit_depth = 0;	// ������� ����� (������ �� �������, ����� ���� 1, 2, 4, 8, 16)
		int color_type = 0;	// ��������� ����� ������ ������������:
							// PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE,
							// PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGB_ALPHA...
		// ��������� 3 ��������� ����� ���� ������ � ����������: ��� �������, ��� ���������� � ��� ��������
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
		// ����, ����� png �������� �� ���� ������� ��� ��� �����, ����������� ��� ����������� � ������ ���

		// png ������ ����� ��������� 16 ��� �� �����, �� ��� ����� ������ 8, ������� ������ �����
		if (bit_depth == 16)
			png_set_strip_16(png_ptr);
		// ����������� ���� ���� �� �������� ������� � ���������� RGB
		if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8)
			png_set_palette_to_rgb(png_ptr);
		// ���� � ���������� ������ ��� �� ����� ��� 8, �� ��������� � ����������� 8-�������
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_gray_1_2_4_to_8(png_ptr);
		// � ��������� ������ �����-�����
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);
		// ��� ��� ����� ������� ����� �������� png_set_expand(png_ptr);

		// ��� ����������� ����������� ��������, ����� ��������� ����� � ����������� �� ��������
		// ��������, �� �� ����� �����:
		// 2.2 - ������ ��� PC ��������� � ���������� �������
		// 2.0 - ��� PC � ������ �������
		// �� 1.7 �� 1.0 - ��� ������ MacOS
		double gamma = 0.0f;
		// ���� ���� ���������� � ����� � �����, �� ������������� �� 2.2
		if ( png_get_gAMA(png_ptr, info_ptr, &gamma) ) png_set_gamma(png_ptr, 2.2, gamma);
		// ����� ������ ��������� ����� ��� ����� � 0.45455 (good guess for GIF images on PCs)
		else png_set_gamma(png_ptr, 2.2, 0.45455);

		// ����� ���� �������������, �������� ���������� � ����������
		png_read_update_info(png_ptr, info_ptr);

		// ����� �������� ��� ������� � ��������� ����������� ��������
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

		//info_ptr->

		// �������� ���-�� ������� �� ������� 
		// ����� ����: 1 (GRAY, PALETTE), 2 (GRAY_ALPHA), 3 (RGB), 4 (RGB_ALPHA ��� RGB + filler byte)
		png_byte channels = png_get_channels(png_ptr, info_ptr);
		// ���������� ���-�� ���� ������ ��� ���� ����� �������� ������
		png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
			
		// ������, �� ����� �������� ������ ����� �������� ��������
		png_byte* data = new png_byte[row_bytes * height];
		// �������� ������, ��� ���������� �� ������ ������
		png_byte **row_pointers = new png_byte * [height];
		// ������������ ������ ���������� �� �������, � ����������� � ������ (res)
		// �.�. ����������� ������������, �� ��������� ���� ����� �����
		for (unsigned int i = 0; i < height; i++)
			row_pointers[i] = data + i * row_bytes;
			//row_pointers[height - i - 1] = data + i * row_bytes;
		
		// ���, ������ ��������
		png_read_image(png_ptr, row_pointers);
		
		// ������ �������������� ���������� � ����� (�� ����� ���� ���������� ��)
		png_read_end(png_ptr, 0);

		// ����������� ������ �� ���������� �� ������
		delete [] row_pointers;
		// ����������� ������ ���������� ��� ���������� libpng
		png_destroy_read_struct(&png_ptr, 0, 0);

		// ��������� ����
		delete data_stream;
		//----------------------------

		PixelFormat format;

		if (color_type == PNG_COLOR_TYPE_GRAY)
			format = PixelFormat::L8;
		else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			format = PixelFormat::L8A8;
		else if (color_type == PNG_COLOR_TYPE_RGB)
			format = PixelFormat::R8G8B8;
		else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
			format = PixelFormat::A8R8G8B8;

		if (format != PixelFormat::Unknow)
		{
			createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
		}
		else
		{
			MYGUI_PLATFORM_LOG(Warning, "Texture '" << _filename << "' format not suported");
		}

		delete data;*/
	}

	void OpenGLTexture::saveToFile(const std::string& _filename)
	{
	}

} // namespace MyGUI
