#include "MyGUI_InputHandlers.h"
#include "MyGUI_Window.h"
#include "MyGUI_GUI.h"
#include <OgreTimer.h>

using namespace Ogre;
using namespace OIS;

namespace MyGUI
{
    //This is the only shared variable between the Mouse and Keyboard listeners
    //TODO: find a way to remove this issue
    Window * m_currentEditWindow;
    
    MouseHandler::MouseHandler() :
        m_iCurrentOffsetCursorX(0.0),
		m_iCurrentOffsetCursorY(0.0),
		m_bIsActiveGUI(false),
		m_bIsFocusWindowCapture(0),
		m_currentWindow(0),
		m_currentFocusWindow(0)
    {
    
    }
    
    bool MouseHandler::mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
	    static Ogre::Timer m_time; //used for double click timing
	    static unsigned int clickcount = 0; //how many times has the user ever clicked the mouse?
		
		++clickcount;
		if (m_bIsFocusWindowCapture)
		{
			m_bIsFocusWindowCapture = false;
			m_currentFocusWindow->_OnMouseButtonPressed(false);
			//eventMouseMove(iPosX, iPosY, 0.0, leftMouseButtonDown); // ��� ����������� �����������
            m_currentFocusWindow->_OnMouseButtonClick(false); // �������� ������
			
			//We need to check that this isn't the first time the user has clicked the mouse
			//  (and thus that the timer has just started), that there's a window to click on,
			//  and finally, that not too much time has passed to count this as a double click.
			if (clickcount && m_currentFocusWindow && m_time.getMilliseconds() < __GUI_TIME_DOUBLE_CLICK)
				m_currentFocusWindow->_OnMouseButtonClick(true);
			else
			    m_time.reset(); // ������ �� ������� �������
		}
		return m_bIsActiveGUI;
	}

	bool MouseHandler::mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id )
	{
		if (id != OIS::MB_Left) //Only responds to left clicks
		    return m_bIsActiveGUI;

		if (m_bIsActiveGUI == false)
		{
			if (m_currentEditWindow) { // ����� ���� �����
				m_currentEditWindow->_OnKeyLostFocus( NULL );
				m_currentEditWindow = 0;
			}
			return false;
		}

		if (!m_currentWindow)
		    return false;
		
		//Move windows to the top of the stack that have been clicked on
		GUI::getSingleton()->upZOrder(m_currentWindow); // �������� ���� ��� �������
		
		if (m_currentFocusWindow) {
			m_bIsFocusWindowCapture = true; // ������ ����
			
			const int Left = m_currentFocusWindow->m_overlayContainer->_getDerivedLeft() * GUI::getSingleton()->getWidth();
			const int Top  = m_currentFocusWindow->m_overlayContainer->_getDerivedTop() *  GUI::getSingleton()->getHeight();

			// �������� ����� ������ ����
			m_iOffsetPressedX = arg.state.X.abs - Left - 1;
			m_iOffsetPressedY = arg.state.Y.abs - Top  - 1;

			m_currentFocusWindow->_OnMouseButtonPressed(true);
		}

		if (m_currentEditWindow != m_currentFocusWindow) { //changing input window

			if (m_currentEditWindow) m_currentEditWindow->_OnKeyLostFocus(m_currentFocusWindow);
			if (m_currentFocusWindow) m_currentFocusWindow->_OnKeySetFocus(m_currentEditWindow);
			m_currentEditWindow = m_currentFocusWindow;

		}

		m_currentWindow->_OnUpZOrder(); // ����������� �� ��������

		return true;
	}

	bool MouseHandler::mouseMoved( const OIS::MouseEvent &arg )
	{
		if (arg.state.Z.rel != 0) { // ������
			if (m_currentFocusWindow) {
				if (arg.state.Z.rel < 0.0)
				    m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBFORWARD, 0);
				else
				    m_currentFocusWindow->_OnKeyButtonPressed(KC_WEBBACK, 0);
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

			//Iterate over all root windows to find the one we're on top of
			//we check in reverse order to respect ZOrder
			for(GUI::RootWindowReverseIterator Iter = GUI::getSingleton()->mRootWindows.rbegin();
			    Iter != GUI::getSingleton()->mRootWindows.rend(); ++Iter) {
				if((*Iter)->check(arg.state.X.abs, arg.state.Y.abs, true)) {
					m_currentWindow = *Iter;
					m_bIsActiveGUI = true;
					break;
				}
			}

			if (m_currentFocusWindow != pOldFocusWindow) { // ���������� �������� ����

				if (pOldFocusWindow) pOldFocusWindow->_OnMouseLostFocus(m_currentFocusWindow);
				if (m_currentFocusWindow) m_currentFocusWindow->_OnMouseSetFocus(pOldFocusWindow);

			}

		}

		GUI::getSingleton()->m_overlayContainerMouse->setPosition(
		    arg.state.X.abs+m_iCurrentOffsetCursorX, arg.state.Y.abs+m_iCurrentOffsetCursorY);

		return m_bIsActiveGUI;
	}
	
	void MouseHandler::ResetFocusedWindows()
    {
        m_currentWindow = 0; // ����� ��������� ������
	    m_currentEditWindow = 0;
	    m_bIsFocusWindowCapture = false; // ����� �������
	    m_currentFocusWindow = 0; // ����� ��������� ����
	}
	
	void MouseHandler::SetCurrentFocusWindow(Window *win)
	{
	    m_currentFocusWindow = win;	
	}
	
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

    KeyboardHandler::KeyboardHandler() :
        m_bShiftChars(0)
    {
    }
    
    //Detects switching from an english to a russian mode on a keyboard (?)
	void KeyboardHandler::detectLangShift(int keyEvent, bool bIsKeyPressed)
	{
		#define __INPUT_SHIFT_MASK 0x01
		#define __INPUT_LANG_MASK 0x02
		// ��� ���������� ������������� ������������ LeftAlt+LeftShift ��� LeftCtrl+LeftShift,LeftShift+LeftAlt ��� LeftShift+LeftCtrl
		static bool bIsFirstKeyPressed = false; // LeftAlt ��� LeftCtrl
		static bool bIsSecondKeyPressed = false; // LeftShift
		static bool bIsTwoKeyPressed = false; // ��� ���� ������

		if ((keyEvent == KC_LSHIFT) || (keyEvent == KC_RSHIFT))
		{
			if (bIsKeyPressed) {
				m_bShiftChars |= __INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = true;
				if (bIsFirstKeyPressed)
				    bIsTwoKeyPressed = true;
			} else {
				m_bShiftChars &= ~__INPUT_SHIFT_MASK;
				bIsSecondKeyPressed = false;
				if ((!bIsFirstKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					if (m_bShiftChars & __INPUT_LANG_MASK)
					    m_bShiftChars &= ~__INPUT_LANG_MASK;
					else
					    m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		}
		else if ((keyEvent == KC_LMENU) || (keyEvent == KC_LCONTROL))
		{
			if (bIsKeyPressed) {
				bIsFirstKeyPressed = true;
				if (bIsSecondKeyPressed) bIsTwoKeyPressed = true;
			} else {
				bIsFirstKeyPressed = false;
				if ((!bIsSecondKeyPressed) && (bIsTwoKeyPressed)) {
					bIsTwoKeyPressed = false;
					
					if (m_bShiftChars & __INPUT_LANG_MASK)
					    m_bShiftChars &= ~__INPUT_LANG_MASK;
					else
					    m_bShiftChars |= __INPUT_LANG_MASK;
				}
			}
		}
		else
		{
			bIsFirstKeyPressed = false;
			bIsSecondKeyPressed = false;
			bIsTwoKeyPressed = false;
		}

	}
	
	bool KeyboardHandler::keyPressed( const OIS::KeyEvent &arg )
	{
		detectLangShift(arg.key, true);
		
		//Pass keystrokes to the current active text widget
		if (m_currentEditWindow)
		{
		    m_currentEditWindow->_OnKeyButtonPressed(arg.key, getKeyChar(arg.key));
		    return true;
		}		
		return false;
	}

	bool KeyboardHandler::keyReleased( const OIS::KeyEvent &arg ) // �������� ��� ���������� ������ �����
	{
		detectLangShift(arg.key, false); // �������� �� ������������ ������
		if (!m_currentEditWindow) return false;
        //		m_currentEditWindow->_OnKeyButtonClick(arg.key);
		return true;
	}
	
	void KeyboardHandler::setKeyFocus(Window * pWindow) // ������ ����� �����
	{

		if (m_currentEditWindow) m_currentEditWindow->_OnKeyLostFocus(pWindow);
		if (pWindow) pWindow->_OnKeySetFocus(m_currentEditWindow);
		m_currentEditWindow = pWindow;

	}
	
	wchar_t KeyboardHandler::getKeyChar(int keyEvent) // ���������� ������ �� ��� ���� ����
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
}