#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

namespace MyGUI
{
	class DirectX11Platform;
}

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;
struct ID3D11Texture2D;

struct HWND__;
typedef HWND__* HWND;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

namespace base
{

	class BaseManager :
		public SdlBaseManager
	{
	public:
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

	private:
		MyGUI::DirectX11Platform* mPlatform = nullptr;

		ID3D11Device* mDevice = nullptr;
		ID3D11DeviceContext* mDeviceContext = nullptr;
		IDXGISwapChain* mSwapChain = nullptr;
		ID3D11Texture2D* mBackBuffer = nullptr;
		ID3D11RenderTargetView* mRenderTarget = nullptr;
	};

}
