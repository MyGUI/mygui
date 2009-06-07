/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeRenderBoxEvent_UpdateViewport
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::RenderBox  *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::RenderBox  * _sender )
		{
			mExportHandle(
				Convert<MyGUI::RenderBox  *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportRenderBoxEvent_DelegateUpdateViewport( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportRenderBoxEvent_AdviseUpdateViewport( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::RenderBox* >(_widget)->eventUpdateViewport = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   


   


   


   


   


   


   


   


   


   


   


}
