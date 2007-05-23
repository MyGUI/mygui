#include "MyGUI_Edit.h"
#include "MyGUI_OIS.h"
#include "MyGUI_GUI.h"
#include "MyGUI_AssetManager.h"
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {

	class GUI;

	Edit::Edit(const __MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent) :
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
			if (m_iSizeY < getFont()->height)
			    bIsFocus = false;
		} else setState(WS_NORMAL);
		if (m_pWindowCursor) m_pWindowCursor->show(bIsFocus);
	}

	void Edit::size(int16 iSizeX, int16 iSizeY) // �������� ������ ����
	{
		Window::size(iSizeX, iSizeY);

		if (!m_pWindowCursor) return;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if (m_pWindowText->m_iSizeY >= getFont()->height)
		{
		    if(m_bIsFocus)
		        m_pWindowCursor->show(true);
        }
		else if (!m_bIsFocus)  m_pWindowCursor->show(false);
	}

	Window *Edit::setCaption(const DisplayString & strText) // ������������� ����� ����
	{
		Window::setCaption(strText);
		
		const __tag_MYGUI_FONT_INFO *font = getFont();

		if (!m_pWindowCursor) return this;
		m_pWindowCursor->move(m_pWindowText->m_sizeCutTextX + m_uOffsetCursor+__GUI_FONT_HOFFSET, m_pWindowCursor->m_iPosY);
		if ((m_pWindowText->m_iSizeY >= font->height) && (m_bIsFocus))
		    m_pWindowCursor->show();
		else if (!m_bIsFocus)
		    m_pWindowCursor->hide();
        
        return this;
	}

	void Edit::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // ���������� ��� ������� ������� �����
	{
		Window::_OnKeyButtonPressed(keyEvent, cText); // ��� ���������
		if (keyEvent == KC_ESCAPE) {
			GUI::getSingleton()->setKeyFocus(0); // ����� ���������� �����
			return;
		}
		DisplayString strText = m_pWindowText->getCaption();
		if (keyEvent == KC_BACK) {
			uint size = (uint)strText.size();
			if (size > 0) strText.resize(size-1);
			else return;
		} else if ((keyEvent == KC_RETURN) || (keyEvent == KC_NUMPADENTER)) { // ������� ����
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_ACCEPT, 0);
			return;
		} else if (keyEvent == OIS::KC_DELETE) {
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_DELETE, 0);
			return;
		} else if (cText == 0) return; // ������� ��� �����
		else strText = strText + cText;

		setCaption(strText);
	}
	
	Edit *Edit::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
    {
        const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
        
        if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
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
		pWindow->m_uOffsetCursor = StringConverter::parseInt(pSkin->data4);
		pWindow->m_uAlign |= uAlign;
		
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		return pWindow;
    }
}
//=========================================================================================
