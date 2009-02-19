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
		MYGUIEXPORT void MYGUICALL ExportWidget_ClearUserStrings( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Widget * >(_native)->clearUserStrings( );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_ClearUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->clearUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetUserString_key_value( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key ,
			Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setUserString(
				Convert<const std::string &>::From(_key) ,
				Convert<const std::string &>::From(_value) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetHeight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getHeight( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getWidth( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetBottom( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getBottom( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetTop( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getTop( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetRight( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getRight( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetLeft( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getLeft( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Align >::Type MYGUICALL ExportWidget_GetAlign( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Align >::To( static_cast< MyGUI::Widget * >(_native)->getAlign( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlign( MyGUI::Widget* _native , Convert< MyGUI::Align >::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setAlign( Convert< MyGUI::Align >::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetAbsoluteTop( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteTop( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportWidget_GetAbsoluteLeft( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteLeft( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::types::TCoord< int > >::Type MYGUICALL ExportWidget_GetAbsoluteCoord( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteCoord( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::types::TRect< int > >::Type MYGUICALL ExportWidget_GetAbsoluteRect( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TRect< int > >::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteRect( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<const MyGUI::types::TPoint< int > &>::Type MYGUICALL ExportWidget_GetAbsolutePosition( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getAbsolutePosition( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::WidgetStyle >::Type MYGUICALL ExportWidget_GetWidgetStyle( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::WidgetStyle >::To( static_cast< MyGUI::Widget * >(_native)->getWidgetStyle( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetWidgetStyle( MyGUI::Widget* _native , Convert< MyGUI::WidgetStyle >::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setWidgetStyle( Convert< MyGUI::WidgetStyle >::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ChangeWidgetSkin_skinname( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _skinname )
		{
			static_cast< MyGUI::Widget * >(_native)->changeWidgetSkin(
				Convert<const std::string &>::From(_skinname) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_AttachToWidget_parent( MyGUI::Widget* _native ,
			Convert< MyGUI::Widget * >::Type _parent )
		{
			static_cast< MyGUI::Widget * >(_native)->attachToWidget(
				Convert< MyGUI::Widget * >::From(_parent) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_DetachFromWidget( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Widget * >(_native)->detachFromWidget( );
		}
	}



   	namespace
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
	


   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetClientWidget( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getClientWidget( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::types::TCoord< int > >::Type MYGUICALL ExportWidget_GetClientCoord( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast< MyGUI::Widget * >(_native)->getClientCoord( ) );
		}
	}



   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabledSilent_enabled( MyGUI::Widget* _native ,
			Convert<bool>::Type _enabled )
		{
			static_cast< MyGUI::Widget * >(_native)->setEnabledSilent(
				Convert<bool>::From(_enabled) );
		}
	}



   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetMaskPick_filename( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _filename )
		{
			static_cast< MyGUI::Widget * >(_native)->setMaskPick(
				Convert<const std::string &>::From(_filename) );
		}
	}



   	namespace
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
	


   	namespace
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
	


   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_FindWidget_name( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _name )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->findWidget(
				Convert<const std::string &>::From(_name) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetChildAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getChildAt(
				Convert<size_t>::From(_index) ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportWidget_GetChildCount( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::Widget * >(_native)->getChildCount( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetParent( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getParent( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsRootWidget( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isRootWidget( ) );
		}
	}



   	namespace
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_SetState_state( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _state )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->setState(
				Convert<const std::string &>::From(_state) ) );
		}
	}



   	namespace
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
	


   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportWidget_GetCaption( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::Widget * >(_native)->getCaption( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCaption( Convert<const Ogre::UTFString &>::From( _value ) );
		}
	}
	


   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord_left_top_width_height( MyGUI::Widget* _native ,
			Convert<float>::Type _left ,
			Convert<float>::Type _top ,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealCoord(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) ,
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize_width_height( MyGUI::Widget* _native ,
			Convert<float>::Type _width ,
			Convert<float>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealSize(
				Convert<float>::From(_width) ,
				Convert<float>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition_left_top( MyGUI::Widget* _native ,
			Convert<float>::Type _left ,
			Convert<float>::Type _top )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealPosition(
				Convert<float>::From(_left) ,
				Convert<float>::From(_top) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord_coord( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TCoord< float > &>::Type _coord )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealCoord(
				Convert<const MyGUI::types::TCoord< float > &>::From(_coord) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize_size( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TSize< float > &>::Type _size )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealSize(
				Convert<const MyGUI::types::TSize< float > &>::From(_size) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition_point( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TPoint< float > &>::Type _point )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealPosition(
				Convert<const MyGUI::types::TPoint< float > &>::From(_point) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCoord_left_top_width_height( MyGUI::Widget* _native ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setCoord(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) ,
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize_width_height( MyGUI::Widget* _native ,
			Convert<int>::Type _width ,
			Convert<int>::Type _height )
		{
			static_cast< MyGUI::Widget * >(_native)->setSize(
				Convert<int>::From(_width) ,
				Convert<int>::From(_height) );
		}
	}



   	namespace
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition_left_top( MyGUI::Widget* _native ,
			Convert<int>::Type _left ,
			Convert<int>::Type _top )
		{
			static_cast< MyGUI::Widget * >(_native)->setPosition(
				Convert<int>::From(_left) ,
				Convert<int>::From(_top) );
		}
	}



   	namespace
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
	


   	namespace
	{
		MYGUIEXPORT Convert<const MyGUI::types::TSize< int > &>::Type MYGUICALL ExportWidget_GetSize( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TSize< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getSize( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetSize( MyGUI::Widget* _native , Convert<const MyGUI::types::TSize< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setSize( Convert<const MyGUI::types::TSize< int > &>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<const MyGUI::types::TPoint< int > &>::Type MYGUICALL ExportWidget_GetPosition( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getPosition( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetPosition( MyGUI::Widget* _native , Convert<const MyGUI::types::TPoint< int > &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setPosition( Convert<const MyGUI::types::TPoint< int > &>::From( _value ) );
		}
	}
	


   	namespace
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getName( ) );
		}
	}






   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_CreateWidgetRealT_type_skin_coord_align_name( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _type ,
			Convert<const std::string &>::Type _skin ,
			Convert<const MyGUI::types::TCoord< float > &>::Type _coord ,
			Convert< MyGUI::Align >::Type _align ,
			Convert<const std::string &>::Type _name )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->createWidgetRealT(
				Convert<const std::string &>::From(_type) ,
				Convert<const std::string &>::From(_skin) ,
				Convert<const MyGUI::types::TCoord< float > &>::From(_coord) ,
				Convert< MyGUI::Align >::From(_align) ,
				Convert<const std::string &>::From(_name) ) );
		}
	}


   	namespace
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_CreateWidgetRealT_type_skin_coord_align( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _type ,
			Convert<const std::string &>::Type _skin ,
			Convert<const MyGUI::types::TCoord< float > &>::Type _coord ,
			Convert< MyGUI::Align >::Type _align )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->createWidgetRealT(
				Convert<const std::string &>::From(_type) ,
				Convert<const std::string &>::From(_skin) ,
				Convert<const MyGUI::types::TCoord< float > &>::From(_coord) ,
				Convert< MyGUI::Align >::From(_align) ) );
		}
	}



}
