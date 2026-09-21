#include "MyGUI_OpenGLESTexture.h"
#include "MyGUI_FileSystemUtility.h"
#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESDiagnostic.h"
#include "MyGUI_OpenGLESPlatform.h"
#include "MyGUI_OpenGLESRTTexture.h"

#include <GLES3/gl3.h>
#include "MyGUI_OpenGLESError.h"
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

	OpenGLESTexture::OpenGLESTexture(const std::string& _name, OpenGLESImageLoader* _loader) :
		mName(_name),
		mImageLoader(_loader)
	{
	}

	OpenGLESTexture::~OpenGLESTexture()
	{
		OpenGLESTexture::destroy();
	}

	const std::string& OpenGLESTexture::getName() const
	{
		return mName;
	}

	void OpenGLESTexture::setUsage(TextureUsage _usage)
	{
		mUsage = _usage.isValue(TextureUsage::Stream) ? GL_STREAM_DRAW
			: _usage.isValue(TextureUsage::Dynamic)
			? GL_DYNAMIC_DRAW
			: GL_STATIC_DRAW;
	}

	void OpenGLESTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format)
	{
		createManual(_width, _height, _usage, _format, nullptr);
	}

	void OpenGLESTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureId, "Texture already exists");
		MYGUI_PLATFORM_ASSERT(_width > 0 && _height > 0, "Texture dimensions must be positive");
		MYGUI_PLATFORM_ASSERT(
			!_usage.isValue(TextureUsage::RenderTarget) || _format == PixelFormat::R8G8B8 ||
				_format == PixelFormat::R8G8B8A8,
			"Luminance render targets are not supported");
		GLint maximum = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maximum);
		MYGUI_PLATFORM_ASSERT(_width <= maximum && _height <= maximum, "Texture dimensions exceed GL_MAX_TEXTURE_SIZE");
		MYGUI_PLATFORM_ASSERT(
			size_t(_width) <= size_t(std::numeric_limits<GLsizeiptr>::max()) / size_t(_height) / 4,
			"Texture transfer size is too large");
		if (_format == PixelFormat::L8)
			mNumElemBytes = 1;
		else if (_format == PixelFormat::L8A8)
			mNumElemBytes = 2;
		else if (_format == PixelFormat::R8G8B8)
			mNumElemBytes = 3;
		else if (_format == PixelFormat::R8G8B8A8)
			mNumElemBytes = 4;
		else
			MYGUI_PLATFORM_EXCEPT("Unsupported texture format");

		mWidth = _width;
		mHeight = _height;
		mDataSize = size_t(_width) * size_t(_height) * mNumElemBytes;
		mOriginalFormat = _format;
		mOriginalUsage = _usage;
		setUsage(_usage);

		glGenTextures(1, &mTextureId);
		TextureBinding texture(mTextureId);
		PixelTransferState transfer(false, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// ES 3 removes luminance formats. Expand them to RGBA, also making all
		// supported formats colour-renderable for portable framebuffer readback.
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			mNumElemBytes == 3 ? GL_RGB8 : GL_RGBA8,
			mWidth,
			mHeight,
			0,
			mNumElemBytes == 3 ? GL_RGB : GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr);
		if (_data)
			upload(static_cast<const unsigned char*>(_data));
	}

	void OpenGLESTexture::upload(const unsigned char* _data)
	{
		const size_t pixels = size_t(mWidth) * size_t(mHeight);
		const size_t channels = mNumElemBytes == 3 ? 3 : 4;
		std::vector<unsigned char> rgba(pixels * channels);
		for (size_t i = 0; i < pixels; ++i)
		{
			const auto* source = _data + i * mNumElemBytes;
			auto* destination = rgba.data() + i * channels;
			destination[0] = source[mNumElemBytes >= 3 ? 2 : 0];
			destination[1] = source[mNumElemBytes >= 3 ? 1 : 0];
			destination[2] = source[0];
			if (channels == 4)
				destination[3] = mNumElemBytes == 4 ? source[3] : mNumElemBytes == 2 ? source[1] : 255;
		}
		TextureBinding texture(mTextureId);
		// PBOs are core in ES 3/WebGL 2. CPU staging avoids unsupported WebGL
		// read mappings; glBufferData also retires storage used by earlier draws.
		if (!mPboID)
			glGenBuffers(1, &mPboID);
		PixelTransferState transfer(false, mPboID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, static_cast<GLsizeiptr>(rgba.size()), rgba.data(), mUsage);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			mWidth,
			mHeight,
			channels == 3 ? GL_RGB : GL_RGBA,
			GL_UNSIGNED_BYTE,
			nullptr);
	}

	void OpenGLESTexture::readback(unsigned char* _data)
	{
		std::vector<unsigned char> rgba(size_t(mWidth) * size_t(mHeight) * 4);
		PixelTransferState transfer(true, 0);
		GLint previous = 0;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previous);
		GLuint framebuffer = 0;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);
		const auto status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
		if (status == GL_FRAMEBUFFER_COMPLETE)
			glReadPixels(0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
		glBindFramebuffer(GL_READ_FRAMEBUFFER, previous);
		glDeleteFramebuffers(1, &framebuffer);
		MYGUI_PLATFORM_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Texture is not readable");
		for (size_t i = 0; i < rgba.size() / 4; ++i)
		{
			const auto* source = rgba.data() + i * 4;
			auto* destination = _data + i * mNumElemBytes;
			if (mNumElemBytes >= 3)
			{
				destination[0] = source[2];
				destination[1] = source[1];
				destination[2] = source[0];
				if (mNumElemBytes == 4)
					destination[3] = source[3];
			}
			else
			{
				destination[0] = source[0];
				if (mNumElemBytes == 2)
					destination[1] = source[3];
			}
		}
	}

	void OpenGLESTexture::destroy()
	{
		delete mRenderTarget;
		mRenderTarget = nullptr;
		delete[] static_cast<unsigned char*>(mBuffer);
		mBuffer = nullptr;
		if (mTextureId)
			glDeleteTextures(1, &mTextureId);
		if (mPboID)
			glDeleteBuffers(1, &mPboID);
		mTextureId = mPboID = 0;
		mWidth = mHeight = 0;
		mLock = mWriteLock = false;
		mDataSize = mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
	}

	void* OpenGLESTexture::lock(TextureUsage _access)
	{
		MYGUI_PLATFORM_ASSERT(mTextureId, "Texture is not created");
		MYGUI_PLATFORM_ASSERT(!mLock, "Texture is already locked");
		MYGUI_PLATFORM_ASSERT(
			_access.isValue(TextureUsage::Read) || _access.isValue(TextureUsage::Write),
			"Invalid lock access");
		auto buffer = std::make_unique<unsigned char[]>(mDataSize);
		if (_access.isValue(TextureUsage::Read))
			readback(buffer.get());
		mWriteLock = _access.isValue(TextureUsage::Write);
		mBuffer = buffer.release();
		mLock = true;
		return mBuffer;
	}

	void OpenGLESTexture::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");
		std::unique_ptr<unsigned char[]> buffer(static_cast<unsigned char*>(mBuffer));
		mBuffer = nullptr;
		mLock = false;
		if (mWriteLock)
			upload(buffer.get());
	}

	void OpenGLESTexture::loadFromFile(const std::string& _filename)
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

	void OpenGLESTexture::saveToFile(const std::string& _filename)
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

	void OpenGLESTexture::setShader(const std::string& _shaderName)
	{
		mShaderName = _shaderName;
	}

	IRenderTarget* OpenGLESTexture::getRenderTarget()
	{
		MYGUI_PLATFORM_ASSERT(mNumElemBytes >= 3, "Luminance render targets are not supported");
		if (mRenderTarget == nullptr)
			mRenderTarget = new OpenGLESRTTexture(mTextureId, mWidth, mHeight);

		return mRenderTarget;
	}

	unsigned int OpenGLESTexture::getTextureId() const
	{
		return mTextureId;
	}

	unsigned int OpenGLESTexture::getShaderId() const
	{
		return mShaderName.empty() ? 0 : OpenGLESRenderManager::getInstance().getShaderProgramId(mShaderName);
	}

} // namespace MyGUI
