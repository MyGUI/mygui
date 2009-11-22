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
#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreTexture.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_OgreRTTexture.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	OgreTexture::OgreTexture(const std::string& _name, const std::string& _group) :
		mName(_name),
		mGroup(_group),
		mNumElemBytes(0),
		mPixelFormat(Ogre::PF_UNKNOWN),
		mUsage(Ogre::TU_DEFAULT),
		mListener(nullptr),
		mRenderTarget(nullptr)
	{
	}

	OgreTexture::~OgreTexture()
	{
		destroy();
	}

	const std::string& OgreTexture::getName()
	{
		return mName;
	}

	void OgreTexture::saveToFile(const std::string& _filename)
	{
		/*Ogre::uchar *readrefdata = (Ogre::uchar*)lock(false);

		Ogre::Image img;
		img = img.loadDynamicImage(readrefdata, mTexture->getWidth(), mTexture->getHeight(), mTexture->getFormat());
		img.save(_filename);

		unlock();*/
	}

	void OgreTexture::setInvalidateListener(ITextureInvalidateListener* _listener)
	{
		mListener = _listener;
	}

	void OgreTexture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (!mTexture.isNull())
		{
			Ogre::TextureManager::getSingleton().remove(mTexture->getName());
			mTexture.setNull();
		}
	}

	int OgreTexture::getWidth()
	{
		return mTexture->getWidth();
	}

	int OgreTexture::getHeight()
	{
		return mTexture->getHeight();
	}

	void* OgreTexture::lock(TextureUsage _access)
	{
		if (_access == TextureUsage::Write)
		{
			return mTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_NORMAL);
		}

		// HOOK for OpenGL
		// {
		mTexture->unload();
		mTexture->setUsage(Ogre::HardwareBuffer::HBU_STATIC);
		mTexture->load();
		// }

		return mTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_NORMAL);
	}

	void OgreTexture::unlock()
	{
		mTexture->getBuffer()->unlock();
	}

	bool OgreTexture::isLocked()
	{
		return mTexture->getBuffer()->isLocked();
	}

	void OgreTexture::setFormat(PixelFormat _format)
	{
		mOriginalFormat = _format;
		mPixelFormat = Ogre::PF_UNKNOWN;
		mNumElemBytes = 0;

		if (_format == PixelFormat::L8)
		{
			mPixelFormat = Ogre::PF_BYTE_L;
			mNumElemBytes = 1;
		}
		else if (_format == PixelFormat::L8A8)
		{
			mPixelFormat = Ogre::PF_BYTE_LA;
			mNumElemBytes = 2;
		}
		else if (_format == PixelFormat::R8G8B8)
		{
			mPixelFormat = Ogre::PF_R8G8B8;
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::R8G8B8A8)
		{
			mPixelFormat = Ogre::PF_A8R8G8B8;
			mNumElemBytes = 4;
		}
	}

	void OgreTexture::setUsage(TextureUsage _usage)
	{
		mOriginalUsage = _usage;
		mUsage = Ogre::TU_DEFAULT;

		if (_usage == TextureUsage::Default)
		{
			mUsage = Ogre::TU_STATIC_WRITE_ONLY;
		}
		else if (_usage == TextureUsage::RenderTarget)
		{
			mUsage = Ogre::TU_RENDERTARGET;
		}
		else if (_usage.isValue(TextureUsage::Static))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = Ogre::TU_STATIC_WRITE_ONLY;
			}
			else
			{
				mUsage = Ogre::TU_STATIC;
			}
		}
		else if (_usage.isValue(TextureUsage::Dynamic))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = Ogre::TU_DYNAMIC_WRITE_ONLY;
			}
			else
			{
				mUsage = Ogre::TU_DYNAMIC;
			}
		}
		else if (_usage.isValue(TextureUsage::Stream))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				mUsage = Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE;
			}
			else
			{
				mUsage = Ogre::TU_DYNAMIC;
			}
		}

	}

	void OgreTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		setFormat(_format);
		setUsage(_usage);

		mTexture = Ogre::TextureManager::getSingleton().createManual(
			mName,
			mGroup,
			Ogre::TEX_TYPE_2D,
			_width,
			_height,
			0,
			mPixelFormat,
			mUsage,
			this);

		mTexture->load();

	}

	void OgreTexture::loadFromFile(const std::string& _filename)
	{
		setUsage(TextureUsage::Default);

		Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

		if ( !manager->resourceExists(_filename) )
		{
			DataManager& resourcer = DataManager::getInstance();
			if (!resourcer.isDataExist(_filename))
			{
				MYGUI_PLATFORM_LOG(Error, "Texture '" + _filename + "' not found, set default texture");
			}
			else
			{
				mTexture = manager->load(_filename, mGroup, Ogre::TEX_TYPE_2D, 0);
			}
		}
		else
		{
			mTexture = manager->getByName(_filename);
		}

		setFormatByOgreTexture();
	}

	void OgreTexture::setFormatByOgreTexture()
	{
		mOriginalFormat = PixelFormat::Unknow;
		mPixelFormat = Ogre::PF_UNKNOWN;
		mNumElemBytes = 0;

		if (!mTexture.isNull())
		{
			mPixelFormat = mTexture->getFormat();

			if (mPixelFormat == Ogre::PF_BYTE_L)
			{
				mOriginalFormat = PixelFormat::L8;
				mNumElemBytes = 1;
			}
			else if (mPixelFormat == Ogre::PF_BYTE_LA)
			{
				mOriginalFormat = PixelFormat::L8A8;
				mNumElemBytes = 2;
			}
			else if (mPixelFormat == Ogre::PF_R8G8B8)
			{
				mOriginalFormat = PixelFormat::R8G8B8;
				mNumElemBytes = 3;
			}
			else if (mPixelFormat == Ogre::PF_A8R8G8B8)
			{
				mOriginalFormat = PixelFormat::R8G8B8A8;
				mNumElemBytes = 4;
			}
			else
			{
				mOriginalFormat = PixelFormat::Unknow;
				mNumElemBytes = Ogre::PixelUtil::getNumElemBytes(mPixelFormat);
			}

		}
	}

	void OgreTexture::loadResource(Ogre::Resource* resource)
	{
		if (mListener)
			mListener->textureInvalidate(this);
	}

	IRenderTarget* OgreTexture::getRenderTarget()
	{
		if (mRenderTarget == nullptr)
			mRenderTarget = new OgreRTTexture(mTexture);

		return mRenderTarget;
	}

} // namespace MyGUI
