/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_VScrollBar_H__
#define __EXPORT_WIDGET_VScrollBar_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeVScrollBarEvent_ScrollChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::VScroll *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::VScroll * _sender ,
			size_t _position )
		{
			mExportHandle(
				Convert<MyGUI::VScroll *>::To( _sender ) ,
				Convert<size_t>::To( _position ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportVScrollBarEvent_DelegateScrollChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBarEvent_AdviseScrollChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::VScroll* >(_widget)->eventScrollChangePosition = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   


   


   


   


   


   


   


   	namespace ScopeVScrollBarProperty_MoveToClick
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportVScrollBar_GetMoveToClick( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::VScroll * >(_native)->getMoveToClick( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetMoveToClick( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setMoveToClick( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarProperty_MinTrackSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScrollBar_GetMinTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getMinTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetMinTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setMinTrackSize( Convert<int>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarProperty_TrackSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScrollBar_GetTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setTrackSize( Convert<int>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarMethod_GetLineSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScrollBar_GetLineSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getLineSize( ) );
		}
	}



   	namespace ScopeVScrollBarProperty_ScrollViewPage
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScrollBar_GetScrollViewPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollViewPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetScrollViewPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollViewPage( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarProperty_ScrollPage
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScrollBar_GetScrollPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetScrollPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollPage( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarProperty_ScrollPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScrollBar_GetScrollPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetScrollPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollPosition( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeVScrollBarProperty_ScrollRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScrollBar_GetScrollRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScrollBar_SetScrollRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollRange( Convert<size_t>::From( _value ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_VScrollBar_H__
