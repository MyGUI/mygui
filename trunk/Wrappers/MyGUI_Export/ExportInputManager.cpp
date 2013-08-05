
#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "Generate/MyGUI_Export_MarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT bool MYGUICALL ExportInputManager_GetKeyFocus( )
	{
		return MyGUI::InputManager::getInstance().isFocusKey();
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_GetMouseFocus( )
	{
		return MyGUI::InputManager::getInstance().isFocusMouse();
	}

	MYGUIEXPORT Convert<MyGUI::Widget*>::Type MYGUICALL ExportInputManager_GetKeyFocusWidget( )
	{
		return Convert<MyGUI::Widget*>::To( MyGUI::InputManager::getInstance().getKeyFocusWidget() );
	}

	MYGUIEXPORT void MYGUICALL ExportInputManager_SetKeyFocusWidget(
		MyGUI::Widget* _widget )
	{
		MyGUI::InputManager::getInstance().setKeyFocusWidget(
			_widget );
	}

	MYGUIEXPORT Convert<MyGUI::Widget*>::Type MYGUICALL ExportInputManager_GetMouseFocusWidget( )
	{
		return Convert<MyGUI::Widget*>::To( MyGUI::InputManager::getInstance().getMouseFocusWidget() );
	}

	MYGUIEXPORT void MYGUICALL ExportInputManager_ResetKeyFocusWidget( )
	{
		MyGUI::InputManager::getInstance().resetKeyFocusWidget( );
	}

	MYGUIEXPORT void MYGUICALL ExportInputManager_AddWidgetModal(
		MyGUI::Widget* _widget )
	{
		MyGUI::InputManager::getInstance().addWidgetModal(
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportInputManager_RemoveWidgetModal(
		MyGUI::Widget* _widget )
	{
		MyGUI::InputManager::getInstance().removeWidgetModal(
			_widget );
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_InjectKeyPress(
		Convert<MyGUI::KeyCode>::Type _key,
		Convert<MyGUI::Char>::Type _char
	)
	{
		return MyGUI::InputManager::getInstance().injectKeyPress(
			Convert<MyGUI::KeyCode>::From( _key ),
			Convert<MyGUI::Char>::From( _char)
			);
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_InjectKeyRelease(
		Convert<MyGUI::KeyCode>::Type _key
	)
	{
		return MyGUI::InputManager::getInstance().injectKeyRelease(
			Convert<MyGUI::KeyCode>::From( _key )
			);
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_InjectMousePress(
		int _absX,
		int _absY,
		Convert<MyGUI::MouseButton>::Type _button
	)
	{
		return MyGUI::InputManager::getInstance().injectMousePress(
			_absX,
			_absY,
			Convert<MyGUI::MouseButton>::From( _button )
			);
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_InjectMouseRelease(
		int _absX,
		int _absY,
		Convert<MyGUI::MouseButton>::Type _button
	)
	{
		return MyGUI::InputManager::getInstance().injectMouseRelease(
			_absX,
			_absY,
			Convert<MyGUI::MouseButton>::From( _button )
			);
	}

	MYGUIEXPORT bool MYGUICALL ExportInputManager_InjectMouseMove(
		int _absX,
		int _absY,
		int _absZ
	)
	{
		return MyGUI::InputManager::getInstance().injectMouseMove(
			_absX,
			_absY,
			_absZ );
	}

	MYGUIEXPORT void MYGUICALL ExportInputManager_GetMousePosition(
		Convert<int&>::Type _x,
		Convert<int&>::Type _y
	)
	{
		const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
		_x = point.left;
		_y = point.top;
	}
}
