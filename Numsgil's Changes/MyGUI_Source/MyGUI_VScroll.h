#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class VScroll : public Window {
    protected:
        VScroll(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
	public:
		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseButton(Window * pWindow, bool bIsLeftButtonPressed);
		void onMouseMove(Window * pWindow, int16 iPosX, int16 iPosY); // уведомление о движении, но не движение
		void onMouseClick(Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		void onKeyButton(Window * pWindow, int keyEvent, char cText); // нажата клавиша
		void onKeyFocus(Window * pWindow, bool bIsFocus); // смена фокуса ввода

		void changePosition(bool bIsUp); // просчет позиции, прорисовка и посылка уведомлений
		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		void recalcScroll(); // пересчитывает позицию скрола
		void setScrollRange(uint16 uSizeScroll); // устанавливает новый размер скролл
		void setScrollPos(uint16 uPosScroll); // устанавливает новую позицию скролла

		Window *m_pWindowTrack; // окно ползунка

		uint16 m_uSizeScroll; // размер, диапазон скрола
		uint16 m_uPosScroll; // текущее положение скролла
		int16 m_iRealMousePosY; // реальное положение курсора мыши

		int m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // размеры элементов скрола, загружаются со скином
		
		static VScroll *create(const Coord &Pos, const Coord &Size, 
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_VSCROLL);

	};

}