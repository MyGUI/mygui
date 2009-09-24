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
#ifndef __MYGUI_DIRECTX_VERTEX_BUFFER_H__
#define __MYGUI_DIRECTX_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_DirectXRenderManager.h"

#include <d3d9.h>

namespace MyGUI
{

	class DirectXVertexBuffer : public IVertexBuffer
	{
	public:
		DirectXVertexBuffer(IDirect3DDevice9 *_device, DirectXRenderManager *_pRenderManager);
		virtual ~DirectXVertexBuffer();

		virtual void setVertextCount(size_t _count);
		virtual size_t getVertextCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		virtual bool setToStream(size_t stream);

	private:
		bool create();
		void destroy();
		void resize();

	private:
		IDirect3DDevice9 *mpD3DDevice;
		IDirect3DVertexBuffer9 *mpBuffer;
	    DirectXRenderManager *pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX_VERTEX_BUFFER_H__
