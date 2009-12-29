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



   	namespace ScopeWidgetEvent_ActionInfo
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Widget *>::Type ,
			Convert<const std::string &>::Type ,
			Convert<const std::string &>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Widget * _sender ,
			const std::string & _key ,
			const std::string & _value )
		{
			mExportHandle(
				Convert<MyGUI::Widget *>::To( _sender ) ,
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
			static_cast< MyGUI::Widget* >(_widget)->eventToolTip = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventRootKeyChangeFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventRootMouseChangeFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonReleased = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventKeyButtonPressed = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventKeySetFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventKeyLostFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonDoubleClick = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonClick = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonReleased = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseButtonPressed = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseWheel = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseMove = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeWidgetEvent_MouseDrag
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseSetFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
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
			static_cast< MyGUI::Widget* >(_widget)->eventMouseLostFocus = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



}
