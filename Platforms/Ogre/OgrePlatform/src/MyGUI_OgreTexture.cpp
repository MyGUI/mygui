/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#include <cstring>
#include "MyGUI_OgreTexture.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_OgreRenderManager.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_OgreRTTexture.h"
#include "MyGUI_OgreDataManager.h"
#include <Ogre.h>

namespace MyGUI
{

	OgreTexture::OgreTexture(const std::string& _name, const std::string& _group) :
		mName(_name),
		mGroup(_group)
	{
		mTmpData.data = nullptr;
	}

	OgreTexture::~OgreTexture()
	{
		destroy();
	}

	const std::string& OgreTexture::getName() const
	{
		return mName;
	}

	void OgreTexture::saveToFile(const std::string& _filename)
	{
		Ogre::uchar* readrefdata = static_cast<Ogre::uchar*>(lock(TextureUsage::Read));

		Ogre::Image img;
		img = img.loadDynamicImage(readrefdata, mTexture->getWidth(), mTexture->getHeight(), mTexture->getFormat());
		img.save(_filename);

		unlock();
	}

	void OgreTexture::setShader(const std::string& _shaderName)
	{
		mShaderInfo = OgreRenderManager::getInstance().getShaderInfo(_shaderName);
	}

	void OgreTexture::setInvalidateListener(ITextureInvalidateListener* _listener)
	{
		mListener = _listener;
	}

	void OgreTexture::destroy()
	{
		delete[] (uint8*)mTmpData.data;
		mTmpData.data = nullptr;

		delete mRenderTarget;
		mRenderTarget = nullptr;

		if (mTexture)
		{
			Ogre::TextureManager::getSingleton().remove(mTexture->getHandle());
			mTexture.reset();
		}
	}

	int OgreTexture::getWidth() const
	{
		return (int)mTexture->getWidth();
	}

	int OgreTexture::getHeight() const
	{
		return (int)mTexture->getHeight();
	}

	void* OgreTexture::lock(TextureUsage _access)
	{
		if (_access == TextureUsage::Write)
		{
			return mTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_WRITE_ONLY);
		}

		// здесь проверить режим создания, и возможно так залочить без пиксель бокса

		// для чтения копируем в пиксель бокс
		delete[] (uint8*)mTmpData.data;
		mTmpData.data = nullptr;

		mTmpData =
			Ogre::PixelBox(mTexture->getWidth(), mTexture->getHeight(), mTexture->getDepth(), mTexture->getFormat());
		mTmpData.data = new uint8[mTexture->getBuffer()->getSizeInBytes()];

		mTexture->getBuffer()->blitToMemory(mTmpData);

		return mTmpData.data;
	}

	void OgreTexture::unlock()
	{
		if (mTexture->getBuffer()->isLocked())
		{
			mTexture->getBuffer()->unlock();
		}
		else
		{
			delete[] (uint8*)mTmpData.data;
			mTmpData.data = nullptr;
		}
	}

	bool OgreTexture::isLocked() const
	{
		return mTexture->getBuffer()->isLocked();
	}

	Ogre::TextureUsage OgreTexture::convertUsage(TextureUsage _usage)
	{
		if (_usage == TextureUsage::Default)
		{
			return Ogre::TU_STATIC_WRITE_ONLY;
		}
		else if (_usage == TextureUsage::RenderTarget)
		{
			return Ogre::TU_RENDERTARGET;
		}
		else if (_usage.isValue(TextureUsage::Static))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				return Ogre::TU_STATIC_WRITE_ONLY;
			}
			else
			{
				return Ogre::TU_STATIC;
			}
		}
		else if (_usage.isValue(TextureUsage::Dynamic))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				return Ogre::TU_DYNAMIC_WRITE_ONLY;
			}
			else
			{
				return Ogre::TU_DYNAMIC;
			}
		}
		else if (_usage.isValue(TextureUsage::Stream))
		{
			if (_usage.isValue(TextureUsage::Write))
			{
				return Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE;
			}
			else
			{
				return Ogre::TU_DYNAMIC;
			}
		}
		return Ogre::TU_DEFAULT;
	}

	Ogre::PixelFormat OgreTexture::convertFormat(PixelFormat _format)
	{
		if (_format == PixelFormat::L8)
			return Ogre::PF_BYTE_L;
		else if (_format == PixelFormat::L8A8)
			return Ogre::PF_BYTE_LA;
		else if (_format == PixelFormat::R8G8B8)
			return Ogre::PF_R8G8B8;
		else if (_format == PixelFormat::R8G8B8A8)
			return Ogre::PF_A8R8G8B8;

		return Ogre::PF_UNKNOWN;
	}

	void OgreTexture::setFormat(PixelFormat _format)
	{
		mOriginalFormat = _format;
		mPixelFormat = convertFormat(_format);
		mNumElemBytes = _format.getBytesPerPixel();
	}

	void OgreTexture::setUsage(TextureUsage _usage)
	{
		mOriginalUsage = _usage;
		mUsage = convertUsage(_usage);
	}

	void OgreTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		setFormat(_format);
		setUsage(_usage);

		mTexture = Ogre::TextureManager::getSingleton()
					   .createManual(mName, mGroup, Ogre::TEX_TYPE_2D, _width, _height, 0, mPixelFormat, mUsage, this);

		mTexture->load();
	}

	void OgreTexture::loadFromFile(const std::string& _filename)
	{
		setUsage(TextureUsage::Default);

		auto createResult = Ogre::TextureManager::getSingleton().createOrRetrieve(
			_filename,
			OgreDataManager::getInstance().getGroup(),
			false,
			nullptr,
			nullptr,
			Ogre::TEX_TYPE_2D,
			0);
		if (!createResult.second)
		{
			MYGUI_PLATFORM_LOG(Error, "Texture '" + _filename + "' not found, set default texture");
		}
		else
		{
			mTexture = std::static_pointer_cast<Ogre::Texture>(createResult.first);
			if (!mTexture->isLoaded())
				mTexture->load();
		}

		setFormatByOgreTexture();
	}

	void OgreTexture::setFormatByOgreTexture()
	{
		mOriginalFormat = PixelFormat::Unknow;
		mPixelFormat = Ogre::PF_UNKNOWN;
		mNumElemBytes = 0;

		if (mTexture)
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
