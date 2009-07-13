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
#ifndef __MYGUI_OPENGL_VERTEX_BUFFER_H__
#define __MYGUI_OPENGL_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_OpenGLRenderManager.h"

#include <d3d9.h>

namespace MyGUI
{

	class OpenGLVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLVertexBuffer(IDirect3DDevice9 *_device, OpenGLRenderManager *_pRenderManager);
		virtual ~OpenGLVertexBuffer();

		virtual void setVertextCount(size_t _count);
		virtual size_t getVertextCount();

		virtual void* lock();
		virtual void unlock();
		virtual bool setToStream(size_t stream);

	private:
		bool create();
		void destroy();
		void resize();

	private:
		IDirect3DDevice9 *mpD3DDevice;
		IDirect3DVertexBuffer9 *mpBuffer;
	    OpenGLRenderManager *pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_VERTEX_BUFFER_H__
