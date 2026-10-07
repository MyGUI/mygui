/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#pragma warning(push, 0)
#include <d3d11.h>
#include <vector>
#pragma warning(pop)
#include "WICImageLoader.h"
#include "WICImageSaver.h"
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11RTTexture.h"
#include "MyGUI_DirectX11Diagnostic.h"

#include <filesystem>
#include <cstring>
#include <memory>
#include <limits>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	namespace
	{

		template<typename ReadPixels>
		HRESULT readTexturePixels(
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			ID3D11Texture2D* texture,
			ReadPixels&& readPixels)
		{
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;

			ID3D11Texture2D* staging = nullptr;
			HRESULT hr = device->CreateTexture2D(&desc, nullptr, &staging);
			bool isMapped = false;
			const auto release = [&](ID3D11Texture2D* value)
			{
				if (isMapped)
					context->Unmap(value, 0);
				value->Release();
			};
			std::unique_ptr<ID3D11Texture2D, decltype(release)> owner(staging, release);
			if (FAILED(hr))
				return hr;
			context->CopyResource(staging, texture);
			D3D11_MAPPED_SUBRESOURCE mapped{};
			hr = context->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
			if (FAILED(hr))
				return hr;
			isMapped = true;
			return readPixels(mapped);
		}

	}

	DirectX11Texture::DirectX11Texture(const std::string& _name, DirectX11RenderManager* _manager) :
		mTexture(nullptr),
		mResourceView(nullptr),
		mWidth(0),
		mHeight(0),
		mName(_name),
		mLock(false),
		mManager(_manager),
		mRenderTarget(nullptr)
	{
	}

	DirectX11Texture::~DirectX11Texture()
	{
		DirectX11Texture::destroy();
	}

	const std::string& DirectX11Texture::getName() const
	{
		return mName;
	}

	void DirectX11Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();
		mTextureUsage = _usage;

		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.Width = mWidth = _width;
		desc.Height = mHeight = _height;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Usage = D3D11_USAGE_DEFAULT;
		if (_usage.isValue(TextureUsage::RenderTarget))
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		else
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		HRESULT hr = mManager->mpD3DDevice->CreateTexture2D(&desc, nullptr, &mTexture);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Texture failed!");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = mManager->mpD3DDevice->CreateShaderResourceView(mTexture, &srvDesc, &mResourceView);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Shader ResourceView failed!");
	}

	void DirectX11Texture::loadFromFile(const std::string& _filename)
	{
		destroy();
		mTextureUsage = TextureUsage::Static | TextureUsage::Read | TextureUsage::Write;

		const auto path = MyGUI::utility::toPath(DirectX11DataManager::getInstance().getDataPath(_filename));
		std::vector<BYTE> pixels;
		HRESULT hr = loadWICImage(
			path.c_str(),
			[&](IWICBitmapSource* source, UINT width, UINT height)
			{
				mWidth = width;
				mHeight = height;
				pixels.resize(size_t(width) * height * 4);
				return source->CopyPixels(nullptr, width * 4, static_cast<UINT>(pixels.size()), pixels.data());
			});
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(hr), "Failed to load texture '" << _filename << "' (error code " << hr << ").");

		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.Width = mWidth;
		desc.Height = mHeight;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pixels.data();
		initData.SysMemPitch = mWidth * 4;
		initData.SysMemSlicePitch = 0;

		hr = mManager->mpD3DDevice->CreateTexture2D(&desc, &initData, &mTexture);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Texture failed for file '" << _filename << "'");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = mManager->mpD3DDevice->CreateShaderResourceView(mTexture, &srvDesc, &mResourceView);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Shader ResourceView failed for file '" << _filename << "'");
	}

	void DirectX11Texture::setShader(const std::string& _shaderName)
	{
		mShaderInfo = DirectX11RenderManager::getInstance().getShaderInfo(_shaderName);
	}

	void DirectX11Texture::destroy()
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
		mLockData.clear();
		mLock = false;

		if (mTexture)
		{
			mTexture->Release();
			mTexture = nullptr;
		}

		if (mResourceView)
		{
			mResourceView->Release();
			mResourceView = nullptr;
		}
	}

	int DirectX11Texture::getWidth() const
	{
		return mWidth;
	}

	int DirectX11Texture::getHeight() const
	{
		return mHeight;
	}

	void* DirectX11Texture::lock(TextureUsage _access)
	{
		if (mLock || !mTexture || (!_access.isValue(TextureUsage::Read) && !_access.isValue(TextureUsage::Write)))
			return nullptr;

		mLockData.resize(size_t(mWidth) * size_t(mHeight) * 4);
		if (_access.isValue(TextureUsage::Read))
		{
			const HRESULT hr = readTexturePixels(
				mManager->mpD3DDevice,
				mManager->mpD3DContext,
				mTexture,
				[&](const D3D11_MAPPED_SUBRESOURCE& mapped)
				{
					const size_t rowBytes = size_t(mWidth) * 4;
					for (int y = 0; y < mHeight; ++y)
						std::memcpy(
							mLockData.data() + size_t(y) * rowBytes,
							static_cast<const unsigned char*>(mapped.pData) + size_t(y) * mapped.RowPitch,
							rowBytes);
					return S_OK;
				});
			MYGUI_PLATFORM_ASSERT(SUCCEEDED(hr), "Failed to read texture pixels (error code " << hr << ").");
		}
		mLockAccess = _access;
		mLock = true;
		return mLockData.data();
	}

	void DirectX11Texture::unlock()
	{
		if (!mLock)
			return;
		mLock = false;

		if (mLockAccess.isValue(TextureUsage::Write))
		{
			mManager->mpD3DContext->UpdateSubresource(mTexture, 0, nullptr, mLockData.data(), mWidth * 4, 0);
		}
		mLockData.clear();
	}

	bool DirectX11Texture::isLocked() const
	{
		return mLock;
	}

	PixelFormat DirectX11Texture::getFormat() const
	{
		return PixelFormat::R8G8B8A8;
	}

	size_t DirectX11Texture::getNumElemBytes() const
	{
		return 4;
	}

	TextureUsage DirectX11Texture::getUsage() const
	{
		return mTextureUsage;
	}

	void DirectX11Texture::saveToFile(const std::string& _filename)
	{
		if (!mTexture)
		{
			MYGUI_PLATFORM_LOG(Warning, "Can't save empty texture to file '" << _filename << "'.");
			return;
		}

		const auto path = MyGUI::utility::toPath(_filename);
		const HRESULT hr = readTexturePixels(
			mManager->mpD3DDevice,
			mManager->mpD3DContext,
			mTexture,
			[&](const D3D11_MAPPED_SUBRESOURCE& mapped)
			{
				if (mHeight <= 0 || mapped.RowPitch > std::numeric_limits<UINT>::max() / UINT(mHeight))
					return E_INVALIDARG;
				return saveWICImage(
					path.c_str(),
					mWidth,
					mHeight,
					mapped.RowPitch,
					static_cast<const BYTE*>(mapped.pData));
			});
		MYGUI_PLATFORM_ASSERT(
			SUCCEEDED(hr),
			"Failed to save texture to file '" << _filename << "' (error code " << hr << ").");
	}

	IRenderTarget* DirectX11Texture::getRenderTarget()
	{
		if (mRenderTarget == nullptr)
			mRenderTarget = new DirectX11RTTexture(this, mManager);
		return mRenderTarget;
	}

} // namespace MyGUI
