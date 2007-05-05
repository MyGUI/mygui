//=========================================================================================
//=========================================================================================
#include "MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class GUI;

	Window::Window(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother) :
		m_overlay(0),
		m_GUI(gui),
		m_pWindowFother(0),
		m_pEventCallback(gui->m_pEventCallback),
		m_uEventCallback(lpSkin->event_info),
		m_iPosX(lpSkin->posX),
		m_iPosY(lpSkin->posY),
		m_iSizeX(lpSkin->sizeX),
		m_iSizeY(lpSkin->sizeY),
		m_iOffsetAliginX(0),
		m_iOffsetAliginY(0),
		m_uAligin(lpSkin->aligin),
		m_uState(WS_NORMAL),
		m_overlayCaption(0),
		m_sizeTextX(0),
		m_sizeTextY(0),
		m_sizeCutTextX(0),
		m_sizeCutTextY(0),
		m_bIsOverlapped(0),
		m_bIsTextShiftPressed(false),
		m_font(gui->m_fontInfo[FONT_DEFAULT]), // ����� �� ���������
		m_fontColour(0xFFFFFF),
		m_uExData(lpSkin->exdata),
		m_uUserData(0)
	{
		m_pWindowText = this;
		m_pWindowClient = this;
		memset((void*)m_paStrSkins, 0, sizeof(String* ) * __SKIN_STATE_COUNT);
		if (lpSkin->pStrSkinDeactived.length()) m_paStrSkins[SKIN_STATE_DEACTIVED] = &lpSkin->pStrSkinDeactived;
		if (lpSkin->pStrSkinNormal.length()) m_paStrSkins[SKIN_STATE_NORMAL] = &lpSkin->pStrSkinNormal;
		if (lpSkin->pStrSkinActived.length()) m_paStrSkins[SKIN_STATE_ACTIVED] = &lpSkin->pStrSkinActived;
		if (lpSkin->pStrSkinPressed.length()) m_paStrSkins[SKIN_STATE_PRESSED] = &lpSkin->pStrSkinPressed;
		if (lpSkin->pStrSkinSelected.length()) m_paStrSkins[SKIN_STATE_SELECTED] = &lpSkin->pStrSkinSelected;

		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayContainer = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "MyGUI_OverlayElement_" + StringConverter::toString((uint32)this))); // � ������ �� ����� ���� ��� this � ����� �������, ������ ����������
		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_iPosX, m_iPosY);
		m_overlayContainer->setDimensions(m_iSizeX, m_iSizeY);
		if (m_paStrSkins[SKIN_STATE_NORMAL]) m_overlayContainer->setMaterialName(*m_paStrSkins[SKIN_STATE_NORMAL]);

		uint16 size = (uint16)m_GUI->m_aWindowChild.size();
		int16 index = -1;

		if (uOverlay == OVERLAY_OVERLAPPED) { // ��������������� ����
			m_bIsOverlapped = true; // ����������
			m_overlay = m_GUI->createOverlay("MyGUI_Overlay_" + StringConverter::toString((uint32)this), m_GUI->m_uMaxZOrder);
			m_GUI->m_uMaxZOrder += __GUI_ZORDER_OVERLAPPED_STEP;
			m_overlay->add2D(m_overlayContainer);

			// ������� ��������� ����� ������ ����, �� ���� �����
			index = m_GUI->m_uOverlappedEnd;
			m_GUI->m_uOverlappedEnd ++;
			if (m_GUI->m_uMaxZOrder >= __GUI_ZORDER_OVERLAPPED_WARNING) {
				// ��������� ���� ���������
				if (m_GUI->m_pEventCallback) m_GUI->m_pEventCallback->onWarningEvent(this, WE_WARNING_ZORDER_MAX);
			}
		} else if (uOverlay == OVERLAY_CHILD) { // ������������ ����
			__ASSERT(pWindowFother != 0); // ���� ����������
			m_pWindowFother = pWindowFother;
			m_pWindowFother->m_overlayContainer->addChild(m_overlayContainer);
			m_pWindowFother->m_aWindowChild.push_back(this); // ��������� ���� � ����
			// ��� �� ���������� , �� ����� �������� ���������
			if (m_uAligin & WA_CENTER_FOTHER) {
				m_iOffsetAliginX = -m_iSizeX;
				m_iOffsetAliginY = -m_iSizeY;
			} else { // Altren - ��� ����� � ���...  (c)
				m_iOffsetAliginX = (m_iPosX << 1) - m_pWindowFother->m_iSizeX;
				m_iOffsetAliginY = (m_iPosY << 1) - m_pWindowFother->m_iSizeY;
			}
			// ��������� ������ ������ ����������
			uint uCountFother = 0;
			Window *pWindow = m_pWindowFother;
			while (pWindow) {
				__ASSERT(pWindow != pWindow->m_pWindowFother); // ����� �� ���������
				pWindow = pWindow->m_pWindowFother;
				uCountFother ++;
			};
			if (uCountFother >= __GUI_ZORDER_OVERLAPPED_STEP) {
				// ��������� ������� ���������
				if (m_GUI->m_pEventCallback) m_GUI->m_pEventCallback->onWarningEvent(this, WE_WARNING_CHILD_MAX);
			}
		} else { // ���, ����������� ����
			__ASSERT(uOverlay < __OVERLAY_COUNT); // ����
			__ASSERT(uOverlay != OVERLAY_MOUSE); // �� �� ����� ������
			m_overlay = m_GUI->m_overlayGUI[uOverlay];
			m_overlay->add2D(m_overlayContainer);
			if (m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // ���� ���� ���������������
				index = 0;
				while (index < m_GUI->m_uOverlappedStart) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < m_GUI->m_aWindowChild[index]->m_overlay->getZOrder()) break;
					index ++;
				};
				m_GUI->m_uOverlappedStart ++;
				m_GUI->m_uOverlappedEnd ++;
			} else { // ���� ���� ���������������
				index = m_GUI->m_uOverlappedEnd;
				while (index < size) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < m_GUI->m_aWindowChild[index]->m_overlay->getZOrder()) break;
					index ++;
				};
			}
		}

		if (index != -1) { // ��������������� ������� � ������
			m_GUI->m_aWindowChild.push_back(0);
			for (int16 pos=size; pos>index; pos--) m_GUI->m_aWindowChild[pos] = m_GUI->m_aWindowChild[pos-1];
			m_GUI->m_aWindowChild[index] = this;
		}

		_LOG("create window (%p)     (%d, %d, %d, %d)   callback(0x%.8X)   aligin(0x%.8X)    overlapped(%d)   data(0x%.8X)", this, m_iPosX, m_iPosY, m_iSizeX, m_iSizeY, m_uEventCallback, m_uAligin, m_bIsOverlapped, m_uExData);

		m_overlayContainer->show();
	}

	Window::~Window()
	{
		// ������� �������
		for (uint i=0; i<m_aWindowChild.size();i++) delete m_aWindowChild[i];
		m_aWindowChild.clear();

		_LOG("destroy window (%p)", this);

		// � ������ � ����
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		if (m_overlayCaption) {
			m_overlayContainer->removeChild(m_overlayCaption->getName());
			overlayManager.destroyOverlayElement(m_overlayCaption);
		}

		if (m_overlay) {
			m_overlay->remove2D(m_overlayContainer);
			overlayManager.destroyOverlayElement(m_overlayContainer);
			if (m_bIsOverlapped) overlayManager.destroy(m_overlay); // ��������������� ����
		} else {
			__ASSERT(m_pWindowFother != 0); // �� ���
			m_pWindowFother->m_overlayContainer->removeChild(m_overlayContainer->getName()); // �������� ����
			overlayManager.destroyOverlayElement(m_overlayContainer);
		}
	}

	void Window::_OnMouseChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �� �������
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_MOUSE_FOCUS) m_pEventCallback->onMouseFocus(this, bIsFocus);
	}

	void Window::_OnMouseMove(int16 iPosX, int16 iPosY) // ���������� ��� �������� ����
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_MOUSE_MOVE) m_pEventCallback->onMouseMove(this, iPosX, iPosY);
	}

	void Window::_OnMouseButtonPressed(bool bIsLeftButtonPressed) // ���������� ��� ������� �������
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_MOUSE_BUTTON) m_pEventCallback->onMouseButton(this, bIsLeftButtonPressed);
	}

	void Window::_OnMouseButtonClick(bool bIsDouble) // ���������� ��� ������� ������� � ���������� �� ��� �� ��������
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_MOUSE_BUTTON) {
			if (!bIsDouble) m_pEventCallback->onMouseClick(this);
			else m_pEventCallback->onMouseDoubleClick(this);
		}
	}

	void Window::_OnKeyChangeFocus(bool bIsFocus) // ���������� ��� ����� ���������� �����
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_KEY_FOCUS) m_pEventCallback->onKeyFocus(this, bIsFocus);
	}

	void Window::_OnKeyButtonPressed(int keyEvent, wchar_t cText) // ���������� ��� ������� ������� �����
	{
		if (!m_pEventCallback) return;
		if (m_uEventCallback&WE_KEY_BUTTON) m_pEventCallback->onKeyButton(this, keyEvent, cText);
	}


	void Window::show(bool bIsShow) // ������ �������� ����
	{
		if (bIsShow) m_overlayContainer->show();
		else m_overlayContainer->hide();
	}

	void Window::move(int16 iPosX, int16 iPosY) // ����������� ����
	{
		m_iPosX = iPosX;
		m_iPosY = iPosY;
		m_overlayContainer->setPosition(iPosX, iPosY);
	}

	void Window::size(int16 iSizeX, int16 iSizeY) // �������� ������ ����
	{
		// ������� ������������� ����� ��������, ����� �������� �� ��
		for (uint i=0; i<m_aWindowChild.size(); i++) { // ������� �������
			m_aWindowChild[i]->aliginWindow(iSizeX, iSizeY); // �����������
		}
		// � ����� ������ ������ �������� ������
		m_iSizeX = iSizeX;
		m_iSizeY = iSizeY;

		m_overlayContainer->setDimensions(iSizeX, iSizeY);
		aliginWindowText();
	}

	void Window::aliginWindow(int16 rNewSizeX, int16 rNewSizeY) // ����������� ���� ������������ ����
	{
		if (!m_pWindowFother) return;
		// �������������� ������������ 
		if (m_uAligin & WA_RIGHT) {
			if (m_uAligin & WA_LEFT) {
				size(m_iSizeX - (m_pWindowFother->m_iSizeX - rNewSizeX), m_iSizeY); // �����������
			}
			else move(m_iPosX - (m_pWindowFother->m_iSizeX - rNewSizeX), m_iPosY); // �������
		} else if (!(m_uAligin & WA_LEFT)) { // �� ����������� ��� ����������
			move(((rNewSizeX - m_iOffsetAliginX)>>1) + m_iOffsetAliginX, m_iPosY);
		}
		if (m_uAligin & WA_BOTTOM) {
			if (m_uAligin & WA_TOP) {
				size(m_iSizeX, m_iSizeY - (m_pWindowFother->m_iSizeY - rNewSizeY)); // �����������
			} else move(m_iPosX, m_iPosY - (m_pWindowFother->m_iSizeY - rNewSizeY)); // �������
		} else if (!(m_uAligin & WA_TOP)) { // �� ��������� ��� ����������
			move(m_iPosX, ((rNewSizeY - m_iOffsetAliginY)>>1) + m_iOffsetAliginY);
		}

	}

	void Window::aliginWindowText() // ��������� ������������ ������
	{
		if (!m_overlayCaption) return;

		if (m_uAligin & __WAT_IS_CUT) {

			if ((m_sizeTextX > (m_iSizeX-__GUI_FONT_SIZE_HOFFSET)) || (m_sizeTextY > m_iSizeY)) { // ����� ��������
				if (m_strWindowText == "") m_strWindowText = m_overlayCaption->getCaption(); // ����� �� ��� ��������
				m_sizeCutTextX = m_iSizeX-__GUI_FONT_SIZE_HOFFSET;
				m_sizeCutTextY = m_iSizeY;
				DisplayString strDest;
				m_GUI->getCutText(m_font, m_sizeCutTextX, m_sizeCutTextY, strDest, m_strWindowText, m_uAligin);
				m_overlayCaption->setCaption(strDest);
				m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);

			} else { // ������� �� �����
				if (m_strWindowText != "") { // ��������������� ����������� �����
					m_overlayCaption->setCaption(m_strWindowText);
					m_strWindowText = "";
					m_sizeCutTextX = m_sizeTextX;
					m_sizeCutTextY = m_sizeTextY;
				}
			}
	
		}

		if (m_uAligin & WAT_RIGHT) {
			if (m_uAligin & WAT_LEFT) { // �� ������
				m_overlayCaption->setLeft((m_iSizeX-m_sizeCutTextX)>>1);
			} else { // �� ������� ����
				m_overlayCaption->setLeft(m_iSizeX-m_sizeCutTextX-__GUI_FONT_HOFFSET);
			}
		}

		if (m_uAligin & WAT_BOTTOM) {
			if (m_uAligin & WAT_TOP) { // �� ������
				m_overlayCaption->setTop((m_iSizeY-m_sizeCutTextY)>>1);
			} else { // �� ������� ����
				m_overlayCaption->setTop(m_iSizeY-m_sizeCutTextY);
			}
		}

	}

	void Window::setWindowText(const DisplayString & strText) // ������������� ����� ����
	{
		if (!m_pWindowText->m_overlayCaption) return;

		m_pWindowText->m_strWindowText = "\0";
		m_GUI->getLenghtText(m_pWindowText->m_font, m_pWindowText->m_sizeTextX, m_pWindowText->m_sizeTextY, strText);
		m_pWindowText->m_sizeCutTextX = m_pWindowText->m_sizeTextX;
		m_pWindowText->m_sizeCutTextY = m_pWindowText->m_sizeTextY;
		try {m_pWindowText->m_overlayCaption->setCaption(strText);	} catch (...) {__ASSERT(false);} // ������������ �������
		m_pWindowText->aliginWindowText();
	}

	const DisplayString & Window::getWindowText() // ���������� ������ ����
	{
		if ((m_pWindowText->m_strWindowText == "\0") && (m_pWindowText->m_overlayCaption)) return m_pWindowText->m_overlayCaption->getCaption();
		return m_pWindowText->m_strWindowText;
	}

	void Window::setState(uint8 uState) // ��������� ����
	{
		uint8 uSkin = __SKIN_COUNT;
		bool bIsShiftText = false;

		if (uState == WS_DEACTIVE) {
			m_uState = WS_DEACTIVE;
			uSkin = SKIN_STATE_DEACTIVED;
		} else if (uState == WS_NORMAL) {
			if (m_uState == __WS_SELECTED) {
				m_uState = __WS_ACTIVED;
				uSkin = SKIN_STATE_ACTIVED;
			} else {
				m_uState = WS_NORMAL;
				uSkin = SKIN_STATE_NORMAL;
			}
		} else if (uState == WS_PRESSED) {
			bIsShiftText = true;
			if (m_uState == __WS_ACTIVED) {
				m_uState = __WS_SELECTED;
				uSkin = SKIN_STATE_SELECTED;
			} else {
				m_uState = WS_PRESSED;
				uSkin = SKIN_STATE_PRESSED;
			}
		}

		if (uSkin != __SKIN_COUNT) { // ������ ����� ���������
			// �������� ����
			if (m_uExData & WES_BUTTON) {
				if (m_paStrSkins[uSkin]) m_overlayContainer->setMaterialName(*m_paStrSkins[uSkin]);
			}
			// �������
			for (uint i=0; i<m_aWindowChild.size(); i++) {
				Window * pChild = m_aWindowChild[i];
				if (pChild->m_uExData & WES_BUTTON) {
					if (pChild->m_paStrSkins[uSkin]) pChild->m_overlayContainer->setMaterialName(*pChild->m_paStrSkins[uSkin]);
				}
			}
		}

		if (bIsShiftText != m_pWindowText->m_bIsTextShiftPressed) { // ����� ������
			m_pWindowText->m_bIsTextShiftPressed = bIsShiftText;
			if (m_pWindowText->m_uAligin & WAT_SHIFT_TEXT_PRESSED) {
				if (m_pWindowText->m_overlayCaption) {
					if (m_pWindowText->m_bIsTextShiftPressed) m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()+__GUI_BUTTON_SHIFT_TEXT_PRESSED);
					else m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()-__GUI_BUTTON_SHIFT_TEXT_PRESSED);
				}
			}
		}

	}

	uint8 Window::getState()  // ��������� ����
	{
		if (m_uState == __WS_ACTIVED) return WS_NORMAL;
		if (m_uState == __WS_SELECTED) return WS_PRESSED;
		return m_uState;
	}

	void Window::showFocus(bool bIsFocus) // ������������� ����
	{
		if (m_uState == WS_DEACTIVE) return;
		uint8 uSkin = __SKIN_COUNT;

		if (bIsFocus) {
			if (m_uState == WS_NORMAL) {
				m_uState = __WS_ACTIVED;
				uSkin = SKIN_STATE_ACTIVED;
			} else if (m_uState == WS_PRESSED) {
				m_uState = __WS_SELECTED;
				uSkin = SKIN_STATE_SELECTED;
			}
		} else {
			if (m_uState == __WS_ACTIVED) {
				m_uState = WS_NORMAL;
				uSkin = SKIN_STATE_NORMAL;
			} else if (m_uState == __WS_SELECTED) {
				m_uState = WS_PRESSED;
				uSkin = SKIN_STATE_PRESSED;
			}
		}

		if (uSkin != __SKIN_COUNT) {
			// �������� ����
			if (m_uExData & WES_BUTTON) {
				if (m_paStrSkins[uSkin]) m_overlayContainer->setMaterialName(*m_paStrSkins[uSkin]);
			}
			// �������
			for (uint i=0; i<m_aWindowChild.size(); i++) {
				Window * pChild = m_aWindowChild[i];
				if (pChild->m_uExData & WES_BUTTON) {
					if (pChild->m_paStrSkins[uSkin]) pChild->m_overlayContainer->setMaterialName(*pChild->m_paStrSkins[uSkin]);
				}
			}
		}

	}

	bool Window::check(int16 iPosX, int16 iPosY, bool bCapture) // bCapture ���������� �� �������� ����
	{
		if (!m_overlayContainer->isVisible()) return false;
		// ���� ������ �� ��� ����� ����������
		if (iPosX < m_iPosX) return false;
		if (iPosY < m_iPosY) return false;
		if (iPosX > m_iPosX+m_iSizeX) return false;
		if (iPosY > m_iPosY+m_iSizeY) return false;
		// ���� �� ��������
		if (m_uState == WS_DEACTIVE) return true; // ����� ��� ���� �������

		if (m_uEventCallback & __WE_IS_ACTION) {
			m_GUI->m_currentFocusWindow = this;
			 // �������� �� ���������
			return true;
		}

		for (uint i=0; i<m_aWindowChild.size(); i++) {
			if (m_aWindowChild[i]->check(iPosX-m_iPosX, iPosY-m_iPosY)) return true;
		}

		return bCapture;
	}

	void Window::setFont(uint8 uFont, uint32 colour) // ������������� ����� ��� ��������
	{
		__ASSERT(uFont < __FONT_COUNT);
		setFont(m_GUI->m_fontInfo[uFont], colour);
	}

	void Window::setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour) // ������������� ����� ��� ��������
	{
		if (lpFont) m_pWindowText->m_font = lpFont;
		m_pWindowText->m_fontColour = colour;
		if (!m_pWindowText->m_overlayCaption) { // ������� �� ������, �������
			OverlayManager &overlayManager = OverlayManager::getSingleton();
			m_pWindowText->m_overlayCaption = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("TextArea", "MyGUI_OverlayElement_TextArea_" + StringConverter::toString((uint32)this)));
			m_pWindowText->m_overlayCaption->setMetricsMode(GMM_PIXELS);
			m_pWindowText->m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);
			m_pWindowText->m_overlayContainer->addChild(m_pWindowText->m_overlayCaption);
		}
		m_pWindowText->m_overlayCaption->setParameter("font_name", m_pWindowText->m_font->name);
		m_pWindowText->m_overlayCaption->setParameter("char_height", StringConverter::toString(m_pWindowText->m_font->height));
//		m_pWindowText->m_overlayCaption->setParameter("space_width", StringConverter::toString(m_pWindowText->m_font->spaceWidth));
		m_pWindowText->m_overlayCaption->setColour(ColourValue((((m_pWindowText->m_fontColour & 0x00FF0000) >> 16) / 256.0), (((m_pWindowText->m_fontColour & 0x0000FF00) >> 8) / 256.0), ((m_pWindowText->m_fontColour & 0x000000FF) / 256.0)));
		m_pWindowText->aliginWindowText();
	}

	MyGUI::Window * Window::createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback) // ������� �������� ����
	{
		__ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = m_GUI->m_windowInfo[uSkin];
		Window * pWindow = new Window(pSkin->subSkins[0], m_GUI, OVERLAY_CHILD, m_pWindowClient);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], m_GUI, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
		}
		pWindow->m_uAligin |= uAligin;
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}

	MyGUI::Window * GUI::createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback) // ������� �������� ����
	{
		__ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = m_windowInfo[uSkin];
		Window * pWindow = new Window(pSkin->subSkins[0], this, uOverlay, 0);
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], this, OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
		}
		pWindow->move(iPosX, iPosY);
		if (iSizeX == -1) iSizeX = pSkin->subSkins[0]->sizeX;
		if (iSizeY == -1) iSizeY = pSkin->subSkins[0]->sizeY;
		pWindow->size(iSizeX, iSizeY);
		if (pEventCallback) pWindow->m_pEventCallback = pEventCallback;
		return pWindow;
	}


}
//=========================================================================================
