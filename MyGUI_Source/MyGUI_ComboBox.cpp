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

	#define __COMBO_CALC_SHOW_LIST { \
		m_pList->size(m_iSizeX, m_pList->m_iSizeY); \
		int16 _iPosY = ((int16)(m_overlayContainer->_getDerivedTop()*m_GUI->m_uHeight)) + 1; \
		if ((_iPosY+m_iSizeY+m_pList->m_iSizeY) >= m_GUI->m_uHeight) m_pList->move(((int16)(m_overlayContainer->_getDerivedLeft()*m_GUI->m_uWidth))+1, _iPosY-m_pList->m_iSizeY); \
		else m_pList->move(((int16)(m_overlayContainer->_getDerivedLeft()*m_GUI->m_uWidth))+1, _iPosY+m_iSizeY); \
	}

	ComboBox::ComboBox(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, gui, uOverlay, pWindowFother),
		m_pList(0),
		m_pEdit(0),
		m_bIsListShow(false),
		m_bHideList(false)
	{
	}

	ComboBox::~ComboBox()
	{
		m_GUI->destroyWindow(m_pList);
	}

	void ComboBox::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow->m_uExData & WES_COMBO) showFocus(bIsFocus);
		else if (pWindow->m_uExData & WES_COMBO_BUTTON) {
			uint8 uSkin;
			if (bIsFocus) uSkin = SKIN_STATE_ACTIVED;
			else uSkin = SKIN_STATE_NORMAL;
			if (pWindow->m_paStrSkins[uSkin]) pWindow->m_overlayContainer->setMaterialName(*pWindow->m_paStrSkins[uSkin]);
		}
	}

	void ComboBox::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) // нажата левая кнопка мыши
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

	void ComboBox::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
	{

		if (pWindow == m_pEdit) return; // окно редактирования

		if (m_bIsListShow) { // странно, сюда никогда не заходит ...
			m_pList->show(false);
			m_bIsListShow = false;
		} else {
			if (m_pList->m_aString.size() != 0) { // показываем если только есть строки
				__COMBO_CALC_SHOW_LIST;

				if (!m_bHideList) {
					m_GUI->setKeyFocus(m_pList);
					m_pList->show(true);
					m_bIsListShow = true;
				} else m_bHideList = false;
			}
		}

		if (!m_bIsListShow) m_GUI->setKeyFocus(this);

	}

	void ComboBox::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса ввода
	{

		if (pWindow == m_pEdit) {
			if (bIsFocus) setState(WS_PRESSED);
			else setState(WS_NORMAL);
			return;
		}

		if (bIsFocus) {
			__COMBO_CALC_SHOW_LIST;
			// отображаем
			m_pList->show(true);
			m_bIsListShow = true;
		} else {
			m_pList->show(false);
			m_bIsListShow = false;
		}
	}

	void ComboBox::onKeyButton(MyGUI::Window * pWindow, int keyEvent, char cText) // нажата клавиша
	{
		if ((keyEvent == KC_UP) || (keyEvent == KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST;
			// отображаем
			m_pList->show(true);
			m_GUI->setKeyFocus(m_pList);
		} else if (keyEvent == KC_ESCAPE) { // при эскейп скрываем список
			m_pList->show(false);
			m_bIsListShow = false;
			if (m_pEdit) m_GUI->setKeyFocus(m_pEdit);
			else m_GUI->setKeyFocus(this);
		}
	}

	void ComboBox::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		Window::_OnKeyButtonPressed(keyEvent, cText);
		if ((keyEvent == KC_UP) || (keyEvent == KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST;
			// отображаем
			m_pList->show(true);
			m_GUI->setKeyFocus(m_pList);
		}
	}

	void ComboBox::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
	{
		if (uEvent == WOE_LIST_SELECT_ACCEPT) {
			setWindowText(m_pList->getString(data));
			m_pList->show(false);
			m_bIsListShow = false;
			if (m_pEdit) {
				// выравнивание курсора
				m_pEdit->size(m_pEdit->m_iSizeX, m_pEdit->m_iSizeY);
				m_GUI->setKeyFocus(m_pEdit);
			} else m_GUI->setKeyFocus(this);
			// отсылаем позицию
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_COMBO_SELECT_ACCEPT, data);
		} else if (uEvent == WOE_EDIT_KEY_ACCEPT) {
			// ентер в поле редактирования, отсылаем 
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_COMBO_SELECT_ACCEPT, ITEM_NON_SELECT);
		} else if (uEvent == WOE_LIST_SELECT_NONE) {
			m_pList->show(false);
			m_bIsListShow = false;
		}
	}

	void ComboBox::setString(uint16 index) // устанавливает строку из списка в поле
	{
		if (index>=m_pList->m_aString.size()) return;
		m_pList->m_uSelectItem = index;
		m_pList->redrawStrings();
		setWindowText(*m_pList->m_aString[index]);
	}

	const DisplayString & ComboBox::getString(uint16 index) // возвращает строку
	{
		// позиция больше, возвращаем строку из едита
		if ( (index >= m_pList->m_aString.size()) && (m_pEdit) ) return m_pEdit->getWindowText();
		return m_pList->getString(index); // внимание, если индекс больше, то вернет адрес статической пустой строки
	}

	MyGUI::ComboBox * GUI::createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		ComboBox * pWindow = new ComboBox(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}

		__ASSERT(__WINDOW_DATA3(pSkin) < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkinTmp = m_windowInfo[__WINDOW_DATA3(pSkin)]; // скин списка
		__ASSERT(__WINDOW_DATA4(pSkinTmp) < __SKIN_COUNT); // низя
		pSkinTmp = m_windowInfo[__WINDOW_DATA4(pSkinTmp)]; // скин кнопок списка
		pWindow->m_pList = createList(300, 100, 300, 300, OVERLAY_POPUP, __WINDOW_DATA3(pSkin));
		pWindow->m_pList->m_pEventCallback = (EventCallback*)pWindow;
		pWindow->m_pList->show(false);
		pWindow->m_pList->m_bIsOneClickActived = true;
		// высота списка по нужному колличеству строк
		pWindow->m_pList->size(300, pSkinTmp->subSkins[0]->sizeY*__WINDOW_DATA4(pSkin) + pWindow->m_pList->m_iSizeY - pWindow->m_pList->m_pWindowClient->m_iSizeY);

		if (__WINDOW_DATA2(pSkin) != 0) { // скин едита
			pWindow->m_pEdit = pWindow->m_pWindowText->createEdit(0, 0, -1, -1, WA_STRETCH, __WINDOW_DATA2(pSkin));
			pWindow->m_pWindowText->m_uEventCallback = WE_NONE;
			pWindow->m_pWindowText = pWindow->m_pEdit;
			pWindow->m_pEdit->m_pEventCallback = (EventCallback*)pWindow;
		}

		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::ComboBox * Window::createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback)
	{
		__ASSERT(uSkin < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		ComboBox * pWindow = new ComboBox(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_TEXT) pWindow->m_pWindowText = pChild;
		}

		__ASSERT(__WINDOW_DATA3(pSkin) < __SKIN_COUNT); // низя
		__LP_MYGUI_WINDOW_INFO pSkinTmp = m_GUI->m_windowInfo[__WINDOW_DATA3(pSkin)]; // скин списка
		__ASSERT(__WINDOW_DATA4(pSkinTmp) < __SKIN_COUNT); // низя
		pSkinTmp = m_GUI->m_windowInfo[__WINDOW_DATA4(pSkinTmp)]; // скин кнопок списка
		pWindow->m_pList = m_GUI->createList(300, 100, 300, 300, OVERLAY_POPUP, __WINDOW_DATA3(pSkin));
		pWindow->m_pList->m_pEventCallback = (EventCallback*)pWindow;
		pWindow->m_pList->show(false);
		pWindow->m_pList->m_bIsOneClickActived = true;
		// высота списка по нужному колличеству строк
		pWindow->m_pList->size(300, pSkinTmp->subSkins[0]->sizeY*__WINDOW_DATA4(pSkin) + pWindow->m_pList->m_iSizeY - pWindow->m_pList->m_pWindowClient->m_iSizeY);

		if (__WINDOW_DATA2(pSkin) != 0) { // скин едита
			pWindow->m_pEdit = pWindow->m_pWindowText->createEdit(0, 0, -1, -1, WA_STRETCH, __WINDOW_DATA2(pSkin));
			pWindow->m_pWindowText->m_uEventCallback = WE_NONE;
			pWindow->m_pWindowText = pWindow->m_pEdit;
			pWindow->m_pEdit->m_pEventCallback = (EventCallback*)pWindow;
		}

		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
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
