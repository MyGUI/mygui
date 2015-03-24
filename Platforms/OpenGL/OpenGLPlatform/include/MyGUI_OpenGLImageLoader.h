/*!
	@file
	@author		George Evmenov
	@date		09/2009
	@module
*/

#ifndef MYGUI_OPENGL_IMAGE_LOADER_H_
#define MYGUI_OPENGL_IMAGE_LOADER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class OpenGLImageLoader
	{
	public:
		OpenGLImageLoader() { }
		virtual ~OpenGLImageLoader() { }

		virtual void* loadImage(int& _width, int& _height, PixelFormat& _format, const std::string& _filename) = 0;
		virtual void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename) = 0;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL_IMAGE_LOADER_H_
