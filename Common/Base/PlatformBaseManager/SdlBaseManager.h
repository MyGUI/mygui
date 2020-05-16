#pragma once

#include <MyGUI.h>

#include "InputManager.h"
#include "PointerManager.h"

#include <SDL2/SDL.h>

namespace base
{

	class SdlBaseManager :
		public input::InputManager,
		public input::PointerManager
	{
	public:
        SdlBaseManager();

        // Block of virtual functions for specific platform implementations
		virtual bool createRender(int _width, int _height, bool _windowed) = 0;
		virtual void destroyRender() = 0;
		virtual void createGuiPlatform() = 0;
		virtual void destroyGuiPlatform() = 0;
		virtual void drawOneFrame() = 0;
		virtual void resizeRender(int _width, int _height) = 0;
		virtual void addResourceLocation(const std::string& _name, bool _recursive = false) = 0;
		virtual void makeScreenShot() { MYGUI_LOG(Warning, "makeScreenShot not implemented"); }

		bool create(int _width = 1024, int _height = 768);
		void destroy();
		void run();
		void quit();

		void setWindowCaption(const std::wstring& _text);

		const std::string& getRootMedia();
		void setResourceFilename(const std::string& _flename);

	/*internal:*/
		void _windowResized(int w, int h);

		virtual void prepare() { }
		virtual MyGUI::MapString getStatistic();
	protected:
		virtual void createScene() { }
		virtual void destroyScene() { }

		virtual void setupResources();

		void injectMouseMove(int _absx, int _absy, int _absz) override;
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;
		void injectKeyRelease(MyGUI::KeyCode _key) override;

		virtual void createGui();
		virtual void destroyGui();

		virtual void setWindowMaximized(bool _value);
		virtual bool getWindowMaximized();

		virtual void setWindowCoord(const MyGUI::IntCoord& _value);
		virtual MyGUI::IntCoord getWindowCoord();

		void* convertPixelData(SDL_Surface *_image, MyGUI::PixelFormat& _myGuiPixelFormat);

	protected:
		SDL_Window* mSdlWindow = nullptr;

	private:
		MyGUI::Gui* mGUI = nullptr;

		bool mPlatformReady = false;
		bool mExit = false;
		SDL_Event mEvent;
		std::string mRootMedia;
		std::string mResourceFileName = "MyGUI_Core.xml";
		bool mWindowOn = false;
		SDL_Keycode	mKeyCode;
		int mFpsCounter = 0;
	};

}
