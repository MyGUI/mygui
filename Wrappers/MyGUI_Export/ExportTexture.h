/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"

namespace Export
{

	namespace ScopeRenderManager_Texture_LoadFromFile
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type,
			Convert<int&>::Type,
			Convert<int&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportRenderManager_DelegateTexture_LoadFromFile(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateIsDataExist");
		}
	}

}
