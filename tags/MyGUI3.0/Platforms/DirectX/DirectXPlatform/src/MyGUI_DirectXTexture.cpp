/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DirectXTexture.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXRTTexture.h"

//#include <d3dx9.h>

namespace MyGUI
{

	DirectXTexture::DirectXTexture(const std::string& _name, IDirect3DDevice9 *_device) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(NULL),
		mNumElemBytes(0),
		mRenderTarget(nullptr),
		mInternalPool(D3DPOOL_MANAGED),
		mInternalFormat(D3DFMT_UNKNOWN),
		mInternalUsage(0)
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

		mInternalUsage = 0;
		mInternalFormat = D3DFMT_UNKNOWN;

		mSize.set(_width, _height);
		mTextureUsage = _usage;
		mPixelFormat = _format;
		mInternalPool = D3DPOOL_MANAGED;

		if (mTextureUsage == TextureUsage::RenderTarget)
		{
			mInternalUsage |= D3DUSAGE_RENDERTARGET;
			mInternalPool = D3DPOOL_DEFAULT;
		}
		else if (mTextureUsage == TextureUsage::Dynamic)
			mInternalUsage |= D3DUSAGE_DYNAMIC;
		else if (mTextureUsage == TextureUsage::Stream)
			mInternalUsage |= D3DUSAGE_DYNAMIC;

		if (mPixelFormat == PixelFormat::R8G8B8A8)
		{
			mInternalFormat = D3DFMT_A8R8G8B8;
			mNumElemBytes = 4;
		}
		else if (mPixelFormat == PixelFormat::R8G8B8)
		{
			mInternalFormat = D3DFMT_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (mPixelFormat == PixelFormat::L8A8)
		{
			mInternalFormat = D3DFMT_A8L8;
			mNumElemBytes = 2;
		}
		else if (mPixelFormat == PixelFormat::L8)
		{
			mInternalFormat = D3DFMT_L8;
			mNumElemBytes = 1;
		}
		else
		{
			//exception
		}

		if (FAILED(mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, mInternalUsage, mInternalFormat, mInternalPool, &mpTexture, NULL)))
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
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (mpTexture != nullptr)
		{
			int nNewRefCount = mpTexture->Release();

			if (nNewRefCount > 0)
			{
				static char strError[255];
				sprintf( strError, "The texture object failed to cleanup properly.\n"
					"Release() returned a reference count of %d", nNewRefCount );
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

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

	IRenderTarget* DirectXTexture::getRenderTarget()
	{
		if (mpTexture == nullptr)
			return nullptr;

		if (mRenderTarget == nullptr)
			mRenderTarget = new DirectXRTTexture(mpD3DDevice, mpTexture);

		return mRenderTarget;
	}

	void DirectXTexture::deviceLost()
	{
		if (mInternalPool == D3DPOOL_DEFAULT)
		{
			destroy();
		}
	}

	void DirectXTexture::deviceRestore()
	{
		if (mInternalPool == D3DPOOL_DEFAULT)
		{
			if (FAILED(mpD3DDevice->CreateTexture(mSize.width, mSize.height, 1, mInternalUsage, mInternalFormat, D3DPOOL_DEFAULT, &mpTexture, NULL)))
			{
				MessageBox( NULL, "Error recreate texture", "ERROR", MB_OK | MB_ICONEXCLAMATION );
				//exception
			}
		}
	}

} // namespace MyGUI
