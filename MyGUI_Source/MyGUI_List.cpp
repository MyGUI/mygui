#include "MyGUI_List.h"
#include "MyGUI_OIS.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_AssetManager.h"
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {

	List::List(const __MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowFother) :
		Window(lpSkin, uOverlay, pWindowFother),
		m_scroll(0),
		m_SkinButton("SKIN_DEFAULT"),
		m_uSizeYButton(1),
		m_uSizeXScroll(0),
		m_uCurrentFillSize(0),
		m_uStartWindow(0),
		m_uCountVisible(0),
		m_uOffsetDrawString(0),
		m_uSelectItem(ITEM_NON_SELECT),
		pWindowSelect(0),
		m_bIsFocus(false),
		m_bIsVisibleScroll(false),
		m_bIsOneClickActived(false)
	{
	}

	List::~List()
	{
		for (uint16 pos=0; pos <(uint16)m_aString.size(); pos++) delete m_aString[pos];
	}


	void List::onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) // нажата клавиша
	{
		uint16 oldPos = m_uSelectItem;
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;

		if (keyEvent == KC_WEBFORWARD) { // скроллинг
			if (size <= m_uCountVisible) return;
			m_uOffsetDrawString ++;
			if ((m_uOffsetDrawString + m_uCountVisible) >= size) m_uOffsetDrawString = size - m_uCountVisible;
			redrawStrings();
			recalcScroll();
			return;
		} else if (keyEvent == KC_WEBBACK) { // скроллинг
			if (size <= m_uCountVisible) return;
			if (m_uOffsetDrawString > 0) m_uOffsetDrawString --;
			redrawStrings();
			recalcScroll();
			return;
		}

		if (keyEvent == KC_ESCAPE) { // эскейп обрабатываем отдельно
			if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
			return;
		}

		if (m_uSelectItem == ITEM_NON_SELECT) m_uSelectItem = 0;
		else if (keyEvent == KC_UP) {
			if (m_uSelectItem > 0) m_uSelectItem --;
		} else if (keyEvent == KC_DOWN) {
			if ((m_uSelectItem + 1) < size) m_uSelectItem ++;
		} else if (keyEvent == KC_HOME) {
			m_uSelectItem = 0;
		} else if (keyEvent == KC_END) {
			m_uSelectItem = size - 1;
		} else if (keyEvent == KC_PGUP) {
			int16 pos = (int16)m_uSelectItem - (int16)m_uCountVisible;
			if (pos < 0) m_uSelectItem = 0;
			else m_uSelectItem = (uint16)pos;
		} else if (keyEvent == KC_PGDOWN) {
			m_uSelectItem += m_uCountVisible;
			if (m_uSelectItem >= size) m_uSelectItem = size - 1;
		} else if ((keyEvent == KC_RETURN) || (keyEvent == KC_NUMPADENTER)) { // клавиша ввод
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
			return;
		} else {
			// пусть отец знает, что нажато
			if (m_pEventCallback) m_pEventCallback->onKeyButton(this, keyEvent, cText);
			return; // непонятная клавиша
		}
		// если элемент не виден, то его показываем
		if (pWindowSelect) { // сброс выделения
			pWindowSelect->setState(WS_NORMAL);
			pWindowSelect = 0;
		}
		if ((m_uSelectItem >= m_uOffsetDrawString) && (m_uSelectItem < (m_uOffsetDrawString + m_uCountVisible))) {
			// элемент виден
			pWindowSelect = m_pWindowClient->mChildWindows[m_uSelectItem-m_uOffsetDrawString+m_uStartWindow];
			if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
		} else {// элемент не виден
			if (m_uSelectItem > (m_uCountVisible >> 1)) m_uOffsetDrawString = m_uSelectItem - (m_uCountVisible >> 1);
			else m_uOffsetDrawString = m_uSelectItem;
			if ((m_uOffsetDrawString + m_uCountVisible) >= size) m_uOffsetDrawString = size - m_uCountVisible;
			redrawStrings();
			recalcScroll();
		}
		// посылаем изменения
		if ((m_pEventCallback) && (oldPos != m_uSelectItem)) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_CHANGE, m_uSelectItem);

	}

	void List::_OnKeyChangeFocus(bool bIsFocus) // вызывается при смене активности ввода
	{
		onKeyFocus(this, bIsFocus);
	}

	void List::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // вызывается при нажатии клавиши клавы
	{
		onKeyButton(this, keyEvent, cText);
	}

	void List::onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса ввода
	{
		if (!bIsFocus) {
			if (pWindowSelect) { // сброс выделения
				pWindowSelect->setState(WS_NORMAL);
			}
			m_bIsFocus = false;
		} else {
			if (pWindowSelect) {
				pWindowSelect->setState(WS_PRESSED);
			}
			m_bIsFocus = true;
		}
		if (m_pEventCallback) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void List::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
	{
		if (bIsFocus) {
			if ((pWindow->m_uUserData + m_uOffsetDrawString) >= m_aString.size()) return;
			pWindow->showFocus(true);
		} else  pWindow->showFocus(false);
		
	}

	void List::onMouseClick(MyGUI::Window * pWindow)
	{
		uint16 oldPos = m_uSelectItem;

		if (pWindowSelect) pWindowSelect->setState(WS_NORMAL);
		uint16 item = pWindow->m_uUserData + m_uOffsetDrawString;
		if (item >= m_aString.size()) {
			m_uSelectItem = ITEM_NON_SELECT;
			if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_NONE, 0);
			return;
		}
		pWindowSelect = pWindow;
		pWindowSelect->setState(WS_PRESSED);
		m_uSelectItem = item;
		// посылаем изменения
		if (m_pEventCallback) {
			if (oldPos != m_uSelectItem) {
				if (m_bIsOneClickActived) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
				else  m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_CHANGE, m_uSelectItem);
			} else m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_NONE, 0); // для составных элементов эта информация тоже нужна
		}
	}

	void List::onMouseDoubleClick(MyGUI::Window * pWindow) // двойной щелчек мыши
	{
		if (m_uSelectItem == ITEM_NON_SELECT) return;
		if (m_bIsOneClickActived) return; // уже посылалось при однократном щелчке
		if (m_pEventCallback) m_pEventCallback->onOtherEvent(this, WOE_LIST_SELECT_ACCEPT, m_uSelectItem);
	}

	void List::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
	{
		if (uEvent == WOE_SCROLL_CHANGE_POS) {
			m_uOffsetDrawString = data;
			redrawStrings();
		}
	}

	void List::size(int16 iSizeX, int16 iSizeY) // изменяем размер окна
	{
		int16 oldSizeY = m_iSizeY;
		Window::size(iSizeX, iSizeY);

		if (oldSizeY == m_iSizeY) return; // высота списка не изменилясь

		while (m_uCurrentFillSize < m_pWindowClient->m_iSizeY) {
			int16 cutSize = m_uSizeXScroll;
			if (!m_bIsVisibleScroll) cutSize = 0; // отрезать ничего не надо
			Window * pChild = m_pWindowClient->spawn<Window>(
			    0, m_uCurrentFillSize, m_pWindowClient->m_iSizeX - cutSize, m_uSizeYButton,
			    WA_TOP|WA_HSTRETCH, m_SkinButton);
			pChild->m_pEventCallback = (EventCallback *)this;
			pChild->m_overlayContainer->hide();
			
			//TODO: Is this function call necessary?
			//pChild->setFont(m_font, m_fontColour);			
			
			pChild->m_uUserData = uint32(m_pWindowClient->mChildWindows.size() - 1 - m_uStartWindow); // порядковый номер окна
			m_uCurrentFillSize += m_uSizeYButton;
		};

		uint8 oldCountVisible = m_uCountVisible;
		m_uCountVisible = 0;

		size_t size = m_pWindowClient->mChildWindows.size();
		for (size_t pos = m_uStartWindow; pos<size; pos++) {
			Window * pWindow = m_pWindowClient->mChildWindows[pos];
			if ((pWindow->m_iPosY + m_uSizeYButton) > m_pWindowClient->m_iSizeY) { // не влазиет
				if (pWindow->m_overlayContainer->isVisible()) pWindow->m_overlayContainer->hide();
				else pos = size; // выходим их цикла
			} else {
				m_uCountVisible ++;
				if (!pWindow->m_overlayContainer->isVisible()) {
					pWindow->m_overlayContainer->show(); // показать
					if ((pWindow->m_uUserData + m_uOffsetDrawString) == m_uSelectItem) {
						pWindowSelect = pWindow;
						if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
					}
				}
			}
		}
	
		if (oldCountVisible >= m_uCountVisible) { // колличество строк не увеличилось
			if (oldCountVisible != m_uCountVisible) recalcScroll();
			return;
		}
		if (m_aString.size() == 0) {
			recalcScroll();
			return;
		}

		int16 visible = (int16)m_aString.size() - m_uOffsetDrawString;
		if (visible < m_uCountVisible) { // есть пустые места снизу
			if (m_uOffsetDrawString != 0) { // и позиция не нулевая
				visible = (int16)m_uOffsetDrawString - ((int16)m_uCountVisible - visible);
				if (visible < 0) visible = 0;
				m_uOffsetDrawString = visible;
				redrawStrings();
			}
		} else {
			for (uint16 draw = oldCountVisible; draw<m_uCountVisible; draw++) {
				Window * pWindow = m_pWindowClient->mChildWindows[draw+m_uStartWindow];
				pWindow->m_pWindowText->setCaption(*m_aString[draw+m_uOffsetDrawString]);
			}
		}

		recalcScroll();

	}

	void List::recalcScroll() // пересчет полосы прокрутки
	{
		if (!m_scroll) return;
		if (m_uCountVisible == 0) return;

		uint16 size = (uint16)m_aString.size();
		if (m_uCountVisible >= size) {
			m_scroll->setScrollRange(0);
			if (m_uExData & WES_LIST_HIDE_SCROLL) { // скрывать , показывать скролл
				if (m_bIsVisibleScroll) {
					m_scroll->show(false);
					m_bIsVisibleScroll = false;

					uint8 size = (uint8)m_pWindowClient->mChildWindows.size();
					for (size_t pos = m_uStartWindow; pos<size; pos++) {
						Window * pWindow = m_pWindowClient->mChildWindows[pos];
						pWindow->size(pWindow->m_iSizeX+m_uSizeXScroll, pWindow->m_iSizeY);
					}

				}
			}
			return;
		}
		// вычисление размера скрола
		m_scroll->setScrollRange(size - m_uCountVisible + 1);
		m_scroll->setScrollPos(m_uOffsetDrawString);

		if (m_uExData & WES_LIST_HIDE_SCROLL) { // скрывать , показывать скролл
			if (!m_bIsVisibleScroll) {
				m_scroll->show(true);
				m_bIsVisibleScroll = true;

				size_t size = m_pWindowClient->mChildWindows.size();
				for (size_t pos = m_uStartWindow; pos<size; pos++) {
					Window * pWindow = m_pWindowClient->mChildWindows[pos];
					pWindow->size(pWindow->m_iSizeX-m_uSizeXScroll, pWindow->m_iSizeY);
				}

			}
		}
	}

	void List::redrawStrings() // перерисовывает все строки
	{
		if (pWindowSelect) { // сброс выделения
			pWindowSelect->setState(WS_NORMAL);
			pWindowSelect = 0;
		}

		size_t size = m_aString.size();
		size_t index = m_uStartWindow;
		uint8 visible = 0;
		for (size_t pos = m_uOffsetDrawString; pos < size; pos++) {
			Window * pWindow = m_pWindowClient->mChildWindows[index];
			pWindow->m_pWindowText->setCaption(*m_aString[pos]);
			if (pos == m_uSelectItem) {
				pWindowSelect = pWindow;
				if (m_bIsFocus) pWindowSelect->setState(WS_PRESSED);
			}
			index ++;
			visible ++;
			if (visible >= m_uCountVisible) return;
		}

		// пустые места
		for (size_t pos = size + 1; pos < m_uCountVisible; pos++) {
			m_pWindowClient->mChildWindows[pos]->setCaption("");
		}

	}

	void List::addString(const DisplayString & strElement, bool bIsRedraw) // добавить строку в список
	{
		m_aString.push_back(new DisplayString(strElement));
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::deleteString(uint16 index, bool bIsRedraw) // удалить строку из списка
	{
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;
		if (index >= size) return;
		delete m_aString[index];
		for (uint16 pos=index+1; pos<size; pos++) {
			m_aString[pos-1] = m_aString[pos];
		}
		m_aString.pop_back();
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::deleteStringAll(bool bIsRedraw) // удаляет все строки
	{
		m_uSelectItem = ITEM_NON_SELECT; // сбрасываем выделенный элемент
		uint16 size = (uint16)m_aString.size();
		if (size == 0) return;
		for (uint16 pos=0; pos<size; pos++) delete m_aString[pos];
		m_aString.clear();
		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	void List::insertString(uint16 index, const DisplayString & strElement, bool bIsRedraw) // вставить строку в список
	{
		m_aString.push_back(NULL); //make room for the new string
		size_t size = m_aString.size();
		
		//cap bounds
		if ((size_t)index >= size)
		    index = uint16(size - 1);
        
        //move elements in the list down to make room for the new one
        for (size_t pos=size-1; pos>(size_t)index; pos--) {
			m_aString[pos] = m_aString[pos-1];
		}
		
		m_aString[index] = new DisplayString(strElement);

		if (bIsRedraw) {
			redrawStrings();
			recalcScroll();
		}
	}

	const DisplayString & List::getString(uint16 index) // возвращает строку
	{
	    static DisplayString ReturnDefault("");
		if (index >= (int16)m_aString.size())
			return ReturnDefault;
		else
		    return *m_aString[index];
	}

	void List::setFont(const String &lpFont, ColourValue colour)
	{
	    //set our font
	    Window::setFont(lpFont, colour);
	    
	    //Set for children in "client"
		for (size_t pos = m_uStartWindow; pos < m_pWindowClient->mChildWindows.size(); pos++) {
			m_pWindowClient->mChildWindows[pos]->setFont(lpFont, colour);
		}
	}

	List *List::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
	{
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if (!pSkin) {
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
		List * pWindow = new List(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
		    
		pWindow->m_uAlign |= uAlign;
		
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // создаем дочерние окна скины
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
			if (pChild->m_uExData & WES_CLIENT) pWindow->m_pWindowClient = pChild;
		}

		if (pSkin->data3 != "") { // есть скролл
			const String &SkinScroll = pSkin->data3;
			pWindow->m_uSizeXScroll = AssetManager::getSingleton()->Skins()->getDefinition(SkinScroll)->subSkins[0]->sizeX;
			pWindow->m_scroll = pWindow->m_pWindowClient->spawn<VScroll>(
			    pWindow->m_pWindowClient->m_iSizeX - pWindow->m_uSizeXScroll, 0,
			    -1, pWindow->m_pWindowClient->m_iSizeY,
			    WA_RIGHT|WA_VSTRETCH, pSkin->data3);
			    
			pWindow->m_uStartWindow = 1;
			pWindow->m_scroll->m_pEventCallback = (EventCallback*)pWindow;
			pWindow->m_bIsVisibleScroll = true;
		}
		
		pWindow->m_SkinButton = pSkin->data4;
		pWindow->m_uSizeYButton = AssetManager::getSingleton()->Skins()->getDefinition(pWindow->m_SkinButton)
		    ->subSkins[0]->sizeY;
		if (pWindow->m_uSizeYButton == 0) pWindow->m_uSizeYButton = 1;

		pWindow->move(PosX, PosY);
		if (SizeX == -1) SizeX = pSkin->subSkins[0]->sizeX;
		if (SizeY == -1) SizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(SizeX, SizeY);
		
		pWindow->setFont(pSkin->fontWindow, pSkin->colour);
		return pWindow;
	}
}