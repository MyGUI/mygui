#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESTexture.h"
#include "MyGUI_OpenGLESVertexBuffer.h"
#include "MyGUI_OpenGLESDiagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataManager.h"

#include <GLES3/gl3.h>
//#include <GLES3/gl2ext.h>

namespace MyGUI
{

	OpenGLESRenderManager& OpenGLESRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OpenGLESRenderManager* OpenGLESRenderManager::getInstancePtr()
	{
		return static_cast<OpenGLESRenderManager*>(RenderManager::getInstancePtr());
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

	std::string OpenGLESRenderManager::loadFileContent(const std::string& _file)
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

	GLuint OpenGLESRenderManager::createShaderProgram(
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

	void OpenGLESRenderManager::initialise(OpenGLESImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		mReferenceCount = 0;

		const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
		mPboIsSupported = extensions && strstr(extensions, "GL_EXT_pixel_buffer_object") != nullptr;

		registerShader("Default", "MyGUI_OpenGLES_VP.glsl", "MyGUI_OpenGLES_FP.glsl");

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLESRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLESRenderManager::createVertexBuffer()
	{
		return new OpenGLESVertexBuffer();
	}

	void OpenGLESRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGLESRenderManager::doRenderRtt(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		glUniform1f(mYScaleUniformLocation, -1.0f);
		doRender(_buffer, _texture, _count);
		glUniform1f(mYScaleUniformLocation, 1.0f);
	}

	void OpenGLESRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLESVertexBuffer* buffer = static_cast<OpenGLESVertexBuffer*>(_buffer);
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		unsigned int texture_id = 0;
		if (_texture)
		{
			OpenGLESTexture* texture = static_cast<OpenGLESTexture*>(_texture);
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
		if (_texture && static_cast<OpenGLESTexture*>(_texture)->getShaderId())
		{
			glUseProgram(mDefaultProgramId);
		}
	}

	void OpenGLESRenderManager::begin()
	{
		++mReferenceCount;

		glUseProgram(mDefaultProgramId);
		glActiveTexture(GL_TEXTURE0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLESRenderManager::end()
	{
		if (--mReferenceCount == 0)
		{
			glDisable(GL_BLEND);
			glUseProgram(0);
		}
	}

	const RenderTargetInfo& OpenGLESRenderManager::getInfo() const
	{
		return mInfo;
	}

	const IntSize& OpenGLESRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OpenGLESRenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	bool OpenGLESRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if (_format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8)
			return true;

		return false;
	}

	void OpenGLESRenderManager::drawOneFrame()
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

	void OpenGLESRenderManager::setViewSize(int _width, int _height)
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

	void OpenGLESRenderManager::registerShader(
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

	bool OpenGLESRenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	unsigned int OpenGLESRenderManager::getShaderProgramId(const std::string& _shaderName) const
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			return iter->second;
		MYGUI_PLATFORM_LOG(
			Error,
			"Failed to get program ID for shader '" << _shaderName << "'. Did you forgot to register shader?");
		return 0;
	}

	ITexture* OpenGLESRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OpenGLESTexture* texture = new OpenGLESTexture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
	}

	void OpenGLESRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OpenGLESRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	void OpenGLESRenderManager::destroyAllResources()
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
