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

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	ExportVertexBuffer::ExportVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mSizeInBytes(0)
	{
	}

	ExportVertexBuffer::~ExportVertexBuffer()
	{
	}

	void ExportVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
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
