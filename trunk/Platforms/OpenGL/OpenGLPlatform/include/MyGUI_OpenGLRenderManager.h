/*!
	@file
	@author		George Evmenov
	@date		07/2009
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
#ifndef __MYGUI_OPENGL_RENDER_MANAGER_H__
#define __MYGUI_OPENGL_RENDER_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_IRenderQueueListener.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include <d3d9.h>

namespace MyGUI
{

	class OpenGLRenderManager :
		public RenderManager
	{
		MYGUI_INSTANCE_HEADER(OpenGLRenderManager);

	public:
		void initialise(IDirect3DDevice9 *_device);
		void shutdown();

		virtual const IntSize& getViewSize() { return mViewSize; }

		virtual VertexColourType getVertexFormat() { return mVertexFormat; }

		virtual IVertexBuffer* createVertexBuffer();
		virtual void destroyVertexBuffer(IVertexBuffer* _buffer);

		void setRenderQueueListener(IRenderQueueListener* _listener);

		virtual void begin();
		virtual void end() { }

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);
		virtual void doRender(IVertexBuffer* _buffer, const std::string& _texture, size_t _count);

	    virtual const RenderTargetInfo& getInfo() { return mInfo; }

		virtual void render();

	private:
	    IDirect3DDevice9 *mpD3DDevice;
		IntSize mViewSize;
		VertexColourType mVertexFormat;
		RenderTargetInfo mInfo;
		IRenderQueueListener* mListener;
		bool mUpdate;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_RENDER_MANAGER_H__
