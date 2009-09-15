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
		mLoader(0),
		mTextureID(0)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
		destroy();
	}

	void OpenGLTexture::_create()
	{
		MYGUI_PLATFORM_ASSERT(!mTextureID, "Texture already exist");

		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::A8R8G8B8;

		glGenTextures(1, &mTextureID);

		glBindTexture(GL_TEXTURE_2D, mTextureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	const std::string& OpenGLTexture::getName()
	{
		return mName;
	}

	void OpenGLTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void OpenGLTexture::create()
	{
		_create();
		if (mLoader != nullptr)
		{
			mLoader->loadResource(this);
		}
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();
		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
		_create();
	}

	void OpenGLTexture::loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format)
	{
		destroy();
		mSize.set(_width, _height);
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = _format;
		_create();

		GLFWimage image_info =
		{
			_width,
			_height,
			PixelFormat::L8A8 ? GL_LUMINANCE_ALPHA : GL_RGBA,
			PixelFormat::L8A8 ? 2 : 4,
			(unsigned char *)_buff
		};

		glfwLoadTextureImage2D(&image_info, 0);
	}

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		_create();

		std::string name = DataManager::getInstance().getDataPath(_filename);

		GLFWimage image_info;
		if (!glfwReadImage(name.c_str(), &image_info, GLFW_NO_RESCALE_BIT | GLFW_ORIGIN_UL_BIT))
		{
			// error load image
			return;
		}

		//check format
		if (image_info.BytesPerPixel == 2 && image_info.Format == GL_LUMINANCE_ALPHA)
			mPixelFormat = PixelFormat::L8A8;
		else if (image_info.BytesPerPixel == 4 && image_info.Format == GL_RGBA)
			mPixelFormat = PixelFormat::A8R8G8B8;
		else
			return;

		mSize.set(image_info.Width, image_info.Height);
		glfwLoadTextureImage2D(&image_info, 0);

		glfwFreeImage(&image_info);
	}

	void OpenGLTexture::destroy()
	{
		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
	}

	int OpenGLTexture::getWidth()
	{
		return mSize.width;
	}

	int OpenGLTexture::getHeight()
	{
		return mSize.height;
	}

	void* OpenGLTexture::lock(bool _discard)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return nullptr;
	}

	void OpenGLTexture::unlock()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	bool OpenGLTexture::isLocked()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return false;
	}

	PixelFormat OpenGLTexture::getFormat()
	{
		return mPixelFormat;
	}

	size_t OpenGLTexture::getNumElemBytes()
	{
		return PixelFormat::L8A8 ? 2 : 4;
	}

	TextureUsage OpenGLTexture::getUsage()
	{
		return mTextureUsage;
	}

	void OpenGLTexture::setViewport(IViewport* _viewport)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::removeViewport()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::begin()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::end()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	const RenderTargetInfo& OpenGLTexture::getInfo()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return mInfo;
	}

	void OpenGLTexture::saveToFile(const std::string& _filename)
	{
	}

} // namespace MyGUI
