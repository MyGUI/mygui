#ifndef MYGUI_OPENGLES_VERTEX_BUFFER_H__
#define MYGUI_OPENGLES_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>

namespace MyGUI
{

	class OpenGLESVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLESVertexBuffer();
		~OpenGLESVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() override;

		Vertex* lock() override;
		void unlock() override;

	/*internal:*/
		unsigned int getBufferID() const
		{
			return mBufferID;
		}
	private:
		void create();
		void destroy();
		void resize();

	private:
		unsigned int mBufferID;
		size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;
	};

} // namespace MyGUI

#endif // MYGUI_OPENGLES_VERTEX_BUFFER_H__
