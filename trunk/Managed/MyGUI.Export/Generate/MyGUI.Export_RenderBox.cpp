/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeRenderBoxEvent_UpdateViewport
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::RenderBox* _sender )
		{
			mExportHandle( *_sender->getUserData<IUnknown>() );
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
