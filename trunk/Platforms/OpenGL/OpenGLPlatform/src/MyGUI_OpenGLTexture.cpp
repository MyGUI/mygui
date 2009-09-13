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

namespace MyGUI
{

	OpenGLTexture::OpenGLTexture(const std::string& _name)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
	}

	const std::string& OpenGLTexture::getName()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return "";
	}

	void OpenGLTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::create()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	void OpenGLTexture::destroy()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
	}

	int OpenGLTexture::getWidth()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return 0;
	}

	int OpenGLTexture::getHeight()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return 0;
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
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return PixelFormat::MAX;
	}

	size_t OpenGLTexture::getNumElemBytes()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return 0;
	}

	TextureUsage OpenGLTexture::getUsage()
	{
		MYGUI_PLATFORM_EXCEPT("is not implemented");
		return TextureUsage::MAX;
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
		return RenderTargetInfo();
	}

} // namespace MyGUI
