/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef __MYGUI_EXPORT_VERTEX_BUFFER_H__
#define __MYGUI_EXPORT_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

namespace MyGUI
{

	class ExportVertexBuffer :
		public IVertexBuffer
	{
	public:
		ExportVertexBuffer();
		virtual ~ExportVertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

	private:
		size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;
	};

} // namespace MyGUI

#endif // __MYGUI_EXPORT_VERTEX_BUFFER_H__
