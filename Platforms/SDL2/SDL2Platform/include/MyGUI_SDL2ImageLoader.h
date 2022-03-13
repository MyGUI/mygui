/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
	@module
*/

#ifndef MYGUI_SDL2_IMAGE_LOADER_H_
#define MYGUI_SDL2_IMAGE_LOADER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"

#include <SDL.h>
#include <string>

namespace MyGUI
{
	class SDL2ImageLoader
	{
	public:
		virtual ~SDL2ImageLoader() { }

		virtual SDL_Texture* loadTexture(const std::string& _filename) = 0;
		virtual void saveImage(
			int _width,
			int _height,
			MyGUI::PixelFormat _format,
			void* _texture,
			const std::string& _filename) = 0;
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_IMAGE_LOADER_H_
