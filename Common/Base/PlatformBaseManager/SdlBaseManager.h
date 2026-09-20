#pragma once

#include <MyGUI.h>

#include "Input/SDL/InputManager.h"
#include "Input/SDL/PointerManager.h"

#include <SDL.h>

#include <chrono>
#include <filesystem>

namespace base
{

	class SdlBaseManager : public input::InputManager, public input::PointerManager
	{
	public:
		SdlBaseManager(uint32_t _windowFlags);

		struct FrameCapture
		{
			enum class Status
			{
				Idle,
				Pending,
				Complete,
				Failed
			};
			Status status{Status::Idle};
			int width{0};
			int height{0};
			std::vector<std::uint8_t> rgba;
			std::vector<float> depth;
			std::string error;
			bool fatal{true};
		};

		// Application defaults are unchanged; native tests request fixed drawable pixels.
		void setWindowOptions(bool _hidden, bool _fixedPixels);
		bool isWindowHidden() const;
		MyGUI::IntSize getDrawableSize() const;
		void resizeWindow(int _width, int _height);
		void requestFrameCapture();
		const FrameCapture& getFrameCapture() const;

		// for platform unit tests only
		virtual bool setHostileRenderState(bool _enabled)
		{
			return false;
		}
		// for platform unit tests only
		virtual bool setSceneDepthProbe(bool _enabled)
		{
			return false;
		}

		// Block of virtual functions for specific platform implementations
		virtual bool createRender(int _width, int _height, bool _windowed) = 0;
		virtual void destroyRender() = 0;
		virtual void createGuiPlatform() = 0;
		virtual void destroyGuiPlatform() = 0;
		virtual void drawOneFrame() = 0;
		virtual void resizeRender(int _width, int _height) = 0;
		virtual void addResourceLocation(const std::filesystem::path& _name, bool _recursive = false) = 0;

		bool create(int _width = 1024, int _height = 768);
		void destroy();
		void run();
		void quit();

		void setWindowCaption(const MyGUI::UString& _text);

		const std::filesystem::path& getRootMedia() const;
		void setResourceFilename(std::string_view _flename);
		void makeScreenShot();
		void setCommandLine(int _argc, char** _argv);

		using VectorPath = std::vector<std::filesystem::path>;
		const VectorPath& getParams() const;
		bool isScreenShotMode() const;

		/*internal:*/
		void _windowResized(int w, int h);

		virtual void prepare()
		{
		}
		virtual MyGUI::MapString getStatistic();

	protected:
		void completeFrameCapture(
			const void* _pixels,
			int _width,
			int _height,
			size_t _pitch,
			bool _bgra,
			bool _bottomUp,
			const std::vector<float>& _depth = {});
		void failFrameCapture(std::string_view _error, bool _fatal = true);
		bool mCaptureRequested{false};
		bool mSceneDepthProbe{false};
		virtual void setupRenderWindow()
		{
		}

		virtual void createScene()
		{
		}
		virtual void destroyScene()
		{
		}

		virtual void setupResources();

		void setupBinaryDir();

		void injectMouseMove(int _absx, int _absy, int _absz) override;
		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id) override;
		void injectKeyPress(MyGUI::KeyCode _key, MyGUI::Char _text) override;
		void injectKeyRelease(MyGUI::KeyCode _key) override;

		virtual void createGui(float _dpiScale);
		virtual void destroyGui();

		virtual void setWindowMaximized(bool _value);
		virtual bool getWindowMaximized() const;

		virtual void setWindowCoord(const MyGUI::IntCoord& _value);
		virtual MyGUI::IntCoord getWindowCoord() const;

		void* convertPixelData(SDL_Surface* _image, MyGUI::PixelFormat& _myGuiPixelFormat);

	protected:
		SDL_Window* mSdlWindow = nullptr;
		bool mEnableVSync = false;
		// SDL window and mouse coordinates may already be logical points (macOS).
		float mWindowScale = 1.0f;
		bool mScreenShotRequested = false;
		std::filesystem::path mScreenShotFile;
		bool mScreenShotExit = false;
		int mScreenShotFramesLeft = 0;
		VectorPath mParams;
		std::filesystem::path mBinaryDir;

	private:
		MyGUI::Gui* mGUI = nullptr;

		uint32_t mWindowFlags = 0;
		bool mPlatformReady = false;
		bool mHiddenWindow = false;
		bool mFixedPixels = false;
		FrameCapture mFrameCapture;
		bool mExit = false;
		SDL_Event mEvent;
		std::filesystem::path mRootMedia;
		std::string mResourceFileName = "MyGUI_Core.xml";
		bool mWindowOn = false;
		SDL_Keycode mKeyCode;
		int mFpsCounter = 0;
		std::chrono::steady_clock::time_point mNextFrameTime{};
	};

}
