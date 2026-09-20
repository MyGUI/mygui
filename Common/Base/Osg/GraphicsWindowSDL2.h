#pragma once

#include <SDL.h>
#include <osgViewer/GraphicsWindow>
#include <functional>

namespace base
{

	// Adapts an existing SDL window to OSG. Owns the GL context, not the window.
	// Window events and input remain with SdlBaseManager.
	class GraphicsWindowSDL2 : public osgViewer::GraphicsWindow
	{
	public:
		static void setContextAttributes();
		explicit GraphicsWindowSDL2(SDL_Window* _window);

		bool valid() const override;
		bool realizeImplementation() override;
		bool isRealizedImplementation() const override;
		void closeImplementation() override;
		bool makeCurrentImplementation() override;
		bool releaseContextImplementation() override;
		void swapBuffersImplementation() override;
		void setSyncToVBlank(bool _enabled) override;
		void grabFocus() override
		{
		}
		void grabFocusIfPointerInWindow() override
		{
		}
		void useCursor(bool) override
		{
		}

		// Runs with this context current, before presenting the completed frame.
		void setBeforeSwapCallback(std::function<void()> _callback);
		void updateDrawableSize();

	protected:
		~GraphicsWindowSDL2() override;

	private:
		SDL_Window* mWindow = nullptr;
		SDL_GLContext mContext = nullptr;
		bool mRealized = false;
		std::function<void()> mBeforeSwap;
	};

}
