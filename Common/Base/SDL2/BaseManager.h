#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_SDL2Platform.h>
#include <MyGUI_SDL2ImageLoader.h>

namespace base
{

	class BaseManager :
		public SdlBaseManager,
		public MyGUI::SDL2ImageLoader
	{
	public:
        BaseManager() : SdlBaseManager(false) { }
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

//		/*internal:*/
		SDL_Texture* loadTexture(const std::string& _filename) override;
		void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename) override;

	private:
		MyGUI::SDL2Platform* mPlatform = nullptr;
		SDL_Renderer *mRenderer = nullptr;
	};

}
