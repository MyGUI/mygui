#include "Precompiled.h"
#include "BaseManager.h"

#include <SDL_image.h>
#include <MyGUI_GL.h>

#include <vector>

namespace base
{
	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		mContext = SDL_GL_CreateContext(mSdlWindow);
		if (mContext == nullptr)
		{
			std::cerr << "Failed to create SDL context: " << SDL_GetError();
			exit(1);
		}
#ifndef __EMSCRIPTEN__
		if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
		{
			std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
			exit(1);
		}
#endif
		return true;
	}

	void BaseManager::destroyRender()
	{
		IMG_Quit();
		SDL_GL_DeleteContext(mContext);
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::OpenGL3Platform();
		setupResources();
		mPlatform->initialise(this);
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
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		if (mScreenShotRequested)
		{
			mScreenShotRequested = false;
			int w, h;
			SDL_GetWindowSize(mSdlWindow, &w, &h);
			std::vector<std::uint8_t> pixels(w * h * 4);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels.data());
			// Flip vertically (OpenGL origin is bottom-left, images expect top-left)
			const int stride = w * 4;
			for (int y = 0; y < h / 2; ++y)
			{
				auto* top = pixels.data() + y * stride;
				auto* bottom = pixels.data() + (h - 1 - y) * stride;
				std::swap_ranges(top, top + stride, bottom);
			}
			saveImage(w, h, MyGUI::PixelFormat::R8G8B8A8, pixels.data(), mScreenShotFile);
		}

		SDL_GL_SwapWindow(mSdlWindow);
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}

	void* BaseManager::loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::OpenGL3DataManager::getInstance().getDataPath(_filename);
		void* result = nullptr;
		SDL_Surface* image = nullptr;
		SDL_Surface* cvtImage = nullptr; // converted surface with RGBA/RGB pixel format
		image = IMG_Load(fullname.c_str());
		MYGUI_ASSERT(image != nullptr, "Failed to load image: " + fullname);

		_width = image->w;
		_height = image->h;

		int bpp = image->format->BytesPerPixel;
		if (bpp < 3)
		{
			result = convertPixelData(image, _format);
		}
		else
		{
			Uint32 pixelFmt = bpp == 3 ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_ARGB8888;
			cvtImage = SDL_ConvertSurfaceFormat(image, pixelFmt, 0);
			result = convertPixelData(cvtImage, _format);
			SDL_FreeSurface(cvtImage);
		}
		SDL_FreeSurface(image);

		return result;
	}

	void BaseManager::saveImage(
		int _width,
		int _height,
		MyGUI::PixelFormat _format,
		void* _texture,
		const std::string& _filename)
	{
		int bpp = _format.getBytesPerPixel();
		Uint32 fmt = (bpp == 3) ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_BGRA32;
		SDL_Surface* surface =
			SDL_CreateRGBSurfaceWithFormatFrom(_texture, _width, _height, bpp * 8, _width * bpp, fmt);
		IMG_SavePNG(surface, _filename.c_str());
		SDL_FreeSurface(surface);
	}

}
