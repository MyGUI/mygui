#include "MyGUI_VScroll.h"
#include "MyGUI_GUI.h"
#include <OgreOverlayContainer.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	class GUI;

	VScroll::VScroll(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
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
		int16 pos = GUI::getSingleton()->m_overlayContainerMouse->getTop() + 
		    SkinManager::getSingleton()->getPointer(GUI::getSingleton()->uPointerType)->iOffsetY - m_iRealMousePosY;
		// границы
		if (pos < m_uHeightButton) pos = m_uHeightButton;
		else if (pos > (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButton - m_uHeightTrack)) {
			pos = (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButton - m_uHeightTrack);
		}
		// передвигаем
		m_pWindowTrack->move(m_pWindowTrack->m_iPosX, pos);
		// если позиция изменилась, то отсылаем сообщение
		int16 track = ((pos-m_uHeightButton) * (m_uSizeScroll)) / (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButtonAll);
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
			m_iRealMousePosY = GUI::getSingleton()->m_overlayContainerMouse->getTop() + 
			    SkinManager::getSingleton()->getPointer(GUI::getSingleton()->uPointerType)->iOffsetY - 
			    m_pWindowTrack->m_iPosY;
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
		int16 pos = m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButtonAll;
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
	
	VScroll *VScroll::create(const Coord &Pos, const Coord &Size, 
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
    {
        __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		VScroll * pWindow = new VScroll(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
        
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			Window * pChild;
			 // создаем дочернии окна скины
			if (pSkin->subSkins[pos]->exdata & WES_VSCROLL_TRACK) {
				// элемент является скролом, прицепляем к клиентской области
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowTrack = pChild;
				pChild->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			}

			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild; // клиентское окно для ползунка
		}
		// размеры кнопок скрола
		pWindow->m_uHeightTrack = atoi(pSkin->data3.c_str());
		pWindow->m_uHeightButton = atoi(pSkin->data4.c_str());
		pWindow->m_uHeightButtonAll = (pWindow->m_uHeightButton << 1) + pWindow->m_uHeightTrack;
		pWindow->m_uAlign |= uAlign;
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		return pWindow;    
    }
}