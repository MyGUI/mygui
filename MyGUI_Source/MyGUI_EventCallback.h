#pragma once

#include "MyGUI_Defines.h"

namespace MyGUI
{
    //This could be implemented as a pure virtual, but you might not want to handle all the functions listed
    class EventCallback // базовый класс для вызовов
	{
	public:
		virtual void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) {}; // нажата левая кнопка мыши
		virtual void onMouseClick(MyGUI::Window * pWindow) {}; // нажата и отпущена левая кнопка мыши на этом же элементе
		virtual void onMouseDoubleClick(MyGUI::Window * pWindow) {}; // двойной щелчек мыши
		virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // смена фокуса
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iParentPosX, int16 iParentPosY) {}; // уведомление о движении, но не движение

		virtual void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // смена фокуса ввода
		virtual void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) {}; // нажата клавиша

		virtual void onWarningEvent(MyGUI::Window * pWindow, uint16 uEvent) {}; // события предепреждающие ошибки
		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) {}; // дополнительные события
		virtual void onFadeEnd(bool bIsFade, uint8 fadeID) {}; // закончилось затемнение
	};
}