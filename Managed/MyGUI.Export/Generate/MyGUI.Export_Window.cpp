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
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWindow_GetSnap( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Window * >(_native)->getSnap( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetSnap( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setSnap( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize_width_height( MyGUI::Widget* _native ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Window * >(_native)->setMaxSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const MyGUI::types::TSize< int > &>::Type MYGUICALL ExportWindow_GetMaxSize( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TSize< int > &>::To( static_cast< MyGUI::Window * >(_native)->getMaxSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize_width_height( MyGUI::Widget* _native ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Window * >(_native)->setMinSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const MyGUI::types::TSize< int > &>::Type MYGUICALL ExportWindow_GetMinSize( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TSize< int > &>::To( static_cast< MyGUI::Window * >(_native)->getMinSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWindow_GetAutoAlpha( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Window * >(_native)->getAutoAlpha( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetAutoAlpha( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setAutoAlpha( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_DestroySmooth( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Window * >(_native)->destroySmooth( );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetVisibleSmooth_visible( MyGUI::Widget* _native ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::Window * >(_native)->setVisibleSmooth(
				Convert<bool>::From(_visible) );
		}
	}



}
