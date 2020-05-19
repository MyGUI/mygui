#include "Precompiled.h"
#include "BaseManager.h"

#include <d3dx9.h>
#include <MyGUI_DirectXPlatform.h>

#include <SDL2/SDL_syswm.h>

namespace base
{
	D3DPRESENT_PARAMETERS mD3dpp;

	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		mD3d = Direct3DCreate9(D3D_SDK_VERSION);

		D3DDISPLAYMODE d3ddm;
		mD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

		memset(&mD3dpp, 0, sizeof(mD3dpp));
		mD3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		mD3dpp.EnableAutoDepthStencil = TRUE;
		mD3dpp.BackBufferCount  = 1;
		mD3dpp.BackBufferFormat = d3ddm.Format;
		mD3dpp.BackBufferWidth  = _width;
		mD3dpp.BackBufferHeight = _height;
		mD3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		mD3dpp.Windowed = _windowed;

		SDL_SysWMinfo sysWMInfo;
		SDL_VERSION(&sysWMInfo.version);
		SDL_GetWindowWMInfo(mSdlWindow, &sysWMInfo);
		mD3dpp.hDeviceWindow = sysWMInfo.info.win.window;

		if (FAILED(mD3d->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mD3dpp.hDeviceWindow,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &mD3dpp, &mDevice)))
		{
			return false;
		}
		return true;
	}

	void BaseManager::destroyRender()
	{
		if (mDevice)
		{
			mDevice->Release();
			mDevice = 0;
		}
		if (mD3d)
		{
			mD3d->Release();
			mD3d = 0;
		}
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::DirectXPlatform();
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
		if (mIsDeviceLost)
		{
			Sleep( 100 );

			HRESULT hr;
			if (FAILED(hr = mDevice->TestCooperativeLevel()))
			{
				if (hr == D3DERR_DEVICELOST)
					return;

				if (hr == D3DERR_DEVICENOTRESET)
				{
					if (mPlatform != nullptr)
						mPlatform->getRenderManagerPtr()->deviceLost();

					hr = mDevice->Reset( &mD3dpp );

					if (FAILED(hr))
						return;

					if (mPlatform != nullptr)
						mPlatform->getRenderManagerPtr()->deviceRestore();
				}

				return;
			}

			mIsDeviceLost = false;
		}

		if (SUCCEEDED(mDevice->BeginScene()))
		{
			mDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x001589FF, 1.0f, 0);
			mPlatform->getRenderManagerPtr()->drawOneFrame();
			mDevice->EndScene();
		}

		if (mDevice->Present(nullptr, nullptr, 0, nullptr) == D3DERR_DEVICELOST)
			mIsDeviceLost = true;
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		if (mDevice != nullptr)
		{
			if (mPlatform != nullptr)
				mPlatform->getRenderManagerPtr()->deviceLost();

			mD3dpp.BackBufferWidth = _width;
			mD3dpp.BackBufferHeight = _height;
			HRESULT hr = mDevice->Reset(&mD3dpp);

			if (hr == D3DERR_INVALIDCALL)
			{
				MessageBox(
					nullptr,
					"Call to Reset() failed with D3DERR_INVALIDCALL! ",
					"ERROR",
					MB_OK | MB_ICONEXCLAMATION);
			}

			if (mPlatform != nullptr)
				mPlatform->getRenderManagerPtr()->deviceRestore();
		}
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

}
