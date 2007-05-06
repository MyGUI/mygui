#include "MyGUI_WindowFrame.h"
#include "MyGUI_GUI.h"
#include <OgreOverlayContainer.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	WindowFrame::WindowFrame(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
		m_iMinSizeX(0),
		m_iMinSizeY(0),
		m_iMaxSizeX(GUI::getSingleton()->m_uWidth),
		m_iMaxSizeY(GUI::getSingleton()->m_uHeight)
	{
	}

	void WindowFrame::setMinMax(int16 iMinSizeX, int16 iMinSizeY, int16 iMaxSizeX, int16 iMaxSizeY) // укстановка минимальных и максимальных размеров
	{
		if (iMinSizeX >= 0) m_iMinSizeX = iMinSizeX;
		if (iMinSizeY >= 0) m_iMinSizeY = iMinSizeY;
		if (iMaxSizeX >= 0) m_iMaxSizeX = iMaxSizeX;
		if (iMaxSizeY >= 0) m_iMaxSizeY = iMaxSizeY;
	}

	void WindowFrame::setMinMaxReal(Real fMinSizeX, Real fMinSizeY, Real fMaxSizeX, Real fMaxSizeY)// укстановка минимальных и максимальных размеров
	{
		if (fMinSizeX >= 0.0) m_iMinSizeX = fMinSizeX * GUI::getSingleton()->m_uWidth;
		if (fMinSizeY >= 0.0) m_iMinSizeY = fMinSizeY * GUI::getSingleton()->m_uHeight;
		if (fMaxSizeX >= 0.0) m_iMaxSizeX = fMaxSizeX * GUI::getSingleton()->m_uWidth;
		if (fMaxSizeY >= 0.0) m_iMaxSizeY = fMaxSizeY * GUI::getSingleton()->m_uHeight;
	}

	void WindowFrame::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) // уведомление о движении, но не движение
	{
		if (pWindow->m_uExData & WES_CAPTION) {
			iPosX -= GUI::getSingleton()->m_iOffsetPressedX+pWindow->m_iPosX;
			iPosY -= GUI::getSingleton()->m_iOffsetPressedY+pWindow->m_iPosY;
			if (m_pWindowParent) { // есть отец
				iPosX -= m_pWindowParent->m_iPosX;
				iPosY -= m_pWindowParent->m_iPosY;
				if (iPosX < 0) iPosX = 0;
				else if ((iPosX + m_iSizeX) > m_pWindowParent->m_iSizeX) iPosX = m_pWindowParent->m_iSizeX - m_iSizeX;
				if (iPosY < 0) iPosY = 0;
				else if ((iPosY + m_iSizeY) > m_pWindowParent->m_iSizeY) iPosY = m_pWindowParent->m_iSizeY - m_iSizeY;
			}
			move(iPosX, iPosY);
		}
		if (pWindow->m_uExData & WES_RESIZE) {
			iPosX -= m_iPosX + GUI::getSingleton()->m_iOffsetPressedX - m_iSizeX + pWindow->m_iPosX;
			iPosY -= m_iPosY + GUI::getSingleton()->m_iOffsetPressedY - m_iSizeY + pWindow->m_iPosY;
			if (m_pWindowParent) { // есть отец
				iPosX -= m_pWindowParent->m_iPosX;
				iPosY -= m_pWindowParent->m_iPosY;
				if ((iPosX + m_iPosX) > m_pWindowParent->m_iSizeX) iPosX = m_pWindowParent->m_iSizeX - m_iPosX;
				if ((iPosY + m_iPosY) > m_pWindowParent->m_iSizeY) iPosY = m_pWindowParent->m_iSizeY - m_iPosY;
			}
			if (iPosX < m_iMinSizeX) iPosX = m_iMinSizeX;
			else if (iPosX > m_iMaxSizeX) iPosX = m_iMaxSizeX;
			if (iPosY < m_iMinSizeY) iPosY = m_iMinSizeY;
			else if (iPosY > m_iMaxSizeY) iPosY = m_iMaxSizeY;
			size(iPosX, iPosY);
		}
	}

	void WindowFrame::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		uint16 flag = 0;
		if (pWindow->m_uExData & WES_CAPTION) flag = WES_CAPTION;
		else if (pWindow->m_uExData & WES_RESIZE) flag = WES_RESIZE;
		else if (pWindow->m_uExData & WES_CLOSE) flag = WES_CLOSE;

		if (flag != 0) {
			if (flag == WES_RESIZE) {
				if (bIsFocus) GUI::getSingleton()->setMousePointer(POINTER_RESIZE);
				else  GUI::getSingleton()->setMousePointer(POINTER_DEFAULT);
			}
			uint8 uSkin = SKIN_STATE_NORMAL;
			if (bIsFocus) uSkin = SKIN_STATE_ACTIVED;
			for (uint i=0; i<m_aWindowChild.size(); i++) {
				Window * pChild = m_aWindowChild[i];
				if (pChild->m_uExData & flag) {
					if (pChild->m_paStrSkins[uSkin]) pChild->m_overlayContainer->setMaterialName(*pChild->m_paStrSkins[uSkin]);
				}
			}
		}
	}

	void WindowFrame::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{
		// кнопка закрыть окно
		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_FRAME_CLOSE, 0);
	}
	
	WindowFrame *WindowFrame::create(const Coord &Pos, const Coord &Size,
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
    {
        __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		WindowFrame * pWindow = new WindowFrame(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay, parent);
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) {
				pWindow->m_pWindowClient = pChild; // клиентское окно
				pChild->m_pWindowText = pWindow->m_pWindowText; // текстовое окно элемента запоминаем в клиентском тоже
			}
		}
		
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		              
		// минимальный размер равен начальному размеру скина
		pWindow->setMinMax(pSkin->subSkins[0]->sizeX, pSkin->subSkins[0]->sizeY,
		    GUI::getSingleton()->m_uWidth,
		    GUI::getSingleton()->m_uHeight);
		return pWindow;    
    }
}