#include "MyGUI_ComboBox.h"
#include "MyGUI_GUI.h"
#include "MyGUI_List.h"
#include "MyGUI_Edit.h"
#include "MyGUI_OIS.h"
#include <OgreOverlayContainer.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	void ComboBox::__COMBO_CALC_SHOW_LIST() {
		m_pList->size(Coord(m_iSizeX, m_pList->m_iSizeY));
		int16 _iPosY = ((int16)(m_overlayContainer->_getDerivedTop()*GUI::getSingleton()->m_uHeight)) + 1;
		if ((_iPosY+m_iSizeY+m_pList->m_iSizeY) >= GUI::getSingleton()->m_uHeight) m_pList->move(((int16)(m_overlayContainer->_getDerivedLeft()*GUI::getSingleton()->m_uWidth))+1, _iPosY-m_pList->m_iSizeY);
		else m_pList->move(((int16)(m_overlayContainer->_getDerivedLeft()*GUI::getSingleton()->m_uWidth))+1, _iPosY+m_iSizeY);
	}

	ComboBox::ComboBox(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		Window(lpSkin, uOverlay, pWindowParent),
		m_pList(0),
		m_pEdit(0),
		m_bIsListShow(false),
		m_bHideList(false)
	{
	}

	ComboBox::~ComboBox()
	{
		GUI::getSingleton()->destroyWindow(m_pList);
	}
	
	void ComboBox::setString(size_t index) {
	    if (index>=m_pList->m_aString.size())
	        return;
	    setWindowText(*m_pList->m_aString[index]); 
    }; // устанавливает строку из списка в поле
	
	ComboBox *ComboBox::addString(const Ogre::DisplayString & strElement)
	{
	    m_pList->addString(strElement);
	    return this;
    }; // добавить строку в список
	
	ComboBox *ComboBox::insertString(uint16 index, const DisplayString & strElement)
	{ 
	    m_pList->insertString(index,strElement);
	    return this;
    }; // вставить строку в список
	
	ComboBox *ComboBox::deleteString(uint16 index)
	{ 
	    m_pList->deleteString(index);
	    return this;
    }; // удалить строку из списка

	void ComboBox::onMouseFocus(Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow->m_uExData & WES_COMBO) showFocus(bIsFocus);
		else if (pWindow->m_uExData & WES_COMBO_BUTTON) {
			uint8 uSkin;
			if (bIsFocus) uSkin = SKIN_STATE_ACTIVED;
			else uSkin = SKIN_STATE_NORMAL;
			if (pWindow->m_paStrSkins[uSkin]) pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[uSkin]);
		}
	}

	void ComboBox::onMouseButton(Window * pWindow, bool bIsLeftButtonPressed) // нажата левая кнопка мыши
	{
		if (pWindow == m_pEdit) return; // окно редактирования
		
		if (pWindow->m_uExData & WES_COMBO) {
			if (bIsLeftButtonPressed) setState(WS_PRESSED);
			else setState(WS_NORMAL);
		} else if (pWindow->m_uExData & WES_COMBO_BUTTON) {
			uint8 uSkin;
			if (bIsLeftButtonPressed) uSkin = SKIN_STATE_SELECTED;
			else uSkin = SKIN_STATE_ACTIVED;
			if (pWindow->m_paStrSkins[uSkin]) pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[uSkin]);
		}

		if (bIsLeftButtonPressed) {
			if (m_bHideList) m_bHideList = false; // не было клика
			else if (m_bIsListShow) m_bHideList = true; // лист был виден
		}

	}

	void ComboBox::onMouseClick(Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{

		if (pWindow == m_pEdit) return; // окно редактирования

		if (m_bIsListShow) { // странно, сюда никогда не заходит ...
			m_pList->show(false);
			m_bIsListShow = false;
		} else {
			if (m_pList->m_aString.size() != 0) { // показываем если только есть строки
				__COMBO_CALC_SHOW_LIST();

				if (!m_bHideList) {
					GUI::getSingleton()->setKeyFocus(m_pList);
					m_pList->show(true);
					m_bIsListShow = true;
				} else m_bHideList = false;
			}
		}

		if (!m_bIsListShow) GUI::getSingleton()->setKeyFocus(this);

	}

	void ComboBox::onKeyFocus(Window * pWindow, bool bIsFocus) // смена фокуса ввода
	{

		if (pWindow == m_pEdit) {
			if (bIsFocus) setState(WS_PRESSED);
			else setState(WS_NORMAL);
			return;
		}

		if (bIsFocus) {
			__COMBO_CALC_SHOW_LIST();
			// отображаем
			m_pList->show(true);
			m_bIsListShow = true;
		} else {
			m_pList->show(false);
			m_bIsListShow = false;
		}
	}

	void ComboBox::onKeyButton(Window * pWindow, int keyEvent, char cText) // нажата клавиша
	{
		if ((keyEvent == OIS::KC_UP) || (keyEvent == OIS::KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST();
			// отображаем
			m_pList->show(true);
			GUI::getSingleton()->setKeyFocus(m_pList);
		} else if (keyEvent == OIS::KC_ESCAPE) { // при эскейп скрываем список
			m_pList->show(false);
			m_bIsListShow = false;
			if (m_pEdit) GUI::getSingleton()->setKeyFocus(m_pEdit);
			else GUI::getSingleton()->setKeyFocus(this);
		}
	}
	
	DisplayString ComboBox::getString(uint16 index) // возвращает строку
	{
		// позиция больше, возвращаем строку из едита
		if (index >= m_pList->m_aString.size()) {
			if (m_pEdit) return m_pEdit->getWindowText();
			return "";
		}
		return *m_pList->m_aString[index];
	}

	void ComboBox::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		Window::_OnKeyButtonPressed(keyEvent, cText);
		if ((keyEvent == OIS::KC_UP) || (keyEvent == OIS::KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST();
			// отображаем
			m_pList->show(true);
			GUI::getSingleton()->setKeyFocus(m_pList);
		}
	}

	void ComboBox::onOtherEvent(Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
	{
		if (uEvent == WOE_LIST_SELECT_ACCEPT) {
			setWindowText(m_pList->getString(data));
			m_pList->show(false);
			m_bIsListShow = false;
			if (m_pEdit) {
				// выравнивание курсора
				m_pEdit->size(m_pEdit->m_iSizeX, m_pEdit->m_iSizeY);
				GUI::getSingleton()->setKeyFocus(m_pEdit);
			} else GUI::getSingleton()->setKeyFocus(this);
			// отсылаем позицию
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_COMBO_SELECT_ACCEPT, data);
		} else if (uEvent == WOE_EDIT_KEY_ACCEPT) {
			// ентер в поле редактирования, отсылаем -1
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_COMBO_SELECT_ACCEPT, (uint32)-1);
		} else if (uEvent == WOE_LIST_SELECT_NONE) {
			m_pList->show(false);
			m_bIsListShow = false;
		}
	}
	
	ComboBox * ComboBox::create(const Coord &Pos, const Coord &Size,
	    Window *parent, const uint16 uAlign, const uint8 uOverlay, const String &uSkin)
    {
        __LP_MYGUI_WINDOW_INFO pSkin = SkinManager::getSingleton()->getSkin(uSkin);
		ComboBox * pWindow = new ComboBox(pSkin->subSkins[0], 
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}
		
		__LP_MYGUI_WINDOW_INFO pSkinTmp = SkinManager::getSingleton()->getSkin(pSkin->data3);
		//                       pSkinTmp = SkinManager::getSingleton()->getSkin(pSkinTmp->data4);
		
		pWindow->m_pList = GUI::getSingleton()->spawn<List>(
		    Coord(300, 100),
		    Coord(300, 300), OVERLAY_POPUP, pSkin->data3);
		    
		pWindow->m_pList->m_pEventCallback = (EventCallback*)pWindow;
		pWindow->m_pList->show(false);
		pWindow->m_pList->m_bIsOneClickActived = true;
		// высота списка по нужному колличеству строк
		pWindow->m_pList->size(
		    Coord(300, pSkinTmp->subSkins[0]->sizeY * atoi(pSkin->data4.c_str())
		    + pWindow->m_pList->m_iSizeY
		    - pWindow->m_pList->m_pWindowClient->m_iSizeY));
		    
		if (pSkin->data2 != "") { // скин едита
			pWindow->m_pEdit = pWindow->m_pWindowText->spawn<Edit>(
			    Coord(0, 0), Coord(-1, -1), WA_STRETCH, pSkin->data2);
			pWindow->m_pWindowText->m_uEventCallback = WE_NONE;
			pWindow->m_pWindowText = pWindow->m_pEdit;
			pWindow->m_pEdit->m_pEventCallback = (EventCallback*)pWindow;
		}
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->m_uAlign |= uAlign;
		pWindow->move(Pos.x, Pos.y);
		pWindow->size(Size.x > 0 ? Size.x : pSkin->subSkins[0]->sizeX,  
		              Size.y > 0 ? Size.y : pSkin->subSkins[0]->sizeY);
		return pWindow;
    }
    
    ComboBox *ComboBox::deleteStringAll()
    {
        m_pList->deleteStringAll();
        return this;
    }
    
    size_t ComboBox::getStringCount() const
    {
        return m_pList->getStringCount();
    }
    
    size_t ComboBox::getCurrentIndex() const
    {
        return m_pList->getCurrentIndex();
    }
}