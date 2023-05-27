#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"

namespace MyGUI
{

	class OpenGLESImageLoader
	{
	public:
		virtual ~OpenGLESImageLoader() = default;

		virtual void* loadImage(int& _width, int& _height, PixelFormat& _format, const std::string& _filename) = 0;
		virtual void saveImage(
			int _width,
			int _height,
			MyGUI::PixelFormat _format,
			void* _texture,
			const std::string& _filename) = 0;
	};

} // namespace MyGUI
