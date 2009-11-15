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

   	namespace ScopeProgressBarMethod_SetProperty
	{
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProperty_key_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProperty(
				Convert<const std::string &>::From( _key ) ,
				Convert<const std::string &>::From( _value ) );
		}
	}



   


   


   


   


   


   


   	namespace ScopeProgressBarProperty_ProgressStartPoint
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportProgressBar_GetProgressStartPoint( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Align>::To( static_cast< MyGUI::Progress * >(_native)->getProgressStartPoint( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressStartPoint( MyGUI::Widget* _native , Convert<MyGUI::Align>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressStartPoint( Convert<MyGUI::Align>::From( _value ) );
		}
	}



   	namespace ScopeProgressBarProperty_ProgressAutoTrack
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportProgressBar_GetProgressAutoTrack( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Progress * >(_native)->getProgressAutoTrack( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressAutoTrack( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressAutoTrack( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeProgressBarProperty_ProgressPosition
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgressBar_GetProgressPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Progress * >(_native)->getProgressPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressPosition( Convert<size_t>::From( _value ) );
		}
	}



   	namespace ScopeProgressBarProperty_ProgressRange
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgressBar_GetProgressRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Progress * >(_native)->getProgressRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgressBar_SetProgressRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressRange( Convert<size_t>::From( _value ) );
		}
	}



   


   


   


}
