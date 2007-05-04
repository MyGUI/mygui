//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	Button::Button(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother)
	{
	}

	void Button::_OnMouseChangeFocus(bool bIsFocus) // вызывается при смене активности от курсора
	{
		Window::_OnMouseChangeFocus(bIsFocus); // для посылки калбэков
		showFocus(bIsFocus);
	}

	void Button::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // вызывается при нажатии клавиши
	{
		Window::_OnMouseButtonPressed(bIsLeftButtonPressed); // для посылки калбэков
		showPressed(bIsLeftButtonPressed);
	}

	void Button::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow->m_uExData & WES_BUTTON) showFocus(bIsFocus);
	}

	void Button::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) // нажата левая кнопка мыши
	{
		if (pWindow->m_uExData & WES_BUTTON) showPressed(bIsLeftButtonPressed);
	}

	void Button::showPressed(bool bIsPressed) // показ но не смена нажатия
	{
		if (m_uState == WS_DEACTIVE) return;
		uint8 uSkin = __SKIN_COUNT;
		bool bIsShiftText = false;

		if (bIsPressed) {
			if (m_uState == WS_NORMAL) {
				uSkin = SKIN_STATE_PRESSED;
			} else if (m_uState == __WS_ACTIVED) {
				uSkin = SKIN_STATE_SELECTED;
			} else return;
			bIsShiftText = true;
		} else {
			if (m_uState == WS_NORMAL) { // возвращаем скин на место
				uSkin = SKIN_STATE_NORMAL;
			} else if (m_uState == __WS_ACTIVED) { // возвращаем скин на место
				uSkin = SKIN_STATE_ACTIVED;
			} else return;
		}

		if (uSkin != __SKIN_COUNT) { // меняем скины состояний
			// основное окно
			if (m_uExData & WES_BUTTON) {
				if (m_paStrSkins[uSkin]) m_overlayContainer->setMaterialName(*m_paStrSkins[uSkin]);
			}
			// детишки
			for (uint i=0; i<m_aWindowChild.size(); i++) {
				Window * pChild = m_aWindowChild[i];
				if (pChild->m_uExData & WES_BUTTON) {
					if (pChild->m_paStrSkins[uSkin]) pChild->m_overlayContainer->setMaterialName(*pChild->m_paStrSkins[uSkin]);
				}
			}
		}

		if (bIsShiftText != m_pWindowText->m_bIsTextShiftPressed) { // сдвиг текста
			m_pWindowText->m_bIsTextShiftPressed = bIsShiftText;
			if (m_pWindowText->m_uAligin & WAT_SHIFT_TEXT_PRESSED) {
				if (m_pWindowText->m_overlayCaption) {
					if (m_pWindowText->m_bIsTextShiftPressed) m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()+__GUI_BUTTON_SHIFT_TEXT_PRESSED);
					else m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()-__GUI_BUTTON_SHIFT_TEXT_PRESSED);
				}
			}
		}

	}

	MyGUI::Button * GUI::createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // создает кнопку
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		Button * pWindow = new Button(pSkin->subSkins[0], this, uOverlay, 0);
		pWindow->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::Button * Window::createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // создает кнопку
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		Button * pWindow = new Button(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		pWindow->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		pWindow->m_uAligin |= uAligin;
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->setWindowText(strWindowText);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

}
//=========================================================================================
