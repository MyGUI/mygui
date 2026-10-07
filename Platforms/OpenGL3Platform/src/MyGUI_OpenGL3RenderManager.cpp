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

	namespace
	{

		struct Shader
		{
			explicit Shader(GLenum type) :
				id(glCreateShader(type))
			{
			}
			~Shader()
			{
				glDeleteShader(id);
			}
			GLuint id;
		};

		void compileShader(GLuint id, const std::string& text)
		{
			const char* source = text.c_str();
			glShaderSource(id, 1, &source, nullptr);
			glCompileShader(id);
			GLint success = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				GLint length = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				std::string message(size_t(length > 0 ? length : 1), '\0');
				glGetShaderInfoLog(id, length, nullptr, message.data());
				MYGUI_PLATFORM_EXCEPT(message.c_str());
			}
		}

	}

	OpenGL3RenderManager::ShaderProgram::~ShaderProgram()
	{
		if (id)
			glDeleteProgram(id);
	}

	std::string OpenGL3RenderManager::loadFileContent(const std::string& _file)
	{
		auto source = DataManager::getInstance().getDataHolder(_file);
		MYGUI_PLATFORM_ASSERT(source, "Failed to load shader '" << _file << "'.");
		return source->readAllText();
	}

	std::unique_ptr<OpenGL3RenderManager::ShaderProgram> OpenGL3RenderManager::createShaderProgram(
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		Shader vertex(GL_VERTEX_SHADER), fragment(GL_FRAGMENT_SHADER);
		compileShader(vertex.id, loadFileContent(_vertexProgramFile));
		compileShader(fragment.id, loadFileContent(_fragmentProgramFile));
		auto program = std::make_unique<ShaderProgram>();
		program->id = glCreateProgram();
		glAttachShader(program->id, vertex.id);
		glAttachShader(program->id, fragment.id);
		glBindAttribLocation(program->id, 0, "VertexPosition");
		glBindAttribLocation(program->id, 1, "VertexColor");
		glBindAttribLocation(program->id, 2, "VertexTexCoord");
		glLinkProgram(program->id);
		glDetachShader(program->id, vertex.id);
		glDetachShader(program->id, fragment.id);

		GLint success = 0;
		glGetProgramiv(program->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint length = 0;
			glGetProgramiv(program->id, GL_INFO_LOG_LENGTH, &length);
			std::string message(size_t(length > 0 ? length : 1), '\0');
			glGetProgramInfoLog(program->id, length, nullptr, message.data());
			MYGUI_PLATFORM_EXCEPT(message.c_str());
		}
		const int texture = glGetUniformLocation(program->id, "Texture");
		program->yScale = glGetUniformLocation(program->id, "YScale");
		MYGUI_PLATFORM_ASSERT(
			texture != -1 && program->yScale != -1,
			"Unable to retrieve Texture or YScale uniform location");
		GLint previous = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &previous);
		glUseProgram(program->id);
		glUniform1i(texture, 0);
		glUniform1f(program->yScale, 1.0f);
		glUseProgram(previous);
		return program;
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
		render(_buffer, _texture, _count, -1.0f);
	}

	void OpenGL3RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		render(_buffer, _texture, _count, 1.0f);
	}

	void OpenGL3RenderManager::render(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, float _yScale)
	{
		const auto* buffer = static_cast<OpenGL3VertexBuffer*>(_buffer);
		MYGUI_PLATFORM_ASSERT(buffer->getBufferID(), "Vertex buffer is not created");
		const auto* texture = static_cast<OpenGL3Texture*>(_texture);
		const auto* program =
			texture && !texture->mShaderName.empty() ? getShaderProgram(texture->mShaderName) : nullptr;
		if (!program)
			program = getShaderProgram("Default");
		MYGUI_PLATFORM_ASSERT(program, "Default shader is not registered");
		glUseProgram(program->id);
		glUniform1f(program->yScale, _yScale);
		glBindTexture(GL_TEXTURE_2D, texture ? texture->getTextureId() : 0);
		glBindVertexArray(buffer->getBufferID());
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_count));
		// Subsequent geometry updates may replace this VAO. The pass restores host state.
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
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

		glUseProgram(getShaderProgramId("Default"));
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
		auto program = createShaderProgram(_vertexProgramFile, _fragmentProgramFile);
		mRegisteredShaders[_shaderName] = std::move(program);
	}

	bool OpenGL3RenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	const OpenGL3RenderManager::ShaderProgram* OpenGL3RenderManager::getShaderProgram(const std::string& _name) const
	{
		const auto iter = mRegisteredShaders.find(_name);
		if (iter != mRegisteredShaders.end())
			return iter->second.get();
		MYGUI_PLATFORM_LOG(Error, "Shader '" << _name << "' is not registered");
		return nullptr;
	}

	unsigned int OpenGL3RenderManager::getShaderProgramId(const std::string& _shaderName) const
	{
		const auto* program = getShaderProgram(_shaderName);
		return program ? program->id : 0;
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

		mRegisteredShaders.clear();
	}

} // namespace MyGUI
