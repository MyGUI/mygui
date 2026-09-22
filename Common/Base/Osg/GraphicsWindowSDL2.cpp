#include "GraphicsWindowSDL2.h"

#include <osg/State>
#include <utility>

namespace base
{

	void GraphicsWindowSDL2::setContextAttributes()
	{
		// Set before SDL_CreateWindow, so SDL chooses a matching pixel format too.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#if OSG_GL3_FEATURES
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#else
		// GL2-built OSG still applies fixed-function state, even for shader drawables.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	GraphicsWindowSDL2::GraphicsWindowSDL2(SDL_Window* _window) :
		mWindow(_window)
	{
		_traits = new osg::GraphicsContext::Traits;
		_traits->doubleBuffer = true;
		if (!mWindow)
			return;

		SDL_Window* previousWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext previousContext = SDL_GL_GetCurrentContext();
		mContext = SDL_GL_CreateContext(mWindow);
		if (!mContext)
			return;

		setState(new osg::State);
		// Camera traversal must update shader matrices from the very first frame.
		getState()->setUseModelViewAndProjectionUniforms(true);
		getState()->setGraphicsContext(this);
		getState()->setContextID(osg::GraphicsContext::createNewContextID());
		updateDrawableSize();
		SDL_GL_MakeCurrent(previousWindow, previousContext);
	}

	GraphicsWindowSDL2::~GraphicsWindowSDL2()
	{
		close(true);
	}

	bool GraphicsWindowSDL2::valid() const
	{
		return mContext != nullptr;
	}

	bool GraphicsWindowSDL2::realizeImplementation()
	{
		// Keep hidden screenshot/test windows hidden. SDL owns window visibility.
		mRealized = valid();
		return mRealized;
	}

	bool GraphicsWindowSDL2::isRealizedImplementation() const
	{
		return mRealized;
	}

	void GraphicsWindowSDL2::closeImplementation()
	{
		mBeforeSwap = {};
		if (mContext)
			SDL_GL_DeleteContext(mContext);
		mContext = nullptr;
		mWindow = nullptr;
		mRealized = false;
	}

	bool GraphicsWindowSDL2::makeCurrentImplementation()
	{
		return valid() && SDL_GL_MakeCurrent(mWindow, mContext) == 0;
	}

	bool GraphicsWindowSDL2::releaseContextImplementation()
	{
		return SDL_GL_MakeCurrent(mWindow, nullptr) == 0;
	}

	void GraphicsWindowSDL2::swapBuffersImplementation()
	{
		if (!mRealized)
			return;
		if (mBeforeSwap)
			mBeforeSwap();
		SDL_GL_SwapWindow(mWindow);
	}

	void GraphicsWindowSDL2::setSyncToVBlank(bool _enabled)
	{
		if (!valid())
			return;
		SDL_Window* previousWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext previousContext = SDL_GL_GetCurrentContext();
		if (makeCurrentImplementation())
		{
			if (SDL_GL_SetSwapInterval(_enabled ? 1 : 0) == 0)
				_traits->vsync = _enabled;
			SDL_GL_MakeCurrent(previousWindow, previousContext);
		}
	}

	void GraphicsWindowSDL2::setBeforeSwapCallback(std::function<void()> _callback)
	{
		mBeforeSwap = std::move(_callback);
	}

	void GraphicsWindowSDL2::updateDrawableSize()
	{
		int width = 0;
		int height = 0;
		SDL_GL_GetDrawableSize(mWindow, &width, &height);
		resized(0, 0, width, height);
	}

}
