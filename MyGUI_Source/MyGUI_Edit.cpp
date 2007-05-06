//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
using namespace OIS;
//=========================================================================================
namespace MyGUI {

	class GUI;

	Edit::Edit(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_pWindowCursor(0),
		m_bIsFocus(false),
		m_uOffsetCursor(0)
	{
	}

	void Edit::_OnMouseChangeFocus(bool bIsFocus) // вызывается при смене активности от курсора
	{
		Window::_OnMouseChangeFocus(bIsFocus); // для каллбеков
		if (bIsFocus) m_GUI->setMousePointer(POINTER_TEXT);
		else m_GUI->setMousePointer(POINTER_DEFAULT);
		showFocus(bIsFocus);
	}

	void Edit::_OnKeyChangeFocus(bool bIsFocus) // вызывается при смене активности ввода
	{
		Window::_OnKeyChangeFocus(bIsFocus); // для каллбеков
		m_bIsFocus = bIsFocus;
		if (bIsFocus) {
			setState(WS_PRESSED);
			if (m_iSizeY < m_font->height) bIsFocus = false;
		} else setState(WS_NORMAL);
		if (m_pWindowCursor) m_pWindowCursor->show(bIsFocus);
	}

	void Edit::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
	{
		Window::size(iSizeX, iSizeY);

		if (!m_pWindowCursor) return;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if ((m_pWindowText->m_iSizeY >= m_font->height) && (m_bIsFocus)) m_pWindowCursor->show(true);
		else if (!m_bIsFocus)  m_pWindowCursor->show(false);
	}

	void Edit::setWindowText(const DisplayString & strText) // устанавливает текст окна
	{
		Window::setWindowText(strText);

		if (!m_pWindowCursor) return;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if ((m_pWindowText->m_iSizeY >= m_font->height) && (m_bIsFocus)) m_pWindowCursor->show(true);
		else if (!m_bIsFocus)  m_pWindowCursor->show(false);
	}

	void Edit::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		Window::_OnKeyButtonPressed(keyEvent, cText); // для каллбеков
		if (keyEvent == KC_ESCAPE) {
			m_GUI->setKeyFocus(0); // сброс активности ввода
			return;
		}
		DisplayString strText = m_pWindowText->getWindowText();
		if (keyEvent == KC_BACK) {
			uint size = (uint)strText.size();
			if (size > 0) strText.resize(size-1);
			else return;
		} else if ((keyEvent == KC_RETURN) || (keyEvent == KC_NUMPADENTER)) { // клавиша ввод
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_ACCEPT, 0);
			return;
		} else if (keyEvent == OIS::KC_DELETE) {
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_DELETE, 0);
			return;
		} else if (cText == 0) return; // клавиша без знака
		else strText = strText + cText;

		setWindowText(strText);
	}

	MyGUI::Edit * GUI::createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // создает окно редактирования
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		Edit * pWindow = new Edit(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild;
			if (pSkin->subSkins[pos]->exdata & WES_EDIT_CURSOR) {
				// элемент является курсором
				pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowCursor = pChild;
				pWindow->m_pWindowCursor->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			}
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->m_uOffsetCursor = __WINDOW_DATA4(pSkin);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::Edit * Window::createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // создает окно редактирования
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		Edit * pWindow = new Edit(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild;
			if (pSkin->subSkins[pos]->exdata & WES_EDIT_CURSOR) {
				// элемент является курсором
				pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowCursor = pChild;
				pWindow->m_pWindowCursor->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			}
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->m_uOffsetCursor = __WINDOW_DATA4(pSkin);
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
