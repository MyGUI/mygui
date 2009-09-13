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
#include "MyGUI_DataManager.h"

#include <d3dx9.h>

namespace MyGUI
{

	DirectXTexture::DirectXTexture(IDirect3DDevice9 *_device, const std::string& _name) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(NULL),
		mLoader(0)
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

	void DirectXTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void DirectXTexture::_create()
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

	void DirectXTexture::create()
	{
		_create();
		if (mLoader != nullptr)
		{
			mLoader->loadResource(this);
		}
	}

	void DirectXTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();
		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
		_create();
	}

	void DirectXTexture::loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format)
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

	void DirectXTexture::loadFromFile(const std::string& _filename)
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

	void DirectXTexture::destroy()
	{
		if (mpTexture)
		{
			mpTexture->Release();
			mpTexture = nullptr;
		}
	}

	int DirectXTexture::getWidth()
	{
		return mSize.width;
	}

	int DirectXTexture::getHeight()
	{
		return mSize.height;
	}

	void* DirectXTexture::lock(bool _discard)
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

	void DirectXTexture::unlock()
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

	bool DirectXTexture::bindToStage(size_t _stage)
	{
		if (SUCCEEDED(mpD3DDevice->SetTexture(_stage, mpTexture)))
			return true;
		return false;
	}

} // namespace MyGUI
