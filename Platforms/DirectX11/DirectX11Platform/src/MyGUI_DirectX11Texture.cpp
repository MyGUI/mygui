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
		HRESULT hr = mManager->mpD3DDevice->CreateTexture2D(&desc, 0, &mTexture);
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


		D3DX11_IMAGE_INFO fileInfo;
		D3DX11GetImageInfoFromFile( fullname.c_str(), NULL, &fileInfo, NULL );

		mWidth = fileInfo.Width;
		mHeight = fileInfo.Height;

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width          = fileInfo.Width;
		loadInfo.Height         = fileInfo.Height;
		loadInfo.FirstMipLevel  = 0;
		loadInfo.MipLevels      = fileInfo.MipLevels;
		loadInfo.Usage          = D3D11_USAGE_DEFAULT;
		loadInfo.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags      = 0;
		loadInfo.Format         = fileInfo.Format;
		loadInfo.Filter         = D3DX11_FILTER_NONE;
		loadInfo.MipFilter      = D3DX11_FILTER_NONE;
		loadInfo.pSrcInfo       = &fileInfo;

		HRESULT hr = D3DX11CreateTextureFromFileA( mManager->mpD3DDevice, fullname.c_str(), &loadInfo, NULL, (ID3D11Resource**)&mTexture, NULL );
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "CreateTextureFromFile failed!");

		D3D11_TEXTURE2D_DESC desc;
		mTexture->GetDesc(&desc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

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
			mWriteData = malloc(mWidth * mHeight * 4);
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
			mManager->mpD3DContext->UpdateSubresource(mTexture, D3D11CalcSubresource(0, 0, 0), 0, mWriteData, mWidth * 4, 0);
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
		return PixelFormat::R8G8B8A8;
	}

	size_t DirectX11Texture::getNumElemBytes()
	{
		return 4;
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

} // namespace MyGUI
