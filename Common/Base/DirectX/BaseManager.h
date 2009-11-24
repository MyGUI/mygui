/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef __BASE_MANAGER_H__
#define __BASE_MANAGER_H__

#include <MyGUI.h>
#include <MyGUI_DirectXPlatform.h>
#include "Base/StatisticInfo.h"
#include "Base/InputFocusInfo.h"

namespace base
{

	class BaseManager :
		public input::InputManager,
		public input::PointerManager
	{
	public:
		BaseManager();
		virtual ~BaseManager();

		virtual void prepare(int argc, char **argv); // инициализация коммандной строки
		bool create(); // создаем начальную точки каркаса приложения
		void destroy(); // очищаем все параметры каркаса приложения
		void run();
		void quit() { mExit = true; }

		void setWindowCaption(const std::string& _text);
		void createDefaultScene() { }

		MyGUI::Gui* getGUI() { return mGUI; }
		const std::string& getRootMedia() { return mRootMedia; }
		void setResourceFilename(const std::string& _flename) { mResourceFileName = _flename; }
		void addResourceLocation(const std::string& _name, bool _recursive = false);
		diagnostic::StatisticInfo * getStatisticInfo() { return mInfo; }

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

	private:
		void createGui();
		void destroyGui();

		void windowAdjustSettings(HWND hWnd, int width, int height, bool fullScreen);
		void updateFPS();

		void resizeRender(int _width, int _height);
		bool createRender(int _width, int _height, bool _windowed);
		void drawOneFrame();
		void destroyRender();

	private:
		MyGUI::Gui* mGUI;
		MyGUI::DirectXPlatform* mPlatform;
		diagnostic::StatisticInfo* mInfo;
		diagnostic::InputFocusInfo* mFocusInfo;

		HWND hWnd;
	    IDirect3D9 *mD3d;
		IDirect3DDevice9 *mDevice;
	    D3DPRESENT_PARAMETERS mD3dpp;
		HINSTANCE hInstance;

		bool mExit;

		std::string mRootMedia;
		std::string mResourceFileName;
	};

} // namespace base

#endif // __BASE_MANAGER_H__
