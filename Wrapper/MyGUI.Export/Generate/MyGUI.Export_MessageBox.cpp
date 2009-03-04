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

   	namespace ScopeMessageBoxEvent_MessageBoxResult
	{
		typedef void (MYGUICALLBACK *ExportHandle)(
			Convert<MyGUI::Message *>::Type ,
			Convert<MyGUI::MessageBoxStyle>::Type );
		ExportHandle mExportHandle = nullptr;
		
		void OnEvent(
			MyGUI::Message * _sender ,
			MyGUI::MessageBoxStyle _result )
		{
			mExportHandle(
				Convert<MyGUI::Message *>::To( _sender ) ,
				Convert<MyGUI::MessageBoxStyle>::To( _result ) );
		}
		
		MYGUIEXPORT void MYGUICALL ExportMessageBoxEvent_DelegateMessageBoxResult( ExportHandle _delegate )
		{
			mExportHandle = _delegate;
		}
		MYGUIEXPORT void MYGUICALL ExportMessageBoxEvent_AdviseMessageBoxResult( MyGUI::Widget* _widget, bool _advise )
		{
			static_cast< MyGUI::Message* >(_widget)->eventMessageBoxResult = _advise ? MyGUI::newDelegate(OnEvent) : nullptr;
		}
	}



   	namespace ScopeMessageBoxMethod_SetMessageStyle
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetMessageStyle_style( MyGUI::Widget* _native,
			Convert<MyGUI::MessageBoxStyle>::Type _style )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageStyle(
				Convert<MyGUI::MessageBoxStyle>::From( _style ) );
		}
	}



   	namespace ScopeMessageBoxMethod_SetMessageButton
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetMessageButton_button( MyGUI::Widget* _native,
			Convert<MyGUI::MessageBoxStyle>::Type _button )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageButton(
				Convert<MyGUI::MessageBoxStyle>::From( _button ) );
		}
	}



   	namespace ScopeMessageBoxMethod_EndMessage
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_EndMessage( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::Message * >(_native)->endMessage( );
		}
	}



   	namespace ScopeMessageBoxMethod_EndMessage
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_EndMessage_result( MyGUI::Widget* _native,
			Convert<MyGUI::MessageBoxStyle>::Type _result )
		{
			static_cast< MyGUI::Message * >(_native)->endMessage(
				Convert<MyGUI::MessageBoxStyle>::From( _result ) );
		}
	}



   	namespace ScopeMessageBoxMethod_SetWindowFade
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetWindowFade_fade( MyGUI::Widget* _native,
			Convert<bool>::Type _fade )
		{
			static_cast< MyGUI::Message * >(_native)->setWindowFade(
				Convert<bool>::From( _fade ) );
		}
	}



   	namespace ScopeMessageBoxMethod_SetMessageIcon
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetMessageIcon_icon( MyGUI::Widget* _native,
			Convert<MyGUI::MessageBoxStyle>::Type _icon )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageIcon(
				Convert<MyGUI::MessageBoxStyle>::From( _icon ) );
		}
	}



   	namespace ScopeMessageBoxMethod_GetDefaultLayer
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMessageBox_GetDefaultLayer( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::Message * >(_native)->getDefaultLayer( ) );
		}
	}



   	namespace ScopeMessageBoxMethod_SetSmoothShow
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetSmoothShow_smooth( MyGUI::Widget* _native,
			Convert<bool>::Type _smooth )
		{
			static_cast< MyGUI::Message * >(_native)->setSmoothShow(
				Convert<bool>::From( _smooth ) );
		}
	}



   	namespace ScopeMessageBoxMethod_AddButtonName
	{
		MYGUIEXPORT Convert<MyGUI::MessageBoxStyle>::Type MYGUICALL ExportMessageBox_AddButtonName_name( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _name )
		{
			return Convert<MyGUI::MessageBoxStyle>::To( static_cast< MyGUI::Message * >(_native)->addButtonName(
				Convert<const Ogre::UTFString &>::From( _name ) ));
		}
	}



   	namespace ScopeMessageBoxMethod_SetMessageText
	{
		MYGUIEXPORT void MYGUICALL ExportMessageBox_SetMessageText_message( MyGUI::Widget* _native,
			Convert<const Ogre::UTFString &>::Type _message )
		{
			static_cast< MyGUI::Message * >(_native)->setMessageText(
				Convert<const Ogre::UTFString &>::From( _message ) );
		}
	}



   


   


}
