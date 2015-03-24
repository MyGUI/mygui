/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_TEXTURE_H_
#define MYGUI_I_TEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_RenderFormat.h"
#include <string>

namespace MyGUI
{
	class ITexture;

	class MYGUI_EXPORT ITextureInvalidateListener
	{
	public:
		virtual ~ITextureInvalidateListener() { }
		// update texture after render device lost event
		virtual void textureInvalidate(ITexture* _texture) = 0;
	};

	class MYGUI_EXPORT ITexture
	{
	public:
		virtual ~ITexture() { }

		virtual const std::string& getName() const = 0;

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) = 0;
		virtual void loadFromFile(const std::string& _filename) = 0;
		virtual void saveToFile(const std::string& _filename) = 0;

		virtual void setInvalidateListener(ITextureInvalidateListener* _listener) { }

		virtual void destroy() = 0;

		virtual void* lock(TextureUsage _access) = 0;
		virtual void unlock() = 0;
		virtual bool isLocked() = 0;

		virtual int getWidth() = 0;
		virtual int getHeight() = 0;

		virtual PixelFormat getFormat() = 0;
		virtual TextureUsage getUsage() = 0;
		virtual size_t getNumElemBytes() = 0;

		virtual IRenderTarget* getRenderTarget()
		{
			return nullptr;
		}
	};

} // namespace MyGUI

#endif // MYGUI_I_TEXTURE_H_
