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
#include "MyGUI_Precompiled.h"
#include "MyGUI_DirectXVertexBuffer.h"
#include "MyGUI_VertexData.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	DirectXVertexBuffer::DirectXVertexBuffer(IDirect3DDevice9 *_device, DirectXRenderManager *_pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mpD3DDevice(_device),
		pRenderManager(_pRenderManager),
		mpBuffer(NULL)
	{
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{
		destroy();
	}

	void DirectXVertexBuffer::setVertextCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			resize();
		}
	}

	size_t DirectXVertexBuffer::getVertextCount()
	{
		return mNeedVertexCount;
	}

	Vertex* DirectXVertexBuffer::lock()
	{
		//assert(!mpBuffer && __FUNCTION__);
		void *lockPtr = nullptr;
		if (SUCCEEDED(mpBuffer->Lock(0, 0, (void **)&lockPtr, 0)))
		{
			return (Vertex*)lockPtr;
		}
		return nullptr;
	}

	void DirectXVertexBuffer::unlock()
	{
		//assert(!mpBuffer && __FUNCTION__);
		if (FAILED(mpBuffer->Unlock()))
		{
			//exception
		}
	}

	bool DirectXVertexBuffer::setToStream(size_t stream)
	{
		//assert(!mpBuffer && __FUNCTION__);
		if (SUCCEEDED(mpD3DDevice->SetStreamSource(stream, mpBuffer, 0, sizeof(MyGUI::Vertex))))
			return true;
		return false;
	}

	bool DirectXVertexBuffer::create()
	{
		DWORD length = mNeedVertexCount * sizeof(MyGUI::Vertex);
		if (SUCCEEDED(mpD3DDevice->CreateVertexBuffer(length, 0, 0, D3DPOOL_MANAGED, &mpBuffer, nullptr)))
			return false;
		return false;
	}

	void DirectXVertexBuffer::destroy()
	{
		if (mpBuffer)
		{
			mpBuffer->Release();
			mpBuffer = nullptr;
		}
	}

	void DirectXVertexBuffer::resize()
	{
		if (mpD3DDevice)
		{
			destroy();
			create();
		}
	}

} // namespace MyGUI
