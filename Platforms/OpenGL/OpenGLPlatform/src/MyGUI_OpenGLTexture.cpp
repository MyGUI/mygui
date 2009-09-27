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

	// функция для чтения данных из файла. ею будет пользоваться библиотека PNG.
	// На вход подается указатель на структуру содержащую информацию о библиотеке
	// (png_structp png_ptr) и число байт, которые нужно прочесть. Необходимые
	// данные запишутся в data.
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

		//FIXME перенести в метод
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

		// Restore old unpack alignment
	    glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );

		if (!_data)
		{
			//создаем текстурнный буфер
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
		/*const int number = 8;	// число байт в сигнатуре

		// открываем файл для чтения
		IDataStream* data_stream = DataManager::getInstance().getData(_filename);
		if (data_stream == nullptr)
		{
			MYGUI_PLATFORM_LOG(Warning, "Texture '" << _filename << "' format not suported");
			return;
		}

		// проверяем сигнатуру файла (первые number байт)
		png_byte sig[number] = {0};
		data_stream->read(sig, sizeof(png_byte) * number);
		if (!png_check_sig(sig, number))
		{ 
			delete data_stream;
			return;
		}
		// проверка прошла успешно - это png-файл

		// создаем внутреннюю структуру png для работы с файлом
		// последние параметры - структура, для функции обработки ошибок и варнинга (последн. 2 параметра)
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if ( !png_ptr )
		{ 
			delete data_stream;
			return;
		}

		// создаем структуру с информацией о файле
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if ( !info_ptr )
		{
			png_destroy_read_struct(&png_ptr, 0, 0);	// убиваем внутреннюю структуру
			delete data_stream;
			return;
		}

		// настраиваем библиотеку на наш способ чтения файла
		// указатель на file можно будет получить потом в PNGReadFunction
		png_set_read_fn(png_ptr, (void*)data_stream, PNGReadFunction);

		// говорим библиотеке, что мы уже прочли number байт, когда проверяли сигнатуру
		png_set_sig_bytes(png_ptr, number);

		// читаем всю информацию о файле
		png_read_info(png_ptr, info_ptr);

		// Эта функция возвращает инфу из info_ptr
		png_uint_32 width = 0, height = 0;	// размер картинки в пикселях
		int bit_depth = 0;	// глубина цвета (одного из каналов, может быть 1, 2, 4, 8, 16)
		int color_type = 0;	// описывает какие каналы присутствуют:
							// PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE,
							// PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGB_ALPHA...
		// последние 3 параметра могут быть нулями и обозначают: тип фильтра, тип компрессии и тип смещения
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
		// если, вдруг png окажется не того формата что нам нужен, постараемся его сконвертить в нужный нам

		// png формат может содержать 16 бит на канал, но нам нужно только 8, поэтому сужаем канал
		if (bit_depth == 16)
			png_set_strip_16(png_ptr);
		// преобразуем файл если он содержит палитру в нормальный RGB
		if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8)
			png_set_palette_to_rgb(png_ptr);
		// если в грэйскейле меньше бит на канал чем 8, то конвертим к нормальному 8-битному
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_gray_1_2_4_to_8(png_ptr);
		// и добавляем полный альфа-канал
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png_ptr);
		// все это можно сделать одной функцией png_set_expand(png_ptr);

		// для правильного отображения картинки, нужно подобрать гамму в зависимости от настроек
		// монитора, но мы будем проще:
		// 2.2 - хорошо для PC мониторов в освещенной комнате
		// 2.0 - для PC в темной комнате
		// от 1.7 до 1.0 - для систем MacOS
		double gamma = 0.0f;
		// если есть информация о гамме в файле, то устанавливаем на 2.2
		if ( png_get_gAMA(png_ptr, info_ptr, &gamma) ) png_set_gamma(png_ptr, 2.2, gamma);
		// иначе ставим дефолтную гамму для файла в 0.45455 (good guess for GIF images on PCs)
		else png_set_gamma(png_ptr, 2.2, 0.45455);

		// после всех трансформаций, апдейтим информацию в библиотеке
		png_read_update_info(png_ptr, info_ptr);

		// опять получаем все размеры и параметры обновленной картинки
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

		//info_ptr->

		// получаем кол-во каналов на пиксель 
		// может быть: 1 (GRAY, PALETTE), 2 (GRAY_ALPHA), 3 (RGB), 4 (RGB_ALPHA или RGB + filler byte)
		png_byte channels = png_get_channels(png_ptr, info_ptr);
		// определяем кол-во байт нужных для того чтобы вместить строку
		png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
			
		// теперь, мы можем выделить память чтобы вместить картинку
		png_byte* data = new png_byte[row_bytes * height];
		// выделяем память, для указателей на каждую строку
		png_byte **row_pointers = new png_byte * [height];
		// сопоставляем массив указателей на строчки, с выделенными в памяти (res)
		// т.к. изображение перевернутое, то указатели идут снизу вверх
		for (unsigned int i = 0; i < height; i++)
			row_pointers[i] = data + i * row_bytes;
			//row_pointers[height - i - 1] = data + i * row_bytes;
		
		// все, читаем картинку
		png_read_image(png_ptr, row_pointers);
		
		// читаем дополнительную информацию о файле (на самом деле игнорируем ее)
		png_read_end(png_ptr, 0);

		// освобождаем память от указателей на строки
		delete [] row_pointers;
		// освобождаем память выделенную для библиотеки libpng
		png_destroy_read_struct(&png_ptr, 0, 0);

		// закрываем файл
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
