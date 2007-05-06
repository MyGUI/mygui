#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class WindowFrame : public Window {
    
    protected:
        WindowFrame(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:

		void setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY);// укстановка минимальных и максимальных размеров
		void setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY);// укстановка минимальных и максимальных размеров

		void onMouseMove(Window * pWindow, int16 iPosX, int16 iPosY); // уведомление о движении, но не движение
		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseClick(Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		int16 m_iMinSizeX, m_iMinSizeY, m_iMaxSizeX, m_iMaxSizeY; // максимальные и минимальные значения размеров окна
		
		static WindowFrame *create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_WINDOWFRAME_CSX);

	};
}