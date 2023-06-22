#include "Precompiled.h"
#include "BaseManager.h"

#include <SDL_image.h>

namespace base
{
	bool BaseManager::createRender(int _width, int _height, bool _windowed)
	{
		mRenderer = SDL_CreateRenderer(mSdlWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

		if (mRenderer == nullptr) {
			std::cerr << "Failed to initialize SDL_Renderer: " << SDL_GetError();
			exit(1);
		}

		// TODO: Why do we need this? We need to understand why SubSkin::doRender() multiplies the size of the quad by 2
//		SDL_RenderSetScale(mRenderer, 0.5, 0.5);

		if (IMG_Init(~0) == 0)
		{
			std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
			exit(1);
		}
		return true;
	}

	void BaseManager::destroyRender()
	{
		SDL_DestroyRenderer(mRenderer);
		IMG_Quit();
	}

	void BaseManager::createGuiPlatform()
	{
		mPlatform = new MyGUI::SDL2Platform();
		setupResources();
		mPlatform->initialise(this, mRenderer);
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
		SDL_SetRenderDrawColor(mRenderer, 10, 20, 30, 0xFF);
		SDL_RenderClear(mRenderer);

		if (mPlatform)
			mPlatform->getRenderManagerPtr()->drawOneFrame();

		SDL_RenderPresent(mRenderer);
	}

	void BaseManager::resizeRender(int _width, int _height)
	{
		// TODO: Maybe we need to resize the renderer?
		printf("Resized\n");
	}

	void BaseManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		mPlatform->getDataManagerPtr()->addResourceLocation(_name, _recursive);
	}


	SDL_Texture* BaseManager::loadTexture(const std::string& _filename)
	{
		std::string fullname = MyGUI::SDL2DataManager::getInstance().getDataPath(_filename);

		SDL_Texture *texture = NULL;
		SDL_Surface *surface = IMG_Load(fullname.c_str());

		MYGUI_ASSERT(surface != nullptr, "Failed to load image: " + fullname);

		auto bpp = surface->format->BytesPerPixel;
		auto newFormat = SDL_PIXELFORMAT_UNKNOWN;
		switch (bpp) {
			case 4:
				newFormat = SDL_PIXELFORMAT_RGBA32;
				break;
			case 3:
				newFormat = SDL_PIXELFORMAT_RGB24;
				break;
		}
		surface = SDL_ConvertSurfaceFormat(surface, newFormat, 0);

		MYGUI_ASSERT(surface != nullptr, "Failed to convert image: " + fullname);

		texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface);

		return texture;
	}

	void BaseManager::saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename)
	{
		SDL_Surface* surface = SDL_CreateRGBSurface(0, _width, _height, _format.getBytesPerPixel() * 8, 0, 0, 0, 0);
		std::memcpy(surface->pixels, _texture, _width * _height * _format.getBytesPerPixel());
		IMG_SavePNG(surface, _filename.c_str());
	}
}
