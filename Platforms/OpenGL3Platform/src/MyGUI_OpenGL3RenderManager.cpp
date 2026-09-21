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
#include "MyGUI_DataStreamHolder.h"
#include "MyGUI_DataManager.h"

#include <MyGUI_GL.h>

namespace MyGUI
{
	namespace
	{

		constexpr std::array<unsigned int, 7> guiStateModes = {
			GL_BLEND,
			GL_CULL_FACE,
			GL_DEPTH_TEST,
			GL_STENCIL_TEST,
			GL_SCISSOR_TEST,
			GL_RASTERIZER_DISCARD,
			GL_COLOR_LOGIC_OP};
		constexpr std::array<unsigned int, 6> blendParameters = {
			GL_BLEND_SRC_RGB,
			GL_BLEND_DST_RGB,
			GL_BLEND_SRC_ALPHA,
			GL_BLEND_DST_ALPHA,
			GL_BLEND_EQUATION_RGB,
			GL_BLEND_EQUATION_ALPHA};

	}

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
		auto stream = DataManager::getInstance().getDataHolder(_file);
		if (!stream)
		{
			MYGUI_PLATFORM_LOG(Error, "Failed to load file content '" << _file << "'.");
			return {};
		}
		return stream->readAllText();
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

		// Pixel buffer objects are core since OpenGL 2.1. This backend requires 3.0.
		// glGetString(GL_EXTENSIONS) is invalid in a core context.
		mPboIsSupported = true;

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
		const auto* buffer = static_cast<OpenGL3VertexBuffer*>(_buffer);
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		unsigned int texture_id = 0;
		if (_texture)
		{
			const auto* texture = static_cast<OpenGL3Texture*>(_texture);
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
		if (mReferenceCount++ == 0)
		{
			for (size_t i = 0; i < guiStateModes.size(); ++i)
				mSavedState.enabled[i] = glIsEnabled(guiStateModes[i]) == GL_TRUE;
			glGetIntegerv(GL_POLYGON_MODE, mSavedState.polygonMode.data());
			for (size_t i = 0; i < blendParameters.size(); ++i)
				glGetIntegerv(blendParameters[i], &mSavedState.blend[i]);
			glGetBooleanv(GL_COLOR_WRITEMASK, mSavedState.colourMask.data());
			glGetBooleanv(GL_DEPTH_WRITEMASK, &mSavedState.depthMask);
			glGetIntegerv(GL_CURRENT_PROGRAM, &mSavedState.program);
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &mSavedState.vertexArray);
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &mSavedState.arrayBuffer);
			glGetIntegerv(GL_ACTIVE_TEXTURE, &mSavedState.activeTexture);
			glActiveTexture(GL_TEXTURE0);
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &mSavedState.texture);
		}

		glUseProgram(mDefaultProgramId);
		glActiveTexture(GL_TEXTURE0);

		for (size_t i = 1; i < guiStateModes.size(); ++i)
			glDisable(guiStateModes[i]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGL3RenderManager::end()
	{
		MYGUI_PLATFORM_ASSERT(mReferenceCount != 0, "Unmatched render target end");
		if (--mReferenceCount == 0)
		{
			for (size_t i = 0; i < guiStateModes.size(); ++i)
			{
				if (mSavedState.enabled[i])
					glEnable(guiStateModes[i]);
				else
					glDisable(guiStateModes[i]);
			}
			if (mSavedState.polygonMode[0] == mSavedState.polygonMode[1])
				glPolygonMode(GL_FRONT_AND_BACK, mSavedState.polygonMode[0]);
			else
			{
				// Separate front/back modes are only possible in compatibility contexts.
				glPolygonMode(GL_FRONT, mSavedState.polygonMode[0]);
				glPolygonMode(GL_BACK, mSavedState.polygonMode[1]);
			}
			glDepthMask(mSavedState.depthMask);
			glColorMask(
				mSavedState.colourMask[0],
				mSavedState.colourMask[1],
				mSavedState.colourMask[2],
				mSavedState.colourMask[3]);
			glBlendFuncSeparate(mSavedState.blend[0], mSavedState.blend[1], mSavedState.blend[2], mSavedState.blend[3]);
			glBlendEquationSeparate(mSavedState.blend[4], mSavedState.blend[5]);
			glUseProgram(mSavedState.program);
			glBindVertexArray(mSavedState.vertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, mSavedState.arrayBuffer);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mSavedState.texture);
			glActiveTexture(mSavedState.activeTexture);
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
		if (!Gui::getInstancePtr())
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent(time / 1000.0f);

		last_time = now_time;

		begin();
		try
		{
			onRenderToTarget(this, mUpdate);
		}
		catch (...)
		{
			end();
			throw;
		}
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
