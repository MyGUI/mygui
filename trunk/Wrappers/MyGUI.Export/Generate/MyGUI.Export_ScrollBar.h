/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_ScrollBar_H__
#define __EXPORT_WIDGET_ScrollBar_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

	namespace ScopeScrollBarEvent_ScrollChangePosition
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::ScrollBar *>::Type ,
			Convert<size_t>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::ScrollBar * _sender ,
			size_t _position )
		{
			mExportHandle(
				Convert<MyGUI::ScrollBar *>::To( _sender ) ,
				Convert<size_t>::To( _position ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportScrollBarEvent_DelegateScrollChangePosition( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBarEvent_AdviseScrollChangePosition( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::ScrollBar* >(_widget)->eventScrollChangePosition += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::ScrollBar* >(_widget)->eventScrollChangePosition -= MyGUI::newDelegate(OnEvent);
		}
	}
	namespace ScopeScrollBarMethod_SetCoord
	{
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetCoord_left_top_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setCoord(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) ,
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}
	namespace ScopeScrollBarMethod_SetSize
	{
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetSize_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setSize(
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}
	namespace ScopeScrollBarMethod_SetPosition
	{
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetPosition_left_top( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setPosition(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) );
		}
	}
	namespace ScopeScrollBarProperty_MoveToClick
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportScrollBar_GetMoveToClick( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ScrollBar * >(_native)->getMoveToClick( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetMoveToClick( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setMoveToClick( Convert<bool>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_MinTrackSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportScrollBar_GetMinTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::ScrollBar * >(_native)->getMinTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetMinTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setMinTrackSize( Convert<int>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_TrackSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportScrollBar_GetTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::ScrollBar * >(_native)->getTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setTrackSize( Convert<int>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_LineSize
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportScrollBar_GetLineSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::ScrollBar * >(_native)->getLineSize( ) );
		}
	}
	namespace ScopeScrollBarProperty_ScrollViewPage
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportScrollBar_GetScrollViewPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ScrollBar * >(_native)->getScrollViewPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetScrollViewPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setScrollViewPage( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_ScrollPage
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportScrollBar_GetScrollPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ScrollBar * >(_native)->getScrollPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetScrollPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setScrollPage( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_ScrollPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportScrollBar_GetScrollPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ScrollBar * >(_native)->getScrollPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetScrollPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setScrollPosition( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_ScrollRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportScrollBar_GetScrollRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ScrollBar * >(_native)->getScrollRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetScrollRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setScrollRange( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeScrollBarProperty_VerticalAlignment
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportScrollBar_GetVerticalAlignment( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ScrollBar * >(_native)->getVerticalAlignment( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportScrollBar_SetVerticalAlignment( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ScrollBar * >(_native)->setVerticalAlignment( Convert<bool>::From( _value ) );
		}
	}
}

#endif // __EXPORT_WIDGET_ScrollBar_H__
