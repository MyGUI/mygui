#include "MyGUI_Window.h"
#include "MyGUI_OIS.h"
#include "MyGUI_GUI.h"

#include <OgreStringConverter.h>
#include <OgreOverlayManager.h>
#include <OgreTechnique.h>
#include <OgreFont.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {
	GUI *GUI::Initialize(uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback)
	{
		m_uWidth = (uWidth);
		m_uHeight = (uHeight);
		m_iCurrentOffsetCursorX = (0.0);
		m_iCurrentOffsetCursorY = (0.0);
		m_uMaxZOrder = (__GUI_ZORDER_OVERLAPPED); // ������ ��� ��������������� ����
		m_bShiftChars = (0);
		m_bIsActiveGUI = (false);
		m_bIsFocusWindowCapture = (0);
		m_currentWindow = (0);
		m_currentFocusWindow = (0);
		m_currentEditWindow = (0);
		m_uOverlappedStart = (0);
		m_uOverlappedEnd = (0);
		m_pEventCallback = (pEventCallback);
		m_bFadeState = (FADE_END);
		m_currentFadeWindow = (0);
		m_pEventCallbackFade = (0);
		
		_LOG("create MyGui object (%p) ,  %d x %d\r\n", this, uWidth, uHeight);

		m_overlayGUI[OVERLAY_FADE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_FADE), __GUI_ZORDER_FADE, false);
		m_overlayGUI[OVERLAY_MOUSE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MOUSE), __GUI_ZORDER_MOUSE);
		m_overlayGUI[OVERLAY_POPUP] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_POPUP), __GUI_ZORDER_POPUP);
		m_overlayGUI[OVERLAY_MAIN] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MAIN), __GUI_ZORDER_MAIN);
		m_overlayGUI[OVERLAY_BACK] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_BACK), __GUI_ZORDER_BACK);

		createSkin();

		createMousePointer();
		setMousePointer(POINTER_DEFAULT);
		
		return this;
	}
	
	void GUI::Shutdown()
	{
		unloadResource();
		while (m_aWindowChild.size() > 0) destroyWindow(m_aWindowChild[0]);
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayGUI[OVERLAY_MOUSE]->remove2D(m_overlayContainerMouse);
		overlayManager.destroyOverlayElement(m_overlayContainerMouse);
		for (uint16 pos=0; pos<__OVERLAY_COUNT; pos++) overlayManager.destroy(m_overlayGUI[pos]);

		_LOG("\r\ndestroy MyGui object (%p)", this);
		
		
	}

	void GUI::destroyWindow(__MYGUI_OVERLAYS overlay) // ���������� ���� � ������� �� ������
	{
		if (overlay == OVERLAY_DESTROY_ALL) {
			// ��� ����
			while (m_aWindowChild.size() > 0) destroyWindow(m_aWindowChild[0]);
		} else if (overlay == OVERLAY_OVERLAPPED) {
			// ��� ���������������
			while (m_uOverlappedStart < m_uOverlappedEnd) destroyWindow(m_aWindowChild[m_uOverlappedEnd-1]);
		} else if ((overlay < __OVERLAY_COUNT) && (overlay != OVERLAY_MOUSE)) {
			// ����� ���� ���� ����� �������
			int16 pos = 0;
			int16 size = (int16)m_aWindowChild.size();
			while (pos < size) {
				if (m_aWindowChild[pos]->m_overlay == m_overlayGUI[overlay]) {
					destroyWindow(m_aWindowChild[pos]);
					pos--;
					size --;
				}
				pos ++;
			}
		}
	}

	void GUI::destroyWindow(MyGUI::Window * pWindow) // ���������� ���� � ������� �� ������
	{
		if (!pWindow) return;

		// ����� ���� �������� ����, � ����� ������� ��� ������������� ����� ������� ����
		m_currentWindow = 0; // ����� ��������� ������
		m_currentEditWindow = 0;
		m_bIsFocusWindowCapture = false; // ����� �������
		m_currentFocusWindow = 0; // ����� ��������� ����

		uint16 size;
		if (pWindow->m_pWindowParent) { // ���� ����
			Window * pParentWindow = pWindow->m_pWindowParent;
			size = (uint16)pParentWindow->m_aWindowChild.size();
			for (uint16 i=0; i<size; i++) { // ���� � ������� ����
				if (pWindow == pParentWindow->m_aWindowChild[i]) { // �������
					pParentWindow->m_aWindowChild[i] = pParentWindow->m_aWindowChild[size-1];
					pParentWindow->m_aWindowChild.pop_back();
					delete pWindow; // ������� ����
					i = size; // �������
				}
			}
		} else  { // ������� ���
			upZOrder(pWindow); // ��������� ���
			size = (uint16)m_aWindowChild.size();
			for (uint16 i=0; i<size; i++) { // ���� � ������� ����
				if (pWindow != m_aWindowChild[i]) continue; // �� ���

				if (i != 0) { // ��������� ��������� ���������� ���� � ��� ��� ��� ������ ���� ���� ���������������
					Window * pWindowBack = m_aWindowChild[i-1];
					if (pWindowBack->m_bIsOverlapped) pWindowBack->_OnUpZOrder();
				}

				if (pWindow->m_bIsOverlapped) { // ���� ���� �������
					m_uMaxZOrder -= __GUI_ZORDER_OVERLAPPED_STEP; // ��������� �������
					m_uOverlappedEnd --;
				} else { // ����������� �������� ���
					__ASSERT(pWindow->m_overlay != 0); // ��� ���
					if (pWindow->m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // ���� ���� ���������������
						m_uOverlappedStart --;
						m_uOverlappedEnd --;
					}
				}

				for (uint16 pos=i; pos<(size-1); pos++) m_aWindowChild[pos] = m_aWindowChild[pos+1];
				m_aWindowChild.pop_back();

				delete pWindow; // ������� ����
				i = size; // �������
			}
		}

		// ������� ���������� ����, ��� ������������ �����������
//		eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
		// ���������� �����������, !!! ��������� ���� �������
		if (m_pEventCallback) m_pEventCallback->onWarningEvent(pWindow, WE_WARNING_CHILD_DELETE);
	}

	void GUI::createMousePointer()
	{
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayContainerMouse = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "MyGUI_overlayContainerMouse"));
		m_overlayContainerMouse->setMetricsMode(GMM_PIXELS);
		m_overlayContainerMouse->setPosition(0.0, 0.0);
		m_overlayContainerMouse->setDimensions(m_pointerInfo[POINTER_DEFAULT]->uSizeX, m_pointerInfo[POINTER_DEFAULT]->uSizeY);
		m_overlayContainerMouse->show();
		m_overlayGUI[OVERLAY_MOUSE]->add2D(m_overlayContainerMouse);
		m_overlayGUI[OVERLAY_MOUSE]->show();

	}

	void GUI::setMousePointer(uint8 uTypePointer) // �������� ���������
	{
		__ASSERT(uTypePointer < __POINTER_COUNT); // ����
		if (m_pointerInfo[uTypePointer]->strMaterialName == "") return;
		m_overlayContainerMouse->setMaterialName(m_pointerInfo[uTypePointer]->strMaterialName);
		m_iCurrentOffsetCursorX = m_pointerInfo[uTypePointer]->iOffsetX;
		m_iCurrentOffsetCursorY = m_pointerInfo[uTypePointer]->iOffsetY;
	}

	void GUI::upZOrder(Window *pWindow) // ������� ���� �� ����� �����
	{ // �������� � �������������� ���� � ������� , ��� ���������� �������� ���������
		if (!pWindow->m_bIsOverlapped) return; // ����� ��� ������
		 // ���������� ��� ��������������� ����
		for (uint16 pos = m_uOverlappedStart; pos<(m_uOverlappedEnd-1); pos++) {
			if (pWindow == m_aWindowChild[pos]) { // ���� ��� ���� ����, �� ������ ��� � �������
				m_aWindowChild[pos] = m_aWindowChild[pos+1];
				m_aWindowChild[pos+1] = pWindow;
				uint16 uZOrder = pWindow->m_overlay->getZOrder();
				pWindow->m_overlay->setZOrder(m_aWindowChild[pos]->m_overlay->getZOrder());
				m_aWindowChild[pos]->m_overlay->setZOrder(uZOrder);
			}
		}
	}

	MyGUI::Window * GUI::getTopWindow() // ���������� ����� ������� ���� �� ���������������
	{
		if (m_uOverlappedStart == m_uOverlappedEnd) return 0;
		return m_aWindowChild[m_uOverlappedEnd-1];
	}

	void GUI::setKeyFocus(Window * pWindow) // ������ ����� �����
	{
		if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(false);
		m_currentEditWindow = pWindow;
		if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(true);
	}


	bool GUI::eventMouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
		if (m_bIsFocusWindowCapture) { // ���� ���� ����������
			m_bIsFocusWindowCapture = false;
			m_currentFocusWindow->_OnMouseButtonPressed(false);
			Window * pOldWindow = m_currentFocusWindow;
//				eventMouseMove(iPosX, iPosY, 0.0, leftMouseButtonDown); // ��� ����������� �����������
			if (pOldWindow == m_currentFocusWindow) {
				m_currentFocusWindow->_OnMouseButtonClick(false); // �������� ������
				if (m_time.getMilliseconds() < __GUI_TIME_DOUBLE_CLICK) {
					// ���� ����� ���� �������� ����� �����
					if (m_currentFocusWindow) m_currentFocusWindow->_OnMouseButtonClick(true);
				}
				else m_time.reset(); // ������ �� ������� �������
			}
		}
		return m_bIsActiveGUI;
	}

	bool GUI::eventMousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
		if (id != OIS::MB_Left) return m_bIsActiveGUI;

		if (!m_bIsActiveGUI) {
			if (m_currentEditWindow) { // ����� ���� �����
				m_currentEditWindow->_OnKeyChangeFocus(false);
				m_currentEditWindow = 0;
			}
			return false;
		}

		if (!m_currentWindow) return false;
		upZOrder(m_currentWindow); // �������� ���� ��� �������
		
		if (m_currentFocusWindow) {
			m_bIsFocusWindowCapture = true; // ������ ����

			// �������� ����� ������ ����
			m_iOffsetPressedX = arg.state.X.abs - ((int16)(m_currentFocusWindow->m_overlayContainer->_getDerivedLeft()*m_uWidth)) - 1;
			m_iOffsetPressedY = arg.state.Y.abs - ((int16)(m_currentFocusWindow->m_overlayContainer->_getDerivedTop()*m_uHeight)) - 1;

			m_currentFocusWindow->_OnMouseButtonPressed(true);
		}

		if (m_currentEditWindow != m_currentFocusWindow) { // ����� ���� �����
			if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(false);
			m_currentEditWindow = m_currentFocusWindow;
			if (m_currentEditWindow) m_currentEditWindow->_OnKeyChangeFocus(true);
		}

		m_currentWindow->_OnUpZOrder(); // ����������� �� ��������

		return true;
	}

	bool GUI::eventMouseMove( const OIS::MouseEvent &arg )
	{
		if (arg.state.Z.rel != 0) { // ������
			if (m_currentFocusWindow) {
				if (arg.state.Z.rel < 0.0) m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBFORWARD, 0);
				else m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBBACK, 0);
			}
			return m_bIsActiveGUI;
		}

		m_bIsActiveGUI = false;

		if (m_bIsFocusWindowCapture) {
			m_currentFocusWindow->_OnMouseMove(arg.state.X.abs, arg.state.Y.abs);
			m_bIsActiveGUI =  true;

		} else {
			static Window * pOldFocusWindow = 0;
			pOldFocusWindow = m_currentFocusWindow;
			m_currentFocusWindow = 0; // ���� ������, �������
			m_currentWindow = 0; // ������� ����

			// ���� ���� ��� ������� ������, �� ����� ���
			for (int16 i=(((int16)m_aWindowChild.size())-1); i>=0; i--) { // ���� � ������� ������������ �� zOrder
				if(m_aWindowChild[i]->check(arg.state.X.abs, arg.state.Y.abs, true)) {
					m_currentWindow = m_aWindowChild[i];
					m_bIsActiveGUI = true;
					i = -1; // ����� �� �����
				}
			}

			if (m_currentFocusWindow != pOldFocusWindow) { // ���������� �������� ����
				if (pOldFocusWindow) pOldFocusWindow->_OnMouseChangeFocus(false);
				if (m_currentFocusWindow) {
					m_currentFocusWindow->_OnMouseChangeFocus(true);
				}
			}

		}

		m_overlayContainerMouse->setPosition(arg.state.X.abs+m_iCurrentOffsetCursorX, arg.state.Y.abs+m_iCurrentOffsetCursorY);

		return m_bIsActiveGUI;
	}

	void GUI::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		#define __INPUT_SHIFT_MASK 0x01
		#define __INPUT_LANG_MASK 0x02
		// ��� ���������� ������������� ������������ LeftAlt+LeftShift ��� LeftCtrl+LeftShift,LeftShift+LeftAlt ��� LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt ��� LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // ��� ���� ������

		if ((keyEvent == KC_LSHIFT) || (keyEvent == KC_RSHIFT)) {
			if (bIsKeyPressed) {
				m_bShiftChars |= __INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed) bIsTwoKeyPressed = true;
			} else {
				m_bShiftChars &= ~__INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					if (m_bShiftChars & __INPUT_LANG_MASK) m_bShiftChars &= ~__INPUT_LANG_MASK;
					else m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		} else if ((keyEvent == KC_LMENU) || (keyEvent == KC_LCONTROL)) {
			if (bIsKeyPressed) {
				bIsFirstKeyPressed = true;
				if (bIsSecondKeyPressed) bIsTwoKeyPressed = true;
			} else {
				bIsFirstKeyPressed = false;
				if ((!bIsSecondKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					if (m_bShiftChars & __INPUT_LANG_MASK) m_bShiftChars &= ~__INPUT_LANG_MASK;
					else m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		} else {
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}

	}

	
	bool GUI::eventKeyPressed( const OIS::KeyEvent &arg ) // �������� ��� ������� ������ �����
	{
		detectLangShift(arg.key, true); // �������� �� ������������ ������
		if (!m_currentEditWindow) return false;
		m_currentEditWindow->_OnKeyButtonPressed(arg.key, getKeyChar(arg.key));
		return true;
	}

	bool GUI::eventKeyReleased( const OIS::KeyEvent &arg ) // �������� ��� ���������� ������ �����
	{
		detectLangShift(arg.key, false); // �������� �� ������������ ������
		if (!m_currentEditWindow) return false;
//		m_currentEditWindow->_OnKeyButtonClick(arg.key);
		return true;
	}

	void GUI::eventWindowResize(uint16 uWidth, uint16 uHeight) // ��������� ������ �������� ����
	{
		m_uWidth = uWidth;
		m_uHeight = uHeight;
		 // ����������
		if (m_currentFadeWindow) m_currentFadeWindow->size(uWidth, uHeight);
		// ���� ���������
		Overlay::Overlay2DElementsIterator iter = m_overlayGUI[MyGUI::OVERLAY_FADE]->get2DElementsIterator();
		if (iter.hasMoreElements()) {
			iter.getNext()->setDimensions(uWidth, uHeight); // ����� ��� � ������ �������� ������ Window ��������
		}
	}

	void GUI::eventUpdateAnimation(Real fTimeLastFrame) // ��������� �������� ���
	{
		if (m_bFadeState != FADE_END) {
			if (m_bFadeState == FADE_UP) { // �������
				m_fCurrentFadeAlpha -= fTimeLastFrame;
				if (m_fCurrentFadeAlpha <= 0.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // �������� ������� ��������
					destroyWindow(m_currentFadeWindow); // ���������� ���� ����������
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(false, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(false, m_uFadeID);
					// ������� ���������� ����, ��� ������������ �����������
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else { // ������� ���������
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			} else { // �����
				m_fCurrentFadeAlpha += fTimeLastFrame;
				if (m_fCurrentFadeAlpha >= 1.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // �������� ������� ��������
					destroyWindow(m_currentFadeWindow); // ���������� ���� ����������
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(true, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(true, m_uFadeID);
					// ������� ���������� ����, ��� ������������ �����������
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else {  // ������� ���������
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			}
		}

	}

	void GUI::fadeScreen(bool bIsFade, uint8 uFadeID, EventCallback *pEventCallback) // �������� �����
	{
		m_pEventCallbackFade = pEventCallback; // ��� ������ �������
		m_uFadeID = uFadeID;
		if (bIsFade) { // �����
			m_bFadeState = FADE_DOWN;
			m_fCurrentFadeAlpha = 0.0;
		} else { //APPEAR - ���������
			m_bFadeState = FADE_UP;
			m_fCurrentFadeAlpha = 1.0;
		}

		m_overlayGUI[OVERLAY_FADE]->show(); // ���������� ������� ��������
		m_overlayGUI[OVERLAY_FADE]->setZOrder(__GUI_ZORDER_FADE); // ��������� �����

		__ASSERT(m_currentFadeWindow == 0);
		m_currentFadeWindow = spawn<Window>(0, 0, m_uWidth, m_uHeight, OVERLAY_FADE, SKIN_FADE);

		MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
		TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);

	}

	Overlay * GUI::createOverlay(String strName, uint16 zOrder, bool bIsShow) // ������� �������
	{
		_LOG("create Overlay '%s' \t zOrder = %d", strName.c_str(), zOrder);

		static String old_name;
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * pOverlay = overlayManager.create(strName);
		pOverlay->setZOrder(zOrder);
		if (bIsShow) pOverlay->show();

		// ���������� ������� ���� ��� �����
		Overlay * pOldOverlay = 0;
		try {pOldOverlay = overlayManager.getByName(old_name);} catch (...) {}
		if (pOldOverlay) {
			pOldOverlay->setZOrder(pOldOverlay->getZOrder());
		}
		old_name = strName;
		return pOverlay;
	}

	wchar_t GUI::getKeyChar(int keyEvent) // ���������� ������ �� ��� ���� ����
	{
		static wchar_t s_chars[4][58] = { // ������ ������ �� 58 ��������
			// ��������� �����
			0,0,L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'0',L'-',L'=',0,0,L'q',L'w',L'e',L'r',L't',L'y',L'u',L'i',L'o',L'p',L'[',L']',
			0,0,L'a',L's',L'd',L'f',L'g',L'h',L'j',L'k',L'l',L';',L'\'',L'`',0,L'\\',L'z',L'x',L'c',L'v',L'b',L'n',L'm',L',',L'.',L'/',0,L'*',0,L' ',
			// ��������� ����� � ������� ��������
			0,0,L'!',L'@',L'#',L'$',L'%',L'^',L'&',L'*',L'(',L')',L'_',L'+',0,0,L'Q',L'W',L'E',L'R',L'T',L'Y',L'U',L'I',L'O',L'P',L'{',L'}',
			0,0,L'A',L'S',L'D',L'F',L'G',L'H',L'J',L'K',L'L',L':',L'"',L'~',0,L'|',L'Z',L'X',L'C',L'V',L'B',L'N',L'M',L'<',L'>',L'?',0,L'*',0,L' ',
			// ������� �����
			0,0,L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9',L'0',L'-',L'=',0,0,L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',
			0,0,L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',0,L'\\',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'.',0,L'*',0,L' ',
			// ������� ����� � ������� ��������
			0,0,L'!',L'"',L'�',L';',L'%',L':',L'?',L'*',L'(',L')',L'_',L'+',0,0,L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',
			0,0,L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',0,L'/',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L'�',L',',0,L'*',0,L' ',
		};
		// �������� ����������
		static wchar_t s_nums[13] = {L'7',L'8',L'9',L'-',L'4',L'5',L'6',L'+',L'1',L'2',L'3',L'0',L'.'};

		if (keyEvent < 58) return s_chars[m_bShiftChars][keyEvent];
		else if (keyEvent < 84) {
			if (keyEvent > 70) return s_nums[keyEvent-71];
		} else if (keyEvent == KC_DIVIDE) return '/'; // �� �������� ����������
		else if (keyEvent == KC_OEM_102) return s_chars[m_bShiftChars][KC_BACKSLASH];
		return 0;
	}

	void GUI::getLenghtText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource) // ���������� ������ ������
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

	void GUI::getCutText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign) // ���������� ���������� ������ ������ ������
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
}