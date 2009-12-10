/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/

#ifndef __MYGUI_DIRECTX_RENDER_MANAGER_H__
#define __MYGUI_DIRECTX_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

struct IDirect3DDevice9;

namespace MyGUI
{

	class DirectXRenderManager :
		public RenderManager,
		public IRenderTarget
	{
		MYGUI_INSTANCE_HEADER(DirectXRenderManager);

	public:
		void initialise(IDirect3DDevice9 *_device);
		void shutdown();

		virtual const IntSize& getViewSize() { return mViewSize; }

		virtual VertexColourType getVertexFormat() { return mVertexFormat; }

		virtual IVertexBuffer* createVertexBuffer();
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		virtual ITexture* createTexture(const std::string& _name);
		virtual void destroyTexture(ITexture* _texture);
		virtual ITexture* getTexture(const std::string& _name);

		virtual void begin();
		virtual void end() { }

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

	    virtual const RenderTargetInfo& getInfo() { return mInfo; }

	/*internal:*/
		void drawOneFrame();
		void setViewSize(int _width, int _height);
		void deviceReset();

	private:
		void destroyAllResources();

	private:
	    IDirect3DDevice9 *mpD3DDevice;
		IntSize mViewSize;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		bool mUpdate;

		typedef std::map<std::string, ITexture*> MapTexture;
		MapTexture mTextures;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_RENDER_MANAGER_H__
