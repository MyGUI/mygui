/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_StaticText_H__
#define __EXPORT_WIDGET_StaticText_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeStaticTextMethod_SetProperty
	{
		MYGUIEXPORT void MYGUICALL ExportStaticText_SetProperty_key_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticText * >(_native)->setProperty(
				Convert<const std::string &>::From( _key ) ,
				Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeStaticTextProperty_TextColour
	{
		MYGUIEXPORT Convert<const MyGUI::Colour &>::Type MYGUICALL ExportStaticText_GetTextColour( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::Colour &>::To( static_cast< MyGUI::StaticText * >(_native)->getTextColour( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticText_SetTextColour( MyGUI::Widget* _native , Convert<const MyGUI::Colour &>::Type _value )
		{
			static_cast< MyGUI::StaticText * >(_native)->setTextColour( Convert<const MyGUI::Colour &>::From( _value ) );
		}
	}



   	namespace ScopeStaticTextProperty_TextAlign
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportStaticText_GetTextAlign( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Align>::To( static_cast< MyGUI::StaticText * >(_native)->getTextAlign( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticText_SetTextAlign( MyGUI::Widget* _native , Convert<MyGUI::Align>::Type _value )
		{
			static_cast< MyGUI::StaticText * >(_native)->setTextAlign( Convert<MyGUI::Align>::From( _value ) );
		}
	}



   	namespace ScopeStaticTextProperty_FontHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportStaticText_GetFontHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::StaticText * >(_native)->getFontHeight( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticText_SetFontHeight( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::StaticText * >(_native)->setFontHeight( Convert<int>::From( _value ) );
		}
	}



   	namespace ScopeStaticTextProperty_FontName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportStaticText_GetFontName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::StaticText * >(_native)->getFontName( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportStaticText_SetFontName( MyGUI::Widget* _native , Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::StaticText * >(_native)->setFontName( Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeStaticTextMethod_GetTextSize
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportStaticText_GetTextSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::StaticText * >(_native)->getTextSize( ) );
		}
	}



   	namespace ScopeStaticTextMethod_GetTextRegion
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord< int >>::Type MYGUICALL ExportStaticText_GetTextRegion( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TCoord< int >>::To( static_cast< MyGUI::StaticText * >(_native)->getTextRegion( ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_StaticText_H__
