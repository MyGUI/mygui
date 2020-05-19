#include "Precompiled.h"
#include "BaseManager.h"

#pragma warning(push, 0)
#include <d3dx11.h>
#pragma warning(pop)
#include <MyGUI_DirectX11Platform.h>

#include <SDL2/SDL_syswm.h>

namespace base
{
	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		static const int numFeatureLevels = 6;

		D3D_FEATURE_LEVEL selectedFeatureLevel = D3D_FEATURE_LEVEL_11_0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = _width;
		swapChainDesc.BufferDesc.Height = _height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = _windowed;

		SDL_SysWMinfo sysWMInfo;
		SDL_VERSION(&sysWMInfo.version);
		SDL_GetWindowWMInfo(mSdlWindow, &sysWMInfo);
		swapChainDesc.OutputWindow = sysWMInfo.info.win.window;

		HRESULT hr = S_OK;

		if (FAILED(hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&mSwapChain,
			&mDevice,
			&selectedFeatureLevel,
			&mDeviceContext)))
		{
			return false;
		}

		hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&mBackBuffer);

		mDevice->CreateRenderTargetView(mBackBuffer, nullptr, &mRenderTarget );

		mDeviceContext->OMSetRenderTargets( 1, &mRenderTarget, nullptr );

		D3D11_VIEWPORT vp;
		vp.Width = (float)_width;
		vp.Height = (float)_height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		mDeviceContext->RSSetViewports( 1, &vp );
		return true;
	}

	void BaseManager::destroyRender()
	{
		if (mRenderTarget)
		{
			mRenderTarget->Release();
			mRenderTarget = nullptr;
		}
		if (mBackBuffer) {
			mBackBuffer->Release();
			mBackBuffer = nullptr;
		}
		if (mSwapChain)
		{
			mSwapChain->Release();
			mSwapChain = nullptr;
		}
		if (mDeviceContext)
		{
			mDeviceContext->Release();
			mDeviceContext = nullptr;
		}
		if (mDevice)
		{
			mDevice->Release();
			mDevice = nullptr;
		}
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::DirectX11Platform();
		mPlatform->initialise(mDevice);
		setupResources();
	}

	void BaseManager::destroyGuiPlatform()
	{
		if (mPlatform)
		{
			mPlatform->shutdown();
			delete mPlatform;
			mPlatform = nullptr;
		}
	}

	void BaseManager::drawOneFrame()
	{
		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		mDeviceContext->ClearRenderTargetView(mRenderTarget, clearColor);
		mPlatform->getRenderManagerPtr()->drawOneFrame();
		mSwapChain->Present(0, 0);
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		if (mDevice != nullptr)
		{
			// Release the current render target view and its back buffer
			mDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			if (mRenderTarget)
			{
				mRenderTarget->Release();
				mRenderTarget = nullptr;
			}
			if (mBackBuffer) {
				mBackBuffer->Release();
				mBackBuffer = nullptr;
			}

			// Resize
			mSwapChain->ResizeBuffers(1, _width, _height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			// Acquire the new back buffer
			mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);

			// Create a new render target from the resized buffer
			mDevice->CreateRenderTargetView(mBackBuffer, nullptr, &mRenderTarget);
			// Set the new render target
			mDeviceContext->OMSetRenderTargets(1, &mRenderTarget, nullptr);

			// Устанавливаем новый вьюпорт
			D3D11_VIEWPORT vp;
			vp.Width = (float)_width;
			vp.Height = (float)_height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0.0f;
			vp.TopLeftY = 0.0f;
			mDeviceContext->RSSetViewports( 1, &vp );
		}
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

}
