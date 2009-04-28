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
#include "MyGUI_Precompiled.h"
#include "MyGUI_Texture.h"
#include "OgreTextureManager.h"

namespace MyGUI
{

	Texture::Texture(const std::string& _name) :
		mName(_name)
	{
	}

	Texture::~Texture()
	{
		destroy();
	}

	void Texture::loadResource(Ogre::Resource* resource)
	{
		mLoader->loadResource(this);
	}

	const std::string& Texture::getName()
	{
		return mName;
	}

	void Texture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void Texture::create()
	{
		mTexture = Ogre::TextureManager::getSingleton().create(mName, ResourceManager::getInstance().getResourceGroup(), true, mLoader == nullptr ? nullptr : this);
		mTexture->setTextureType(Ogre::TEX_TYPE_2D);
		mTexture->setNumMipmaps(0);
		mTexture->load();
	}

	void Texture::loadFromMemory(const void* _buff, int _width, int _height, TextureFormat _format)
	{
		size_t data_size = _width * _height;
		Ogre::PixelFormat format;

		if (_format == TextureFormat::A8R8G8B8)
		{
			data_size *= 4;
			format = Ogre::PF_A8R8G8B8;
		}
		else if (_format == TextureFormat::L8A8)
		{
			data_size *= 2;
			format = Ogre::PF_BYTE_LA;
		}
		else
		{
			data_size *= 4;
			format = Ogre::PF_A8R8G8B8;
		}


		// FIXME хз что сделать надо, старый вариант падает, а с этим по идее утечка (не могу проверить)
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(const_cast<void*>(_buff), data_size, true) );
#else
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(const_cast<void*>(_buff), data_size) );
#endif

		Ogre::Image img;
		img.loadRawData(memStream, _width, _height, format);

		 //Call internal _loadImages, not loadImage since that's external and
		// will determine load status etc again, and this is a manual loader inside load()
		Ogre::ConstImagePtrList imagePtrs;
		imagePtrs.push_back(&img);

		mTexture->_loadImages( imagePtrs );
	}

	void Texture::loadFromFile(const std::string& _filename)
	{
		Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

		if ( false == manager->resourceExists(_filename) )
		{
			const std::string& group = ResourceManager::getInstance().getResourceGroup();
			if (!helper::isFileExist(_filename, group))
			{
				MYGUI_LOG(Error, "Texture '" + _filename + "' not found, set default texture");
			}
			else
			{
				mTexture = manager->load(_filename, group, Ogre::TEX_TYPE_2D, 0);
			}
		}
		else
		{
			mTexture = manager->getByName(_filename);
		}
	}

	void Texture::destroy()
	{
		Ogre::TextureManager::getSingleton().remove(mTexture->getName());
	}

	int Texture::getWidth()
	{
		return mTexture->getWidth();
	}

	int Texture::getHeight()
	{
		return mTexture->getHeight();
	}

} // namespace MyGUI
