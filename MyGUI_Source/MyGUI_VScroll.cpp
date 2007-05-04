//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	VScroll::VScroll(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_pWindowTrack(0),
		m_uSizeScroll(0),
		m_uPosScroll(0)
	{
		setState(WS_DEACTIVE);
	}

	void VScroll::onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText) // нажата клавиша
	{
		// просто передаем отцу
		if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
	}
	void VScroll::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса ввода
	{
		// просто передаем отцу
		if (m_pEventCallback) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void VScroll::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) // уведомление о движении, но не движение
	{
		if (m_uSizeScroll < 2) return;
		if (!m_pWindowTrack) return;
		// узнаем текущее положение курсора
		int16 pos = m_GUI->m_overlayContainerMouse->getTop() + m_GUI->m_iCurrentOffsetCursorY - m_iRealMousePosY;
		// границы
		if (pos < m_uHeightButton) pos = m_uHeightButton;
		else if (pos > (m_pWindowTrack->m_pWindowFother->m_iSizeY - m_uHeightButton - m_uHeightTrack)) {
			pos = (m_pWindowTrack->m_pWindowFother->m_iSizeY - m_uHeightButton - m_uHeightTrack);
		}
		// передвигаем
		m_pWindowTrack->move(m_pWindowTrack->m_iPosX, pos);
		// если позиция изменилась, то отсылаем сообщение
		int16 track = ((pos-m_uHeightButton) * (m_uSizeScroll)) / (m_pWindowTrack->m_pWindowFother->m_iSizeY - m_uHeightButtonAll);
		if (track == m_uSizeScroll) track --;
		if (track != m_uPosScroll) {
			m_uPosScroll = track;
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_SCROLL_CHANGE_POS, m_uPosScroll);
		}

	}

	void VScroll::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed)
	{

		if (pWindow == m_pWindowTrack) { // ползунок
			if (bIsLeftButtonPressed) pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[SKIN_STATE_SELECTED]);
			else {
				pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[SKIN_STATE_ACTIVED]);
				setScrollPos(m_uPosScroll);
			}
			m_iRealMousePosY = m_GUI->m_overlayContainerMouse->getTop() + m_GUI->m_iCurrentOffsetCursorY - m_pWindowTrack->m_iPosY;
			return;
		}

		uint16 flag = 0;
		if (pWindow->m_uExData & WES_VSCROLL_UP) flag = WES_VSCROLL_UP;
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) flag = WES_VSCROLL_DOWN;

		if (flag != 0) {
			uint8 uSkin = SKIN_STATE_ACTIVED;
			if (bIsLeftButtonPressed) uSkin = SKIN_STATE_SELECTED;
			for (uint i=0; i<m_aWindowChild.size(); i++) {
				Window * pChild = m_aWindowChild[i];
				if (pChild->m_uExData & flag) {
					if (pChild->m_paStrSkins[uSkin]) pChild->m_overlayContainer->setMaterialName(*pChild->m_paStrSkins[uSkin]);
				}
			}
		}
	}

	void VScroll::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{
		if (pWindow->m_uExData & WES_VSCROLL_UP) changePosition(true);
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) changePosition(false);
	}

	void VScroll::changePosition(bool bIsUp) // просчет позиции, прорисовка и посылка уведомлений
	{
		if (m_uSizeScroll < 2) return;
		int16 pos = m_pWindowTrack->m_pWindowFother->m_iSizeY - m_uHeightButtonAll;
		if (bIsUp) {
			if (m_uPosScroll != 0) m_uPosScroll --;
		} else {
			if ((m_uPosScroll+1) < m_uSizeScroll) m_uPosScroll ++;
		}
		pos = pos * m_uPosScroll / (m_uSizeScroll-1);
		if (m_pWindowTrack) m_pWindowTrack->move(m_pWindowTrack->m_iPosX, pos+m_uHeightButton);
		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_SCROLL_CHANGE_POS, m_uPosScroll);

	}

	void VScroll::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
	{
		Window::size(iSizeX, iSizeY);
		recalcScroll();
	}

	void VScroll::setScrollRange(uint16 uSizeScroll) // устанавливает новый размер скролл
	{
		m_uSizeScroll = uSizeScroll;
		m_uPosScroll = 0;
		if (m_uSizeScroll > 1) { // показываем и разблокируем линейки
			setState(WS_NORMAL);
			if (m_pWindowTrack) m_pWindowTrack->show(true);
		} else { // скрываем ползунок и заблокируем линейки
			setState(WS_DEACTIVE);
			if (m_pWindowTrack) m_pWindowTrack->show(false);
		}
		recalcScroll();
	}

	void VScroll::setScrollPos(uint16 uPosScroll) // устанавливает новую позицию скролла
	{
		if (m_uSizeScroll == 0) return;
		if (uPosScroll >= m_uSizeScroll) m_uPosScroll = m_uSizeScroll-1;
		else m_uPosScroll = uPosScroll;
		recalcScroll();
	}

	void VScroll::recalcScroll()
	{
		if ((m_pWindowTrack) && (m_uSizeScroll > 1)) {
			int16 pos = m_iSizeY - m_uHeightButtonAll;
			pos = pos * m_uPosScroll / (m_uSizeScroll-1);
			m_pWindowTrack->move(m_pWindowTrack->m_iPosX, pos+m_uHeightButton);
		}
	}

	void VScroll::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{

		if (pWindow == m_pWindowTrack) { // ползунок
			if (bIsFocus) pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[SKIN_STATE_ACTIVED]);
			else pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[SKIN_STATE_NORMAL]);
			return;
		}

		uint16 flag = 0;
		if (pWindow->m_uExData & WES_VSCROLL_UP) flag = WES_VSCROLL_UP;
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) flag = WES_VSCROLL_DOWN;

		if (flag != 0) {
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

	MyGUI::VScroll * GUI::createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // создает скролл
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		VScroll * pWindow = new VScroll(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			Window * pChild;
			 // создаем дочернии окна скины
			if (pSkin->subSkins[pos]->exdata & WES_VSCROLL_TRACK) {
				// элемент является скролом, прицепляем к клиентской области
				pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowTrack = pChild;
				pChild->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			}

			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild; // клиентское окно для ползунка
		}
		// размеры кнопок скрола
		pWindow->m_uHeightTrack = __WINDOW_DATA3(pSkin);
		pWindow->m_uHeightButton = __WINDOW_DATA4(pSkin);
		pWindow->m_uHeightButtonAll = (pWindow->m_uHeightButton << 1) + pWindow->m_uHeightTrack;
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::VScroll * Window::createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // создает скролл
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		VScroll * pWindow = new VScroll(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			Window * pChild;
			 // создаем дочернии окна скины
			if (pSkin->subSkins[pos]->exdata & WES_VSCROLL_TRACK) {
				// элемент является скролом, прицепляем к клиентской области
				pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowTrack = pChild;
				pChild->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			}
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild; // клиентское окно для ползунка
		}
		// размеры кнопок скрола
		pWindow->m_uHeightTrack = __WINDOW_DATA3(pSkin);
		pWindow->m_uHeightButton = __WINDOW_DATA4(pSkin);
		pWindow->m_uHeightButtonAll = (pWindow->m_uHeightButton << 1) + pWindow->m_uHeightTrack;
		pWindow->m_uAligin |= uAligin;
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
