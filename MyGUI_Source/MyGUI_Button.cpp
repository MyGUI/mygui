#include "MyGUI_Button.h"
#include "MyGUI_GUI.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	class GUI;

	Button::Button(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent)
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
			if (m_pWindowText->m_uAlign & WAT_SHIFT_TEXT_PRESSED) {
				if (m_pWindowText->m_overlayCaption) {
					if (m_pWindowText->m_bIsTextShiftPressed) m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()+__GUI_BUTTON_SHIFT_TEXT_PRESSED);
					else m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()-__GUI_BUTTON_SHIFT_TEXT_PRESSED);
				}
			}
		}

	}

	Button *Button::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, uint8 uSkin)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = GUI::getSingleton()->m_windowInfo[uSkin];
		
		Button * pWindow = new Button(pSkin->subSkins[0], parent ? OVERLAY_CHILD : uOverlay, parent ? parent->m_pWindowClient : NULL);
		
		pWindow->m_uEventCallback |= WE_MOUSE_BUTTON | WE_KEY_BUTTON;
		for (size_t pos = 1; pos < pSkin->subSkins.size(); ++pos) {
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT)
			    pWindow->m_pWindowText = pChild;
		}		
		
		pWindow->m_uAlign |= uAlign;
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		
		return pWindow;
		
	}

}