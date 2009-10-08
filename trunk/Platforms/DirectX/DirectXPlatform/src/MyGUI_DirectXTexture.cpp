/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
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
#include "MyGUI_DirectXDataManager.h"

#include <d3dx9.h>

namespace MyGUI
{

	DirectXTexture::DirectXTexture(const std::string& _name, IDirect3DDevice9 *_device) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(NULL),
		mNumElemBytes(0)
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

	void DirectXTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();

		unsigned long usage = 0;
		D3DFORMAT format = D3DFMT_UNKNOWN;

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;

		if (mTextureUsage == TextureUsage::Dynamic)
			usage |= D3DUSAGE_DYNAMIC;
		if (mTextureUsage == TextureUsage::Stream)
			usage |= D3DUSAGE_DYNAMIC;

		if (mPixelFormat == PixelFormat::R8G8B8A8)
		{
			format = D3DFMT_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == PixelFormat::R8G8B8)
		{
			format = D3DFMT_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == PixelFormat::L8A8)
		{
			format = D3DFMT_A8L8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == PixelFormat::L8)
		{
			format = D3DFMT_L8;
			mNumElemBytes = 1;
		}
		else
		{
			//exception
		}

		if (FAILED(mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, usage, format, D3DPOOL_MANAGED, &mpTexture, NULL)))
		{
			//exception
		}

	}

	void DirectXTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::R8G8B8A8;
    
		std::string fullname = DirectXDataManager::getInstance().getDataPath(_filename);

		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(fullname.c_str(), &info);

		if (info.Format == D3DFMT_A8R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8A8;
			mNumElemBytes = 4;
		}
		else if (info.Format == D3DFMT_R8G8B8)
		{
			mPixelFormat = PixelFormat::R8G8B8;
			mNumElemBytes = 3;
		}
		else if (info.Format == D3DFMT_A8L8)
		{
			mPixelFormat = PixelFormat::L8A8;
			mNumElemBytes = 2;
		}
		else if (info.Format == D3DFMT_L8)
		{
			mPixelFormat = PixelFormat::L8;
			mNumElemBytes = 1;
		}
		else
		{
			//exception
		}

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

	void* DirectXTexture::lock(TextureUsage _access)
	{
		D3DLOCKED_RECT d3dlr;
		if (_access == TextureUsage::Write)
		{
			if (SUCCEEDED(mpTexture->LockRect(0, &d3dlr, NULL, D3DLOCK_DISCARD)))
			{
				mLock = true;
			}
			else
			{
				//exception
				return nullptr;
			}
		}
		else
		{
			if (SUCCEEDED(mpTexture->LockRect(0, &d3dlr, NULL, D3DLOCK_READONLY)))
			{
				mLock = true;
			}
			else
			{
				//exception
				return nullptr;
			}
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
		return mNumElemBytes;
	}

	TextureUsage DirectXTexture::getUsage()
	{
		return mTextureUsage;
	}

	bool DirectXTexture::bindToStage(size_t _stage)
	{
		if (SUCCEEDED(mpD3DDevice->SetTexture(_stage, mpTexture)))
			return true;
		return false;
	}

} // namespace MyGUI
