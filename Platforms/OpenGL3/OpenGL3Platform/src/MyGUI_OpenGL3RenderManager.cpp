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

#include "GL/glew.h"

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

	OpenGL3RenderManager::OpenGL3RenderManager() :
		mUpdate(false),
		mImageLoader(nullptr),
		mPboIsSupported(false),
		mIsInitialise(false)
	{
	}


  GLuint buildShader(const char* text, GLenum type)
  {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &text, 0);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
      GLint len = 0;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

      GLchar* buffer = new GLchar[len];
      glGetShaderInfoLog(id, len, NULL, buffer);
      std::string infoLog = buffer;
      delete[] buffer;

      MYGUI_PLATFORM_EXCEPT(infoLog);
    }

    return id;
  }

  GLuint OpenGL3RenderManager::createShaderProgram(void)
  {
    const char vertexShader[] =
      "#version 130\n" // GLSL 1.30 = OpenGL 3.0
      "out vec4 Color;\n"
      "out vec2 TexCoord;\n"
      "in vec3 VertexPosition;\n"
      "in vec4 VertexColor;\n"
      "in vec2 VertexTexCoord;\n"
      "uniform float YScale;\n"
      "void main()\n"
      "{\n"
      "  TexCoord = VertexTexCoord;\n"
      "  Color = VertexColor;\n"
      "  vec4 vpos = vec4(VertexPosition,1.0);\n"
      "  vpos.y *= YScale;\n"
      "  gl_Position = vpos;\n"
      "}\n"
      ;

    const char fragmentShader[] =
      "#version 130\n"
      "in vec4 Color; \n"
      "in vec2 TexCoord;\n"
      "out vec4 FragColor;\n"
      "uniform sampler2D Texture;\n"
      "void main(void)\n"
      "{\n"
      "  FragColor = texture2D(Texture, TexCoord) * Color;\n"
      "}\n"
      ;

    GLuint vsID = buildShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fsID = buildShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLuint progID = glCreateProgram();
    glAttachShader(progID, vsID);
    glAttachShader(progID, fsID);

    // setup vertex attribute positions for vertex buffer
    glBindAttribLocation(progID, 0, "VertexPosition");
    glBindAttribLocation(progID, 1, "VertexColor");
    glBindAttribLocation(progID, 2, "VertexTexCoord");
    glBindFragDataLocation(progID, 0, "FragColor");

    glLinkProgram(progID);

    GLint success;
    glGetProgramiv(progID, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
      GLint len = 0;
      glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &len);

      GLchar* buffer = new GLchar[len];
      glGetProgramInfoLog(progID, len, NULL, buffer);
      std::string infoLog = buffer;
      delete[] buffer;

      MYGUI_PLATFORM_EXCEPT(infoLog);
    }
    glDeleteShader(vsID); // flag for deletion on call to glDeleteProgram
    glDeleteShader(fsID);

    int textureUniLoc = glGetUniformLocation(progID, "Texture");
    if (textureUniLoc == -1) {
      MYGUI_PLATFORM_EXCEPT("Unable to retrieve uniform variable location");
    }
    mYScaleUniformLocation = glGetUniformLocation(progID, "YScale");
    if (mYScaleUniformLocation == -1) {
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

		glewInit();

    if (!(GLEW_VERSION_3_0)) {
      const char *version = (const char *) glGetString(GL_VERSION);
      MYGUI_PLATFORM_EXCEPT(std::string("OpenGL 3.0 or newer not available, current version is ") + version);
    }

		mPboIsSupported = glewIsExtensionSupported("GL_EXT_pixel_buffer_object") != 0;

    mProgramID = createShaderProgram();

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

  void OpenGL3RenderManager::doRenderRTT(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
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
			texture_id = texture->getTextureID();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(buffer_id);
    glDrawArrays(GL_TRIANGLES, 0, _count);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGL3RenderManager::begin()
	{
    ++mReferenceCount;

    glUseProgram(mProgramID);
    glActiveTexture(GL_TEXTURE0);
  
    glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

	void OpenGL3RenderManager::end()
	{
    if (--mReferenceCount == 0) {
      glDisable(GL_BLEND);
      glUseProgram(0);
    }
  }

	const RenderTargetInfo& OpenGL3RenderManager::getInfo()
	{
		return mInfo;
	}

	const IntSize& OpenGL3RenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OpenGL3RenderManager::getVertexFormat()
	{
		return mVertexFormat;
	}

	bool OpenGL3RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
    if (_format == PixelFormat::R8G8B8 ||
        _format == PixelFormat::R8G8B8A8) 
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

		onFrameEvent((float)((double)(time) / (double)1000));

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

		onResizeView(mViewSize);
		mUpdate = true;
	}

	bool OpenGL3RenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
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

    if (mProgramID) {
      glDeleteProgram(mProgramID);
      mProgramID = 0;
    }
  }

} // namespace MyGUI
