/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportDataManager.h"

namespace Export
{

	namespace ScopeDataManager_AddResourceLocation
	{
		MYGUIEXPORT void MYGUICALL ExportDataManager_AddResourceLocation(
			Convert<const std::string&>::Type _path,
			Convert<bool>::Type _recursive)
		{
			MyGUI::ExportDataManager::getInstance().addResourceLocation(
				Convert<const std::string&>::From(_path),
				Convert<bool>::From(_recursive));
		}
	}

}
