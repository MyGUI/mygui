/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_ProgressBar_H__
#define __EXPORT_WIDGET_ProgressBar_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

	namespace ScopeProgressBarMethod_SetCoord
	{
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetCoord_left_top_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setCoord(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) ,
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}
	namespace ScopeProgressBarMethod_SetSize
	{
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetSize_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setSize(
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}
	namespace ScopeProgressBarMethod_SetPosition
	{
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetPosition_left_top( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setPosition(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) );
		}
	}
	namespace ScopeProgressBarProperty_FlowDirection
	{
		MYGUIEXPORT Convert<MyGUI::FlowDirection>::Type MYGUICALL ExportProgressBar_GetFlowDirection( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::FlowDirection>::To( static_cast< MyGUI::ProgressBar * >(_native)->getFlowDirection( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetFlowDirection( MyGUI::Widget* _native , Convert<MyGUI::FlowDirection>::Type _value )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setFlowDirection( Convert<MyGUI::FlowDirection>::From( _value ) );
		}
	}
	namespace ScopeProgressBarProperty_ProgressAutoTrack
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportProgressBar_GetProgressAutoTrack( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ProgressBar * >(_native)->getProgressAutoTrack( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressAutoTrack( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setProgressAutoTrack( Convert<bool>::From( _value ) );
		}
	}
	namespace ScopeProgressBarProperty_ProgressPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgressBar_GetProgressPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ProgressBar * >(_native)->getProgressPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setProgressPosition( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeProgressBarProperty_ProgressRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgressBar_GetProgressRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::ProgressBar * >(_native)->getProgressRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::ProgressBar * >(_native)->setProgressRange( Convert<size_t>::From( _value ) );
		}
	}
	namespace ScopeProgressBarProperty_Type
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportProgressBar_IsType( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::ProgressBar * >(_native)->isType( ) );
		}
	}
	
}

#endif // __EXPORT_WIDGET_ProgressBar_H__
