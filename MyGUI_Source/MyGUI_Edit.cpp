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

	Edit::Edit(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, strMaterialElement, uOverlay, pWindowParent),
		m_pWindowCursor(0),
		m_bIsFocus(false),
		m_uOffsetCursor(0)
	{
	}

	void Edit::_OnMouseSetFocus(MyGUI::Window * pWindowOld) // вызывается при смене активности от курсора
	{
		Window::_OnMouseSetFocus(pWindowOld); // для каллбеков
		GUI::getSingleton()->setMousePointer(POINTER_TEXT);
		showFocus(true);
	}

	void Edit::_OnMouseLostFocus(MyGUI::Window * pWindowNew) // вызывается при смене активности от курсора
	{
		Window::_OnMouseSetFocus(pWindowNew); // для каллбеков
		GUI::getSingleton()->setMousePointer(POINTER_DEFAULT);
		showFocus(false);
	}

	void Edit::_OnKeySetFocus(MyGUI::Window * pWindowOld) // вызывается при смене активности ввода
	{
		Window::_OnKeySetFocus(pWindowOld); // для каллбеков
		m_bIsFocus = true;
		setState(WS_PRESSED);
		if (m_pWindowCursor) m_pWindowCursor->show( ! (m_iSizeY < getFont()->height));
	}

	void Edit::_OnKeyLostFocus(MyGUI::Window * pWindowNew) // вызывается при смене активности ввода
	{
		Window::_OnKeyLostFocus(false); // для каллбеков
		m_bIsFocus = false;
		setState(WS_NORMAL);
		if (m_pWindowCursor) m_pWindowCursor->show(false);
	}

	void Edit::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
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

	Window *Edit::setCaption(const DisplayString & strText) // устанавливает текст окна
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

	void Edit::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		Window::_OnKeyButtonPressed(keyEvent, cText); // для каллбеков
		if (keyEvent == KC_ESCAPE) {
			GUI::getSingleton()->setKeyFocus(0); // сброс активности ввода
			return;
		}
		DisplayString strText = m_pWindowText->getCaption();
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

		setCaption(strText);
	}
	
	Edit *Edit::createWindow(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
    {
        const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
        
        if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		Edit * pWindow = new Edit(pSkin->subSkins[0],
			pSkin->SkinElement,
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild;
			if (pSkin->subSkins[pos]->exdata & WES_EDIT_CURSOR) {
				// элемент является курсором
				pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow->m_pWindowClient);
				pWindow->m_pWindowCursor = pChild;
				pWindow->m_pWindowCursor->show(false);
			} else {
				// обычный элемент
				pChild = new Window(pSkin->subSkins[pos], pSkin->SkinElement, OVERLAY_CHILD, pWindow);
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
