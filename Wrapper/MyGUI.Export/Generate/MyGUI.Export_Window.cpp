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





   	namespace ScopeWindowMethod_SetProperty
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetProperty_key_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setProperty(
				Convert<const std::string &>::From( _key ) ,
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeWindowProperty_Snap
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



   


   


   


   


   


   


   	namespace ScopeWindowMethod_SetMaxSize
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Window * >(_native)->setMaxSize(
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}



   	namespace ScopeWindowProperty_MaxSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportWindow_GetMaxSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::Window * >(_native)->getMaxSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMaxSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setMaxSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWindowMethod_SetMinSize
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Window * >(_native)->setMinSize(
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}



   	namespace ScopeWindowProperty_MinSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportWindow_GetMinSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::Window * >(_native)->getMinSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWindow_SetMinSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setMinSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   


   	namespace ScopeWindowProperty_AutoAlpha
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



   


   	namespace ScopeWindowMethod_SetVisibleSmooth
	{
		MYGUIEXPORT void MYGUICALL ExportWindow_SetVisibleSmooth_value( MyGUI::Widget* _native,
			Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Window * >(_native)->setVisibleSmooth(
				Convert<bool>::From( _value ) );
		}
	}



   


   


   


   


}
