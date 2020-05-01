#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

namespace MyGUI
{
	class DirectXPlatform;
}

struct IDirect3D9;
struct IDirect3DDevice9;

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
		MyGUI::DirectXPlatform* mPlatform = nullptr;

		IDirect3D9* mD3d = nullptr;
		IDirect3DDevice9* mDevice = nullptr;
		bool mIsDeviceLost = false;
	};

}
