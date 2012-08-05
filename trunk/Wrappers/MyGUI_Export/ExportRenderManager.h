/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"

namespace Export
{

	namespace ScopeRenderManager_DoRender
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<size_t>::Type,
			Convert<const std::string&>::Type,
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportRenderManager_DelegateDoRender(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
		}
	}

}
