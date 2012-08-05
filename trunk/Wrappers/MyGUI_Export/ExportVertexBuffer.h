/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"

namespace Export
{

	namespace ScopeRenderManager_Vertex_Lock
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<void*&>::Type,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportRenderManager_DelegateVertex_Lock(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
	}

	namespace ScopeRenderManager_Vertex_Unlock
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportRenderManager_DelegateVertex_Unlock(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
	}

}
