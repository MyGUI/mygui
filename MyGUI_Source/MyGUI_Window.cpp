#include "MyGUI_GUI.h"
#include "MyGUI_Defines.h"
#include "MyGUI_Skin.h"
#include "MyGUI_GUI.h"
#include "MyGUI_Window.h"
#include "MyGUI_WindowFrame.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_Tab.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_List.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_Message.h"
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Window::Window(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent) :
		m_overlay(0),
		m_pWindowParent(0),
		m_pEventCallback(GUI::getSingleton()->m_pEventCallback),
		m_uEventCallback(lpSkin->event_info),
		m_iPosX(lpSkin->posX),
		m_iPosY(lpSkin->posY),
		m_iSizeX(lpSkin->sizeX),
		m_iSizeY(lpSkin->sizeY),
		m_iOffsetAlignX(0),
		m_iOffsetAlignY(0),
		m_uAlign(lpSkin->align),
		m_uState(WS_NORMAL),
		m_overlayCaption(0),
		m_sizeTextX(0),
		m_sizeTextY(0),
		m_sizeCutTextX(0),
		m_sizeCutTextY(0),
		m_bIsOverlapped(0),
		m_bIsTextShiftPressed(false),
		m_font(GUI::getSingleton()->m_fontInfo[FONT_DEFAULT]), // ����� �� ���������
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
		m_overlayContainer = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement(
		    "Panel", "MyGUI_OverlayElement_" + StringConverter::toString((uint32)this))); // � ������ �� ����� ���� ��� this � ����� �������, ������ ����������
		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_iPosX, m_iPosY);
		m_overlayContainer->setDimensions(m_iSizeX, m_iSizeY);
		if (m_paStrSkins[SKIN_STATE_NORMAL]) m_overlayContainer->setMaterialName(*m_paStrSkins[SKIN_STATE_NORMAL]);

		uint16 size = (uint16)GUI::getSingleton()->m_aWindowChild.size();
		int16 index = -1;

		if (uOverlay == OVERLAY_OVERLAPPED) { // ��������������� ����
			m_bIsOverlapped = true; // ����������
			m_overlay = GUI::getSingleton()->createOverlay("MyGUI_Overlay_" + StringConverter::toString((uint32)this), GUI::getSingleton()->m_uMaxZOrder);
			GUI::getSingleton()->m_uMaxZOrder += __GUI_ZORDER_OVERLAPPED_STEP;
			m_overlay->add2D(m_overlayContainer);

			// ������� ��������� ����� ������ ����, �� ���� �����
			index = GUI::getSingleton()->m_uOverlappedEnd;
			GUI::getSingleton()->m_uOverlappedEnd ++;
			if (GUI::getSingleton()->m_uMaxZOrder >= __GUI_ZORDER_OVERLAPPED_WARNING) {
				// ��������� ���� ���������
				if (GUI::getSingleton()->m_pEventCallback) GUI::getSingleton()->m_pEventCallback->onWarningEvent(this, WE_WARNING_ZORDER_MAX);
			}
		} else if (uOverlay == OVERLAY_CHILD) { // ������������ ����
			__ASSERT(pWindowParent != 0); // ���� ����������
			m_pWindowParent = pWindowParent;
			m_pWindowParent->m_overlayContainer->addChild(m_overlayContainer);
			m_pWindowParent->m_aWindowChild.push_back(this); // ��������� ���� � ����
			// ��� �� ���������� , �� ����� �������� ���������
			if (m_uAlign & WA_CENTER_FOTHER) {
				m_iOffsetAlignX = -m_iSizeX;
				m_iOffsetAlignY = -m_iSizeY;
			} else { // Altren - ��� ����� � ���...  (c)
				m_iOffsetAlignX = (m_iPosX << 1) - m_pWindowParent->m_iSizeX;
				m_iOffsetAlignY = (m_iPosY << 1) - m_pWindowParent->m_iSizeY;
			}
			// ��������� ������ ������ ����������
			uint uCountParent = 0;
			Window *pWindow = m_pWindowParent;
			while (pWindow) {
				__ASSERT(pWindow != pWindow->m_pWindowParent); // ����� �� ���������
				pWindow = pWindow->m_pWindowParent;
				uCountParent ++;
			};
			if (uCountParent >= __GUI_ZORDER_OVERLAPPED_STEP) {
				// ��������� ������� ���������
				if (GUI::getSingleton()->m_pEventCallback) GUI::getSingleton()->m_pEventCallback->onWarningEvent(this, WE_WARNING_CHILD_MAX);
			}
		} else { // ���, ����������� ����
			__ASSERT(uOverlay < __OVERLAY_COUNT); // ����
			__ASSERT(uOverlay != OVERLAY_MOUSE); // �� �� ����� ������
			m_overlay = GUI::getSingleton()->m_overlayGUI[uOverlay];
			m_overlay->add2D(m_overlayContainer);
			if (m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // ���� ���� ���������������
				index = 0;
				while (index < GUI::getSingleton()->m_uOverlappedStart) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < GUI::getSingleton()->m_aWindowChild[index]->m_overlay->getZOrder()) break;
					index ++;
				};
				GUI::getSingleton()->m_uOverlappedStart ++;
				GUI::getSingleton()->m_uOverlappedEnd ++;
			} else { // ���� ���� ���������������
				index = GUI::getSingleton()->m_uOverlappedEnd;
				while (index < size) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < GUI::getSingleton()->m_aWindowChild[index]->m_overlay->getZOrder()) break;
					index ++;
				};
			}
		}

		if (index != -1) { // ��������������� ������� � ������
			GUI::getSingleton()->m_aWindowChild.push_back(0);
			for (int16 pos=size; pos>index; pos--) GUI::getSingleton()->m_aWindowChild[pos] = GUI::getSingleton()->m_aWindowChild[pos-1];
			GUI::getSingleton()->m_aWindowChild[index] = this;
		}

		_LOG("create window (%p)     (%d, %d, %d, %d)   callback(0x%.8X)   align(0x%.8X)    overlapped(%d)   data(0x%.8X)", this, m_iPosX, m_iPosY, m_iSizeX, m_iSizeY, m_uEventCallback, m_uAlign, m_bIsOverlapped, m_uExData);

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
			__ASSERT(m_pWindowParent != 0); // �� ���
			m_pWindowParent->m_overlayContainer->removeChild(m_overlayContainer->getName()); // �������� ����
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
		if (m_uEventCallback & WE_MOUSE_MOVE) {
			int16 iParentPosX = iPosX;
			int16 iParentPosY = iPosY;
			// ������������ ����
			if (m_pWindowParent) {
				iParentPosX -= (int16)(m_overlayContainer->_getDerivedLeft() * (Real)GUI::getSingleton()->m_uWidth) - m_iPosX + 1;
				iParentPosY -= (int16)(m_overlayContainer->_getDerivedTop() * (Real)GUI::getSingleton()->m_uHeight) - m_iPosY + 1;
			}
			m_pEventCallback->onMouseMove(this, iPosX, iPosY, iParentPosX, iParentPosY);
		}
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

//	void Window::_OnKeyButtonClick(int keyEvent)
//	{
//		if (!m_pEventCallback) return;
//		if (m_uEventCallback&WE_KEY_BUTTON) m_pEventCallback->onKeyClick(this, keyEvent);
//	}


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
			m_aWindowChild[i]->alignWindow(iSizeX, iSizeY); // �����������
		}
		// � ����� ������ ������ �������� ������
		m_iSizeX = iSizeX;
		m_iSizeY = iSizeY;

		m_overlayContainer->setDimensions(iSizeX, iSizeY);
		alignWindowText();
	}

	void Window::alignWindow(int16 rNewSizeX, int16 rNewSizeY) // ����������� ���� ������������ ����
	{
		if (!m_pWindowParent) return;
		// �������������� ������������ 
		if (m_uAlign & WA_RIGHT) {
			if (m_uAlign & WA_LEFT) {
				size(m_iSizeX - (m_pWindowParent->m_iSizeX - rNewSizeX), m_iSizeY); // �����������
			}
			else move(m_iPosX - (m_pWindowParent->m_iSizeX - rNewSizeX), m_iPosY); // �������
		} else if (!(m_uAlign & WA_LEFT)) { // �� ����������� ��� ����������
			move(((rNewSizeX - m_iOffsetAlignX)>>1) + m_iOffsetAlignX, m_iPosY);
		}
		if (m_uAlign & WA_BOTTOM) {
			if (m_uAlign & WA_TOP) {
				size(m_iSizeX, m_iSizeY - (m_pWindowParent->m_iSizeY - rNewSizeY)); // �����������
			} else move(m_iPosX, m_iPosY - (m_pWindowParent->m_iSizeY - rNewSizeY)); // �������
		} else if (!(m_uAlign & WA_TOP)) { // �� ��������� ��� ����������
			move(m_iPosX, ((rNewSizeY - m_iOffsetAlignY)>>1) + m_iOffsetAlignY);
		}

	}

	void Window::alignWindowText() // ��������� ������������ ������
	{
		if (!m_overlayCaption) return;

		if (m_uAlign & __WAT_IS_CUT) {

			if ((m_sizeTextX > (m_iSizeX-__GUI_FONT_SIZE_HOFFSET)) || (m_sizeTextY > m_iSizeY)) { // ����� ��������
				if (m_strWindowText == "") m_strWindowText = m_overlayCaption->getCaption(); // ����� �� ��� ��������
				m_sizeCutTextX = m_iSizeX-__GUI_FONT_SIZE_HOFFSET;
				m_sizeCutTextY = m_iSizeY;
				DisplayString strDest;
				GUI::getSingleton()->getCutText(m_font, m_sizeCutTextX, m_sizeCutTextY, strDest, m_strWindowText, m_uAlign);
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

		if (m_uAlign & WAT_RIGHT) {
			if (m_uAlign & WAT_LEFT) { // �� ������
				m_overlayCaption->setLeft((m_iSizeX-m_sizeCutTextX)>>1);
			} else { // �� ������� ����
				m_overlayCaption->setLeft(m_iSizeX-m_sizeCutTextX-__GUI_FONT_HOFFSET);
			}
		}

		if (m_uAlign & WAT_BOTTOM) {
			if (m_uAlign & WAT_TOP) { // �� ������
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
		GUI::getSingleton()->getLenghtText(m_pWindowText->m_font, m_pWindowText->m_sizeTextX, m_pWindowText->m_sizeTextY, strText);
		m_pWindowText->m_sizeCutTextX = m_pWindowText->m_sizeTextX;
		m_pWindowText->m_sizeCutTextY = m_pWindowText->m_sizeTextY;
		try {m_pWindowText->m_overlayCaption->setCaption(strText);	} catch (...) {__ASSERT(false);} // ������������ �������
		m_pWindowText->alignWindowText();
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
			if (m_pWindowText->m_uAlign & WAT_SHIFT_TEXT_PRESSED) {
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
			GUI::getSingleton()->m_currentFocusWindow = this;
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
		setFont(GUI::getSingleton()->m_fontInfo[uFont], colour);
	}

	void Window::setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour) // ������������� ����� ��� ��������
	{
		if (lpFont)
		    m_pWindowText->m_font = lpFont;
		    
		m_pWindowText->m_fontColour = colour;
		
		if (!m_pWindowText->m_overlayCaption) { // ������� �� ������, �������
			m_pWindowText->m_overlayCaption = static_cast<OverlayContainer*>(
                OverlayManager::getSingleton().createOverlayElement("TextArea", "MyGUI_OverlayElement_TextArea_" + 
                StringConverter::toString((uint32)this)));
			
			m_pWindowText->m_overlayCaption->setMetricsMode(GMM_PIXELS);
			m_pWindowText->m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);
			m_pWindowText->m_overlayContainer->addChild(m_pWindowText->m_overlayCaption);
		}
		
		m_pWindowText->m_overlayCaption->setParameter("font_name", m_pWindowText->m_font->name);
		m_pWindowText->m_overlayCaption->setParameter("char_height", StringConverter::toString(m_pWindowText->m_font->height));
//		m_pWindowText->m_overlayCaption->setParameter("space_width", StringConverter::toString(m_pWindowText->m_font->spaceWidth));
		m_pWindowText->m_overlayCaption->setColour(ColourValue((((m_pWindowText->m_fontColour & 0x00FF0000) >> 16) / 256.0), (((m_pWindowText->m_fontColour & 0x0000FF00) >> 8) / 256.0), ((m_pWindowText->m_fontColour & 0x000000FF) / 256.0)));
		m_pWindowText->alignWindowText();
	}	
	
    Window *Window::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, uint8 uSkin)
    {
        __ASSERT(uSkin < __SKIN_COUNT); // ����
		__LP_MYGUI_WINDOW_INFO pSkin = GUI::getSingleton()->m_windowInfo[uSkin];
		
		Window * pWindow = new Window(pSkin->subSkins[0],
		    parent ? OVERLAY_CHILD : uOverlay,
		    parent ? parent->m_pWindowClient : NULL);
        
		for (uint pos=1; pos<pSkin->subSkins.size(); pos++) {
			 // ������� �������� ���� �����
			Window *pChild = new Window(pSkin->subSkins[pos], OVERLAY_CHILD, pWindow);
			pChild->m_pEventCallback = (EventCallback*)pWindow;
		}
		
		pWindow->m_uAlign |= uAlign;
		pWindow->move(PosX, PosY);
		pWindow->size(SizeX > 0 ? SizeX : pSkin->subSkins[0]->sizeX,  
		              SizeY > 0 ? SizeY : pSkin->subSkins[0]->sizeY);
		return pWindow;
    }
}
//=========================================================================================
