/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"

namespace Export
{

	namespace ScopeDataManager_IsDataExist
	{
		typedef Convert<bool>::Type(MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateIsDataExist(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateIsDataExist");
		}
	}
	namespace ScopeDataManager_GetDataPath
	{
		typedef Convert<const std::string&>::Type(MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateGetDataPath(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateGetDataPath");
		}
	}
	namespace ScopeDataManager_GetData
	{
		typedef Convert<size_t>::Type(MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type,
			Convert<void*&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateGetData(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateGetData");
		}
	}
	namespace ScopeDataManager_FreeData
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateFreeData(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateFreeData");
		}
	}
	namespace ScopeDataManager_GetDataListSize
	{
		typedef Convert<size_t>::Type(MYGUICALLBACK *ExportHandle)(
			Convert<const std::string&>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateGetDataListSize(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateGetDataListSize");
		}
	}
	namespace ScopeDataManager_GetDataListItem
	{
		typedef Convert<const std::string&>::Type(MYGUICALLBACK *ExportHandle)(
			Convert<size_t>::Type);
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateGetDataListItem(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateGetDataListItem");
		}
	}
	namespace ScopeDataManager_GetDataListComplete
	{
		typedef void (MYGUICALLBACK *ExportHandle)( );
		ExportHandle mExportHandle = nullptr;
		
		MYGUIEXPORT void MYGUICALL ExportDataManager_DelegateGetDataListComplete(ExportHandle _delegate)
		{
			mExportHandle = _delegate;
			//MYGUI_PLATFORM_LOG(Info, "ExportDataManager_DelegateGetDataListComplete");
		}
	}

}
