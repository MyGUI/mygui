/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef BASE_MANAGER_H_
#define BASE_MANAGER_H_

#include <MyGUI.h>

#include "InputManager.h"
#include "PointerManager.h"

namespace MyGUI
{
	class DirectX11Platform;
}

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;


struct HWND__;
typedef HWND__* HWND;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

namespace base
{

	class BaseManager :
		public input::InputManager,
		public input::PointerManager
	{
	public:
		BaseManager();
		virtual ~BaseManager();

		virtual void prepare(); // инициализация коммандной строки
		bool create(int _width = 1024, int _height = 768); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();
		void quit();

		void setWindowCaption(const std::wstring& _text);
		void makeScreenShot() { }

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);
		void addResourceLocation(const std::string& _name, bool _recursive = false);

		size_t getWindowHandle();

		MyGUI::MapString getStatistic() { return MyGUI::MapString(); }

		/*internal:*/
		void _windowResized();

	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
		virtual void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id);
		virtual void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text);
		virtual void injectKeyRelease(MyGUI::KeyCode _key);

		virtual void createGui();
		virtual void destroyGui();

		virtual void setWindowMaximized(bool _value);
		virtual bool getWindowMaximized();

		virtual void setWindowCoord(const MyGUI::IntCoord& _value);
		virtual MyGUI::IntCoord getWindowCoord();

	private:
		void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);

		void resizeRender(int _width, int _height);
		bool createRender(int _width, int _height, bool _windowed);
		void drawOneFrame();
		void destroyRender();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::DirectX11Platform* mPlatform;

		HWND hWnd;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RenderTargetView* mRenderTarget;
		HINSTANCE hInstance;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
	};

} // namespace base

#endif // BASE_MANAGER_H_
