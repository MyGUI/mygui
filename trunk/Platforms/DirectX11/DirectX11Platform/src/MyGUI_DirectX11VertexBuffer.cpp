/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#include <d3dx11.h>
#include "MyGUI_DirectX11VertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_DirectX11Diagnostic.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	DirectX11VertexBuffer::DirectX11VertexBuffer(DirectX11RenderManager* _pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mBuffer(nullptr),
		mManager(_pRenderManager)
	{
	}

	DirectX11VertexBuffer::~DirectX11VertexBuffer()
	{
		destroy();
	}

	void DirectX11VertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount && _count != 0)
		{
			mNeedVertexCount = _count;
			resize();
		}
	}

	size_t DirectX11VertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* DirectX11VertexBuffer::lock()
	{
		if (!mBuffer) create();
		D3D11_MAPPED_SUBRESOURCE map;
		memset(&map, 0, sizeof(map));
		mManager->mpD3DContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		return static_cast<Vertex*>(map.pData);
	}

	void DirectX11VertexBuffer::unlock()
	{
		if (mBuffer) mManager->mpD3DContext->Unmap(mBuffer, 0);
	}

	bool DirectX11VertexBuffer::create()
	{
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.ByteWidth = sizeof(Vertex) * (mNeedVertexCount);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HRESULT hr = mManager->mpD3DDevice->CreateBuffer(&desc, 0, &mBuffer);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Create Buffer failed!");
		return hr == S_OK ? true : false;
	}

	void DirectX11VertexBuffer::destroy()
	{
		if (mBuffer)
		{
			mBuffer->Release();
			mBuffer = 0;
		}
	}

	void DirectX11VertexBuffer::resize()
	{
		destroy();
		create();
	}

} // namespace MyGUI
