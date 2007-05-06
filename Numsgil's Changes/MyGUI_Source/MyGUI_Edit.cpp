#include "MyGUI_Edit.h"
#include "MyGUI_GUI.h"

using namespace Ogre;
using namespace std;

namespace MyGUI {

	class GUI;

	Edit::Edit(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
		m_pWindowCursor(0),
		m_bIsFocus(false),
		m_uOffsetCursor(0)
	{
	}

	void Edit::_OnMouseChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �� �������
	{
		Window::_OnMouseChangeFocus(bIsFocus); // ��� ���������
		if (bIsFocus) GUI::getSingleton()->setMousePointer(POINTER_TEXT);
		else GUI::getSingleton()->setMousePointer(POINTER_DEFAULT);
		showFocus(bIsFocus);
	}

	void Edit::_OnKeyChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �����
	{
		Window::_OnKeyChangeFocus(bIsFocus); // ��� ���������
		m_bIsFocus = bIsFocus;
		if (bIsFocus) {
			setState(WS_PRESSED);
			if (m_iSizeY < m_font->height) bIsFocus = false;
		} else setState(WS_NORMAL);
		if (m_pWindowCursor) m_pWindowCursor->show(bIsFocus);
	}

	void Edit::size(int16 iSizeX, int16 iSizeY) // �������� ������ ����
	{
		Window::size(iSizeX, iSizeY);

		if (!m_pWindowCursor) return;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if ((m_pWindowText->m_iSizeY >= m_font->height) && (m_bIsFocus)) m_pWindowCursor->show(true);
		else if (!m_bIsFocus)  m_pWindowCursor->show(false);
	}

	Edit *Edit::setWindowText(const DisplayString & strText) // ������������� ����� ����
	{
		Window::setWindowText(strText);

		if (!m_pWindowCursor) return this;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if ((m_pWindowText->m_iSizeY >= m_font->height) && (m_bIsFocus)) m_pWindowCursor->show(true);
		else if (!m_bIsFocus)  m_pWindowCursor->show(false);
		return this;
	}

	void Edit::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // ���������� ��� ������� ������� �����
	{
		Window::_OnKeyButtonPressed(keyEvent, cText); // ��� ���������
		if (keyEvent == OIS::KC_ESCAPE) {
			GUI::getSingleton()->setKeyFocus(0); // ����� ���������� �����
			return;
		}
		DisplayString strText = m_pWindowText->getWindowText();
		if (keyEvent == OIS::KC_BACK) {
			uint size = (uint)strText.size();
			if (size > 0) strText.resize(size-1);
			else return;
		} else if ((keyEvent == OIS::KC_RETURN) || (keyEvent == OIS::KC_NUMPADENTER)) { // ������� ����
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_ACCEPT, 0);
			return;
		} else if (cText == 0) return; // ������� ��� �����
		else strText = strText + cText;

		setWindowText(strText);
	}
	
	Edit * Edit::create(const Coord &Pos, const Coord &Size,
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
	{
	    __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		Edit * pWindow = new Edit(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window * pChild;
			if (pSkin->subSkins[pos]->exdata & WES_EDIT_CURSOR) {
				// ������� �������� ��������
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowCursor = pChild;
				pWindow->m_pWindowCursor->show(false);
			} else {
				// ������� �������
				pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			}
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->m_uOffsetCursor = atoi(pSkin->data4.c_str());
		pWindow->m_uAlign |= uAlign;
		
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		return pWindow;
    }
}