#pragma once

#include "MyGUI_Coord.h"
#include "MyGUI_Defines.h"

namespace MyGUI
{
    //This could be implemented as a pure virtual, but you might not want to handle all the functions listed
    class EventCallback
	{
	public:
		virtual void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed) {};
		virtual void onMouseClick(Window * pWindow) {};
		virtual void onMouseDoubleClick(Window * pWindow) {};
		virtual void onMouseFocus(Window * pWindow, bool bIsFocus) {};
		virtual void onMouseMove(Window * pWindow, int16 iPosX, int16 iPosY) {};

		virtual void onKeyFocus(Window * pWindow, bool bIsFocus) {};
		virtual void onKeyButton(Window * pWindow, int keyEvent, wchar_t cText) {};

		virtual void onWarningEvent(Window * pWindow, uint16 uEvent) {};
		virtual void onOtherEvent(Window * pWindow, uint16 uEvent, uint32 data) {};
		virtual void onFadeEnd(bool bIsFade, uint8 fadeID) {};
	};
}