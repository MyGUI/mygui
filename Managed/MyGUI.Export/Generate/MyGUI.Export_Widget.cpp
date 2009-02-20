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



   	namespace ScopeWidgetEvent_ActionInfo
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<const std::string &>::Type ,
			Convert<const std::string &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			const std::string & _key ,
			const std::string & _value )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<const std::string &>::To( _key ) ,
				Convert<const std::string &>::To( _value ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateActionInfo( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseActionInfo( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventActionInfo = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_ToolTip
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<const MyGUI::ToolTipInfo &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			const MyGUI::ToolTipInfo & _info )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<const MyGUI::ToolTipInfo &>::To( _info ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateToolTip( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseToolTip( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventToolTip = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_RootKeyChangeFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			bool _focus )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<bool>::To( _focus ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateRootKeyChangeFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseRootKeyChangeFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventRootKeyChangeFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_RootMouseChangeFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			bool _focus )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<bool>::To( _focus ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateRootMouseChangeFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseRootMouseChangeFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventRootMouseChangeFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_KeyButtonReleased
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::KeyCode>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::KeyCode _key )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::KeyCode>::To( _key ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyButtonReleased( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyButtonReleased( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonReleased = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_KeyButtonPressed
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::KeyCode>::Type ,
			Convert<unsigned int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::KeyCode _key ,
			unsigned int _char )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::KeyCode>::To( _key ) ,
				Convert<unsigned int>::To( _char ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyButtonPressed( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyButtonPressed( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonPressed = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_KeySetFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::Widget * _old )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::Widget *>::To( _old ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeySetFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeySetFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventKeySetFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_KeyLostFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::Widget * _new )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::Widget *>::To( _new ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyLostFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyLostFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventKeyLostFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonDoubleClick
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender )
		{
			mExportHandle( *_sender->getUserData<IUnknown>() );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonDoubleClick( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonDoubleClick( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonDoubleClick = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonClick
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender )
		{
			mExportHandle( *_sender->getUserData<IUnknown>() );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonClick( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonClick( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonClick = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonReleased
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<int>::Type ,
			Convert<int>::Type ,
			Convert<MyGUI::MouseButton>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			int _left ,
			int _top ,
			MyGUI::MouseButton _id )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) ,
				Convert<MyGUI::MouseButton>::To( _id ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonReleased( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonReleased( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonReleased = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonPressed
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<int>::Type ,
			Convert<int>::Type ,
			Convert<MyGUI::MouseButton>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			int _left ,
			int _top ,
			MyGUI::MouseButton _id )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) ,
				Convert<MyGUI::MouseButton>::To( _id ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonPressed( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonPressed( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonPressed = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseWheel
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			int _rel )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<int>::To( _rel ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseWheel( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseWheel( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseWheel = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseMove
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<int>::Type ,
			Convert<int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			int _left ,
			int _top )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseMove( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseMove( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseMove = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseDrag
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<int>::Type ,
			Convert<int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			int _left ,
			int _top )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseDrag( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseDrag( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseDrag = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseSetFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::Widget * _old )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::Widget *>::To( _old ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseSetFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseSetFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseSetFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseLostFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)( IUnknown _wrapper,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent( MyGUI::Widget* _sender,
			MyGUI::Widget * _new )
		{
			mExportHandle( *_sender->getUserData<IUnknown>(),
				Convert<MyGUI::Widget *>::To( _new ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseLostFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseLostFocus( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Widget* >(_widget)->eventMouseLostFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetMethod_ClearUserStrings
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ClearUserStrings( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Widget * >(_native)->clearUserStrings( );
		}
	}



   	namespace ScopeWidgetMethod_IsUserString
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace ScopeWidgetMethod_ClearUserString
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_ClearUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->clearUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace ScopeWidgetMethod_GetUserString
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetUserString_key( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _key )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getUserString(
				Convert<const std::string &>::From(_key) ) );
		}
	}



   	namespace ScopeWidgetMethod_SetUserString
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
		MYGUIEXPORT Convert< MyGUI::Align >::Type MYGUICALL ExportWidget_GetAlign( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Align >::To( static_cast< MyGUI::Widget * >(_native)->getAlign( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetAlign( MyGUI::Widget* _native , Convert< MyGUI::Align >::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setAlign( Convert< MyGUI::Align >::From( _value ) );
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
		MYGUIEXPORT Convert< MyGUI::types::TCoord< int > >::Type MYGUICALL ExportWidget_GetAbsoluteCoord( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteCoord( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsoluteRect
	{
		MYGUIEXPORT Convert< MyGUI::types::TRect< int > >::Type MYGUICALL ExportWidget_GetAbsoluteRect( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TRect< int > >::To( static_cast< MyGUI::Widget * >(_native)->getAbsoluteRect( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetAbsolutePosition
	{
		MYGUIEXPORT Convert<const MyGUI::types::TPoint< int > &>::Type MYGUICALL ExportWidget_GetAbsolutePosition( MyGUI::Widget* _native )
		{
			return Convert<const MyGUI::types::TPoint< int > &>::To( static_cast< MyGUI::Widget * >(_native)->getAbsolutePosition( ) );
		}
	}



   	namespace ScopeWidgetProperty_WidgetStyle
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
	


   	namespace ScopeWidgetMethod_ChangeWidgetSkin
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_ChangeWidgetSkin_skinname( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _skinname )
		{
			static_cast< MyGUI::Widget * >(_native)->changeWidgetSkin(
				Convert<const std::string &>::From(_skinname) );
		}
	}



   	namespace ScopeWidgetMethod_AttachToWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_AttachToWidget_parent( MyGUI::Widget* _native ,
			Convert< MyGUI::Widget * >::Type _parent )
		{
			static_cast< MyGUI::Widget * >(_native)->attachToWidget(
				Convert< MyGUI::Widget * >::From(_parent) );
		}
	}



   	namespace ScopeWidgetMethod_DetachFromWidget
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_DetachFromWidget( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Widget * >(_native)->detachFromWidget( );
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
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetClientWidget( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getClientWidget( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetClientCoord
	{
		MYGUIEXPORT Convert< MyGUI::types::TCoord< int > >::Type MYGUICALL ExportWidget_GetClientCoord( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::types::TCoord< int > >::To( static_cast< MyGUI::Widget * >(_native)->getClientCoord( ) );
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
		MYGUIEXPORT void MYGUICALL ExportWidget_SetEnabledSilent_enabled( MyGUI::Widget* _native ,
			Convert<bool>::Type _enabled )
		{
			static_cast< MyGUI::Widget * >(_native)->setEnabledSilent(
				Convert<bool>::From(_enabled) );
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
		MYGUIEXPORT void MYGUICALL ExportWidget_SetMaskPick_filename( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _filename )
		{
			static_cast< MyGUI::Widget * >(_native)->setMaskPick(
				Convert<const std::string &>::From(_filename) );
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
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_FindWidget_name( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _name )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->findWidget(
				Convert<const std::string &>::From(_name) ) );
		}
	}



   	namespace ScopeWidgetMethod_GetChildAt
	{
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetChildAt_index( MyGUI::Widget* _native ,
			Convert<size_t>::Type _index )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getChildAt(
				Convert<size_t>::From(_index) ) );
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
		MYGUIEXPORT Convert< MyGUI::Widget * >::Type MYGUICALL ExportWidget_GetParent( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::Widget * >::To( static_cast< MyGUI::Widget * >(_native)->getParent( ) );
		}
	}



   	namespace ScopeWidgetMethod_IsRootWidget
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsRootWidget( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isRootWidget( ) );
		}
	}



   	namespace ScopeWidgetMethod_SetState
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_SetState_state( MyGUI::Widget* _native ,
			Convert<const std::string &>::Type _state )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->setState(
				Convert<const std::string &>::From(_state) ) );
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
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportWidget_GetCaption( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::Widget * >(_native)->getCaption( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setCaption( Convert<const Ogre::UTFString &>::From( _value ) );
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
	


   	namespace ScopeWidgetMethod_SetRealCoord
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



   	namespace ScopeWidgetMethod_SetRealSize
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



   	namespace ScopeWidgetMethod_SetRealPosition
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



   	namespace ScopeWidgetMethod_SetRealCoord
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealCoord_coord( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TCoord< float > &>::Type _coord )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealCoord(
				Convert<const MyGUI::types::TCoord< float > &>::From(_coord) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealSize
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealSize_size( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TSize< float > &>::Type _size )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealSize(
				Convert<const MyGUI::types::TSize< float > &>::From(_size) );
		}
	}



   	namespace ScopeWidgetMethod_SetRealPosition
	{
		MYGUIEXPORT void MYGUICALL ExportWidget_SetRealPosition_point( MyGUI::Widget* _native ,
			Convert<const MyGUI::types::TPoint< float > &>::Type _point )
		{
			static_cast< MyGUI::Widget * >(_native)->setRealPosition(
				Convert<const MyGUI::types::TPoint< float > &>::From(_point) );
		}
	}



   	namespace ScopeWidgetMethod_SetCoord
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



   	namespace ScopeWidgetMethod_SetSize
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



   	namespace ScopeWidgetMethod_SetPosition
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
	


   	namespace ScopeWidgetProperty_Size
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
	


   	namespace ScopeWidgetProperty_Position
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
	


   	namespace ScopeWidgetMethod_GetName
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportWidget_GetName( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Widget * >(_native)->getName( ) );
		}
	}






   	namespace ScopeWidgetMethod_CreateWidgetRealT
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


   	namespace ScopeWidgetMethod_CreateWidgetRealT
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
