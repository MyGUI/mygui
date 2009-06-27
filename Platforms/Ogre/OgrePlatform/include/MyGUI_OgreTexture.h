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
#ifndef __MYGUI_OGRE_TEXTURE_H__
#define __MYGUI_OGRE_TEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_IManualResourceLoader.h"
#include "MyGUI_RenderFormat.h"

#include <OgreResource.h>
#include <OgreTexture.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreTexture : public ITexture, public Ogre::ManualResourceLoader
	{
	public:
		OgreTexture(const std::string& _name, const std::string& _group);
		virtual ~OgreTexture();

		virtual const std::string& getName();

		virtual void setManualResourceLoader(IManualResourceLoader* _loader);

		virtual void create();
		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual void* lock(bool _discard);
		virtual void unlock();
		virtual bool isLocked();

		virtual int getWidth();
		virtual int getHeight();

		virtual PixelFormat getFormat();
		virtual TextureUsage getUsage();
		virtual size_t getNumElemBytes();

		virtual void setViewport(IViewport* _viewport);
		virtual void removeViewport();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
		virtual void doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo() { return mRenderTargetInfo; }

		Ogre::TexturePtr getOgreTexture() { return mTexture; }

	private:
		virtual void loadResource(Ogre::Resource* resource);

		Ogre::TextureUsage getOgreUsage(TextureUsage _usage);
		Ogre::PixelFormat getOgreFormat(PixelFormat _format);
		size_t getOgreNumByte(Ogre::PixelFormat _format);

		TextureUsage getUsage(Ogre::TextureUsage _usage);

	private:
		IManualResourceLoader* mLoader;
		Ogre::TexturePtr mTexture;
		std::string mName;
		std::string mGroup;

		Ogre::Viewport* mViewport;
		Ogre::RenderTarget* mRenderTexture;
		RenderTargetInfo mRenderTargetInfo;
		Ogre::Viewport* mSaveViewport;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_TEXTURE_H__
