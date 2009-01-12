/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_WIDGET_H__
#define __MMYGUI_WIDGET_H__

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_Align.h"
#include "MMyGUI_IntCoord.h"
#include "MMyGUI_IntSize.h"
#include "MMyGUI_IntPoint.h"
#include "MMyGUI_Delegate1.h"
#include "MMyGUI_Delegate2.h"
#include "MMyGUI_Delegate3.h"
#include "MMyGUI_Delegate4.h"
#include "MMyGUI_Common.h"

namespace MMyGUI
{

	public ref class Widget
	{

		//--------------------------------------------------------------------
		// объ€вление типов и конструкторов
		MMYGUI_DECLARE_BASE(Widget);


		//--------------------------------------------------------------------
		// проперти паблик интерфейса
		MMYGUI_DECLARE_PROPERTY(Alpha, float);
		MMYGUI_DECLARE_PROPERTY_IS(Visible, bool);

		MMYGUI_DECLARE_PROPERTY(Coord, IntCoord);
		MMYGUI_DECLARE_PROPERTY(Size, IntSize);
		MMYGUI_DECLARE_PROPERTY(Position, IntPoint);


		//--------------------------------------------------------------------
		// делегаты паблик интерфейса
		MMYGUI_DECLARE_DELEGATE2(MouseLostFocus, MyGUI::Widget*, MyGUI::Widget*);
		MMYGUI_DECLARE_DELEGATE2(MouseSetFocus, MyGUI::Widget*, MyGUI::Widget*);

		MMYGUI_DECLARE_DELEGATE3(MouseDrag, MyGUI::Widget*, int, int);
		MMYGUI_DECLARE_DELEGATE3(MouseMove, MyGUI::Widget*, int, int);
		MMYGUI_DECLARE_DELEGATE2(MouseWheel, MyGUI::Widget*, int);

		MMYGUI_DECLARE_DELEGATE4(MouseButtonPressed, MyGUI::Widget*, int, int, MyGUI::MouseButton);
		MMYGUI_DECLARE_DELEGATE4(MouseButtonReleased, MyGUI::Widget*, int, int, MyGUI::MouseButton);
		MMYGUI_DECLARE_DELEGATE1(MouseButtonClick, MyGUI::Widget*);
		MMYGUI_DECLARE_DELEGATE1(MouseButtonDoubleClick, MyGUI::Widget*);

		MMYGUI_DECLARE_DELEGATE2(KeyLostFocus, MyGUI::Widget*, MyGUI::Widget*);
		MMYGUI_DECLARE_DELEGATE2(KeySetFocus, MyGUI::Widget*, MyGUI::Widget*);

		MMYGUI_DECLARE_DELEGATE3(KeyButtonPressed, MyGUI::Widget*, MyGUI::KeyCode, MyGUI::Char);
		MMYGUI_DECLARE_DELEGATE2(KeyButtonReleased, MyGUI::Widget*, MyGUI::KeyCode);

		MMYGUI_DECLARE_DELEGATE2(RootMouseChangeFocus, MyGUI::Widget*, bool);
		MMYGUI_DECLARE_DELEGATE2(RootKeyChangeFocus, MyGUI::Widget*, bool);

	};

} // namespace MMyGUI

#endif // __MMYGUI_WIDGET_H__
