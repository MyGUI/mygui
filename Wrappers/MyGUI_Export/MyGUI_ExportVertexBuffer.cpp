/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_ExportDiagnostic.h"

namespace MyGUI
{

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
		delete mVertex;
		mVertex = nullptr;
	}

	void ExportVertexBuffer::setVertexCount(size_t _count)
	{
		mNeedVertexCount = _count;

		if (mVertex != nullptr)
		{
			delete mVertex;
			mVertex = nullptr;
		}

		mVertex = new Vertex[mNeedVertexCount];
	}

	size_t ExportVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* ExportVertexBuffer::lock()
	{
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

}
