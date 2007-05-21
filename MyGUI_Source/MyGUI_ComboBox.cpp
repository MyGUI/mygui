#include "MyGUI_ComboBox.h"
#include "MyGUI_OIS.h"
#include "MyGUI_GUI.h"
#include "MyGUI_Edit.h"
#include "MyGUI_AssetManager.h"
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {
    
    //Ctor
    ComboBox::ComboBox(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	  
	  : Window(AssetManager::getSingleton()->Skins()->getDefinition(Skin)->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL),
	  
	    m_pList(0),
		m_pEdit(0),
		m_bIsListShow(false),
		m_bHideList(false)
	{
	    
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочернии окна скины
			Window * pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, this);
			pChild->m_pEventCallback = (EventCallback*)this;
			if (pChild->m_uExData & WES_TEXT)
			    this->setWindowText(pChild);
		}
		
		const __tag_MYGUI_SKIN_INFO * pSkinTmpA = AssetManager::getSingleton()->Skins()->getDefinition(pSkin->data3);
		const __tag_MYGUI_SKIN_INFO * pSkinTmp  = AssetManager::getSingleton()->Skins()->getDefinition(pSkinTmpA->data4); // скин кнопок списка
		
		this->m_pList = GUI::getSingleton()->spawn<List>(
		    300, 300, 300, 300, OVERLAY_POPUP, pSkin->data3);
		    
		this->m_pList->m_pEventCallback = (EventCallback*)this;
		this->m_pList->show(false);
		this->m_pList->m_bIsOneClickActived = true;
		// высота списка по нужному колличеству строк
		this->m_pList->size(
		    300, pSkinTmp->subSkins[0]->sizeY * StringConverter::parseInt(pSkin->data4)
		    + this->m_pList->m_iSizeY
		    - this->m_pList->m_pWindowClient->m_iSizeY);
		    
		if (!pSkin->data2.empty()) { // скин едита
			this->m_pEdit = this->m_pWindowText->spawn<Edit>(
			    0, 0, -1, -1, WA_STRETCH, pSkin->data2);
			this->m_pWindowText->m_uEventCallback = WE_NONE;
			this->m_pWindowText = this->m_pEdit;
			this->m_pEdit->m_pEventCallback = (EventCallback*)this;
		}
		this->setFont(pSkin->fontWindow, pSkin->colour);
		this->m_uAlign |= uAlign;
		this->move(PosX, PosY);
		this->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		           SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
	}

	void ComboBox::__COMBO_CALC_SHOW_LIST() {
		
		m_pList->size(m_iSizeX, m_pList->m_iSizeY);
		
		const unsigned int _iPosY = m_overlayContainer->_getDerivedTop()*GUI::getSingleton()->getHeight() + 1;
		const unsigned int MoveX  = m_overlayContainer->_getDerivedLeft()*GUI::getSingleton()->getWidth() + 1;
		
		if ((_iPosY+m_iSizeY+m_pList->m_iSizeY) >= GUI::getSingleton()->getHeight())
		    m_pList->move(MoveX, _iPosY-m_pList->m_iSizeY);
		else
		    m_pList->move(MoveX, _iPosY+m_iSizeY);
	}

	ComboBox::~ComboBox()
	{
		GUI::getSingleton()->destroyWindow(m_pList);
	}

	void ComboBox::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (pWindow->m_uExData & WES_COMBO)
		    showFocus(bIsFocus);
		else if (pWindow->m_uExData & WES_COMBO_BUTTON)
			pWindow->setSkinState(bIsFocus ? SKIN_STATE_ACTIVE : SKIN_STATE_NORMAL);
	}

	void ComboBox::onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) // нажата левая кнопка мыши
	{
		if (pWindow == m_pEdit) return; // окно редактирования
		
		if (pWindow->m_uExData & WES_COMBO) {
			if (bIsLeftButtonPressed)
			    setState(WS_PRESSED);
			else
			    setState(WS_NORMAL);
		} else if (pWindow->m_uExData & WES_COMBO_BUTTON)
			pWindow->setSkinState(bIsLeftButtonPressed ? SKIN_STATE_SELECTED : SKIN_STATE_ACTIVE);

		if (bIsLeftButtonPressed) {
		    if (m_bHideList)
		        m_bHideList = false; // не было клика
			else if (m_bIsListShow)
			    m_bHideList = true; // лист был виден
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

	void ComboBox::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса ввода
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

	void ComboBox::onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) // нажата клавиша
	{
		if ((keyEvent == KC_UP) || (keyEvent == KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST();
			// отображаем
			m_pList->show(true);
			GUI::getSingleton()->setKeyFocus(m_pList);
		} else if (keyEvent == KC_ESCAPE) { // при эскейп скрываем список
			m_pList->show(false);
			m_bIsListShow = false;
			if (m_pEdit) GUI::getSingleton()->setKeyFocus(m_pEdit);
			else GUI::getSingleton()->setKeyFocus(this);
		}
	}

	void ComboBox::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		Window::_OnKeyButtonPressed(keyEvent, cText);
		if ((keyEvent == KC_UP) || (keyEvent == KC_DOWN)) { // при нажатии вверх или вниз, показываем список
			if (m_pList->m_aString.size() == 0) return; // показываем если только есть строки
			__COMBO_CALC_SHOW_LIST();
			// отображаем
			m_pList->show(true);
			GUI::getSingleton()->setKeyFocus(m_pList);
		}
	}

	void ComboBox::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
	{
		if (uEvent == WOE_LIST_SELECT_ACCEPT) {
			setCaption(m_pList->getString(data));
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
			// ентер в поле редактирования, отсылаем 
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_COMBO_SELECT_ACCEPT, ITEM_NON_SELECT);
		} else if (uEvent == WOE_EDIT_KEY_DELETE) {
			// делит в поле редактирования, отсылаем 
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_EDIT_KEY_DELETE, ITEM_NON_SELECT);
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
		setCaption(*m_pList->m_aString[index]);
	}

	const DisplayString & ComboBox::getString(uint16 index) // возвращает строку
	{
		// позиция больше, возвращаем строку из едита
		if ( (index >= m_pList->m_aString.size()) && (m_pEdit) ) return m_pEdit->getCaption();
		return m_pList->getString(index); // внимание, если индекс больше, то вернет адрес статической пустой строки
	}

	Window *ComboBox::setCaption(const DisplayString & strText)
	{
		if (m_pEdit) m_pEdit->setCaption(strText);
		else Window::setCaption(strText);
		m_pList->m_uSelectItem = ITEM_NON_SELECT;
		return this;
	}
}