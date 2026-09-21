/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_FileSystemUtility.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_OpenGLPlatform.h"
#include "MyGUI_OpenGLRTTexture.h"

#include <MyGUI_GL.h>
#include <array>
#include <limits>
#include <memory>

namespace MyGUI
{
	namespace
	{

		class TextureBinding
		{
		public:
			explicit TextureBinding(GLuint _texture)
			{
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &mPrevious);
				glBindTexture(GL_TEXTURE_2D, _texture);
			}
			~TextureBinding()
			{
				glBindTexture(GL_TEXTURE_2D, mPrevious);
			}

		private:
			GLint mPrevious{};
		};

		class PixelBufferBinding
		{
		public:
			PixelBufferBinding(bool _read, GLuint _buffer) :
				mTarget(_read ? GL_PIXEL_PACK_BUFFER : GL_PIXEL_UNPACK_BUFFER)
			{
				if (!OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
					return;
				mSupported = true;
				glGetIntegerv(_read ? GL_PIXEL_PACK_BUFFER_BINDING : GL_PIXEL_UNPACK_BUFFER_BINDING, &mPrevious);
				glBindBuffer(mTarget, _buffer);
			}
			~PixelBufferBinding()
			{
				if (mSupported)
					glBindBuffer(mTarget, mPrevious);
			}

		private:
			bool mSupported{false};
			GLenum mTarget;
			GLint mPrevious{};
		};

		constexpr std::array<GLenum, 4> packParameters =
			{GL_PACK_ALIGNMENT, GL_PACK_ROW_LENGTH, GL_PACK_SKIP_ROWS, GL_PACK_SKIP_PIXELS};
		constexpr std::array<GLenum, 4> unpackParameters =
			{GL_UNPACK_ALIGNMENT, GL_UNPACK_ROW_LENGTH, GL_UNPACK_SKIP_ROWS, GL_UNPACK_SKIP_PIXELS};

		// The API exposes tightly packed 2D bytes, independently of host GL state.
		class PixelTransferState
		{
		public:
			PixelTransferState(bool _read, GLuint _buffer) :
				mBinding(_read, _buffer),
				mParameters(_read ? packParameters : unpackParameters)
			{
				for (size_t i = 0; i < mParameters.size(); ++i)
				{
					glGetIntegerv(mParameters[i], &mPrevious[i]);
					glPixelStorei(mParameters[i], i == 0 ? 1 : 0);
				}
			}
			~PixelTransferState()
			{
				for (size_t i = 0; i < mParameters.size(); ++i)
					glPixelStorei(mParameters[i], mPrevious[i]);
			}

		private:
			PixelBufferBinding mBinding;
			std::array<GLenum, 4> mParameters;
			std::array<GLint, 4> mPrevious{};
		};

	}

	OpenGLTexture::OpenGLTexture(const std::string& _name, OpenGLImageLoader* _loader) :
		mName(_name),
		mImageLoader(_loader)
	{
	}

	OpenGLTexture::~OpenGLTexture()
	{
		OpenGLTexture::destroy();
	}

	const std::string& OpenGLTexture::getName() const
	{
		return mName;
	}

	void OpenGLTexture::setUsage(TextureUsage _usage)
	{
		// Usage is an allocation hint; each lock supplies its actual access mode.
		if (_usage.isValue(TextureUsage::Stream))
			mUsage = GL_STREAM_DRAW;
		else if (_usage.isValue(TextureUsage::Dynamic) || _usage.isValue(TextureUsage::RenderTarget))
			mUsage = GL_DYNAMIC_DRAW;
		else
			mUsage = GL_STATIC_DRAW;
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, nullptr);
	}

	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureId, "Texture already exist");

		//FIXME move to method
		mInternalPixelFormat = 0;
		mPixelFormat = 0;
		mNumElemBytes = 0;
		if (_format == PixelFormat::L8)
		{
			mInternalPixelFormat = GL_LUMINANCE8;
			mPixelFormat = GL_LUMINANCE;
			mNumElemBytes = 1;
		}
		else if (_format == PixelFormat::L8A8)
		{
			mInternalPixelFormat = GL_LUMINANCE8_ALPHA8;
			mPixelFormat = GL_LUMINANCE_ALPHA;
			mNumElemBytes = 2;
		}
		else if (_format == PixelFormat::R8G8B8)
		{
			mInternalPixelFormat = GL_RGB8;
			mPixelFormat = GL_BGR;
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::R8G8B8A8)
		{
			mInternalPixelFormat = GL_RGBA8;
			mPixelFormat = GL_BGRA;
			mNumElemBytes = 4;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("format not support");
		}

		MYGUI_PLATFORM_ASSERT(_width > 0 && _height > 0, "Texture dimensions must be positive");
		MYGUI_PLATFORM_ASSERT(
			size_t(_width) <= size_t(std::numeric_limits<GLsizeiptr>::max()) / size_t(_height) / mNumElemBytes,
			"Texture transfer size is too large");
		GLint maximumSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximumSize);
		MYGUI_PLATFORM_ASSERT(_width <= maximumSize && _height <= maximumSize, "Texture exceeds maximum dimensions");
		mWidth = _width;
		mHeight = _height;
		mDataSize = size_t(_width) * size_t(_height) * mNumElemBytes;
		setUsage(_usage);
		//MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;

		glGenTextures(1, &mTextureId);
		TextureBinding texture(mTextureId);
		PixelTransferState transfer(false, 0);
		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			mInternalPixelFormat,
			mWidth,
			mHeight,
			0,
			mPixelFormat,
			GL_UNSIGNED_BYTE,
			(GLvoid*)_data);
	}

	void OpenGLTexture::destroy()
	{
		if (mLock)
		{
			if (mPboLock)
			{
				PixelBufferBinding binding(false, mPboID);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			}
			else
				delete[] static_cast<unsigned char*>(mBuffer);
		}
		mWriteLock = false;
		mPboLock = false;
		delete mRenderTarget;
		mRenderTarget = nullptr;

		if (mTextureId != 0)
		{
			glDeleteTextures(1, &mTextureId);
			mTextureId = 0;
		}
		if (mPboID != 0)
		{
			glDeleteBuffers(1, &mPboID);
			mPboID = 0;
		}

		mWidth = 0;
		mHeight = 0;
		mLock = false;
		mPixelFormat = 0;
		mDataSize = 0;
		mUsage = 0;
		mBuffer = nullptr;
		mInternalPixelFormat = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
	}

	void* OpenGLTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTextureId, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLock, "Texture is already locked");
		const bool read = _access.isValue(TextureUsage::Read);
		const bool write = _access.isValue(TextureUsage::Write);
		MYGUI_PLATFORM_ASSERT(read || write, "Texture lock requires read or write access");

		TextureBinding texture(mTextureId);
		const bool pbo = write && OpenGLRenderManager::getInstance().isPixelBufferObjectSupported();
		if (pbo)
		{
			// Allocate lazily, including for textures initially loaded from an image.
			if (!mPboID)
				glGenBuffers(1, &mPboID);
			PixelBufferBinding binding(false, mPboID);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, nullptr, mUsage);
			if (read)
			{
				// Refill fresh storage from the texture before exposing a read/write lock.
				// The texture, not an older upload buffer, is authoritative after RTT draws.
				PixelTransferState transfer(true, mPboID);
				glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, nullptr);
			}
			mBuffer = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, read ? GL_READ_WRITE : GL_WRITE_ONLY);
			MYGUI_PLATFORM_ASSERT(mBuffer, "Error texture lock");
		}
		else
		{
			auto bytes = std::make_unique<unsigned char[]>(mDataSize);
			if (read)
			{
				PixelTransferState transfer(true, 0);
				glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, bytes.get());
			}
			mBuffer = bytes.release();
		}
		mPboLock = pbo;
		mWriteLock = write;
		mLock = true;
		return mBuffer;
	}

	void OpenGLTexture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		const bool write = mWriteLock;
		const bool pbo = mPboLock;
		mLock = false;
		mWriteLock = false;
		mPboLock = false;
		if (!pbo)
		{
			std::unique_ptr<unsigned char[]> bytes(static_cast<unsigned char*>(mBuffer));
			mBuffer = nullptr;
			if (write)
			{
				TextureBinding texture(mTextureId);
				PixelTransferState transfer(false, 0);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, bytes.get());
			}
			return;
		}

		mBuffer = nullptr;
		PixelBufferBinding binding(false, mPboID);
		const GLboolean valid = glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
		MYGUI_PLATFORM_ASSERT(valid == GL_TRUE, "Texture upload buffer contents were lost");
		TextureBinding texture(mTextureId);
		PixelTransferState transfer(false, mPboID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, nullptr);
	}

	void OpenGLTexture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			std::unique_ptr<unsigned char[]> data(
				static_cast<unsigned char*>(mImageLoader->loadImage(width, height, format, _filename)));
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data.get());
			}
		}
	}

	void OpenGLTexture::saveToFile(const std::string& _filename)
	{
		if (mImageLoader)
		{
			const auto path = MyGUI::utility::toPath(_filename);
			void* data = lock(TextureUsage::Read);
			try
			{
				mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, path);
			}
			catch (...)
			{
				unlock();
				throw;
			}
			unlock();
		}
	}

	void OpenGLTexture::setShader(const std::string& _shaderName)
	{
		MYGUI_PLATFORM_LOG(Warning, "OpenGLTexture::setShader is not implemented");
	}

	IRenderTarget* OpenGLTexture::getRenderTarget()
	{
		MYGUI_PLATFORM_ASSERT(mTextureId, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(
			OpenGLRenderManager::getInstance().isFormatSupported(mOriginalFormat, TextureUsage::RenderTarget),
			"Render target format is not supported");
		if (mRenderTarget == nullptr)
			mRenderTarget = new OpenGLRTTexture(mTextureId);

		return mRenderTarget;
	}

	unsigned int OpenGLTexture::getTextureId() const
	{
		return mTextureId;
	}

	int OpenGLTexture::getWidth() const
	{
		return mWidth;
	}

	int OpenGLTexture::getHeight() const
	{
		return mHeight;
	}

	bool OpenGLTexture::isLocked() const
	{
		return mLock;
	}

	PixelFormat OpenGLTexture::getFormat() const
	{
		return mOriginalFormat;
	}

	TextureUsage OpenGLTexture::getUsage() const
	{
		return mOriginalUsage;
	}

	size_t OpenGLTexture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

} // namespace MyGUI
