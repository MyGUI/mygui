#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class WindowFrame : public Window {

	public:
		WindowFrame(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
		
		static WindowFrame *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_WINDOWFRAME_CSX);

		void setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY);// укстановка минимальных и максимальных размеров
		void setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY);// укстановка минимальных и максимальных размеров

		void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY); // уведомление о движении, но не движение
		void onMouseSetFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowOld); // смена фокуса
		void onMouseLostFocus(MyGUI::Window * pWindow, MyGUI::Window * pWindowNew); // смена фокуса
		void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		int16 m_iMinSizeX, m_iMinSizeY, m_iMaxSizeX, m_iMaxSizeY; // максимальные и минимальные значения размеров окна

	};

}