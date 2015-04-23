/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_VERTEX_BUFFER_H_
#define MYGUI_OPENGL3_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
//#include "MyGUI_OpenGL3RenderManager.h"

namespace MyGUI
{

	class OpenGL3VertexBuffer : public IVertexBuffer
	{
	public:
		OpenGL3VertexBuffer();
		virtual ~OpenGL3VertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	/*internal:*/
		void destroy();
		void create();

		unsigned int getBufferID() const
		{
			return mVAOID;
		}

	private:
    unsigned int mVAOID;
    unsigned int mBufferID;
        //size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_VERTEX_BUFFER_H_
