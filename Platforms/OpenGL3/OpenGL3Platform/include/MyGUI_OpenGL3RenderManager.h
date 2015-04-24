/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_RENDER_MANAGER_H_
#define MYGUI_OPENGL3_RENDER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_OpenGL3ImageLoader.h"

namespace MyGUI
{

	class OpenGL3RenderManager :
		public RenderManager,
		public IRenderTarget
	{
	public:
		OpenGL3RenderManager();

		void initialise(OpenGL3ImageLoader* _loader = 0);
		void shutdown();

		static OpenGL3RenderManager& getInstance();
		static OpenGL3RenderManager* getInstancePtr();

		/** @see OpenGL3RenderManager::getViewSize */
		virtual const IntSize& getViewSize() const;

		/** @see OpenGL3RenderManager::getVertexFormat */
		virtual VertexColourType getVertexFormat();

		/** @see OpenGL3RenderManager::isFormatSupported */
		virtual bool isFormatSupported(PixelFormat _format, TextureUsage _usage);

		/** @see OpenGL3RenderManager::createVertexBuffer */
		virtual IVertexBuffer* createVertexBuffer();
		/** @see OpenGL3RenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		/** @see OpenGL3RenderManager::createTexture */
		virtual ITexture* createTexture(const std::string& _name);
		/** @see OpenGL3RenderManager::destroyTexture */
		virtual void destroyTexture(ITexture* _texture);
		/** @see OpenGL3RenderManager::getTexture */
		virtual ITexture* getTexture(const std::string& _name);


		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();
		/** @see IRenderTarget::doRender */
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
		/** @see IRenderTarget::getInfo */
		virtual const RenderTargetInfo& getInfo();

    /* for use with RTT, flips Y coordinate when rendering */
    void doRenderRTT(IVertexBuffer* _buffer, ITexture* _texture, size_t _count); 

	/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);
		bool isPixelBufferObjectSupported() const;
    unsigned int createShaderProgram(void);

	private:
		void destroyAllResources();

	private:
		IntSize mViewSize;
		bool mUpdate;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
    unsigned int mProgramID;
    unsigned int mReferenceCount; // for nested rendering
    int mYScaleUniformLocation;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
		OpenGL3ImageLoader* mImageLoader;
		bool mPboIsSupported;
        
		bool mIsInitialise;
  };

} // namespace MyGUI

#endif // MYGUI_OPENGL3_RENDER_MANAGER_H_
