/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_DIRECTX_RENDER_MANAGER_H__
#define __MYGUI_DIRECTX_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include <d3d9.h>

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
