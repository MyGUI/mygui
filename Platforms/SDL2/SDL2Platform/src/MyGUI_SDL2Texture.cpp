/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#include "MyGUI_SDL2Texture.h"
#include "MyGUI_SDL2RenderManager.h"
#include "MyGUI_SDL2Diagnostic.h"
#include "MyGUI_SDL2Platform.h"

namespace MyGUI
{

	SDL2Texture::SDL2Texture(const std::string& _name, SDL2ImageLoader* _loader, SDL2RenderManager *_manager, SDL_Renderer *renderer) :
		mName(_name),
		mWidth(0),
		mHeight(0),
		mPixelFormat(SDL_PIXELFORMAT_UNKNOWN),
		mInternalPixelFormat(0),
		mUsage(0),
		mAccess(0),
		mNumElemBytes(0),
		mDataSize(0),
		mPboID(0),
		mLock(false),
		mBuffer(nullptr),
		mImageLoader(_loader),
		mRenderTarget(nullptr),
		mRenderer(renderer),
		mTexture(nullptr)
	{
	}

	SDL2Texture::~SDL2Texture()
	{
		destroy();
	}

	const std::string& SDL2Texture::getName() const
	{
		return mName;
	}

	void SDL2Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, nullptr);
	}

	void SDL2Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT((_data == nullptr && mTexture) || !mTexture, "Texture already exist");

		if (!mTexture) {
			mInternalPixelFormat = 0;
			mPixelFormat = SDL_PIXELFORMAT_UNKNOWN;
			mNumElemBytes = 0;
			if (_format == PixelFormat::R8G8B8) {
				mPixelFormat = SDL_PIXELFORMAT_BGR24;
				mNumElemBytes = 3;
			} else if (_format == PixelFormat::R8G8B8A8) {
				mPixelFormat = SDL_PIXELFORMAT_BGRA32;
				mNumElemBytes = 4;
			} else {
				MYGUI_PLATFORM_EXCEPT("Format not supported");
			}

			mWidth = _width;
			mHeight = _height;
			mDataSize = _width * _height * mNumElemBytes;
			//MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

			mOriginalFormat = _format;
			mOriginalUsage = _usage;

			int access = 0;
			if (_usage.isValue(TextureUsage::RenderTarget)) {
				access = SDL_TEXTUREACCESS_TARGET;
			} else if (_usage.isValue(TextureUsage::Stream)) {
				access = SDL_TEXTUREACCESS_STREAMING;
			} else if (_usage.isValue(TextureUsage::Static)) {
				access = SDL_TEXTUREACCESS_STATIC;
			}

			mTexture = SDL_CreateTexture(mRenderer, mPixelFormat, access, _width, _height);

			char* pixels = nullptr;
			int pitch;
			if (access == SDL_TEXTUREACCESS_STREAMING) {
				SDL_LockTexture(mTexture, nullptr, (void **) &pixels, &pitch);
				// FIXME: This is actually incorrect, pitch can be > width
				memcpy(pixels, _data, pitch * _height);
				SDL_UnlockTexture(mTexture);
			} else {
				SDL_UpdateTexture(mTexture, nullptr, _data, _width * mNumElemBytes);
			}
		}

		SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
		SDL_SetTextureScaleMode(mTexture, SDL_ScaleModeBest);
	}

	void SDL2Texture::destroy()
	{
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}

		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}

		mWidth = 0;
		mHeight = 0;
		mLock = false;
		mPixelFormat = SDL_PIXELFORMAT_UNKNOWN;
		mDataSize = 0;
		mUsage = 0;
		mBuffer = nullptr;
		mInternalPixelFormat = 0;
		mAccess = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
	}

	void* SDL2Texture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTexture, "Texture is not created");

		if (_access == TextureUsage::Read)
		{
			mBuffer = nullptr;
			// FIXME: This actually doesn't work, because the texture needs to be streaming for this to work, and because this will not necessarily return the actual texture data. The only way is keeping a copy of the texture data around, which is wasteful, so I haven't implemented that yet...
			SDL_LockTexture(mTexture, nullptr, &mBuffer, nullptr);

			mLock = true;

			return mBuffer;
		}

		if (mAccess == TextureUsage::Stream) {
			mBuffer = nullptr;
			SDL_LockTexture(mTexture, nullptr, &mBuffer, nullptr);
			mLock = true;
		} else {
			mBuffer = new char[mDataSize];
		}
		return mBuffer;
	}

	void SDL2Texture::unlock()
	{
		if (mLock)
		{
			SDL_UnlockTexture(mTexture);

			return;
		}

		auto result = SDL_UpdateTexture(mTexture, nullptr, mBuffer, mWidth * mNumElemBytes);
		if (result != 0) {
			MYGUI_PLATFORM_EXCEPT("Error updating texture: " << SDL_GetError());
		}
		delete[] mBuffer;

//		if (!SDL2RenderManager::getInstance().isPixelBufferObjectSupported())
//		{
//			//Fallback if PBO's are not supported
//			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);
//			delete[] (char*)mBuffer;
//		}
//		else
//		{
//			// release the mapped buffer
//			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
//
//			// copy pixels from PBO to texture object
//			// Use offset instead of ponter.
//			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, nullptr);
//
//			// it is good idea to release PBOs with ID 0 after use.
//			// Once bound with 0, all pixel operations are back to normal ways.
//			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
//		}
//
//		glBindTexture(GL_TEXTURE_2D, 0);
//		mBuffer = nullptr;
//		mLock = false;
	}

	void SDL2Texture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			mTexture = mImageLoader->loadTexture(_filename);
			if (mTexture)
			{
				uint32_t format;
				int access;
				SDL_QueryTexture(mTexture, &format, &access, &mWidth, &mHeight);
				PixelFormat pixelFormat;
				switch (format) {
					case SDL_PIXELFORMAT_RGBA8888:
					case SDL_PIXELFORMAT_BGRA8888:
					case SDL_PIXELFORMAT_ABGR8888:
					case SDL_PIXELFORMAT_ARGB8888:
						mNumElemBytes = 4;
						pixelFormat = PixelFormat::R8G8B8A8;
						break;
					case SDL_PIXELFORMAT_RGB888:
					case SDL_PIXELFORMAT_BGR888:
						mNumElemBytes = 3;
						pixelFormat = PixelFormat::R8G8B8;
						break;
					default:
						MYGUI_PLATFORM_EXCEPT("Unsupported texture format '" << SDL_GetPixelFormatName(format) << "'");
				}
				mPixelFormat = static_cast<SDL_PixelFormatEnum>(format);

				mDataSize = mWidth * mHeight * mNumElemBytes;

				createManual(mWidth, mHeight, TextureUsage::Static | TextureUsage::Write, pixelFormat);
			}
		}
	}

	void SDL2Texture::saveToFile(const std::string& _filename)
	{
		if (mImageLoader)
		{
			void* data = lock(TextureUsage::Read);
			mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, _filename);
			unlock();
		}
	}

	void SDL2Texture::setShader(const std::string& _shaderName)
	{
		MYGUI_PLATFORM_LOG(Warning, "SDL2Texture::setShader is not implemented (for shader '" << _shaderName << "')");
	}

	IRenderTarget* SDL2Texture::getRenderTarget()
	{
		MYGUI_PLATFORM_LOG(Warning, "SDL2Texture::getRenderTarget is not implemented");
		// TODO
//		if (mRenderTarget == nullptr)
//			mRenderTarget = new SDL2RTTexture(mTextureId);
//
//		return mRenderTarget;

		return nullptr;
	}

	int SDL2Texture::getWidth() const
	{
		return mWidth;
	}

	int SDL2Texture::getHeight() const
	{
		return mHeight;
	}

	bool SDL2Texture::isLocked() const
	{
		return mLock;
	}

	PixelFormat SDL2Texture::getFormat() const
	{
		return mOriginalFormat;
	}

	TextureUsage SDL2Texture::getUsage() const
	{
		return mOriginalUsage;
	}

	size_t SDL2Texture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

} // namespace MyGUI
