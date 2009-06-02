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
#include "MyGUI_OgreViewport.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_OgreRenderManager.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	OgreTexture::OgreTexture(const std::string& _name, const std::string& _group) :
		mName(_name),
		mGroup(_group),
		mViewport(nullptr),
		mRenderTexture(nullptr),
		mLoader(nullptr)
	{
	}

	OgreTexture::~OgreTexture()
	{
		destroy();
	}

	void OgreTexture::loadResource(Ogre::Resource* resource)
	{
		mLoader->loadResource(this);
	}

	const std::string& OgreTexture::getName()
	{
		return mName;
	}

	const std::string& OgreTexture::getGroup()
	{
		return mGroup;
	}

	void OgreTexture::setManualResourceLoader(IManualResourceLoader* _loader)
	{
		mLoader = _loader;
	}

	void OgreTexture::create()
	{
		mTexture = Ogre::TextureManager::getSingleton().create(
			mName,
			mGroup,
			true,
			mLoader == nullptr ? nullptr : this);
		mTexture->setTextureType(Ogre::TEX_TYPE_2D);
		mTexture->setNumMipmaps(0);
		mTexture->load();
	}

	void OgreTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		mTexture = Ogre::TextureManager::getSingleton().createManual(
			mName,
			mGroup,
			Ogre::TEX_TYPE_2D,
			_width,
			_height,
			0,
			getOgreFormat( _format ),
			getOgreUsage( _usage ),
			mLoader == nullptr ? nullptr : this);

		mTexture->load();

		if (_usage == TextureUsage::RenderTarget)
		{
			mViewport = mTexture->getBuffer()->getRenderTarget()->addViewport(nullptr);

			Ogre::Root * root = Ogre::Root::getSingletonPtr();
			if (root != nullptr)
			{
				Ogre::RenderSystem* system = root->getRenderSystem();
				if (system != nullptr)
				{
					mRenderTargetInfo.maximumDepth = system->getMaximumDepthInputValue();
					mRenderTargetInfo.hOffset = system->getHorizontalTexelOffset() / float(_width);
					mRenderTargetInfo.vOffset = system->getVerticalTexelOffset() / float(_height);
					mRenderTargetInfo.aspectCoef = float(_height) / float(_width);
					mRenderTargetInfo.pixScaleX = 1.0 / float(_width);
					mRenderTargetInfo.pixScaleY = 1.0 / float(_height);
				}
			}
		}
	}

	void OgreTexture::loadFromMemory(const void* _buff, int _width, int _height, PixelFormat _format)
	{
		Ogre::PixelFormat format = getOgreFormat(_format);
		size_t data_size = _width * _height * getOgreNumByte(format);

		// FIXME хз что сделать надо, старый вариант падает, а с этим по идее утечка (не могу проверить)
		// памятью не владеть, удалят снаружи, кто ее и создал
#if OGRE_VERSION < MYGUI_DEFINE_VERSION(1, 6, 0)
		Ogre::DataStreamPtr memStream( new Ogre::MemoryDataStream(const_cast<void*>(_buff), data_size, false) );
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

	void OgreTexture::loadFromFile(const std::string& _filename)
	{
		Ogre::TextureManager* manager = Ogre::TextureManager::getSingletonPtr();

		if ( false == manager->resourceExists(_filename) )
		{
			DataManager& resourcer = DataManager::getInstance();
			if (!resourcer.isDataExist(_filename, mGroup))
			{
				MYGUI_LOG(Error, "Texture '" + _filename + "' not found, set default texture");
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
	}

	void OgreTexture::saveToFile(const std::string& _filename)
	{
		Ogre::uchar *readrefdata = (Ogre::uchar*)lock(false);

		Ogre::Image img;
		img = img.loadDynamicImage(readrefdata, mTexture->getWidth(), mTexture->getHeight(), mTexture->getFormat());
		img.save(_filename);

		unlock();
	}

	void OgreTexture::destroy()
	{
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

	void* OgreTexture::lock(bool _discard)
	{
		if (_discard)
		{
			return mTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_DISCARD);
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

	PixelFormat OgreTexture::getFormat()
	{
		Ogre::PixelFormat format = mTexture->getFormat();

		if (format == Ogre::PF_A8R8G8B8) return PixelFormat::A8R8G8B8;
		else if (format == Ogre::PF_BYTE_LA) return PixelFormat::L8A8;
		return PixelFormat::A8R8G8B8;
	}

	size_t OgreTexture::getNumElemBytes()
	{
		return getOgreNumByte( mTexture->getFormat() );
	}

	Ogre::PixelFormat OgreTexture::getOgreFormat(PixelFormat _format)
	{
		if (_format == PixelFormat::A8R8G8B8) return Ogre::PF_A8R8G8B8;
		else if (_format == PixelFormat::L8A8) return Ogre::PF_BYTE_LA;
		return Ogre::PF_A8R8G8B8;
	}

	size_t OgreTexture::getOgreNumByte(Ogre::PixelFormat _format)
	{
		return Ogre::PixelUtil::getNumElemBytes(_format);
	}

	Ogre::TextureUsage OgreTexture::getOgreUsage(TextureUsage _usage)
	{
		if (_usage == TextureUsage::Static) return Ogre::TU_STATIC;
		else if (_usage == TextureUsage::Dynamic) return Ogre::TU_DYNAMIC;
		else if (_usage == TextureUsage::WriteOnly) return Ogre::TU_WRITE_ONLY;
		else if (_usage == TextureUsage::StaticWriteOnly) return Ogre::TU_STATIC_WRITE_ONLY;
		else if (_usage == TextureUsage::DynamicWriteOnly) return Ogre::TU_DYNAMIC_WRITE_ONLY;
		else if (_usage == TextureUsage::DynamicWriteOnlyDiscardable) return Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE;
		else if (_usage == TextureUsage::RenderTarget) return Ogre::TU_RENDERTARGET;

		return Ogre::TU_DEFAULT;
	}

	TextureUsage OgreTexture::getUsage(Ogre::TextureUsage _usage)
	{
		if (_usage == Ogre::TU_STATIC) return TextureUsage::Static;
		else if (_usage == Ogre::TU_DYNAMIC) return TextureUsage::Dynamic;
		else if (_usage == Ogre::TU_WRITE_ONLY) return TextureUsage::WriteOnly;
		else if (_usage == Ogre::TU_STATIC_WRITE_ONLY) return TextureUsage::StaticWriteOnly;
		else if (_usage == Ogre::TU_DYNAMIC_WRITE_ONLY) return TextureUsage::DynamicWriteOnly;
		else if (_usage == Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE) return TextureUsage::DynamicWriteOnlyDiscardable;
		else if (_usage == Ogre::TU_RENDERTARGET) return TextureUsage::RenderTarget;

		return TextureUsage::Default;
	}

	TextureUsage OgreTexture::getUsage()
	{
		return getUsage( (Ogre::TextureUsage)mTexture->getUsage() );
	}

	void OgreTexture::setViewport(IViewport* _viewport)
	{
		Ogre::RenderTexture* target = mTexture->getBuffer()->getRenderTarget();

		if ( mRenderTexture != target && target != nullptr && _viewport != nullptr )
		{
			mRenderTexture = target;

			mRenderTexture->removeAllViewports();
			mViewport = mRenderTexture->addViewport( static_cast<OgreViewport*>(_viewport)->getCamera() );
			mViewport->setClearEveryFrame( true );
			mViewport->setOverlaysEnabled( false );
		}
	}

	void OgreTexture::removeViewport()
	{
		if (mRenderTexture != nullptr)
		{
			mRenderTexture->removeAllViewports();
			//FIXME
			Ogre::Root::getSingleton().getRenderSystem()->destroyRenderTexture( mTexture->getName() );
			mRenderTexture = nullptr;
		}
	}

	void OgreTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OgreRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

	void OgreTexture::doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count)
	{
		OgreRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

	Ogre::Viewport* gSaveViewport = nullptr;

	void OgreTexture::begin()
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		gSaveViewport = system->_getViewport();
		system->_setViewport(mViewport);
		system->clearFrameBuffer( Ogre::FBT_COLOUR, Ogre::ColourValue(0, 0, 1 - float(rand() % 1000) / float(1000) , 1) );
	}

	void OgreTexture::end()
	{
		Ogre::RenderSystem* system = Ogre::Root::getSingleton().getRenderSystem();
		system->_setViewport(gSaveViewport);
	}

} // namespace MyGUI
