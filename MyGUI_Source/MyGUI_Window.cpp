#include "MyGUI_Window.h"
#include "MyGUI_AssetManager.h"
#include "MyGUI_GUI.h"
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreFont.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	Window::Window(const __tag_MYGUI_SUBSKIN_INFO *lpSkin, uint8 uOverlay, Window *pWindowParent) :
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
		m_font(FONT_DEFAULT),
		m_fontColour(1,1,1),
		m_uExData(lpSkin->exdata),
		m_uUserData(0)
	{
		m_pWindowText = this;
		m_pWindowClient = this;
		
		if (lpSkin->pStrSkinDeactived.length()) m_paStrSkins[SKIN_STATE_DEACTIVED] = lpSkin->pStrSkinDeactived;
		if (lpSkin->pStrSkinNormal.length()) m_paStrSkins[SKIN_STATE_NORMAL] = lpSkin->pStrSkinNormal;
		if (lpSkin->pStrSkinActived.length()) m_paStrSkins[SKIN_STATE_ACTIVED] = lpSkin->pStrSkinActived;
		if (lpSkin->pStrSkinPressed.length()) m_paStrSkins[SKIN_STATE_PRESSED] = lpSkin->pStrSkinPressed;
		if (lpSkin->pStrSkinSelected.length()) m_paStrSkins[SKIN_STATE_SELECTED] = lpSkin->pStrSkinSelected;

		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayContainer = static_cast<PanelOverlayElement*>(overlayManager.createOverlayElement(
		    "Panel", "MyGUI_OverlayElement_" + StringConverter::toString((uint32)this))); // � ������ �� ����� ���� ��� this � ����� �������, ������ ����������
		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_iPosX, m_iPosY);
		m_overlayContainer->setDimensions(m_iSizeX, m_iSizeY);
		if (!m_paStrSkins[SKIN_STATE_NORMAL].empty())
		    m_overlayContainer->setMaterialName(m_paStrSkins[SKIN_STATE_NORMAL]);

		size_t size = GUI::getSingleton()->mRootWindows.size();
		int16 index = -1;

		if (uOverlay == OVERLAY_OVERLAPPED) { // ��������������� ����
			m_bIsOverlapped = true; // ����������
			m_overlay = GUI::getSingleton()->createOverlay(
			    "MyGUI_Overlay_" + StringConverter::toString((uint32)this), GUI::getSingleton()->m_uMaxZOrder);
			GUI::getSingleton()->m_uMaxZOrder += __GUI_ZORDER_OVERLAPPED_STEP;
			m_overlay->add2D(m_overlayContainer);

			// ������� ��������� ����� ������ ����, �� ���� �����
			index = (int16) GUI::getSingleton()->m_uOverlappedEnd;
			GUI::getSingleton()->m_uOverlappedEnd ++;
			if (GUI::getSingleton()->m_uMaxZOrder >= __GUI_ZORDER_OVERLAPPED_WARNING) {
				// ��������� ���� ���������
				if (GUI::getSingleton()->m_pEventCallback) GUI::getSingleton()->m_pEventCallback->onWarningEvent(this, WE_WARNING_ZORDER_MAX);
			}
		} else if (uOverlay == OVERLAY_CHILD) { // ������������ ����
			__ASSERT(pWindowParent != 0); // ���� ����������
			m_pWindowParent = pWindowParent;
			m_pWindowParent->m_overlayContainer->addChild(m_overlayContainer);
			m_pWindowParent->mChildWindows.push_back(this); // ��������� ���� � ����
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
				while (index < (int16)GUI::getSingleton()->m_uOverlappedStart) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder()) break;
					index ++;
				};
				GUI::getSingleton()->m_uOverlappedStart ++;
				GUI::getSingleton()->m_uOverlappedEnd ++;
			} else { // ���� ���� ���������������
				index = (int16)GUI::getSingleton()->m_uOverlappedEnd;
				while (index < (int16)size) { // ����� ����� ��� �������
					if (m_overlay->getZOrder() < GUI::getSingleton()->mRootWindows[index]->m_overlay->getZOrder()) break;
					index ++;
				};
			}
		}

		if (index != -1) { // ��������������� ������� � ������
			GUI::getSingleton()->mRootWindows.push_back(NULL);
			for (size_t pos = size; (int16)pos > index; pos--)
			    GUI::getSingleton()->mRootWindows[pos] = GUI::getSingleton()->mRootWindows[pos-1];
			GUI::getSingleton()->mRootWindows[index] = this;
		}

		_LOG("create window (%p)     (%d, %d, %d, %d)   callback(0x%.8X)   align(0x%.8X)    overlapped(%d)   data(0x%.8X)", this, m_iPosX, m_iPosY, m_iSizeX, m_iSizeY, m_uEventCallback, m_uAlign, m_bIsOverlapped, m_uExData);

		m_overlayContainer->show();
	}

	Window::~Window()
	{
		// ������� �������
		while(!mChildWindows.empty())
		{
		    delete mChildWindows.back();
		    mChildWindows.pop_back();
		}

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
		for (uint i=0; i<mChildWindows.size(); i++) { // ������� �������
			mChildWindows[i]->alignWindow(iSizeX, iSizeY); // �����������
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
				getCutText(AssetManager::getSingleton()->Fonts()->getDefinition(m_font),
				    m_sizeCutTextX, m_sizeCutTextY, strDest, m_strWindowText, m_uAlign);
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
		getLengthText(AssetManager::getSingleton()->Fonts()->getDefinition(m_pWindowText->m_font),
		    m_pWindowText->m_sizeTextX, m_pWindowText->m_sizeTextY, strText);
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
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;
		bool bIsShiftText = false;

		if (uState == WS_DEACTIVE) {
			m_uState = WS_DEACTIVE;
			Skin = SKIN_STATE_DEACTIVED;
		} else if (uState == WS_NORMAL) {
			if (m_uState == __WS_SELECTED) {
				m_uState = __WS_ACTIVED;
				Skin = SKIN_STATE_ACTIVED;
			} else {
				m_uState = WS_NORMAL;
				Skin = SKIN_STATE_NORMAL;
			}
		} else if (uState == WS_PRESSED) {
			bIsShiftText = true;
			if (m_uState == __WS_ACTIVED) {
				m_uState = __WS_SELECTED;
				Skin = SKIN_STATE_SELECTED;
			} else {
				m_uState = WS_PRESSED;
				Skin = SKIN_STATE_PRESSED;
			}
		}

		if (Skin != __SKIN_STATE_COUNT) { // ������ ����� ���������
			// �������� ����
			if (m_uExData & WES_BUTTON) {
				if (!m_paStrSkins[Skin].empty())
				    m_overlayContainer->setMaterialName(m_paStrSkins[Skin]);
			}
			// �������
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & WES_BUTTON) {
					if (!pChild->m_paStrSkins[Skin].empty())
					    pChild->m_overlayContainer->setMaterialName(pChild->m_paStrSkins[Skin]);
				}
			}
		}

		if (bIsShiftText != m_pWindowText->m_bIsTextShiftPressed) { // ����� ������
			m_pWindowText->m_bIsTextShiftPressed = bIsShiftText;
			if (m_pWindowText->m_uAlign & WAT_SHIFT_TEXT) {
				if (m_pWindowText->m_overlayCaption) {
					if (m_pWindowText->m_bIsTextShiftPressed)
					    m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()
                            + __GUI_BUTTON_SHIFT_TEXT_PRESSED);
					else
					    m_pWindowText->m_overlayCaption->setTop(m_pWindowText->m_overlayCaption->getTop()
					        - __GUI_BUTTON_SHIFT_TEXT_PRESSED);
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
		__SKIN_STATES Skin = __SKIN_STATE_COUNT;

		if (bIsFocus) {
			if (m_uState == WS_NORMAL) {
				m_uState = __WS_ACTIVED;
				Skin = SKIN_STATE_ACTIVED;
			} else if (m_uState == WS_PRESSED) {
				m_uState = __WS_SELECTED;
				Skin = SKIN_STATE_SELECTED;
			}
		} else {
			if (m_uState == __WS_ACTIVED) {
				m_uState = WS_NORMAL;
				Skin = SKIN_STATE_NORMAL;
			} else if (m_uState == __WS_SELECTED) {
				m_uState = WS_PRESSED;
				Skin = SKIN_STATE_PRESSED;
			}
		}

		if (Skin != __SKIN_STATE_COUNT) {
			// �������� ����
			if (m_uExData & WES_BUTTON) {
				if (!m_paStrSkins[Skin].empty())
				    m_overlayContainer->setMaterialName(m_paStrSkins[Skin]);
			}
			// �������
			for (uint i=0; i<mChildWindows.size(); i++) {
				Window * pChild = mChildWindows[i];
				if (pChild->m_uExData & WES_BUTTON && !pChild->m_paStrSkins[Skin].empty())
				    pChild->m_overlayContainer->setMaterialName(pChild->m_paStrSkins[Skin]);
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
			GUI::getSingleton()->SetCurrentFocusWindow(this);
			 // �������� �� ���������
			return true;
		}

		for (uint i=0; i<mChildWindows.size(); i++) {
			if (mChildWindows[i]->check(iPosX-m_iPosX, iPosY-m_iPosY)) return true;
		}

		return bCapture;
	}

	void Window::setFont(const String &Font, ColourValue colour) // ������������� ����� ��� ��������
	{
	    m_pWindowText->m_font = Font;
		setFont(AssetManager::getSingleton()->Fonts()->getDefinition(Font), colour );
	}

	void Window::setFont(const __tag_MYGUI_FONT_INFO *lpFont, ColourValue colour) // ������������� ����� ��� ��������
	{
		m_pWindowText->m_fontColour = colour;
		
		if (!m_pWindowText->m_overlayCaption) { // ������� �� ������, �������
			m_pWindowText->m_overlayCaption = static_cast<OverlayContainer*>(
                OverlayManager::getSingleton().createOverlayElement("TextArea", "MyGUI_OverlayElement_TextArea_" + 
                StringConverter::toString((uint32)this)));
			
			m_pWindowText->m_overlayCaption->setMetricsMode(GMM_PIXELS);
			m_pWindowText->m_overlayCaption->setLeft(__GUI_FONT_HOFFSET);
			m_pWindowText->m_overlayContainer->addChild(m_pWindowText->m_overlayCaption);
		}
		
		m_pWindowText->m_overlayCaption->setParameter("font_name", lpFont->name);
		m_pWindowText->m_overlayCaption->setParameter("char_height", StringConverter::toString(lpFont->height));
//		m_pWindowText->m_overlayCaption->setParameter("space_width", StringConverter::toString(lpFont->spaceWidth));
		m_pWindowText->m_overlayCaption->setColour(colour);
		m_pWindowText->alignWindowText();
	}
	
	void Window::getLengthText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource) // ���������� ������ ������
	{
		sizeY = font->height;
		sizeX = 0;
		Real len = 0;

		for (DisplayString::const_iterator index = strSource.begin(); index != strSource.end(); index++) {

			if ( index.getCharacter() == ' ') len += font->spaceWidth;
			else if (index.getCharacter() == 10) { // ������� ������

				if (len > sizeX) sizeX = len;
				len = 0;
				sizeY += font->height;

			} else len += font->font->getGlyphAspectRatio(index.getCharacter()) * font->height;
		}

		if (len > sizeX) sizeX = len;
	}

	void Window::getCutText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign) // ���������� ���������� ������ ������ ������
	{
		strDest.clear();
		// ������ ������
		if (strSource.empty()) {sizeY = 0;sizeX = 0;	return;}
		Real len = sizeX;
		float oldLen = len; // ��������� ��� ��� ����������
		// �������� �����
		if (sizeX <= 0) {sizeY = 0;sizeX = 0;	return;}
		// ���� ��� ����� �� ����������
		bool multiLine = ((uAlign & WAT_MULTI_LINE) != 0);
		bool isDot = (((uAlign & WAT_ADD_DOT) != 0) && (!multiLine));
		if (isDot) {
			if (len < font->sizeTripleDot) {	sizeX = 0;sizeY = 0;return;	}
			len -= font->sizeTripleDot;
		}
		// � ������ ���� ��� ������
		int16 height = font->height;
		if (sizeY < height) {sizeY = 0;sizeX = 0;return;}

		float sizeChar = 0;
		bool breakWord = ((uAlign & WAT_BREAK_WORD) != 0);

		strDest.resize(strSource.length() + 32);// ����� ������� ������, ���� ����� ���� �� �������
		DisplayString::iterator indexDest = strDest.begin();
		DisplayString::const_iterator indexSourceSpace = strSource.end(); // ����� ���������� �������
		DisplayString::iterator indexDestSpace = strDest.end(); // ����� ���������� �������


		if (uAlign & WAT_CUT_RIGHT) { // �������� ������

			DisplayString::const_iterator indexSource = strSource.begin();

			while (true) {
				if (indexSource.getCharacter() == ' ') {
					sizeChar = font->spaceWidth;
					indexSourceSpace = indexSource; // ���������� ��������� ������
					indexDestSpace = indexDest; // ���������� ��������� ������
					// � �� �� �����������
				} else if (indexSource.getCharacter() == '\n') { // ����� ������
					if ((height + font->height) > sizeY) indexSource = strSource.end(); // ������� �� �����
					else {
						len = sizeX;
						height += font->height;
						indexSourceSpace = strSource.end(); // ���������� ��������� ������
					}
				} else sizeChar = font->font->getGlyphAspectRatio(indexSource.getCharacter()) * font->height;

				if (sizeChar > len) {
					if (isDot) {
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
					}

					if (multiLine) { // ������������� ����

						if ((height + font->height) > sizeY) indexSource = strSource.end(); // ������� �� �����
						else {
							if (breakWord) { // ������� �� ������
								// ������������ ����� � ������ �� ���������
								if (indexSourceSpace != strSource.end()) { // ���������� ������� �� ��������� ������
									indexSource = indexSourceSpace;
									indexDest = indexDestSpace;
								}
							}
							indexDest.setCharacter('\n');  indexDest.moveNext();
							height += font->height;
							while (indexSource != strSource.end()) { // ������� ������� ������� ����� ������
								if ((indexSource.getCharacter() != ' ') && (indexSource.getCharacter() != '\n')) {
									if (sizeChar > sizeX) {
										// ����� �������� � ����, � ������� ����� ��������� ������
										indexSource = strSource.end(); // ������� �� �����
										break;
									}
									indexDest.setCharacter(indexSource.getCharacter());  indexDest.moveNext();
									len = sizeX - sizeChar;
									indexSourceSpace = strSource.end(); // ���������� ��������� ������
									break;
								};
								indexSource.moveNext();
							};

						}

					} else indexSource = strSource.end(); // ������� �� �����

				} else {
					indexDest.setCharacter(indexSource.getCharacter());  indexDest.moveNext();
					len -= sizeChar;
				}

				if (indexSource != strSource.end()) indexSource.moveNext();
				else break; // ������� �� �����

			};

		} else { // �������� �����

			DisplayString::const_iterator indexSource = strSource.end();
			indexSource.movePrev();
			bool exit = false;
			while (true) {
				if (indexSource.getCharacter() == ' ') {
					sizeChar = font->spaceWidth;
					indexSourceSpace = indexSource; // ���������� ��������� ������
				} else if (indexSource.getCharacter() == '\n') { // ����� ������
					if ((height + font->height) > sizeY) {
						exit = true;
					} else {
						len = sizeX;
						height += font->height;
						indexSourceSpace = strSource.end(); // ���������� ��������� ������
					}
				} else sizeChar = font->font->getGlyphAspectRatio(indexSource.getCharacter()) * font->height;

				if (sizeChar > len) exit = true; // �������
				else len -= sizeChar; // ������� ������

				if ((indexSource != strSource.begin()) && (!exit)) indexSource.movePrev(); // ��������� ��������
				else { // ����� ���������

					if (isDot) {
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
						indexDest.setCharacter('.');  indexDest.moveNext();
					}

					if (indexSource != strSource.end()) indexSource.moveNext(); // ���������

					// � ������ ����� � � ������������
					while (indexSource != strSource.end()) {
						indexDest.setCharacter(indexSource.getCharacter());
						indexDest.moveNext();
						indexSource.moveNext();
					};

					break; // ������� �� �����
				}

			};

		}

		sizeX = oldLen - len;
		sizeY = height;

	}	
	
    Window *Window::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	    Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin)
    {
        
		const __tag_MYGUI_SKIN_INFO * pSkin = AssetManager::getSingleton()->Skins()->getDefinition(Skin);
		
		if(!pSkin)
		{
		    _LOG("\n\t[ERROR] Attempting to use a non existant skin \"%s\".  Will set to SKIN_DEFAULT", Skin.c_str());
		    pSkin = AssetManager::getSingleton()->Skins()->getDefinition(SKIN_DEFAULT);
		}
		
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