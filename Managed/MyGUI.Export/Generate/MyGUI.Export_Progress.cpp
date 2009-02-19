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

   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Align >::Type MYGUICALL ExportProgress_GetProgressStartPoint( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Align >::To( static_cast< MyGUI::Progress * >(_native)->getProgressStartPoint( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgress_SetProgressStartPoint( MyGUI::Widget* _native , Convert< MyGUI::Align >::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressStartPoint( Convert< MyGUI::Align >::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportProgress_GetProgressAutoTrack( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Progress * >(_native)->getProgressAutoTrack( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgress_SetProgressAutoTrack( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressAutoTrack( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportProgress_GetProgressFillTrack( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Progress * >(_native)->getProgressFillTrack( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgress_SetProgressFillTrack( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressFillTrack( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgress_GetProgressPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Progress * >(_native)->getProgressPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgress_SetProgressPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressPosition( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportProgress_GetProgressRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Progress * >(_native)->getProgressRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportProgress_SetProgressRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::Progress * >(_native)->setProgressRange( Convert<size_t>::From( _value ) );
		}
	}
	


}
