/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#include <d3dx11.h>
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11RTTexture.h"
#include "MyGUI_DirectX11Diagnostic.h"

MyGUI::ICreateTextureCallbackStruct* MyGUI::DirectX11Texture::sCreateTextureCallback = nullptr;

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

	static DXGI_FORMAT getDXGIFormatFromPixelFormat(PixelFormat _format)
	{
		switch(_format.getValue())
		{
		case PixelFormat::L8:
			return DXGI_FORMAT_R8_UNORM;
		case PixelFormat::L8A8:
			return DXGI_FORMAT_R8G8_UNORM;
		case PixelFormat::R8G8B8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PixelFormat::R8G8B8A8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PixelFormat::Unknow:
		default:
			MYGUI_PLATFORM_ASSERT(false, "Unknown pixel format!");
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	
	void DirectX11Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		destroy();

		mPixelFormat = _format;
		
		D3D11_TEXTURE2D_DESC desc;
		desc.ArraySize = 1;
		desc.Width = mWidth = _width;
		desc.Height = mHeight = _height;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;		
		desc.Format = getDXGIFormatFromPixelFormat(_format);//DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.Usage = D3D11_USAGE_DEFAULT;
		if (_usage == TextureUsage::RenderTarget)
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		else
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		HRESULT hr = mManager->mpD3DDevice->CreateTexture2D(&desc, 0, &mTexture);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Texture failed!");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;//DXGI_FORMAT_B8G8R8A8_UNORM;
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
		HRESULT hr;

		if( sCreateTextureCallback )
		{
			mTexture = sCreateTextureCallback->createTextureFromFile( fullname );

			D3D11_TEXTURE2D_DESC desc;
			mTexture->GetDesc( &desc );

			mWidth = desc.Width;
			mHeight = desc.Height;
		}

		D3D11_TEXTURE2D_DESC desc;
		mTexture->GetDesc(&desc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		mPixelFormat = PixelFormat::R8G8B8A8;
		
		hr = mManager->mpD3DDevice->CreateShaderResourceView(mTexture, &srvDesc, &mResourceView);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Shader ResourceView failed!");
	}

	void DirectX11Texture::destroy()
	{
		if ( mTexture )
		{
			mTexture->Release();
			mTexture = 0;
		}

		if ( mResourceView )
		{
			mResourceView->Release();
			mResourceView = 0;
		}
	}

	int DirectX11Texture::getWidth()
	{
		return mWidth;
	}

	int DirectX11Texture::getHeight()
	{
		return mHeight;
	}

	void* DirectX11Texture::lock(TextureUsage _access)
	{
		if ( mLock ) return 0;
		mLock = true;

		if ( _access == TextureUsage::Write )
		{
			mWriteData = malloc(mWidth * mHeight * getNumElemBytes());
			return mWriteData;
		}
		return 0;
	}

	void DirectX11Texture::unlock()
	{
		if ( !mLock ) return;
		mLock = false;

		if ( mWriteData )
		{
			mManager->mpD3DContext->UpdateSubresource(mTexture, D3D11CalcSubresource( 0, 0, 0 ), 0, mWriteData, mWidth * getNumElemBytes(), 0);
			free(mWriteData);
			mWriteData = 0;
		}
	}

	bool DirectX11Texture::isLocked()
	{
		return mLock;
	}

	PixelFormat DirectX11Texture::getFormat()
	{
		return mPixelFormat;
	}

	size_t DirectX11Texture::getNumElemBytes()
	{
		switch(mPixelFormat.getValue())
		{
		case PixelFormat::L8:
			return 1;
		case PixelFormat::L8A8:
			return 2;
		case PixelFormat::R8G8B8:
			return 4;
		case PixelFormat::R8G8B8A8:
			return 4;
		case PixelFormat::Unknow:
		default:
			MYGUI_PLATFORM_ASSERT(false, "Unknown pixel format!");
			return 0;
		}
	}

	TextureUsage DirectX11Texture::getUsage()
	{
		return mTextureUsage;
	}

	IRenderTarget* DirectX11Texture::getRenderTarget()
	{
		if ( mRenderTarget == 0 ) mRenderTarget = new DirectX11RTTexture(this, mManager);
		return mRenderTarget;
	}

	ID3D11Texture2D* DirectX11Texture::getTexture()
	{
		return mTexture;
	}

	void DirectX11Texture::setCreateTextureCallback( ICreateTextureCallbackStruct* _callbackStruct )
	{
		sCreateTextureCallback = _callbackStruct;
	}

} // namespace MyGUI
