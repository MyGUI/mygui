/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#include "MyGUI_SDL2VertexBuffer.h"
#include "MyGUI_SDL2Diagnostic.h"

namespace MyGUI
{

	const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	SDL2VertexBuffer::SDL2VertexBuffer() :
		mVertexCount(0),
		mNeedVertexCount(0),
		mSizeInBytes(0),
		mInternalData(nullptr)
	{
	}

	SDL2VertexBuffer::~SDL2VertexBuffer()
	{
		destroy();
	}

	void SDL2VertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t SDL2VertexBuffer::getVertexCount() const
	{
		return mNeedVertexCount;
	}

	Vertex* SDL2VertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		MYGUI_PLATFORM_ASSERT(mInternalData, "Vertex buffer in not created");

//		// Use glMapBuffer
//		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
//
//		// Discard the buffer
//		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, nullptr, GL_STREAM_DRAW);
//
//		Vertex* pBuffer = reinterpret_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

//		MYGUI_PLATFORM_ASSERT(pBuffer, "Error lock vertex buffer");

//		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return mInternalData;
	}

	inline uint32_t fromBGRA(uint32_t color) {
		return ((color & 0x000000FF) << 16) |
				((color & 0x00FF0000) >> 16) |
				(color & 0xFF00FF00);
	}

	void SDL2VertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mInternalData, "Vertex buffer in not created");

		for (int j = 0; j < mVertexCount; ++j) {
			auto &vertex = mVertices[j];
			auto &otherVertex = mInternalData[j];
			vertex.position = {otherVertex.x, -otherVertex.y};
			*(uint32_t*)&vertex.color = fromBGRA(otherVertex.colour);
			vertex.tex_coord = {otherVertex.u, otherVertex.v};
		}

//		MYGUI_PLATFORM_ASSERT(result, "Error unlock vertex buffer");
	}

	void SDL2VertexBuffer::create()
	{
		MYGUI_PLATFORM_ASSERT(!mInternalData, "Vertex buffer already exist");

		mSizeInBytes = mVertexCount * sizeof(Vertex);
		mInternalData = new Vertex[mVertexCount];
		mVertices.resize(mVertexCount);
	}

	void SDL2VertexBuffer::destroy()
	{
		if (mInternalData != nullptr)
		{
			delete[] mInternalData;
			mInternalData = nullptr;
		}
		mVertices.resize(0);
	}

	void SDL2VertexBuffer::resize()
	{
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		destroy();
		create();
	}

} // namespace MyGUI
