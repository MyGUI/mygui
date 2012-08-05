/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_ExportDiagnostic.h"
#include "ExportVertexBuffer.h"

namespace MyGUI
{

	ExportVertexBuffer::ExportVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(0),
		mSizeInBytes(0),
		mId(0)
	{
		mId = (size_t)this;
	}

	ExportVertexBuffer::~ExportVertexBuffer()
	{
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
		void* vertexes = nullptr;

		if (Export::ScopeRenderManager_Vertex_Lock::mExportHandle != nullptr)
			Export::ScopeRenderManager_Vertex_Lock::mExportHandle(Export::Convert<void*&>::To(vertexes), Export::Convert<size_t>::To(mNeedVertexCount));

		return reinterpret_cast<Vertex*>(vertexes);
	}

	void ExportVertexBuffer::unlock()
	{
		if (Export::ScopeRenderManager_Vertex_Unlock::mExportHandle != nullptr)
			Export::ScopeRenderManager_Vertex_Unlock::mExportHandle(Export::Convert<size_t>::To(mId));
	}

	size_t ExportVertexBuffer::getId()
	{
		return mId;
	}

} // namespace MyGUI
