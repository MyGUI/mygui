/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgRTTexture.h"
#include "MyGUI_OsgRenderManager.h"

#include <stdexcept>

#include <osg/GL>
#include <osg/Image>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

namespace MyGUI
{

	OsgTexture::OsgTexture(const std::string& _name, OsgRenderManager* _manager) :
		mName(_name),
		mRenderManager(_manager),
		mFormat(PixelFormat::Unknow),
		mUsage(TextureUsage::Default),
		mNumElemBytes(0),
		mWidth(0),
		mHeight(0)
	{
	}

	OsgTexture::OsgTexture(osg::Texture2D* _texture, osg::StateSet* _injectState) :
		mRenderManager(nullptr),
		mTexture(_texture),
		mInjectState(_injectState),
		mFormat(PixelFormat::Unknow),
		mUsage(TextureUsage::Default),
		mNumElemBytes(0),
		mWidth(_texture->getTextureWidth()),
		mHeight(_texture->getTextureHeight())
	{
	}

	OsgTexture::~OsgTexture()
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
	}

	const std::string& OsgTexture::getName() const
	{
		return mName;
	}

	void OsgTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		// the old render target is bound to the old texture
		delete mRenderTarget;
		mRenderTarget = nullptr;

		GLenum glfmt = GL_NONE;
		size_t numelems = 0;
		switch (_format.getValue())
		{
		case PixelFormat::L8:
			glfmt = GL_LUMINANCE;
			numelems = 1;
			break;
		case PixelFormat::L8A8:
			glfmt = GL_LUMINANCE_ALPHA;
			numelems = 2;
			break;
		case PixelFormat::R8G8B8:
			glfmt = GL_BGR;
			numelems = 3;
			break;
		case PixelFormat::R8G8B8A8:
			// The engine stores R8G8B8A8 texture data in memory as B,G,R,A
			// (see the OpenGL/OpenGL3 backends which upload with GL_BGRA),
			// so the source format must be BGRA to keep the channels unswapped.
			glfmt = GL_BGRA;
			numelems = 4;
			break;
		default:
			MYGUI_PLATFORM_LOG(Critical, "Texture format not supported");
			MYGUI_PLATFORM_EXCEPT("Texture format not supported");
		}

		mTexture = new osg::Texture2D();
		mTexture->setTextureSize(_width, _height);
		mTexture->setSourceFormat(glfmt);
		mTexture->setSourceType(GL_UNSIGNED_BYTE);

		mTexture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
		mTexture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
		mTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		mTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

		mWidth = _width;
		mHeight = _height;
		mFormat = _format;
		mUsage = _usage;
		mNumElemBytes = numelems;
	}

	void OsgTexture::destroy()
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
		mTexture = nullptr;
		mLockedImage = nullptr;
		mFormat = PixelFormat::Unknow;
		mUsage = TextureUsage::Default;
		mNumElemBytes = 0;
		mWidth = 0;
		mHeight = 0;
	}

	void OsgTexture::loadFromFile(const std::string& _filename)
	{
		if (mRenderManager == nullptr)
		{
			MYGUI_PLATFORM_LOG(Critical, "No render manager set");
			MYGUI_PLATFORM_EXCEPT("No render manager set");
		}

		osg::ref_ptr<osg::Image> image = mRenderManager->loadImage(_filename);
		if (!image.valid())
		{
			MYGUI_PLATFORM_LOG(Critical, "Failed to load image '" << _filename << "'");
			MYGUI_PLATFORM_EXCEPT("Failed to load image '" << _filename << "'");
		}

		mTexture = new osg::Texture2D(image);
		mTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
		mTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
		mTexture->setTextureWidth(image->s());
		mTexture->setTextureHeight(image->t());
		// disable mip-maps
		mTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);

		mWidth = image->s();
		mHeight = image->t();

		mUsage = TextureUsage::Static;
	}

	void OsgTexture::saveToFile(const std::string& _filename)
	{
		osg::Image* image = mTexture.valid() ? mTexture->getImage() : nullptr;
		if (image == nullptr)
		{
			MYGUI_PLATFORM_LOG(Warning, "Texture is not created or has no image to save");
			return;
		}

		if (!osgDB::writeImageFile(*image, _filename))
		{
			MYGUI_PLATFORM_LOG(Warning, "Failed to save image to file " << _filename);
		}
	}

	void* OsgTexture::lock(TextureUsage /*_access*/)
	{
		if (!mTexture.valid())
		{
			MYGUI_PLATFORM_LOG(Critical, "Texture is not created");
			MYGUI_PLATFORM_EXCEPT("Texture is not created");
		}
		if (mLockedImage.valid())
		{
			MYGUI_PLATFORM_LOG(Critical, "Texture already locked");
			MYGUI_PLATFORM_EXCEPT("Texture already locked");
		}

		mLockedImage = new osg::Image();
		mLockedImage->allocateImage(
			mTexture->getTextureWidth(),
			mTexture->getTextureHeight(),
			mTexture->getTextureDepth(),
			mTexture->getSourceFormat(),
			mTexture->getSourceType());

		return mLockedImage->data();
	}

	void OsgTexture::unlock()
	{
		if (!mLockedImage.valid())
		{
			MYGUI_PLATFORM_LOG(Critical, "Texture not locked");
			MYGUI_PLATFORM_EXCEPT("Texture not locked");
		}

		// mTexture might be in use by the draw thread, so create a new texture instead and use that.
		osg::ref_ptr<osg::Texture2D> newTexture = new osg::Texture2D;
		newTexture->setTextureSize(getWidth(), getHeight());
		newTexture->setSourceFormat(mTexture->getSourceFormat());
		newTexture->setSourceType(mTexture->getSourceType());
		newTexture->setFilter(osg::Texture::MIN_FILTER, mTexture->getFilter(osg::Texture::MIN_FILTER));
		newTexture->setFilter(osg::Texture::MAG_FILTER, mTexture->getFilter(osg::Texture::MAG_FILTER));
		newTexture->setWrap(osg::Texture::WRAP_S, mTexture->getWrap(osg::Texture::WRAP_S));
		newTexture->setWrap(osg::Texture::WRAP_T, mTexture->getWrap(osg::Texture::WRAP_T));
		newTexture->setImage(mLockedImage.get());
		// Tell the texture it can get rid of the image for static textures (since
		// they aren't expected to update much at all).
		newTexture->setUnRefImageDataAfterApply(mUsage.isValue(TextureUsage::Static));

		mTexture = newTexture;

		mLockedImage = nullptr;
	}

	bool OsgTexture::isLocked() const
	{
		return mLockedImage.valid();
	}

	int OsgTexture::getWidth() const
	{
		return mWidth;
	}

	int OsgTexture::getHeight() const
	{
		return mHeight;
	}

	PixelFormat OsgTexture::getFormat() const
	{
		return mFormat;
	}

	TextureUsage OsgTexture::getUsage() const
	{
		return mUsage;
	}

	size_t OsgTexture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

	IRenderTarget* OsgTexture::getRenderTarget()
	{
		if (mRenderTarget == nullptr && mRenderManager != nullptr && mTexture.valid())
			mRenderTarget = new OsgRTTexture(mTexture.get(), mRenderManager, mWidth, mHeight);
		return mRenderTarget;
	}

	void OsgTexture::setShader(const std::string& /*_shaderName*/)
	{
		MYGUI_PLATFORM_LOG(Warning, "OsgTexture::setShader is not implemented");
	}

	osg::Texture2D* OsgTexture::getTexture() const
	{
		return mTexture.get();
	}

	osg::StateSet* OsgTexture::getInjectState()
	{
		return mInjectState;
	}

} // namespace MyGUI
