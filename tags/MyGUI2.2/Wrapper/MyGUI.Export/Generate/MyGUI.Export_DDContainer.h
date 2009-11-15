/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_DDContainer_H__
#define __EXPORT_WIDGET_DDContainer_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeDDContainerEvent_UpdateDropState
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<MyGUI::Widget *>::Type ,
			Convert<const MyGUI::DDWidgetState &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			MyGUI::Widget * _items ,
			const MyGUI::DDWidgetState & _state )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _items ) ,
				Convert<const MyGUI::DDWidgetState &>::To( _state ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateUpdateDropState( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseUpdateDropState( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->eventUpdateDropState = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}





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



   	namespace ScopeDDContainerEvent_ChangeDDState
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<MyGUI::DDItemState>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			MyGUI::DDItemState _state )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<MyGUI::DDItemState>::To( _state ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateChangeDDState( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseChangeDDState( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->eventChangeDDState = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeDDContainerEvent_DropResult
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<const MyGUI::DDItemInfo &>::Type ,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			const MyGUI::DDItemInfo & _info ,
			bool _result )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<const MyGUI::DDItemInfo &>::To( _info ) ,
				Convert<bool>::To( _result ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateDropResult( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseDropResult( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->eventDropResult = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeDDContainerEvent_RequestDrop
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<const MyGUI::DDItemInfo &>::Type ,
			Convert<bool &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			const MyGUI::DDItemInfo & _info ,
			bool & _result )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<const MyGUI::DDItemInfo &>::To( _info ) ,
				Convert<bool &>::To( _result ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateRequestDrop( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseRequestDrop( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->eventRequestDrop = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeDDContainerEvent_StartDrag
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::DDContainer *>::Type ,
			Convert<const MyGUI::DDItemInfo &>::Type ,
			Convert<bool &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::DDContainer * _sender ,
			const MyGUI::DDItemInfo & _info ,
			bool & _result )
		{
			mExportHandle(
				Convert<MyGUI::DDContainer *>::To( _sender ) ,
				Convert<const MyGUI::DDItemInfo &>::To( _info ) ,
				Convert<bool &>::To( _result ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_DelegateStartDrag( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainerEvent_AdviseStartDrag( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::DDContainer* >(_widget)->eventStartDrag = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeDDContainerProperty_NeedDragDrop
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportDDContainer_GetNeedDragDrop( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::DDContainer * >(_native)->getNeedDragDrop( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainer_SetNeedDragDrop( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::DDContainer * >(_native)->setNeedDragDrop( Convert<bool>::From( _value ) );
		}
	}



   


   


}

#endif // __EXPORT_WIDGET_DDContainer_H__
