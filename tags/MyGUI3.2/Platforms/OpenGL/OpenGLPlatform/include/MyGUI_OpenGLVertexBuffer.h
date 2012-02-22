/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef __MYGUI_OPENGL_VERTEX_BUFFER_H__
#define __MYGUI_OPENGL_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
//#include "MyGUI_OpenGLRenderManager.h"

namespace MyGUI
{

	class OpenGLVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		void destroy();
		void create();

		unsigned int getBufferID() const
		{
			return mBufferID;
		}

	private:
		unsigned int mBufferID;
		size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_VERTEX_BUFFER_H__
