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

   	namespace ScopeWidgetEvent_ToolTip
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<const MyGUI::ToolTipInfo &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			const MyGUI::ToolTipInfo & _info )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<const MyGUI::ToolTipInfo &>::To( _info ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateToolTip( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseToolTip( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventToolTip += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventToolTip -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_RootKeyChangeFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			bool _focus )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<bool>::To( _focus ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateRootKeyChangeFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseRootKeyChangeFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventRootKeyChangeFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventRootKeyChangeFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_RootMouseChangeFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<bool>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			bool _focus )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<bool>::To( _focus ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateRootMouseChangeFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseRootMouseChangeFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventRootMouseChangeFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventRootMouseChangeFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_KeyButtonReleased
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::KeyCode>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::KeyCode _key )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::KeyCode>::To( _key ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyButtonReleased( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyButtonReleased( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonReleased += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonReleased -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_KeyButtonPressed
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::KeyCode>::Type ,
			Convert<unsigned int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::KeyCode _key ,
			unsigned int _char )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::KeyCode>::To( _key ) ,
				Convert<unsigned int>::To( _char ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyButtonPressed( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyButtonPressed( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonPressed += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonPressed -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_KeySetFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::Widget * _old )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _old ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeySetFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeySetFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventKeySetFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventKeySetFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_KeyLostFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::Widget * _new )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _new ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateKeyLostFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseKeyLostFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventKeyLostFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventKeyLostFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonDoubleClick
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonDoubleClick( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonDoubleClick( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonDoubleClick += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonDoubleClick -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonClick
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseButtonClick( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseButtonClick( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonClick += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonClick -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonReleased
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<int>::Type ,
			Convert<int>::Type ,
			Convert<MyGUI::MouseButton>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			int _left ,
			int _top ,
			MyGUI::MouseButton _id )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
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
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonReleased += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonReleased -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseButtonPressed
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<int>::Type ,
			Convert<int>::Type ,
			Convert<MyGUI::MouseButton>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			int _left ,
			int _top ,
			MyGUI::MouseButton _id )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
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
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonPressed += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonPressed -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseWheel
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			int _rel )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<int>::To( _rel ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseWheel( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseWheel( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseWheel += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseWheel -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseMove
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<int>::Type ,
			Convert<int>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			int _left ,
			int _top )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseMove( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseMove( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseMove += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseMove -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseDrag
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<int>::Type ,
			Convert<int>::Type ,
			Convert<MyGUI::MouseButton>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			int _left ,
			int _top ,
			MyGUI::MouseButton _value4 )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<int>::To( _left ) ,
				Convert<int>::To( _top ) ,
				Convert<MyGUI::MouseButton>::To( _value4 ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseDrag( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseDrag( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseDrag += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseDrag -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseSetFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::Widget * _old )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _old ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseSetFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseSetFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseSetFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseSetFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetEvent_MouseLostFocus
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<MyGUI::Widget *>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			MyGUI::Widget * _new )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
				Convert<MyGUI::Widget *>::To( _new ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_DelegateMouseLostFocus( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportWidgetEvent_AdviseMouseLostFocus( MyGUI::Widget* _widget, bool _advise )
		{
			if (_advise)
				static_cast< MyGUI::Widget* >(_widget)->eventMouseLostFocus += MyGUI::newDelegate(OnEvent);
			else
				static_cast< MyGUI::Widget* >(_widget)->eventMouseLostFocus -= MyGUI::newDelegate(OnEvent);
		}
	}



   	namespace ScopeWidgetMethod_GetRootKeyFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetRootKeyFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getRootKeyFocus( ) );
		}
	}



   	namespace ScopeWidgetMethod_GetRootMouseFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetRootMouseFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getRootMouseFocus( ) );
		}
	}



   	namespace ScopeWidgetMethod_IsMaskPickInside
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_IsMaskPickInside_point_coord( MyGUI::Widget* _native,
			Convert<const MyGUI::types::TPoint< int > &>::Type _point ,
			Convert<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->isMaskPickInside(
				Convert<const MyGUI::types::TPoint< int > &>::From( _point ) ,
				Convert<const MyGUI::types::TCoord< int > &>::From( _coord ) ));
		}
	}



   


   


   	namespace ScopeWidgetProperty_InheritsPick
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetInheritsPick( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getInheritsPick( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetInheritsPick( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setInheritsPick( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_NeedMouseFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetNeedMouseFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getNeedMouseFocus( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedMouseFocus( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setNeedMouseFocus( Convert<bool>::From( _value ) );
		}
	}



   	namespace ScopeWidgetProperty_NeedKeyFocus
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportWidget_GetNeedKeyFocus( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::Widget * >(_native)->getNeedKeyFocus( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportWidget_SetNeedKeyFocus( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::Widget * >(_native)->setNeedKeyFocus( Convert<bool>::From( _value ) );
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



}
