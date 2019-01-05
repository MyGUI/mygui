/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include <d3dx9.h>
#include "MyGUI_DirectXVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_DirectXDiagnostic.h"

namespace MyGUI
{

	const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	DirectXVertexBuffer::DirectXVertexBuffer(IDirect3DDevice9* _device, DirectXRenderManager* _pRenderManager) :
		mNeedVertexCount(0),
		mVertexCount(0),
		mpD3DDevice(_device),
		pRenderManager(_pRenderManager),
		mpBuffer(nullptr)
	{
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{
		destroy();
	}

	void DirectXVertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t DirectXVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* DirectXVertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		void* lockPtr = nullptr;
		HRESULT result = mpBuffer->Lock(0, 0, (void**)&lockPtr, 0);
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to lock vertex buffer (error code " << result << ").");
		}
		return reinterpret_cast<Vertex*>(lockPtr);
	}

	void DirectXVertexBuffer::unlock()
	{
		HRESULT result = mpBuffer->Unlock();
		if (FAILED(result))
		{
			MYGUI_PLATFORM_EXCEPT("Failed to unlock vertex buffer (error code " << result << ").");
		}
	}

	bool DirectXVertexBuffer::setToStream(size_t stream)
	{
		if (SUCCEEDED(mpD3DDevice->SetStreamSource(stream, mpBuffer, 0, sizeof(MyGUI::Vertex))))
			return true;
		return false;
	}

	void DirectXVertexBuffer::create()
	{
		DWORD length = mVertexCount * sizeof(Vertex);
		bool created = SUCCEEDED(mpD3DDevice->CreateVertexBuffer(length, 0, 0, D3DPOOL_MANAGED, &mpBuffer, nullptr));
		MYGUI_PLATFORM_ASSERT(created, "Create Buffer failed!");
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
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		destroy();
		create();
	}

} // namespace MyGUI
