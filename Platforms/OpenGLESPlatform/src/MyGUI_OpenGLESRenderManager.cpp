#include "MyGUI_OpenGLESRenderManager.h"
#include "MyGUI_OpenGLESTexture.h"
#include "MyGUI_OpenGLESVertexBuffer.h"
#include "MyGUI_OpenGLESDiagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataStreamHolder.h"
#include "MyGUI_DataManager.h"

#include <GLES3/gl3.h>

namespace MyGUI
{
	namespace
	{

		constexpr std::array<GLenum, 8> guiStateModes = {
			GL_BLEND,
			GL_CULL_FACE,
			GL_DEPTH_TEST,
			GL_STENCIL_TEST,
			GL_SCISSOR_TEST,
			GL_RASTERIZER_DISCARD,
			GL_SAMPLE_ALPHA_TO_COVERAGE,
			GL_SAMPLE_COVERAGE};
		constexpr std::array<GLenum, 6> blendParameters = {
			GL_BLEND_SRC_RGB,
			GL_BLEND_DST_RGB,
			GL_BLEND_SRC_ALPHA,
			GL_BLEND_DST_ALPHA,
			GL_BLEND_EQUATION_RGB,
			GL_BLEND_EQUATION_ALPHA};

	}


	OpenGLESRenderManager& OpenGLESRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OpenGLESRenderManager* OpenGLESRenderManager::getInstancePtr()
	{
		return static_cast<OpenGLESRenderManager*>(RenderManager::getInstancePtr());
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

	OpenGLESRenderManager::ShaderProgram::~ShaderProgram()
	{
		if (id)
			glDeleteProgram(id);
	}

	std::string OpenGLESRenderManager::loadFileContent(const std::string& _file)
	{
		auto source = DataManager::getInstance().getDataHolder(_file);
		MYGUI_PLATFORM_ASSERT(source, "Failed to load shader '" << _file << "'.");
		return source->readAllText();
	}

	std::unique_ptr<OpenGLESRenderManager::ShaderProgram> OpenGLESRenderManager::createShaderProgram(
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

	void OpenGLESRenderManager::initialise(OpenGLESImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		mStates.clear();
		GLint majorVersion = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		MYGUI_PLATFORM_ASSERT(majorVersion >= 3, "OpenGL ES 3 is required");
		mPboIsSupported = true;

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
		render(_buffer, _texture, _count, -1.0f);
	}

	void OpenGLESRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		render(_buffer, _texture, _count, 1.0f);
	}

	void OpenGLESRenderManager::render(IVertexBuffer* _buffer, ITexture* _texture, size_t _count, float _yScale)
	{
		const auto* buffer = static_cast<OpenGLESVertexBuffer*>(_buffer);
		MYGUI_PLATFORM_ASSERT(buffer->getBufferID(), "Vertex buffer is not created");
		const auto* texture = static_cast<OpenGLESTexture*>(_texture);
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

	void OpenGLESRenderManager::begin()
	{
		SavedState state;
		for (size_t i = 0; i < guiStateModes.size(); ++i)
			state.enabled[i] = glIsEnabled(guiStateModes[i]) == GL_TRUE;
		for (size_t i = 0; i < blendParameters.size(); ++i)
			glGetIntegerv(blendParameters[i], &state.blend[i]);
		glGetBooleanv(GL_COLOR_WRITEMASK, state.colourMask.data());
		glGetBooleanv(GL_DEPTH_WRITEMASK, &state.depthMask);
		glGetIntegerv(GL_CURRENT_PROGRAM, &state.program);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &state.vertexArray);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &state.arrayBuffer);
		glGetIntegerv(GL_ACTIVE_TEXTURE, &state.activeTexture);
		glActiveTexture(GL_TEXTURE0);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &state.texture);
		glGetIntegerv(GL_SAMPLER_BINDING, &state.sampler);
		mStates.push_back(state);
		glUseProgram(getShaderProgramId("Default"));
		glBindSampler(0, 0);
		for (size_t i = 1; i < guiStateModes.size(); ++i)
			glDisable(guiStateModes[i]);
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLESRenderManager::end()
	{
		MYGUI_PLATFORM_ASSERT(!mStates.empty(), "Unmatched render target end");
		const auto state = mStates.back();
		mStates.pop_back();
		for (size_t i = 0; i < guiStateModes.size(); ++i)
		{
			if (state.enabled[i])
				glEnable(guiStateModes[i]);
			else
				glDisable(guiStateModes[i]);
		}
		glDepthMask(state.depthMask);
		glColorMask(state.colourMask[0], state.colourMask[1], state.colourMask[2], state.colourMask[3]);
		glBlendFuncSeparate(state.blend[0], state.blend[1], state.blend[2], state.blend[3]);
		glBlendEquationSeparate(state.blend[4], state.blend[5]);
		glUseProgram(state.program);
		glBindVertexArray(state.vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, state.arrayBuffer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, state.texture);
		glBindSampler(0, state.sampler);
		glActiveTexture(state.activeTexture);
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
		return (_format == PixelFormat::L8 || _format == PixelFormat::L8A8) &&
			!_usage.isValue(TextureUsage::RenderTarget);
	}

	void OpenGLESRenderManager::drawOneFrame()
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

		onResizeView(mViewSize);
		mUpdate = true;
	}

	void OpenGLESRenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		auto program = createShaderProgram(_vertexProgramFile, _fragmentProgramFile);
		mRegisteredShaders[_shaderName] = std::move(program);
	}

	bool OpenGLESRenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	const OpenGLESRenderManager::ShaderProgram* OpenGLESRenderManager::getShaderProgram(const std::string& _name) const
	{
		const auto iter = mRegisteredShaders.find(_name);
		if (iter != mRegisteredShaders.end())
			return iter->second.get();
		MYGUI_PLATFORM_LOG(Error, "Shader '" << _name << "' is not registered");
		return nullptr;
	}

	unsigned int OpenGLESRenderManager::getShaderProgramId(const std::string& _shaderName) const
	{
		const auto* program = getShaderProgram(_shaderName);
		return program ? program->id : 0;
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

		mRegisteredShaders.clear();
	}

} // namespace MyGUI
