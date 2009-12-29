/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/

#ifndef __MYGUI_OPENGL_RENDER_MANAGER_H__
#define __MYGUI_OPENGL_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_OpenGLImageLoader.h"

namespace MyGUI
{

	class OpenGLRenderManager :
		public RenderManager,
		public IRenderTarget
	{
		MYGUI_INSTANCE_HEADER(OpenGLRenderManager);

	public:
		void initialise(OpenGLImageLoader* _loader = 0);
		void shutdown();

		virtual const IntSize& getViewSize();

		virtual VertexColourType getVertexFormat();

		virtual IVertexBuffer* createVertexBuffer();
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		virtual ITexture* createTexture(const std::string& _name);
		virtual void destroyTexture(ITexture* _texture);
		virtual ITexture* getTexture(const std::string& _name);


		// IRenderTarget implement
		virtual void begin();
		virtual void end();
		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
	    virtual const RenderTargetInfo& getInfo();


	/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);

	private:
		void destroyAllResources();

	private:
		IntSize mViewSize;
		bool mUpdate;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
		OpenGLImageLoader* mImageLoader;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_RENDER_MANAGER_H__
