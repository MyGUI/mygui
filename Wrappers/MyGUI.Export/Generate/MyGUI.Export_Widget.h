/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_Widget_H__
#define __EXPORT_WIDGET_Widget_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeWidgetMethod_SetCaptionWithNewLine
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaptionWithNewLine_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCaptionWithNewLine(
				Convert<const std::string &>::From( _value ) );
		}
	}



   


   	namespace ScopeWidgetMethod_GetWidgetStyle
	{
		MYGUIEXPORT Convert<MyGUI::WidgetStyle>::Type MYGUICALL ExportWidget_GetWidgetStyle( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::WidgetStyle>::To( static_cast< MyGUI::Widget * >(_native)->getWidgetStyle( ) );
		}
	}



   	namespace ScopeWidgetMethod_SetWidgetStyle
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetWidgetStyle_style_layer( MyGUI::Widget* _native,
			Convert<MyGUI::WidgetStyle>::Type _style ,
			Convert<const std::string &>::Type _layer )
		{
			static_cast< MyGUI::Widget * >(_native)->setWidgetStyle(
				Convert<MyGUI::WidgetStyle>::From( _style ) ,
				Convert<const std::string &>::From( _layer ) );
		}
	}



   	namespace ScopeWidgetMethod_ChangeWidgetSkin
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ChangeWidgetSkin_skinname( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _skinname )
		{
			static_cast< MyGUI::Widget * >(_native)->changeWidgetSkin(
				Convert<const std::string &>::From( _skinname ) );
		}
	}



   	namespace ScopeWidgetMethod_AttachToWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_AttachToWidget_parent_style_layer( MyGUI::Widget* _native,
			Convert<MyGUI::Widget *>::Type _parent ,
			Convert<MyGUI::WidgetStyle>::Type _style ,
			Convert<const std::string &>::Type _layer )
		{
			static_cast< MyGUI::Widget * >(_native)->attachToWidget(
				Convert<MyGUI::Widget *>::From( _parent ) ,
				Convert<MyGUI::WidgetStyle>::From( _style ) ,
				Convert<const std::string &>::From( _layer ) );
		}
	}



   	namespace ScopeWidgetMethod_DetachFromWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_DetachFromWidget_layer( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _layer )
		{
			static_cast< MyGUI::Widget * >(_native)->detachFromWidget(
				Convert<const std::string &>::From( _layer ) );
		}
	}



   	namespace ScopeWidgetProperty_EnableToolTip
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetEnableToolTip( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getEnableToolTip( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnableToolTip( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setEnableToolTip( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_NeedToolTip
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetNeedToolTip( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getNeedToolTip( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedToolTip( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setNeedToolTip( Convert<bool>::From( _value ) );
		}
	}



   


   


   	namespace ScopeWidgetMethod_GetClientWidget
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetClientWidget( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::Widget * >(_native)->getClientWidget( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetClientCoord
	{
		MYGUIEXPORT Convert<MyGUI::types::TCoord< int >>::Type MYGUICALL ExportWidget_GetClientCoord( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::types::TCoord< int >>::To( static_cast< MyGUI::Widget * >(_native)->getClientCoord( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetLayerName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetLayerName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getLayerName( ) );
		}
	}



   	namespace ScopeWidgetProperty_Pointer
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetPointer( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getPointer( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPointer( MyGUI::Widget* _native , Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setPointer( Convert<const std::string &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetEnabledSilent
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabledSilent_value( MyGUI::Widget* _native,
			Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setEnabledSilent(
				Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_Enabled
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsEnabled( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isEnabled( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabled( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setEnabled( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetMethod_SetMaskPick
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetMaskPick_filename( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _filename )
		{
			static_cast< MyGUI::Widget * >(_native)->setMaskPick(
				Convert<const std::string &>::From( _filename ) );
		}
	}



   	namespace ScopeWidgetProperty_InheritsPick
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsInheritsPick( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isInheritsPick( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetInheritsPick( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setInheritsPick( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetProperty_NeedMouseFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsNeedMouseFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isNeedMouseFocus( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedMouseFocus( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setNeedMouseFocus( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetProperty_NeedKeyFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsNeedKeyFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isNeedKeyFocus( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedKeyFocus( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setNeedKeyFocus( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetMethod_FindWidget
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_FindWidget_name( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _name )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::Widget * >(_native)->findWidget(
				Convert<const std::string &>::From( _name ) ));
		}
	}



   	namespace ScopeWidgetMethod_GetChildAt
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetChildAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::Widget * >(_native)->getChildAt(
				Convert<size_t>::From( _index ) ));
		}
	}



   	namespace ScopeWidgetMethod_GetChildCount
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportWidget_GetChildCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Widget * >(_native)->getChildCount( ) );
		}
	}



   


   	namespace ScopeWidgetMethod_GetParent
	{
		MYGUIEXPORT Convert<MyGUI::Widget *>::Type MYGUICALL ExportWidget_GetParent( MyGUI::Widget* _native )
		{
			return Convert<MyGUI::Widget *>::To( static_cast< MyGUI::Widget * >(_native)->getParent( ) );
		}
	}



   	namespace ScopeWidgetMethod_IsRootWidget
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsRootWidget( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isRootWidget( ) );
		}
	}



   	namespace ScopeWidgetMethod_SetColour
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetColour_value( MyGUI::Widget* _native,
			Convert<const MyGUI::Colour &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setColour(
				Convert<const MyGUI::Colour &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetState
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_SetState_value( MyGUI::Widget* _native,
			Convert<const std::string &>::Type _value )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->setState(
				Convert<const std::string &>::From( _value ) ));
		}
	}



   	namespace ScopeWidgetProperty_InheritsAlpha
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsInheritsAlpha( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isInheritsAlpha( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetInheritsAlpha( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setInheritsAlpha( Convert<bool>::From( _value ) );
		}
	}
	


   	namespace ScopeWidgetProperty_Alpha
	{
		MYGUIEXPORT Convert<float>::Type MYGUICALL ExportWidget_GetAlpha( MyGUI::Widget* _native )
		{
			return Convert<float>::To( static_cast< MyGUI::Widget * >(_native)->getAlpha( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlpha( MyGUI::Widget* _native , Convert<float>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setAlpha( Convert<float>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_Caption
	{
		MYGUIEXPORT Convert<const MyGUI::UString &>::Type MYGUICALL ExportWidget_GetCaption( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::UString &>::To( static_cast< MyGUI::Widget * >(_native)->getCaption( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption( MyGUI::Widget* _native , Convert<const MyGUI::UString &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCaption( Convert<const MyGUI::UString &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetAlign
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlign_value( MyGUI::Widget* _native,
			Convert<MyGUI::Align>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setAlign(
				Convert<MyGUI::Align>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetVisible
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetVisible_value( MyGUI::Widget* _native,
			Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setVisible(
				Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord_left_top_width_height( MyGUI::Widget* _native,
			Convert<float>::Type _left ,
			Convert<float>::Type _top ,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealCoord(
				Convert<float>::From( _left ) ,
				Convert<float>::From( _top ) ,
				Convert<float>::From( _width ) ,
				Convert<float>::From( _height ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize_width_height( MyGUI::Widget* _native,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealSize(
				Convert<float>::From( _width ) ,
				Convert<float>::From( _height ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition_left_top( MyGUI::Widget* _native,
			Convert<float>::Type _left ,
			Convert<float>::Type _top )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealPosition(
				Convert<float>::From( _left ) ,
				Convert<float>::From( _top ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< float > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealCoord(
				Convert<const MyGUI::types::TCoord< float > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TSize< float > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealSize(
				Convert<const MyGUI::types::TSize< float > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TPoint< float > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealPosition(
				Convert<const MyGUI::types::TPoint< float > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCoord_left_top_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setCoord(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) ,
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}



   	namespace ScopeWidgetMethod_SetSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize_width_height( MyGUI::Widget* _native,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setSize(
				Convert<int>::From( _width ) ,
				Convert<int>::From( _height ) );
		}
	}



   	namespace ScopeWidgetMethod_SetPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition_left_top( MyGUI::Widget* _native,
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			static_cast< MyGUI::Widget * >(_native)->setPosition(
				Convert<int>::From( _left ) ,
				Convert<int>::From( _top ) );
		}
	}



   	namespace ScopeWidgetMethod_SetCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCoord_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TCoord< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCoord(
				Convert<const MyGUI::types::TCoord< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setSize(
				Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_SetPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition_value( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TPoint< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setPosition(
				Convert<const MyGUI::types::TPoint< int > &>::From( _value ) );
		}
	}



   	namespace ScopeWidgetMethod_GetName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getName( ) );
		}
	}



   


   


   


   


   


   


   


   


   


   


   


   


   


   


   


}

#endif // __EXPORT_WIDGET_Widget_H__
