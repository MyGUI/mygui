/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#ifndef __MYGUI_DIRECTX11_VERTEX_BUFFER_H__
#define __MYGUI_DIRECTX11_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_DirectX11RenderManager.h"

struct ID3D11Buffer;

namespace MyGUI
{

	class DirectX11VertexBuffer : public IVertexBuffer
	{
	public:
		DirectX11VertexBuffer(DirectX11RenderManager* _pRenderManager);
		virtual ~DirectX11VertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	private:
		bool create();
		void destroy();
		void resize();

	private:
		DirectX11RenderManager* mManager;
		size_t                  mVertexCount;
		size_t                  mNeedVertexCount;

	public:
		ID3D11Buffer*           mBuffer;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX11_VERTEX_BUFFER_H__
