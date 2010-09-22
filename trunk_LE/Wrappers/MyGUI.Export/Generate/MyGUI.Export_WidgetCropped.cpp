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

   	namespace ScopeWidgetMethod_GetHeight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getHeight( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getWidth( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetBottom
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetBottom( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getBottom( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetTop
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetTop( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getTop( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetRight
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetRight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getRight( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetLeft
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetLeft( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getLeft( ) );
		}
	}



   	namespace ScopeWidgetProperty_Align
	{
		MYGUIEXPORT Convert<MyGUI::Align>::Type MYGUICALL ExportWidget_GetAlign( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Align>::To( static_cast< MyGUI::Widget * >(_native)->getAlign( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlign( MyGUI::Widget* _native , Convert<MyGUI::Align>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setAlign( Convert<MyGUI::Align>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsoluteTop
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetAbsoluteTop( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteTop( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsoluteLeft
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetAbsoluteLeft( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteLeft( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsoluteCoord
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord< int >>::Type MYGUICALL ExportWidget_GetAbsoluteCoord( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TCoord< int >>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteCoord( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsoluteRect
	{
		MYGUIEXPORT Convert<MyGUI::types::TRect< int >>::Type MYGUICALL ExportWidget_GetAbsoluteRect( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TRect< int >>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteRect( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsolutePosition
	{
		MYGUIEXPORT Convert<const MyGUI::types::TPoint< int > &>::Type MYGUICALL ExportWidget_GetAbsolutePosition( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getAbsolutePosition( ) );
		}
	}



   	namespace ScopeWidgetProperty_Visible
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsVisible( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isVisible( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetVisible( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setVisible( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetProperty_Size
	{
		MYGUIEXPORT Convert<MyGUI::types::TSize< int >>::Type MYGUICALL ExportWidget_GetSize( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TSize< int >>::To( static_cast< MyGUI::Widget * >(_native)->getSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_Position
	{
		MYGUIEXPORT Convert<MyGUI::types::TPoint< int >>::Type MYGUICALL ExportWidget_GetPosition( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TPoint< int >>::To( static_cast< MyGUI::Widget * >(_native)->getPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition( MyGUI::Widget* _native , Convert<const MyGUI::types::TPoint< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_Coord
	{
		MYGUIEXPORT Convert<const MyGUI::types::TCoord< int > &>::Type MYGUICALL ExportWidget_GetCoord( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TCoord< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getCoord( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCoord( MyGUI::Widget* _native , Convert<const MyGUI::types::TCoord< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCoord( Convert<const MyGUI::types::TCoord< int > &>::From( _value ) );
		}
	}



   


}
