/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_I_TEXTURE_H__
#define __MYGUI_I_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"
#include "MyGUI_IRenderResource.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IViewport.h"

namespace MyGUI
{

	class IManualResourceLoader;

	class MYGUI_EXPORT ITexture : public IRenderTarget, public IRenderResource
	{
	public:
		virtual ~ITexture() { }

		virtual const std::string& getName() = 0;
		//virtual const std::string& getGroup() = 0;

		virtual void setManualResourceLoader(IManualResourceLoader* _value) = 0;

		virtual void create() = 0;
		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format) = 0;
		virtual void loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format) = 0;
		virtual void loadFromFile(const std::string& _filename) = 0;
		virtual void saveToFile(const std::string& _filename) = 0;

		virtual void destroy() = 0;

		virtual void* lock(bool _discard = true) = 0;
		virtual void unlock() = 0;
		virtual bool isLocked() = 0;

		virtual int getWidth() = 0;
		virtual int getHeight() = 0;

		virtual PixelFormat getFormat() = 0;
		virtual TextureUsage getUsage() = 0;
		virtual size_t getNumElemBytes() = 0;

		virtual void setViewport(IViewport* _value) = 0;
		virtual void removeViewport() = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_I_TEXTURE_H__
