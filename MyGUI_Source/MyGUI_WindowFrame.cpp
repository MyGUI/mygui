//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	WindowFrame::WindowFrame(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_iMinSizeX(0),
		m_iMinSizeY(0),
		m_iMaxSizeX(gui->m_uWidth),
		m_iMaxSizeY(gui->m_uHeight)
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
		if (fMinSizeX >= 0.0) m_iMinSizeX = fMinSizeX * m_GUI->m_uWidth;
		if (fMinSizeY >= 0.0) m_iMinSizeY = fMinSizeY * m_GUI->m_uHeight;
		if (fMaxSizeX >= 0.0) m_iMaxSizeX = fMaxSizeX * m_GUI->m_uWidth;
		if (fMaxSizeY >= 0.0) m_iMaxSizeY = fMaxSizeY * m_GUI->m_uHeight;
	}

	void WindowFrame::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iFotherPosX, int16 iFotherPosY) // уведомление о движении, но не движение
	{
		if (pWindow->m_uExData & WES_CAPTION) {
			iPosX -= m_GUI->m_iOffsetPressedX+pWindow->m_iPosX;
			iPosY -= m_GUI->m_iOffsetPressedY+pWindow->m_iPosY;
			if (m_pWindowFother) { // есть отец
				iPosX -= m_pWindowFother->m_iPosX;
				iPosY -= m_pWindowFother->m_iPosY;
				if (iPosX < 0) iPosX = 0;
				else if ((iPosX + m_iSizeX) > m_pWindowFother->m_iSizeX) iPosX = m_pWindowFother->m_iSizeX - m_iSizeX;
				if (iPosY < 0) iPosY = 0;
				else if ((iPosY + m_iSizeY) > m_pWindowFother->m_iSizeY) iPosY = m_pWindowFother->m_iSizeY - m_iSizeY;
			}
			move(iPosX, iPosY);
		}
		if (pWindow->m_uExData & WES_RESIZE) {
			iPosX -= m_iPosX + m_GUI->m_iOffsetPressedX - m_iSizeX + pWindow->m_iPosX;
			iPosY -= m_iPosY + m_GUI->m_iOffsetPressedY - m_iSizeY + pWindow->m_iPosY;
			if (m_pWindowFother) { // есть отец
				iPosX -= m_pWindowFother->m_iPosX;
				iPosY -= m_pWindowFother->m_iPosY;
				if ((iPosX + m_iPosX) > m_pWindowFother->m_iSizeX) iPosX = m_pWindowFother->m_iSizeX - m_iPosX;
				if ((iPosY + m_iPosY) > m_pWindowFother->m_iSizeY) iPosY = m_pWindowFother->m_iSizeY - m_iPosY;
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
				if (bIsFocus) m_GUI->setMousePointer(POINTER_RESIZE);
				else  m_GUI->setMousePointer(POINTER_DEFAULT);
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

	MyGUI::WindowFrame * Window::createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // перекрывающееся окно
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		WindowFrame * pWindow = new WindowFrame(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, this);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) {
				pWindow->m_pWindowClient = pChild; // клиентское окно
				pChild->m_pWindowText = pWindow->m_pWindowText; // текстовое окно элемента запоминаем в клиентском тоже
			}
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		// минимальный размер равен начальному размеру скина
		pWindow->setMinMax(pSkin->subSkins[0]->sizeX, pSkin->subSkins[0]->sizeY, m_GUI->m_uWidth, m_GUI->m_uHeight);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}


	MyGUI::WindowFrame * GUI::createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // перекрывающееся окно
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		WindowFrame * pWindow = new WindowFrame(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) {
				pWindow->m_pWindowClient = pChild; // клиентское окно
				pChild->m_pWindowText = pWindow->m_pWindowText; // текстовое окно элемента запоминаем в клиентском тоже
			}
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		// минимальный размер равен начальному размеру скина
		pWindow->setMinMax(pSkin->subSkins[0]->sizeX, pSkin->subSkins[0]->sizeY, m_uWidth, m_uHeight);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
