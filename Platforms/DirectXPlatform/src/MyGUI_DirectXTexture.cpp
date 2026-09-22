/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include <d3d9.h>
#include <wincodec.h>
#include <vector>
#include "WICImageSaver.h"
#include "MyGUI_DirectXTexture.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXRTTexture.h"
#include "MyGUI_DirectXDiagnostic.h"

#include <filesystem>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	DirectXTexture::DirectXTexture(const std::string& _name, IDirect3DDevice9* _device) :
		mName(_name),
		mpD3DDevice(_device),
		mpTexture(nullptr),
		mNumElemBytes(0),
		mLock(false),
		mRenderTarget(nullptr),
		mInternalPool(D3DPOOL_MANAGED),
		mInternalFormat(D3DFMT_UNKNOWN),
		mInternalUsage(0)
	{
	}

	DirectXTexture::~DirectXTexture()
	{
		DirectXTexture::destroy();
	}

	const std::string& DirectXTexture::getName() const
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

		if (mTextureUsage.isValue(TextureUsage::RenderTarget))
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
			MYGUI_PLATFORM_EXCEPT("Creating texture with unknown pixel formal.");
		}

		HRESULT result = mpD3DDevice->CreateTexture(
			mSize.width,
			mSize.height,
			1,
			mInternalUsage,
			mInternalFormat,
			mInternalPool,
			&mpTexture,
			nullptr);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT(
				"Failed to create texture (error code "
				<< result << "): size '" << mSize << "' internal usage '" << mInternalUsage << "' internal format '"
				<< mInternalFormat << "'.");
		}
	}

	void DirectXTexture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Default;
		mPixelFormat = PixelFormat::R8G8B8A8;
		mNumElemBytes = 4;
		mInternalUsage = 0;
		mInternalPool = D3DPOOL_MANAGED;
		mInternalFormat = D3DFMT_A8R8G8B8;

		std::string fullnameUtf8 = DirectXDataManager::getInstance().getDataPath(_filename);
		const auto fullname = MyGUI::utility::toPath(fullnameUtf8);

		HRESULT coInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		bool comInitialized = (coInit == S_OK || coInit == S_FALSE);
		if (FAILED(coInit) && coInit != RPC_E_CHANGED_MODE)
		{
			MYGUI_PLATFORM_EXCEPT("Failed to initialize COM (error code " << coInit << ").");
		}

		IWICImagingFactory* wicFactory = nullptr;
		HRESULT result =
			CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
		if (FAILED(result))
		{
			if (comInitialized)
				CoUninitialize();
			MYGUI_PLATFORM_EXCEPT("Failed to create WIC imaging factory (error code " << result << ").");
		}

		IWICBitmapDecoder* decoder = nullptr;
		result = wicFactory->CreateDecoderFromFilename(
			fullname.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&decoder);
		if (FAILED(result))
		{
			wicFactory->Release();
			if (comInitialized)
				CoUninitialize();
			MYGUI_PLATFORM_EXCEPT("Failed to decode texture '" << _filename << "' (error code " << result << ").");
		}

		IWICBitmapFrameDecode* frame = nullptr;
		result = decoder->GetFrame(0, &frame);
		if (FAILED(result))
		{
			decoder->Release();
			wicFactory->Release();
			if (comInitialized)
				CoUninitialize();
			MYGUI_PLATFORM_EXCEPT("Failed to get frame from '" << _filename << "' (error code " << result << ").");
		}

		UINT width = 0, height = 0;
		frame->GetSize(&width, &height);
		mSize.set(width, height);

		IWICFormatConverter* converter = nullptr;
		result = wicFactory->CreateFormatConverter(&converter);
		if (SUCCEEDED(result))
		{
			result = converter->Initialize(
				frame,
				GUID_WICPixelFormat32bppBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.0f,
				WICBitmapPaletteTypeCustom);
		}
		if (FAILED(result))
		{
			if (converter)
				converter->Release();
			frame->Release();
			decoder->Release();
			wicFactory->Release();
			if (comInitialized)
				CoUninitialize();
			MYGUI_PLATFORM_EXCEPT("Failed to convert format for '" << _filename << "' (error code " << result << ").");
		}

		result = mpD3DDevice->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &mpTexture, nullptr);
		if (FAILED(result))
		{
			converter->Release();
			frame->Release();
			decoder->Release();
			wicFactory->Release();
			if (comInitialized)
				CoUninitialize();
			MYGUI_PLATFORM_EXCEPT(
				"Failed to create texture '"
				<< _filename << "' (error code " << result << "): size '" << mSize << "'.");
		}

		D3DLOCKED_RECT lockedRect;
		result = mpTexture->LockRect(0, &lockedRect, nullptr, 0);
		if (SUCCEEDED(result))
		{
			UINT stride = width * 4;
			UINT imageSize = stride * height;
			BYTE* pixels = static_cast<BYTE*>(lockedRect.pBits);

			if (stride == static_cast<UINT>(lockedRect.Pitch))
			{
				converter->CopyPixels(nullptr, stride, imageSize, pixels);
			}
			else
			{
				BYTE* temp = new BYTE[imageSize];
				converter->CopyPixels(nullptr, stride, imageSize, temp);
				for (UINT y = 0; y < height; ++y)
					memcpy(pixels + y * lockedRect.Pitch, temp + y * stride, stride);
				delete[] temp;
			}
			mpTexture->UnlockRect(0);
		}

		converter->Release();
		frame->Release();
		decoder->Release();
		wicFactory->Release();

		if (comInitialized)
			CoUninitialize();

		if (FAILED(result))
		{
			mpTexture->Release();
			mpTexture = nullptr;
			MYGUI_PLATFORM_EXCEPT("Failed to lock texture '" << _filename << "' (error code " << result << ").");
		}
	}

	void DirectXTexture::destroy()
	{
		if (mLock)
		{
			if (mStagingSurface)
				mStagingSurface->UnlockRect();
			else
				mpTexture->UnlockRect(0);
			mLock = false;
		}
		if (mStagingSurface)
		{
			mStagingSurface->Release();
			mStagingSurface = nullptr;
		}
		mWriteLock = false;
		mLockedRect = {};
		mLockBuffer.clear();
		delete mRenderTarget;
		mRenderTarget = nullptr;

		if (mpTexture != nullptr)
		{
			int nNewRefCount = mpTexture->Release();

			if (nNewRefCount > 0)
			{
				MYGUI_PLATFORM_EXCEPT(
					"The texture object failed to cleanup properly.\n"
					"Release() returned a reference count of '"
					<< nNewRefCount << "'.");
			}

			mpTexture = nullptr;
		}
	}

	int DirectXTexture::getWidth() const
	{
		return mSize.width;
	}

	int DirectXTexture::getHeight() const
	{
		return mSize.height;
	}

	void* DirectXTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mpTexture, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLock, "Texture is already locked");
		const bool read = _access.isValue(TextureUsage::Read);
		const bool write = _access.isValue(TextureUsage::Write);
		MYGUI_PLATFORM_ASSERT(read || write, "Texture lock requires read or write access");
		DWORD lockFlag = write ? 0 : D3DLOCK_READONLY;
		if (write && !read && (mInternalUsage & D3DUSAGE_DYNAMIC) != 0)
			lockFlag = D3DLOCK_DISCARD;

		HRESULT result;
		if ((mInternalUsage & D3DUSAGE_RENDERTARGET) != 0)
		{
			// Default-pool render targets are not lockable. Use a matching system-memory
			// surface, refreshing it from the GPU for every read or read/write lock.
			if (!mStagingSurface)
			{
				result = mpD3DDevice->CreateOffscreenPlainSurface(
					mSize.width,
					mSize.height,
					mInternalFormat,
					D3DPOOL_SYSTEMMEM,
					&mStagingSurface,
					nullptr);
				MYGUI_PLATFORM_ASSERT(SUCCEEDED(result), "Failed to create texture staging surface: " << result);
			}
			if (read)
			{
				IDirect3DSurface9* surface = nullptr;
				result = mpTexture->GetSurfaceLevel(0, &surface);
				MYGUI_PLATFORM_ASSERT(SUCCEEDED(result), "Failed to get render target surface: " << result);
				result = mpD3DDevice->GetRenderTargetData(surface, mStagingSurface);
				surface->Release();
				MYGUI_PLATFORM_ASSERT(SUCCEEDED(result), "Failed to read render target pixels: " << result);
			}
			result = mStagingSurface->LockRect(&mLockedRect, nullptr, lockFlag);
		}
		else
			result = mpTexture->LockRect(0, &mLockedRect, nullptr, lockFlag);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to lock texture (error code " << result << ").");
		}

		const size_t rowBytes = size_t(mSize.width) * mNumElemBytes;
		try
		{
			MYGUI_PLATFORM_ASSERT(
				mLockedRect.pBits && mLockedRect.Pitch >= 0 && size_t(mLockedRect.Pitch) >= rowBytes,
				"Invalid texture lock pitch");
			if (size_t(mLockedRect.Pitch) != rowBytes)
			{
				// ITexture::lock exposes tightly packed pixels, not the driver's row padding.
				mLockBuffer.resize(rowBytes * size_t(mSize.height));
				if (read)
				{
					const auto* source = static_cast<const unsigned char*>(mLockedRect.pBits);
					for (size_t y = 0; y < size_t(mSize.height); ++y)
						memcpy(mLockBuffer.data() + y * rowBytes, source + y * size_t(mLockedRect.Pitch), rowBytes);
				}
			}
		}
		catch (...)
		{
			if (mStagingSurface)
				mStagingSurface->UnlockRect();
			else
				mpTexture->UnlockRect(0);
			mLockedRect = {};
			throw;
		}

		mWriteLock = write;
		mLock = true;
		return size_t(mLockedRect.Pitch) == rowBytes ? mLockedRect.pBits : mLockBuffer.data();
	}

	void DirectXTexture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		const size_t rowBytes = size_t(mSize.width) * mNumElemBytes;
		if (mWriteLock && size_t(mLockedRect.Pitch) != rowBytes)
		{
			auto* destination = static_cast<unsigned char*>(mLockedRect.pBits);
			for (size_t y = 0; y < size_t(mSize.height); ++y)
				memcpy(destination + y * size_t(mLockedRect.Pitch), mLockBuffer.data() + y * rowBytes, rowBytes);
		}
		HRESULT result = mStagingSurface ? mStagingSurface->UnlockRect() : mpTexture->UnlockRect(0);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to unlock texture (error code " << result << ").");
		}

		mLock = false;
		const bool write = mWriteLock;
		mWriteLock = false;
		mLockedRect = {};
		if (mStagingSurface && write)
		{
			IDirect3DSurface9* surface = nullptr;
			result = mpTexture->GetSurfaceLevel(0, &surface);
			MYGUI_PLATFORM_ASSERT(SUCCEEDED(result), "Failed to get render target surface: " << result);
			result = mpD3DDevice->UpdateSurface(mStagingSurface, nullptr, surface, nullptr);
			surface->Release();
			MYGUI_PLATFORM_ASSERT(SUCCEEDED(result), "Failed to upload render target pixels: " << result);
		}
	}

	bool DirectXTexture::isLocked() const
	{
		return mLock;
	}

	PixelFormat DirectXTexture::getFormat() const
	{
		return mPixelFormat;
	}

	size_t DirectXTexture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

	TextureUsage DirectXTexture::getUsage() const
	{
		return mTextureUsage;
	}

	void DirectXTexture::saveToFile(const std::string& _filename)
	{
		if (!mpTexture)
		{
			MYGUI_PLATFORM_LOG(Warning, "Can't save empty texture to file '" << _filename << "'.");
			return;
		}

		D3DSURFACE_DESC desc;
		mpTexture->GetLevelDesc(0, &desc);

		IDirect3DSurface9* surface = nullptr;
		mpTexture->GetSurfaceLevel(0, &surface);
		if (!surface)
		{
			MYGUI_PLATFORM_LOG(Warning, "Failed to get surface level for saving.");
			return;
		}

		IDirect3DSurface9* tempSurface = nullptr;
		D3DLOCKED_RECT lockedRect;
		HRESULT hr;

		if (desc.Pool == D3DPOOL_DEFAULT)
		{
			hr = mpD3DDevice->CreateOffscreenPlainSurface(
				desc.Width,
				desc.Height,
				desc.Format,
				D3DPOOL_SYSTEMMEM,
				&tempSurface,
				nullptr);
			if (FAILED(hr))
			{
				surface->Release();
				MYGUI_PLATFORM_EXCEPT("Failed to create offscreen surface (error code " << hr << ").");
			}
			hr = mpD3DDevice->GetRenderTargetData(surface, tempSurface);
			if (FAILED(hr))
			{
				tempSurface->Release();
				surface->Release();
				MYGUI_PLATFORM_EXCEPT("Failed to get render target data (error code " << hr << ").");
			}
			hr = tempSurface->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
		}
		else
		{
			hr = surface->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
		}

		if (FAILED(hr))
		{
			if (tempSurface)
				tempSurface->Release();
			surface->Release();
			MYGUI_PLATFORM_EXCEPT("Failed to lock surface (error code " << hr << ").");
		}

		// Convert to 32-bit BGRA
		UINT width = desc.Width;
		UINT height = desc.Height;
		UINT bpp = 4;
		switch (desc.Format)
		{
		case D3DFMT_A8R8G8B8: bpp = 4; break;
		case D3DFMT_R8G8B8: bpp = 3; break;
		case D3DFMT_A8L8: bpp = 2; break;
		case D3DFMT_L8: bpp = 1; break;
		default: break;
		}

		BYTE* srcData = static_cast<BYTE*>(lockedRect.pBits);
		INT dstStride = width * 4;
		std::vector<BYTE> convertedData;
		BYTE* pixels = srcData;

		if (bpp != 4u || lockedRect.Pitch != dstStride)
		{
			convertedData.resize(height * dstStride);
			for (UINT y = 0; y < height; ++y)
			{
				BYTE* dst = convertedData.data() + y * dstStride;
				BYTE* src = srcData + y * lockedRect.Pitch;
				if (bpp == 4)
				{
					memcpy(dst, src, dstStride);
				}
				else if (bpp == 3)
				{
					for (UINT x = 0; x < width; ++x)
					{
						dst[x * 4 + 0] = src[x * 3 + 0];
						dst[x * 4 + 1] = src[x * 3 + 1];
						dst[x * 4 + 2] = src[x * 3 + 2];
						dst[x * 4 + 3] = 255;
					}
				}
				else if (bpp == 2)
				{
					for (UINT x = 0; x < width; ++x)
					{
						BYTE l = src[x * 2 + 0];
						BYTE a = src[x * 2 + 1];
						dst[x * 4 + 0] = l;
						dst[x * 4 + 1] = l;
						dst[x * 4 + 2] = l;
						dst[x * 4 + 3] = a;
					}
				}
				else
				{
					for (UINT x = 0; x < width; ++x)
					{
						dst[x * 4 + 0] = src[x];
						dst[x * 4 + 1] = src[x];
						dst[x * 4 + 2] = src[x];
						dst[x * 4 + 3] = 255;
					}
				}
			}
			pixels = convertedData.data();
		}

		const auto path = MyGUI::utility::toPath(_filename);

		HRESULT coInit = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		bool comInitialized = (coInit == S_OK || coInit == S_FALSE);

		hr = MyGUI::saveWICImage(path.c_str(), width, height, dstStride, pixels);

		if (comInitialized)
			CoUninitialize();

		if (desc.Pool == D3DPOOL_DEFAULT)
		{
			if (tempSurface)
			{
				tempSurface->UnlockRect();
				tempSurface->Release();
			}
		}
		else
		{
			surface->UnlockRect();
		}
		surface->Release();

		if (FAILED(hr))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to save texture to file '" << _filename << "' (error code " << hr << ").");
		}
	}

	IRenderTarget* DirectXTexture::getRenderTarget()
	{
		if (mpTexture == nullptr || (mInternalUsage & D3DUSAGE_RENDERTARGET) == 0)
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
			HRESULT result = mpD3DDevice->CreateTexture(
				mSize.width,
				mSize.height,
				1,
				mInternalUsage,
				mInternalFormat,
				D3DPOOL_DEFAULT,
				&mpTexture,
				nullptr);
			if (FAILED(result))
			{
				MYGUI_PLATFORM_EXCEPT("Failed to recreate texture on device restore (error code " << result << ").");
			}
		}
	}

} // namespace MyGUI
