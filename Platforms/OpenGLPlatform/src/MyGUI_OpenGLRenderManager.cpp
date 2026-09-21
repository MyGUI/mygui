/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLTexture.h"
#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

#include <MyGUI_GL.h>
#include <cstdio>
#include <cstring>

namespace MyGUI
{

	OpenGLRenderManager& OpenGLRenderManager::getInstance()
	{
		return *getInstancePtr();
	}

	OpenGLRenderManager* OpenGLRenderManager::getInstancePtr()
	{
		return static_cast<OpenGLRenderManager*>(RenderManager::getInstancePtr());
	}

	void OpenGLRenderManager::initialise(OpenGLImageLoader* _loader)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
		const auto hasExtension = [extensions](const char* name)
		{
			if (!extensions)
				return false;
			const size_t length = std::strlen(name);
			const char* position = extensions;
			while ((position = std::strstr(position, name)) != nullptr)
			{
				if ((position == extensions || position[-1] == ' ') &&
					(position[length] == ' ' || position[length] == '\0'))
					return true;
				position += length;
			}
			return false;
		};
		int major = 0, minor = 0;
		const auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		if (version)
			std::sscanf(version, "%d.%d", &major, &minor);
		mPboIsSupported = major > 2 || (major == 2 && minor >= 1) || hasExtension("GL_ARB_pixel_buffer_object") ||
			hasExtension("GL_EXT_pixel_buffer_object");
		mSeparateFramebufferSupported =
			major >= 3 || hasExtension("GL_ARB_framebuffer_object") || hasExtension("GL_EXT_framebuffer_blit");
		mFramebufferSupported = mSeparateFramebufferSupported || hasExtension("GL_EXT_framebuffer_object");
		mRasterizerDiscardSupported = major >= 3 || hasExtension("GL_EXT_transform_feedback");
		mRectangleTextureSupported = major >= 3 || hasExtension("GL_ARB_texture_rectangle") ||
			hasExtension("GL_EXT_texture_rectangle") || hasExtension("GL_NV_texture_rectangle");
		mVertexProgramSupported = hasExtension("GL_ARB_vertex_program");
		mFragmentProgramSupported = hasExtension("GL_ARB_fragment_program");
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &mTextureUnits);
		glGetIntegerv(GL_MAX_CLIP_PLANES, &mClipPlanes);

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLRenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* OpenGLRenderManager::createVertexBuffer()
	{
		return new OpenGLVertexBuffer();
	}

	void OpenGLRenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void OpenGLRenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		const auto* buffer = static_cast<OpenGLVertexBuffer*>(_buffer);
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");

		unsigned int texture_id = 0;
		if (_texture)
		{
			const auto* texture = static_cast<OpenGLTexture*>(_texture);
			texture_id = texture->getTextureId();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);

		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

		// enable vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// before draw, specify vertex and index arrays with their offsets
		size_t offset = 0;
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offset);
		offset += (sizeof(float) * 3);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offset);
		offset += (4);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offset);

		glDrawArrays(GL_TRIANGLES, 0, _count);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLRenderManager::begin()
	{
		RenderState state;
		glGetIntegerv(GL_CURRENT_PROGRAM, &state.program);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &state.arrayBuffer);
		mStates.push_back(state);
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glUseProgram(0);
		for (auto array :
			 {GL_NORMAL_ARRAY, GL_INDEX_ARRAY, GL_EDGE_FLAG_ARRAY, GL_SECONDARY_COLOR_ARRAY, GL_FOG_COORDINATE_ARRAY})
			glDisableClientState(array);
		if (mVertexProgramSupported)
			glDisable(GL_VERTEX_PROGRAM_ARB);
		if (mFragmentProgramSupported)
			glDisable(GL_FRAGMENT_PROGRAM_ARB);
		if (mRasterizerDiscardSupported)
			glDisable(GL_RASTERIZER_DISCARD_EXT);
		for (int unit = 0; unit < mTextureUnits; ++unit)
		{
			glActiveTexture(GL_TEXTURE0 + unit);
			glClientActiveTexture(GL_TEXTURE0 + unit);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			if (mRectangleTextureSupported)
				glDisable(GL_TEXTURE_RECTANGLE_ARB);
			for (auto mode :
				 {GL_TEXTURE_1D,
				  GL_TEXTURE_2D,
				  GL_TEXTURE_3D,
				  GL_TEXTURE_CUBE_MAP,
				  GL_TEXTURE_GEN_S,
				  GL_TEXTURE_GEN_T,
				  GL_TEXTURE_GEN_R,
				  GL_TEXTURE_GEN_Q})
				glDisable(mode);
		}
		glActiveTexture(GL_TEXTURE0);
		glClientActiveTexture(GL_TEXTURE0);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-1, 1, -1, 1, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		for (auto mode :
			 {GL_LIGHTING,
			  GL_DEPTH_TEST,
			  GL_FOG,
			  GL_CULL_FACE,
			  GL_SCISSOR_TEST,
			  GL_STENCIL_TEST,
			  GL_ALPHA_TEST,
			  GL_COLOR_LOGIC_OP,
			  GL_COLOR_SUM,
			  GL_SAMPLE_ALPHA_TO_COVERAGE,
			  GL_SAMPLE_ALPHA_TO_ONE,
			  GL_SAMPLE_COVERAGE,
			  GL_POLYGON_STIPPLE,
			  GL_POLYGON_SMOOTH,
			  GL_POLYGON_OFFSET_FILL,
			  GL_POLYGON_OFFSET_LINE,
			  GL_POLYGON_OFFSET_POINT})
			glDisable(mode);
		for (int plane = 0; plane < mClipPlanes; ++plane)
			glDisable(GL_CLIP_PLANE0 + plane);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderManager::end()
	{
		MYGUI_PLATFORM_ASSERT(!mStates.empty(), "Unbalanced render pass");
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glActiveTexture(GL_TEXTURE0);
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glPopAttrib();
		glPopClientAttrib();
		const auto state = mStates.back();
		mStates.pop_back();
		glUseProgram(state.program);
		glBindBuffer(GL_ARRAY_BUFFER, state.arrayBuffer);
	}

	const RenderTargetInfo& OpenGLRenderManager::getInfo() const
	{
		return mInfo;
	}

	const IntSize& OpenGLRenderManager::getViewSize() const
	{
		return mViewSize;
	}

	VertexColourType OpenGLRenderManager::getVertexFormat() const
	{
		return mVertexFormat;
	}

	bool OpenGLRenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		// Luminance formats are sampleable, but not guaranteed colour-renderable.
		if (_usage.isValue(TextureUsage::RenderTarget))
			return mFramebufferSupported && (_format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8);
		return _format == PixelFormat::R8G8B8 || _format == PixelFormat::R8G8B8A8 || _format == PixelFormat::L8 ||
			_format == PixelFormat::L8A8;
	}

	void OpenGLRenderManager::drawOneFrame()
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

	void OpenGLRenderManager::setViewSize(int _width, int _height)
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

	void OpenGLRenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		MYGUI_PLATFORM_LOG(Warning, "OpenGLRenderManager::registerShader is not implemented");
	}

	bool OpenGLRenderManager::isPixelBufferObjectSupported() const
	{
		return mPboIsSupported;
	}

	bool OpenGLRenderManager::isSeparateFramebufferSupported() const
	{
		return mSeparateFramebufferSupported;
	}

	ITexture* OpenGLRenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OpenGLTexture* texture = new OpenGLTexture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
	}

	void OpenGLRenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* OpenGLRenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	void OpenGLRenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

} // namespace MyGUI
