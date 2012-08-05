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
		mSizeInBytes(0)
	{
	}

	ExportVertexBuffer::~ExportVertexBuffer()
	{
	}

	void ExportVertexBuffer::setVertexCount(size_t _count)
	{
		if (mNeedVertexCount != _count)
		{
			mNeedVertexCount = _count;
			// resize
		}
	}

	size_t ExportVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* ExportVertexBuffer::lock()
	{
		return nullptr;
	}

	void ExportVertexBuffer::unlock()
	{
	}

} // namespace MyGUI
