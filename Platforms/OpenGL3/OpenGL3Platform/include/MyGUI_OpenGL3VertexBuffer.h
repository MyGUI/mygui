/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef MYGUI_OPENGL3_VERTEX_BUFFER_H_
#define MYGUI_OPENGL3_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

namespace MyGUI
{

	class OpenGL3VertexBuffer : public IVertexBuffer
	{
	public:
		~OpenGL3VertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		/*internal:*/
		unsigned int getBufferID() const
		{
			return mVAOID;
		}

	private:
		void destroy();
		void create();
		void resize();

	private:
		unsigned int mVAOID{0};
		unsigned int mBufferID{0};
		size_t mVertexCount{0};
		size_t mNeedVertexCount{0};
		size_t mSizeInBytes{0};
	};

} // namespace MyGUI

#endif // MYGUI_OPENGL3_VERTEX_BUFFER_H_
