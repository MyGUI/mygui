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
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScroll_GetMinTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getMinTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetMinTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setMinTrackSize( Convert<int>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScroll_GetTrackSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getTrackSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetTrackSize( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setTrackSize( Convert<int>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportVScroll_GetLineSize( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::VScroll * >(_native)->getLineSize( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScroll_GetScrollViewPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollViewPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetScrollViewPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollViewPage( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScroll_GetScrollPage( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollPage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetScrollPage( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollPage( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScroll_GetScrollPosition( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetScrollPosition( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollPosition( Convert<size_t>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportVScroll_GetScrollRange( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::VScroll * >(_native)->getScrollRange( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportVScroll_SetScrollRange( MyGUI::Widget* _native , Convert<size_t>::Type _value )
		{
			static_cast< MyGUI::VScroll * >(_native)->setScrollRange( Convert<size_t>::From( _value ) );
		}
	}
	


}
