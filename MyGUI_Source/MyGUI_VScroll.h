#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

    class VScroll : public Window {
        VScroll(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
    public:
        static VScroll *createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
            Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_VSCROLL);

        void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
        void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed);
        void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iParentPosX, int16 iParentPosY); // уведомление о движении, но не движение
        void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

        void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша
        void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса ввода

        void changePosition(bool bIsUp); // просчет позиции, прорисовка и посылка уведомлений
        void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
        void recalcScroll(); // пересчитывает позицию скрола
        void setScrollRange(uint16 uSizeScroll); // устанавливает новый размер скролл
        void setScrollPos(uint16 uPosScroll); // устанавливает новую позицию скролла
        inline uint16 getScrollPos() {return m_uPosScroll;}; // возвращает текущую позицию скрола

        Window *m_pWindowTrack; // окно ползунка

        uint16 m_uSizeScroll; // размер, диапазон скрола
        uint16 m_uPosScroll; // текущее положение скролла
        int16 m_iRealMousePosY; // реальное положение курсора мыши

        uint8 m_uHeightTrack, m_uHeightButton, m_uHeightButtonAll; // размеры элементов скрола, загружаются со скином

    };

}