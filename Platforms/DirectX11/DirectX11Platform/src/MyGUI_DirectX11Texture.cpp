/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#pragma warning(push, 0)
#include <d3d11.h>
#include <wincodec.h>
#include <vector>
#pragma warning(pop)
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11RTTexture.h"
#include "MyGUI_DirectX11Diagnostic.h"

namespace MyGUI
{

	DirectX11Texture::DirectX11Texture(const std::string& _name, DirectX11RenderManager* _manager) :
		mTexture(nullptr),
		mWriteData(nullptr),
		mResourceView(nullptr),
		mWidth(0),
		mHeight(0),
		mName(_name),
		mNumElemBytes(0),
		mLock(false),
		mManager(_manager),
		mRenderTarget(nullptr)
	{
	}

	DirectX11Texture::~DirectX11Texture()
	{
		destroy();
	}

	const std::string& DirectX11Texture::getName() const
	{
		return mName;
	}

	void DirectX11Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();

		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.Width = mWidth = _width;
		desc.Height = mHeight = _height;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Usage = D3D11_USAGE_DEFAULT;
		if (_usage == TextureUsage::RenderTarget)
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

		std::string fullname = DirectX11DataManager::getInstance().getDataPath(_filename);
		std::wstring wfullname(fullname.begin(), fullname.end());

		IWICImagingFactory* wicFactory = nullptr;
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&wicFactory));
		MYGUI_PLATFORM_ASSERT(SUCCEEDED(hr), "Failed to create WIC imaging factory!");

		IWICBitmapDecoder* decoder = nullptr;
		hr = wicFactory->CreateDecoderFromFilename(
			wfullname.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&decoder);
		if (FAILED(hr))
		{
			wicFactory->Release();
			MYGUI_PLATFORM_ASSERT(false, "Failed to decode image file!");
		}

		IWICBitmapFrameDecode* frame = nullptr;
		hr = decoder->GetFrame(0, &frame);
		if (FAILED(hr))
		{
			decoder->Release();
			wicFactory->Release();
			MYGUI_PLATFORM_ASSERT(false, "Failed to get image frame!");
		}

		IWICFormatConverter* converter = nullptr;
		hr = wicFactory->CreateFormatConverter(&converter);
		if (SUCCEEDED(hr))
		{
			hr = converter->Initialize(
				frame,
				GUID_WICPixelFormat32bppBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.0f,
				WICBitmapPaletteTypeMedianCut);
		}
		if (FAILED(hr))
		{
			frame->Release();
			decoder->Release();
			wicFactory->Release();
			MYGUI_PLATFORM_ASSERT(false, "Failed to convert image format!");
		}

		UINT imageWidth = 0, imageHeight = 0;
		converter->GetSize(&imageWidth, &imageHeight);
		mWidth = imageWidth;
		mHeight = imageHeight;

		std::vector<unsigned char> pixels(imageWidth * imageHeight * 4);
		hr = converter->CopyPixels(nullptr, imageWidth * 4, pixels.size(), pixels.data());

		converter->Release();
		frame->Release();
		decoder->Release();
		wicFactory->Release();

		MYGUI_PLATFORM_ASSERT(SUCCEEDED(hr), "Failed to copy image pixels!");

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
		initData.SysMemPitch = imageWidth * 4;
		initData.SysMemSlicePitch = 0;

		hr = mManager->mpD3DDevice->CreateTexture2D(&desc, &initData, &mTexture);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Texture failed!");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = mManager->mpD3DDevice->CreateShaderResourceView(mTexture, &srvDesc, &mResourceView);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Shader ResourceView failed!");
	}

	void DirectX11Texture::setShader(const std::string& _shaderName)
	{
		mShaderInfo = DirectX11RenderManager::getInstance().getShaderInfo(_shaderName);
	}

	void DirectX11Texture::destroy()
	{
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
		if (mLock)
			return nullptr;
		mLock = true;

		if (_access == TextureUsage::Write)
		{
			mWriteData = malloc(mWidth * mHeight * 4);
			return mWriteData;
		}
		return nullptr;
	}

	void DirectX11Texture::unlock()
	{
		if (!mLock)
			return;
		mLock = false;

		if (mWriteData)
		{
			mManager->mpD3DContext
				->UpdateSubresource(mTexture, D3D11CalcSubresource(0, 0, 0), nullptr, mWriteData, mWidth * 4, 0);
			free(mWriteData);
			mWriteData = nullptr;
		}
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

	IRenderTarget* DirectX11Texture::getRenderTarget()
	{
		if (mRenderTarget == nullptr)
			mRenderTarget = new DirectX11RTTexture(this, mManager);
		return mRenderTarget;
	}

} // namespace MyGUI
