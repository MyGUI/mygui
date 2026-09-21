/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGL3Texture.h"
#include "MyGUI_FileSystemUtility.h"
#include "MyGUI_OpenGL3RenderManager.h"
#include "MyGUI_OpenGL3Diagnostic.h"
#include "MyGUI_OpenGL3Platform.h"
#include "MyGUI_OpenGL3RTTexture.h"

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
				glGetIntegerv(_read ? GL_PIXEL_PACK_BUFFER_BINDING : GL_PIXEL_UNPACK_BUFFER_BINDING, &mPrevious);
				glBindBuffer(mTarget, _buffer);
			}
			~PixelBufferBinding()
			{
				glBindBuffer(mTarget, mPrevious);
			}

		private:
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

	OpenGL3Texture::OpenGL3Texture(const std::string& _name, OpenGL3ImageLoader* _loader) :
		mName(_name),
		mImageLoader(_loader)
	{
	}

	OpenGL3Texture::~OpenGL3Texture()
	{
		OpenGL3Texture::destroy();
	}

	const std::string& OpenGL3Texture::getName() const
	{
		return mName;
	}

	void OpenGL3Texture::setUsage(TextureUsage _usage)
	{
		// Usage is an allocation hint; each lock supplies its actual access mode.
		if (_usage.isValue(TextureUsage::Stream))
			mUsage = GL_STREAM_DRAW;
		else if (_usage.isValue(TextureUsage::Dynamic) || _usage.isValue(TextureUsage::RenderTarget))
			mUsage = GL_DYNAMIC_DRAW;
		else
			mUsage = GL_STATIC_DRAW;
	}

	void OpenGL3Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, nullptr);
	}

	void OpenGL3Texture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureId, "Texture already exist");

		//FIXME move to method
		mInternalPixelFormat = 0;
		mPixelFormat = 0;
		mNumElemBytes = 0;
		if (_format == PixelFormat::R8G8B8)
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

	void OpenGL3Texture::destroy()
	{
		if (mLock)
		{
			if (mWriteLock)
			{
				PixelBufferBinding binding(false, mPboID);
				glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			}
			else
				delete[] static_cast<unsigned char*>(mBuffer);
		}
		mWriteLock = false;
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

	void* OpenGL3Texture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTextureId, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLock, "Texture is already locked");
		const bool read = _access.isValue(TextureUsage::Read);
		const bool write = _access.isValue(TextureUsage::Write);
		MYGUI_PLATFORM_ASSERT(read || write, "Texture lock requires read or write access");

		TextureBinding texture(mTextureId);
		if (write)
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
			PixelTransferState transfer(true, 0);
			glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, bytes.get());
			mBuffer = bytes.release();
		}
		mWriteLock = write;
		mLock = true;
		return mBuffer;
	}

	void OpenGL3Texture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		const bool write = mWriteLock;
		mLock = false;
		mWriteLock = false;
		if (!write)
		{
			delete[] static_cast<unsigned char*>(mBuffer);
			mBuffer = nullptr;
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

	void OpenGL3Texture::loadFromFile(const std::string& _filename)
	{
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			void* data = mImageLoader->loadImage(width, height, format, _filename);
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
				delete[] (unsigned char*)data;
			}
		}
	}

	void OpenGL3Texture::saveToFile(const std::string& _filename)
	{
		if (mImageLoader)
		{
			const auto path = MyGUI::utility::toPath(_filename);
			void* data = lock(TextureUsage::Read);
			mImageLoader->saveImage(mWidth, mHeight, mOriginalFormat, data, path);
			unlock();
		}
	}

	void OpenGL3Texture::setShader(const std::string& _shaderName)
	{
		mProgramId = OpenGL3RenderManager::getInstance().getShaderProgramId(_shaderName);
	}

	IRenderTarget* OpenGL3Texture::getRenderTarget()
	{
		if (mRenderTarget == nullptr)
			mRenderTarget = new OpenGL3RTTexture(mTextureId);

		return mRenderTarget;
	}

	unsigned int OpenGL3Texture::getTextureId() const
	{
		return mTextureId;
	}

	unsigned int OpenGL3Texture::getShaderId() const
	{
		return mProgramId;
	}

	int OpenGL3Texture::getWidth() const
	{
		return mWidth;
	}

	int OpenGL3Texture::getHeight() const
	{
		return mHeight;
	}

	bool OpenGL3Texture::isLocked() const
	{
		return mLock;
	}

	PixelFormat OpenGL3Texture::getFormat() const
	{
		return mOriginalFormat;
	}

	TextureUsage OpenGL3Texture::getUsage() const
	{
		return mOriginalUsage;
	}

	size_t OpenGL3Texture::getNumElemBytes() const
	{
		return mNumElemBytes;
	}

} // namespace MyGUI
