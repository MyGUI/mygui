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

#include <d3dx9.h>

namespace MyGUI
{

	OpenGLTexture::OpenGLTexture(IDirect3DDevice9 *_device, const std::string& _name) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(NULL)
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

	void OpenGLTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void OpenGLTexture::_create()
	{
	    unsigned long usage = 0;

		if (mTextureUsage == TextureUsage::RenderTarget)
			usage |= D3DUSAGE_RENDERTARGET;
		if (mTextureUsage == TextureUsage::Dynamic)
			usage |= D3DUSAGE_DYNAMIC;
		if (mTextureUsage == TextureUsage::DynamicWriteOnly)
			usage |= D3DUSAGE_DYNAMIC;
		if (mTextureUsage == TextureUsage::DynamicWriteOnlyDiscardable)
			usage |= D3DUSAGE_DYNAMIC;

		if (FAILED(mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, usage,
			mPixelFormat == PixelFormat::L8A8 ? D3DFMT_A8L8 : D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mpTexture, NULL)))
		{
			//exception
		}
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
		void *ptr = lock(true);

		// size = w * h * bits
		int n = _format == PixelFormat::L8A8 ? 2 : 4;
		memcpy(ptr, _buff, _width * _height * n);
		unlock();
	}

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::A8R8G8B8;
    
		std::string fullname = DataManager::getInstance().getDataPath(_filename, /*"General", */true, true, true);

		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(fullname.c_str(), &info);
		/*
		if (info.Format == D3DFMT_A8R8G8B8)
		  mPixelFormat = PixelFormat::A8R8G8B8;
		*/

		mSize.set(info.Width, info.Height);
		if (FAILED(D3DXCreateTextureFromFile(mpD3DDevice, fullname.c_str(), &mpTexture)))
		{
			char buf[1024];
			sprintf(buf, "texture = %s\nloaded failed!", _filename.c_str());
			MessageBox(0, buf, 0, MB_ICONERROR);
		}
	}

	void OpenGLTexture::destroy()
	{
		if (mpTexture)
		{
			mpTexture->Release();
			mpTexture = nullptr;
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
		D3DLOCKED_RECT d3dlr;
		if (SUCCEEDED(mpTexture->LockRect(0, &d3dlr, NULL, _discard ? D3DLOCK_DISCARD : 0)))
		{
			mLock = true;
		}
		else
		{
			//exception
			return nullptr;
		}
		return d3dlr.pBits;
	}

	void OpenGLTexture::unlock()
	{
		if (SUCCEEDED(mpTexture->UnlockRect(0)))
		{
			mLock = false;
		}
		else
		{
			//exception
		}
	}

	bool OpenGLTexture::isLocked()
	{
		return mLock;
	}

	PixelFormat OpenGLTexture::getFormat()
	{
		return mPixelFormat;
	}

	size_t OpenGLTexture::getNumElemBytes()
	{
		return 4;
	}

	TextureUsage OpenGLTexture::getUsage()
	{
		return mTextureUsage;
	}

	void* OpenGLTexture::_getRenderTarget()
	{
		return nullptr;
	}

	bool OpenGLTexture::bindToStage(size_t _stage)
	{
		if (SUCCEEDED(mpD3DDevice->SetTexture(_stage, mpTexture)))
			return true;
		return false;
	}

} // namespace MyGUI
