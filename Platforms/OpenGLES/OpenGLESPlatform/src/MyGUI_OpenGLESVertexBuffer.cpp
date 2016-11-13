#include "MyGUI_OpenGLESVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_OpenGLESDiagnostic.h"

#include "platform.h"
#include <assert.h>

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	OpenGLESVertexBuffer::OpenGLESVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mBufferID(0),
		mSizeInBytes(0)
	{
	}

	OpenGLESVertexBuffer::~OpenGLESVertexBuffer()
	{
		destroy();
	}

	void OpenGLESVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			destroy();
			create();
		}
	}

	size_t OpenGLESVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* OpenGLESVertexBuffer::lock()
	{
		MYGUI_PLATFORM_ASSERT(mBufferID, "Vertex buffer in not created");

		// Use glMapBuffer
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		CHECK_GL_ERROR_DEBUG();

		// Discard the buffer
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, 0, GL_STREAM_DRAW);
		CHECK_GL_ERROR_DEBUG();


		Vertex* pBuffer = (Vertex*) glMapBufferRange(
			GL_ARRAY_BUFFER,
			0,
			mSizeInBytes,
			GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
		CHECK_GL_ERROR_DEBUG();

		MYGUI_PLATFORM_ASSERT(pBuffer, "Error lock vertex buffer");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR_DEBUG();

		return pBuffer;
	}

	void OpenGLESVertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mBufferID, "Vertex buffer in not created");

		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		CHECK_GL_ERROR_DEBUG();
		GLboolean result = glUnmapBuffer(GL_ARRAY_BUFFER);
		CHECK_GL_ERROR_DEBUG();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR_DEBUG();

		MYGUI_PLATFORM_ASSERT(result, "Error unlock vertex buffer");
	}

	void OpenGLESVertexBuffer::destroy()
	{
		if (mBufferID != 0)
		{
			glDeleteBuffers(1, (GLuint * ) & mBufferID);
			CHECK_GL_ERROR_DEBUG();
			mBufferID = 0;
		}
	}

	void OpenGLESVertexBuffer::create()
	{
		MYGUI_PLATFORM_ASSERT(!mBufferID, "Vertex buffer already exist");

		mSizeInBytes = mNeedVertexCount * sizeof(MyGUI::Vertex);
		void* data = 0;

		glGenBuffers(1, (GLuint * ) & mBufferID); //wdy
		CHECK_GL_ERROR_DEBUG();
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		CHECK_GL_ERROR_DEBUG();
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, data, GL_STREAM_DRAW);
		CHECK_GL_ERROR_DEBUG();


		// check data size in VBO is same as input array, if not return 0 and delete VBO
		int bufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint * ) & bufferSize); //wdy
		CHECK_GL_ERROR_DEBUG();
		if (mSizeInBytes != (size_t) bufferSize)
		{
			destroy();
			MYGUI_PLATFORM_EXCEPT("Data size is mismatch with input array");
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR_DEBUG();
	}

} // namespace MyGUI
