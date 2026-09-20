/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgTexture.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_OsgRTTexture.h"
#include "MyGUI_OsgRenderManager.h"

#include <algorithm>
#include <cstring>

#include <osg/GL>
#include <osg/Image>
#include <osg/Program>
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osg/Uniform>
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
		MYGUI_PLATFORM_ASSERT(!isLocked(), "Cannot recreate a locked texture");
		MYGUI_PLATFORM_ASSERT(_width > 0 && _height > 0, "Invalid texture dimensions");
		MYGUI_PLATFORM_ASSERT(mRenderManager->isFormatSupported(_format, _usage), "Texture format/usage not supported");
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
		mImage = nullptr;
		if (!_usage.isValue(TextureUsage::RenderTarget))
		{
			mImage = new osg::Image;
			mImage->allocateImage(_width, _height, 1, glfmt, GL_UNSIGNED_BYTE, 1);
			std::fill_n(mImage->data(), mImage->getTotalSizeInBytes(), 0);
			mImage->setOrigin(osg::Image::TOP_LEFT);
			uploadImage();
		}
	}

	void OsgTexture::destroy()
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
		mTexture = nullptr;
		mLockedImage = nullptr;
		mImage = nullptr;
		mReadOnlyLock = false;
		mFormat = PixelFormat::Unknow;
		mUsage = TextureUsage::Default;
		mNumElemBytes = 0;
		mWidth = 0;
		mHeight = 0;
	}

	void OsgTexture::loadFromFile(const std::string& _filename)
	{
		osg::ref_ptr<osg::Image> image = mRenderManager->loadImage(_filename);
		if (!image.valid())
		{
			MYGUI_PLATFORM_LOG(Critical, "Failed to load image '" << _filename << "'");
			MYGUI_PLATFORM_EXCEPT("Failed to load image '" << _filename << "'");
		}

		MYGUI_PLATFORM_ASSERT(image->getDataType() == GL_UNSIGNED_BYTE, "Unsupported image pixel type");
		PixelFormat format;
		switch (image->getPixelFormat())
		{
		case GL_LUMINANCE: format = PixelFormat::L8; break;
		case GL_ALPHA:
		case GL_LUMINANCE_ALPHA: format = PixelFormat::L8A8; break;
		case GL_RGB:
		case GL_BGR: format = PixelFormat::R8G8B8; break;
		case GL_RGBA:
		case GL_BGRA: format = PixelFormat::R8G8B8A8; break;
		default: MYGUI_PLATFORM_EXCEPT("Unsupported image pixel format");
		}
		createManual(image->s(), image->t(), TextureUsage::Static | TextureUsage::Write, format);
		// Canonical, tightly packed CPU pixels match MyGUI's BGR(A) byte order.
		// Do not mutate images returned by a shared application image cache.
		const size_t sourceBytes = image->getPixelSizeInBits() / 8;
		for (int y = 0; y < mHeight; ++y)
		{
			for (int x = 0; x < mWidth; ++x)
			{
				const unsigned char* source = image->data(x, y);
				unsigned char* dest = mImage->data(x, y);
				if (image->getPixelFormat() == GL_ALPHA)
				{
					dest[0] = 255;
					dest[1] = source[0];
				}
				else
				{
					std::copy_n(source, sourceBytes, dest);
					if (image->getPixelFormat() == GL_RGB || image->getPixelFormat() == GL_RGBA)
						std::swap(dest[0], dest[2]);
				}
			}
		}
		uploadImage();
	}

	void OsgTexture::saveToFile(const std::string& _filename)
	{
		osg::Image* image = mImage.get();
		if (image == nullptr)
		{
			MYGUI_PLATFORM_LOG(Warning, "Texture is not created or has no image to save");
			return;
		}

		// osgDB writers expect bottom-up rows. Keep CPU pixels and pending draws unchanged.
		osg::ref_ptr<osg::Image> output = image;
		if (image->getOrigin() == osg::Image::TOP_LEFT)
		{
			output = new osg::Image(*image, osg::CopyOp::DEEP_COPY_ALL);
			output->flipVertical();
			output->setOrigin(osg::Image::BOTTOM_LEFT);
		}
		if (!osgDB::writeImageFile(*output, _filename))
		{
			MYGUI_PLATFORM_LOG(Warning, "Failed to save image to file " << _filename);
		}
	}

	void* OsgTexture::lock(TextureUsage _access)
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

		MYGUI_PLATFORM_ASSERT(mImage.valid() && mRenderTarget == nullptr, "Render-target pixels require GPU readback");
		mReadOnlyLock = _access.isValue(TextureUsage::Read) && !_access.isValue(TextureUsage::Write);
		if (mReadOnlyLock)
			mLockedImage = mImage;
		else
			mLockedImage = new osg::Image(*mImage, osg::CopyOp::DEEP_COPY_ALL);
		return mLockedImage->data();
	}

	void OsgTexture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLockedImage.valid(), "Texture not locked");
		if (!mReadOnlyLock)
		{
			mImage = mLockedImage;
			uploadImage();
		}
		mLockedImage = nullptr;
		mReadOnlyLock = false;
	}

	void OsgTexture::uploadImage()
	{
		osg::ref_ptr<osg::Image> upload = mImage;
		if (mFormat == PixelFormat::L8 || mFormat == PixelFormat::L8A8)
		{
			// OpenGL 3.2 core (including macOS) has no legacy luminance formats
			// or guaranteed texture swizzle. Expand only the GPU upload.
			upload = new osg::Image;
			upload->allocateImage(mWidth, mHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1);
			for (int y = 0; y < mHeight; ++y)
				for (int x = 0; x < mWidth; ++x)
				{
					const unsigned char* source = mImage->data(x, y);
					unsigned char* dest = upload->data(x, y);
					dest[0] = dest[1] = dest[2] = source[0];
					dest[3] = mFormat == PixelFormat::L8A8 ? source[1] : 255;
				}
		}
		osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(*mTexture);
		texture->setImage(upload);
		texture->setSourceFormat(upload->getPixelFormat());
		texture->setSourceType(GL_UNSIGNED_BYTE);
		texture->setInternalFormat(mFormat == PixelFormat::R8G8B8 ? GL_RGB8 : GL_RGBA8);
		texture->setResizeNonPowerOfTwoHint(false);
		// Keep CPU pixels for read locks and later contexts; old batches retain the old texture.
		texture->setUnRefImageDataAfterApply(false);
		mTexture = texture;
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
		if (mRenderTarget == nullptr && mTexture.valid())
		{
			MYGUI_PLATFORM_ASSERT(!isLocked(), "Cannot render to a locked texture");
			MYGUI_PLATFORM_ASSERT(
				mUsage.isValue(TextureUsage::RenderTarget),
				"Texture was not created as a render target");
			mRenderTarget = new OsgRTTexture(mTexture.get(), mRenderManager, mWidth, mHeight);
		}
		return mRenderTarget;
	}

	void OsgTexture::setShader(const std::string& _shaderName)
	{
		osg::Program* program = mRenderManager->getShaderProgram(_shaderName);
		if (program == nullptr)
			return;

		// Pending batches may still reference the previous shader state.
		mShaderStateSet = new osg::StateSet;
		mShaderStateSet->setAttributeAndModes(program, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);

		// bind the 'Texture' sampler to texture unit 0, matching the texture bound
		// for each batch in osgDrawBatches
		if (!mShaderStateSet->getUniform("Texture"))
			mShaderStateSet->addUniform(new osg::Uniform("Texture", 0));
	}

	osg::Texture2D* OsgTexture::getTexture() const
	{
		return mTexture.get();
	}

	osg::StateSet* OsgTexture::getInjectState()
	{
		return mInjectState;
	}

	osg::StateSet* OsgTexture::getShaderStateSet()
	{
		return mShaderStateSet;
	}

} // namespace MyGUI
