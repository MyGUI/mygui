#include "MyGUI_VScroll.h"
#include "MyGUI_GUI.h"
#include "MyGUI_AssetManager.h"
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

    VScroll::VScroll(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
        
      : Window(AssetManager::getSingleton()->Skins()->getDefinition(Skin)->subSkins[0],
            parent ? OVERLAY_CHILD              : uOverlay,
		    parent ? parent->m_pWindowClient    : NULL),
        m_pWindowTrack(0),
		m_uSizeScroll(0),
		m_uPosScroll(0)
	{
		setState(WS_DEACTIVATED);
		
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			Window * pChild;
			 // создаем дочернии окна скины
			if (pSkin->subSkins[pos]->exdata & WES_VSCROLL_TRACK) {
				// элемент является скролом, прицепляем к клиентской области
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, this->m_pWindowClient);
				this->m_pWindowTrack = pChild;
				pChild->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, this);
			}

			pChild->m_pEventCallback = (EventCallback*)this;
			if (pChild->m_uExData & WES_CLIENT) this->m_pWindowClient = pChild; // клиентское окно для ползунка
		}
		// размеры кнопок скрола
		this->m_uHeightTrack = StringConverter::parseInt(pSkin->data3);
		this->m_uHeightButton = StringConverter::parseInt(pSkin->data4);
		this->m_uHeightButtonAll = (this->m_uHeightButton * 2) + this->m_uHeightTrack;
		this->m_uAlign |= uAlign;
		this->move(PosX, PosY);
		this->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		this->setFont(pSkin->fontWindow, pSkin->colour);
	}

	void VScroll::onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) // нажата клавиша
	{
		// просто передаем отцу
		if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
	}
	void VScroll::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса ввода
	{
		// просто передаем отцу
		if (m_pEventCallback) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void VScroll::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iParentPosX, int16 iParentPosY) // уведомление о движении, но не движение
	{
		if (m_uSizeScroll < 2) return;
		if (!m_pWindowTrack) return;
		// узнаем текущее положение курсора
		int16 pos = GUI::getSingleton()->m_overlayContainerMouse->getTop() + 
		            GUI::getSingleton()->m_iCurrentOffsetCursorY - m_iRealMousePosY;
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
			if (bIsLeftButtonPressed)
			    pWindow->setSkinState(SKIN_STATE_SELECTED);
			else {
				pWindow->setSkinState(SKIN_STATE_ACTIVE);
				setScrollPos(m_uPosScroll);
			}
			m_iRealMousePosY = GUI::getSingleton()->m_overlayContainerMouse->getTop() + 
			    GUI::getSingleton()->m_iCurrentOffsetCursorY - m_pWindowTrack->m_iPosY;
			return;
		}

		uint16 flag = 0;
		if (pWindow->m_uExData & WES_VSCROLL_UP) flag = WES_VSCROLL_UP;
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) flag = WES_VSCROLL_DOWN;

		if (flag != 0) {
			__SKIN_STATES Skin = SKIN_STATE_ACTIVE;
			if (bIsLeftButtonPressed) Skin = SKIN_STATE_SELECTED;
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & flag)
				    pChild->setSkinState(Skin);
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
			setState(WS_DEACTIVATED);
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
			if (bIsFocus)
			    pWindow->setSkinState(SKIN_STATE_ACTIVE);
			else
			    pWindow->setSkinState(SKIN_STATE_NORMAL);
			return;
		}

		uint16 flag = 0;
		if (pWindow->m_uExData & WES_VSCROLL_UP) flag = WES_VSCROLL_UP;
		else if (pWindow->m_uExData & WES_VSCROLL_DOWN) flag = WES_VSCROLL_DOWN;

		if (flag != 0) {
			__SKIN_STATES Skin = SKIN_STATE_NORMAL;
			
			if (bIsFocus)
			    Skin = SKIN_STATE_ACTIVE;
			
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & flag)
					pChild->setSkinState(Skin);
			}
		}
	}
}