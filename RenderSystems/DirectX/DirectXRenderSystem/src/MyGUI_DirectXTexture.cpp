/*!
	@file
	@author		Albert Semenov
	@date		04/2009
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
#include "MyGUI_DirectXTexture.h"

namespace MyGUI
{

	DirectXTexture::DirectXTexture(const std::string& _name, const std::string& _group) :
		mName(_name),
		mGroup(_group)
	{
	}

	DirectXTexture::~DirectXTexture()
	{
		destroy();
	}

	const std::string& DirectXTexture::getName()
	{
		return mName;
	}

	const std::string& DirectXTexture::getGroup()
	{
		return mGroup;
	}

	void DirectXTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void DirectXTexture::create()
	{
	}

	void DirectXTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
	}

	void DirectXTexture::loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format)
	{
		mSize.set(_width, _height);
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = _format;
	}

	void DirectXTexture::loadFromFile(const std::string& _filename)
	{
		mSize.set(1024, 1024);
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::A8R8G8B8;
	}

	void DirectXTexture::destroy()
	{
	}

	int DirectXTexture::getWidth()
	{
		return mSize.width;
	}

	int DirectXTexture::getHeight()
	{
		return mSize.height;
	}

	void* DirectXTexture::lock()
	{
		mLock = true;
		return nullptr;
	}

	void DirectXTexture::unlock()
	{
		mLock = false;
	}

	bool DirectXTexture::isLocked()
	{
		return mLock;
	}

	PixelFormat DirectXTexture::getFormat()
	{
		return mPixelFormat;
	}

	size_t DirectXTexture::getNumElemBytes()
	{
		return 4;
	}

	TextureUsage DirectXTexture::getUsage()
	{
		return mTextureUsage;
	}

	void* DirectXTexture::_getRenderTarget()
	{
		return nullptr;
	}

} // namespace MyGUI
