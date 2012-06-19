/*!
	@file
	@author		Albert Semenov
	@date		06/2012
*/

#ifndef __MYGUI_DUMMY_VERTEX_BUFFER_H__
#define __MYGUI_DUMMY_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_DummyRenderManager.h"

//struct IDirect3DDevice9;
//struct IDirect3DVertexBuffer9;

namespace MyGUI
{

	class DummyVertexBuffer :
		public IVertexBuffer
	{
	public:
		DummyVertexBuffer(/*IDirect3DDevice9* _device, */DummyRenderManager* _pRenderManager);
		virtual ~DummyVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		//virtual bool setToStream(size_t stream);

	//private:
		//bool create();
		//void destroy();
		//void resize();

	private:
		//IDirect3DDevice9* mpD3DDevice;
		//IDirect3DVertexBuffer9* mpBuffer;
		DummyRenderManager* pRenderManager;

		size_t mVertexCount;
		size_t mNeedVertexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_DUMMY_VERTEX_BUFFER_H__
