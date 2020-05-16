#ifndef MYGUI_OPENGLES_IMAGE_LOADER_H__
#define MYGUI_OPENGLES_IMAGE_LOADER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class OpenGLESImageLoader
	{
	public:
		virtual ~OpenGLESImageLoader() { }

		virtual void* loadImage(int& _width, int& _height, PixelFormat& _format, const std::string& _filename) = 0;
		virtual void saveImage(
			int _width,
			int _height,
			MyGUI::PixelFormat _format,
			void* _texture,
			const std::string& _filename) = 0;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGLES_IMAGE_LOADER_H__
