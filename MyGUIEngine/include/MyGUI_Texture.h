/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*//*
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
#ifndef __MYGUI_TEXTURE_H__
#define __MYGUI_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IManualResourceLoader.h"
#include "MyGUI_RenderFormat.h"

#include <OgreResource.h>
#include <OgreTexture.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class MYGUI_EXPORT Texture : public ITexture, public Ogre::ManualResourceLoader
	{
	public:
		Texture(const std::string& _name);
		virtual ~Texture();

		virtual const std::string& getName();

		virtual void setManualResourceLoader(IManualResourceLoader* _loader);

		virtual void create();
		virtual void loadFromMemory(const void* _buff, int _width, int _height, TextureFormat _format);
		virtual void loadFromFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth();
		virtual int getHeight();

	private:
		virtual void loadResource(Ogre::Resource* resource);

	private:
		IManualResourceLoader* mLoader;
		Ogre::TexturePtr mTexture;
		std::string mName;
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_H__
