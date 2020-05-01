#pragma once

#include "Base/PlatformBaseManager/SdlBaseManager.h"

#include <MyGUI_OpenGLPlatform.h>

namespace base
{

	class BaseManager :
			public SdlBaseManager,
			public MyGUI::OpenGLImageLoader
	{
	public:
		bool createRender(int _width, int _height, bool _windowed) override;
		void destroyRender() override;
		void drawOneFrame() override;
		void resizeRender(int _width, int _height) override;
		void addResourceLocation(const std::string& _name, bool _recursive = false) override;
		void createGuiPlatform() override;
		void destroyGuiPlatform() override;

//		/*internal:*/
		void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename) override;
		void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename) override;

	private:
		MyGUI::OpenGLPlatform* mPlatform = nullptr;
		SDL_GLContext mContext = nullptr;
	};

}
