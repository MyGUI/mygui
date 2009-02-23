/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "ExportMarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

   	namespace ScopeDDContainerEvent_DragWidgetInfo
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<MyGUI::Widget * &>::Type ,
			Convert<MyGUI::types::TCoord< int > &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			MyGUI::Widget * & _item ,
			MyGUI::types::TCoord< int > & _dimension )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<MyGUI::Widget * &>::To( _item ) ,
				Convert<MyGUI::types::TCoord< int > &>::To( _dimension ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateDragWidgetInfo( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseDragWidgetInfo( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->requestDragWidgetInfo = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}

} // namespace Export
