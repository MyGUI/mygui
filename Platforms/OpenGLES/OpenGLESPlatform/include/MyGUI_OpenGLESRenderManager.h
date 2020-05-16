#ifndef MYGUI_OPENGLES_RENDER_MANAGER_H__
#define MYGUI_OPENGLES_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_OpenGLESImageLoader.h"

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

namespace MyGUI
{

	class OpenGLESRenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		OpenGLESRenderManager();

		void initialise(OpenGLESImageLoader* _loader = nullptr);
		void shutdown();

		static OpenGLESRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static OpenGLESRenderManager* getInstancePtr()
		{
			return static_cast<OpenGLESRenderManager*>(RenderManager::getInstancePtr());
		}

		/** @see RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const;

		/** @see RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat();

		/** @see RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);

		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();
		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo();

		/** @see RenderManager::setViewSize */
		void setViewSize(int _width, int _height) override;

	/*internal:*/
		void drawOneFrame();
		bool isPixelBufferObjectSupported() const;

	private:
		void destroyAllResources();
		GLuint BuildShader(const char* source, GLenum shaderType) const;
		GLuint BuildProgram(const char* vertexShaderSource, const char* fragmentShaderSource) const;

	private:
		IntSize mViewSize;
		bool mUpdate;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
		OpenGLESImageLoader* mImageLoader;
		bool mPboIsSupported;

		bool mIsInitialise;

		GLuint _positionSlot;
		GLuint _colorSlot;
		GLuint _texSlot;

		GLuint mProgram;
		GLuint mVertShader;
		GLuint mFragShader;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGLES_RENDER_MANAGER_H__
