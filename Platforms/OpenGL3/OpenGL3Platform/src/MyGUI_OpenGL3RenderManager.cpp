/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGL3RenderManager.h"
#include "MyGUI_OpenGL3Texture.h"
#include "MyGUI_OpenGL3VertexBuffer.h"
#include "MyGUI_OpenGL3Diagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataManager.h"

#include <GL/gl.h>

namespace MyGUI
{

	OpenGL3RenderManager& OpenGL3RenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OpenGL3RenderManager* OpenGL3RenderManager::getInstancePtr()
	{
		return static_cast<OpenGL3RenderManager*>(RenderManager::getInstancePtr());
	}

	static GLuint buildShader(const std::string& text, GLenum type)
	{
		GLuint id = glCreateShader(type);
		const char* c_str = text.c_str();
		glShaderSource(id, 1, &c_str, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint len = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

			GLchar* buffer = new GLchar[len];
			glGetShaderInfoLog(id, len, nullptr, buffer);
			std::string infoLog = buffer;
			delete[] buffer;

			MYGUI_PLATFORM_EXCEPT(infoLog);
		}

		return id;
	}

	std::string OpenGL3RenderManager::loadFileContent(const std::string& _file)
	{
		std::string fullPath = DataManager::getInstance().getDataPath(_file);
		if (fullPath.empty())
		{
			MYGUI_PLATFORM_LOG(Error, "Failed to load file content '" << _file << "'.");
			return {};
		}
		std::ifstream fileStream(DataManager::getInstance().getDataPath(_file));
		std::stringstream buffer;
		buffer << fileStream.rdbuf();
		return buffer.str();
	}

	GLuint OpenGL3RenderManager::createShaderProgram(
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		GLuint vsID = buildShader(loadFileContent(_vertexProgramFile), GL_VERTEX_SHADER);
		GLuint fsID = buildShader(loadFileContent(_fragmentProgramFile), GL_FRAGMENT_SHADER);

		GLuint progID = glCreateProgram();
		glAttachShader(progID, vsID);
		glAttachShader(progID, fsID);

		// setup vertex attribute positions for vertex buffer
		glBindAttribLocation(progID, 0, "VertexPosition");
		glBindAttribLocation(progID, 1, "VertexColor");
		glBindAttribLocation(progID, 2, "VertexTexCoord");

		glLinkProgram(progID);

		GLint success;
		glGetProgramiv(progID, GL_LINK_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint len = 0;
			glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &len);

			GLchar* buffer = new GLchar[len];
			glGetProgramInfoLog(progID, len, nullptr, buffer);
			std::string infoLog = buffer;
			delete[] buffer;

			MYGUI_PLATFORM_EXCEPT(infoLog);
		}
		glDeleteShader(vsID); // flag for deletion on call to glDeleteProgram
		glDeleteShader(fsID);

		int textureUniLoc = glGetUniformLocation(progID, "Texture");
		if (textureUniLoc == -1)
		{
			MYGUI_PLATFORM_EXCEPT("Unable to retrieve uniform variable location");
		}
		mYScaleUniformLocation = glGetUniformLocation(progID, "YScale");
		if (mYScaleUniformLocation == -1)
		{
			MYGUI_PLATFORM_EXCEPT("Unable to retrieve YScale variable location");
		}
		glUseProgram(progID);
		glUniform1i(textureUniLoc, 0); // set active sampler for 'Texture' to GL_TEXTURE0
		glUniform1f(mYScaleUniformLocation, 1.0f);
		glUseProgram(0);

		return progID;
	}

	void OpenGL3RenderManager::initialise(OpenGL3ImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		mReferenceCount = 0;

		int major = 0, minor = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		if (major < 3)
		{
			const char* version = (const char*)glGetString(GL_VERSION);
			MYGUI_PLATFORM_EXCEPT(std::string("OpenGL 3.0 or newer not available, current version is ") + version);
		}

		const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
		mPboIsSupported = extensions && strstr(extensions, "GL_EXT_pixel_buffer_object") != nullptr;

		registerShader("Default", "MyGUI_OpenGL3_VP.glsl", "MyGUI_OpenGL3_FP.glsl");

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGL3RenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGL3RenderManager::createVertexBuffer()
	{
		return new OpenGL3VertexBuffer();
	}

	void OpenGL3RenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGL3RenderManager::doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		glUniform1f(mYScaleUniformLocation, -1.0f);
		doRender(_buffer, _texture, _count);
		glUniform1f(mYScaleUniformLocation, 1.0f);
	}

	void OpenGL3RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGL3VertexBuffer* buffer = static_cast<OpenGL3VertexBuffer*>(_buffer);
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		unsigned int texture_id = 0;
		if (_texture)
		{
			OpenGL3Texture* texture = static_cast<OpenGL3Texture*>(_texture);
			texture_id = texture->getTextureId();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
			if (texture->getShaderId())
			{
				glUseProgram(texture->getShaderId());
			}
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);

		glBindVertexArray(buffer_id);
		glDrawArrays(GL_TRIANGLES, 0, _count);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		if (_texture && static_cast<OpenGL3Texture*>(_texture)->getShaderId())
		{
			glUseProgram(mDefaultProgramId);
		}
	}

	void OpenGL3RenderManager::begin()
	{
		++mReferenceCount;

		glUseProgram(mDefaultProgramId);
		glActiveTexture(GL_TEXTURE0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGL3RenderManager::end()
	{
		if (--mReferenceCount == 0)
		{
			glDisable(GL_BLEND);
			glUseProgram(0);
		}
	}

	const RenderTargetInfo& OpenGL3RenderManager::getInfo() const
	{
		return mInfo;
	}

	const IntSize& OpenGL3RenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OpenGL3RenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	bool OpenGL3RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if (_format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8)
			return true;

		return false;
	}

	void OpenGL3RenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent(time / 1000.0f);

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void OpenGL3RenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		glViewport(0, 0, _width, _height);

		onResizeView(mViewSize);
		mUpdate = true;
	}

	void OpenGL3RenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			glDeleteProgram(iter->second);
		mRegisteredShaders[_shaderName] = createShaderProgram(_vertexProgramFile, _fragmentProgramFile);
		if (_shaderName == "Default")
			mDefaultProgramId = mRegisteredShaders[_shaderName];
	}

	bool OpenGL3RenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	unsigned int OpenGL3RenderManager::getShaderProgramId(const std::string& _shaderName) const
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			return iter->second;
		MYGUI_PLATFORM_LOG(
			Error,
			"Failed to get program ID for shader '" << _shaderName << "'. Did you forgot to register shader?");
		return 0;
	}

	ITexture* OpenGL3RenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OpenGL3Texture* texture = new OpenGL3Texture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
	}

	void OpenGL3RenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OpenGL3RenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	void OpenGL3RenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();

		for (const auto& programId : mRegisteredShaders)
		{
			glDeleteProgram(programId.second);
		}
		mRegisteredShaders.clear();
		mDefaultProgramId = 0;
	}

} // namespace MyGUI
