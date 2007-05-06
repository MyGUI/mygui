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

	void VScroll::onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText) // ������ �������
	{
		// ������ �������� ����
		if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
	}
	void VScroll::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // ����� ������ �����
	{
		// ������ �������� ����
		if (m_pEventCallback) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void VScroll::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) // ����������� � ��������, �� �� ��������
	{
		if (m_uSizeScroll < 2) return;
		if (!m_pWindowTrack) return;
		// ������ ������� ��������� �������
		int16 pos = GUI::getSingleton()->m_overlayContainerMouse->getTop() + 
		    SkinManager::getSingleton()->getPointer(GUI::getSingleton()->uPointerType)->iOffsetY - m_iRealMousePosY;
		// �������
		if (pos < m_uHeightButton) pos = m_uHeightButton;
		else if (pos > (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButton - m_uHeightTrack)) {
			pos = (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButton - m_uHeightTrack);
		}
		// �����������
		m_pWindowTrack->move(m_pWindowTrack->m_iPosX, pos);
		// ���� ������� ����������, �� �������� ���������
		int16 track = ((pos-m_uHeightButton) * (m_uSizeScroll)) / (m_pWindowTrack->m_pWindowParent->m_iSizeY - m_uHeightButtonAll);
		if (track == m_uSizeScroll) track --;
		if (track != m_uPosScroll) {
			m_uPosScroll = track;
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_SCROLL_CHANGE_POS, m_uPosScroll);
		}

	}

	void VScroll::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed)
	{

		if (pWindow == m_pWindowTrack) { // ��������
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

	void VScroll::onMouseClick(MyGUI::Window * pWindow) // ������ � �������� ����� ������ ���� �� ���� �� ��������
	{
		if (pWindow->m_uExData & WES_VSCROLL_UP) changePosition(true);
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) changePosition(false);
	}

	void VScroll::changePosition(bool bIsUp) // ������� �������, ���������� � ������� �����������
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

	void VScroll::size(int16 iSizeX, int16 iSizeY) // �������� ������ ����
	{
		Window::size(iSizeX, iSizeY);
		recalcScroll();
	}

	void VScroll::setScrollRange(uint16 uSizeScroll) // ������������� ����� ������ ������
	{
		m_uSizeScroll = uSizeScroll;
		m_uPosScroll = 0;
		if (m_uSizeScroll > 1) { // ���������� � ������������ �������
			setState(WS_NORMAL);
			if (m_pWindowTrack) m_pWindowTrack->show(true);
		} else { // �������� �������� � ����������� �������
			setState(WS_DEACTIVE);
			if (m_pWindowTrack) m_pWindowTrack->show(false);
		}
		recalcScroll();
	}

	void VScroll::setScrollPos(uint16 uPosScroll) // ������������� ����� ������� �������
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

	void VScroll::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // ����� ������
	{

		if (pWindow == m_pWindowTrack) { // ��������
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
			 // ������� �������� ���� �����
			if (pSkin->subSkins[pos]->exdata & WES_VSCROLL_TRACK) {
				// ������� �������� �������, ���������� � ���������� �������
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowTrack = pChild;
				pChild->show(false);
			} else {
				// ������� �������
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			}

			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild; // ���������� ���� ��� ��������
		}
		// ������� ������ ������
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