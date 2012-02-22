﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_Button_H__
#define __EXPORT_WIDGET_Button_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeButtonMethod_SetImageName
	{
		MYGUIEXPORT void MYGUICALL ExportButton_SetImageName_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			static_cast< MyGUI::Button * >(_native)->setImageName(
				Convert<const std::string &>::From( _name ) );
		}
	}



   	namespace ScopeButtonMethod_SetImageGroup
	{
		MYGUIEXPORT void MYGUICALL ExportButton_SetImageGroup_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			static_cast< MyGUI::Button * >(_native)->setImageGroup(
				Convert<const std::string &>::From( _name ) );
		}
	}



   	namespace ScopeButtonMethod_SetImageResource
	{
		MYGUIEXPORT void MYGUICALL ExportButton_SetImageResource_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			static_cast< MyGUI::Button * >(_native)->setImageResource(
				Convert<const std::string &>::From( _name ) );
		}
	}



   	namespace ScopeButtonProperty_ModeImage
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportButton_GetModeImage( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Button * >(_native)->getModeImage( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportButton_SetModeImage( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Button * >(_native)->setModeImage( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeButtonProperty_StateSelected
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportButton_GetStateSelected( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Button * >(_native)->getStateSelected( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportButton_SetStateSelected( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Button * >(_native)->setStateSelected( Convert<bool>::From( _value ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_Button_H__
