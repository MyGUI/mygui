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

   


   


   	namespace ScopeWidgetMethod_ClearUserStrings
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ClearUserStrings( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Widget * >(_native)->clearUserStrings( );
		}
	}



   	namespace ScopeWidgetMethod_IsUserString
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsUserString_key( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isUserString(
				Convert<const std::string &>::From( _key ) ));
		}
	}



   	namespace ScopeWidgetMethod_ClearUserString
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_ClearUserString_key( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->clearUserString(
				Convert<const std::string &>::From( _key ) ));
		}
	}



   	namespace ScopeWidgetMethod_GetUserString
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetUserString_key( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getUserString(
				Convert<const std::string &>::From( _key ) ));
		}
	}



   	namespace ScopeWidgetMethod_SetUserString
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetUserString_key_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setUserString(
				Convert<const std::string &>::From( _key ) ,
				Convert<const std::string &>::From( _value ) );
		}
	}



}
