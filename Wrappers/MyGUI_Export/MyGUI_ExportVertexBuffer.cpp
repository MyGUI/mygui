/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportVertexBuffer.h"
#include "MyGUI_ExportDiagnostic.h"

namespace MyGUI
{

	const size_t VERTEX_BUFFER_REALLOCK_STEP = 5 * VertexQuad::VertexCount;

	ExportVertexBuffer::ExportVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(0),
		mSizeInBytes(0),
		mId(0),
		mVertex(nullptr),
		mChange(false)
	{
		mId = (size_t)this;
	}

	ExportVertexBuffer::~ExportVertexBuffer()
	{
		destroy();
	}

	void ExportVertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t ExportVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* ExportVertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount || mVertexCount == 0)
			resize();

		return mVertex;
	}

	void ExportVertexBuffer::unlock()
	{
		mChange = true;
	}

	size_t ExportVertexBuffer::getId()
	{
		return mId;
	}

	void ExportVertexBuffer::setChange(bool _value)
	{
		mChange = _value;
	}

	bool ExportVertexBuffer::getChange() const
	{
		return mChange;
	}

	void ExportVertexBuffer::create()
	{
		mVertex = new Vertex[mVertexCount];
	}

	void ExportVertexBuffer::destroy()
	{
		delete mVertex;
		mVertex = nullptr;
	}

	void ExportVertexBuffer::resize()
	{
		mVertexCount = mNeedVertexCount + VERTEX_BUFFER_REALLOCK_STEP;
		destroy();
		create();
	}

}
