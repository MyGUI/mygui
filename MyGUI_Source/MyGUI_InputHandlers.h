#pragma once
/*****************************
The idea is that the core GUI object inherits from these two classes instead of the OIS listeners,
so that these classes can add some extra functionality and leave the implementation away from the GUI
object
***************************/

#include "MyGUI_OIS.h"
#include "MyGUI_Defines.h"

namespace MyGUI
{
    class MouseHandler : public OIS::MouseListener
    {
        //TODO: make this a real time modifyable parameter
        static const int __GUI_TIME_DOUBLE_CLICK = 250; //measured in milliseconds
        
        bool m_bIsFocusWindowCapture; // ���������� �� ����
        Window * m_currentFocusWindow; // ������� ���� ������� ����� ����������� �� ����
        Window * m_currentWindow; // ���� ��� ������� ������, �� ������ ����� ���
        bool m_bIsActiveGUI; // ������� �� GUI
		        
        public:
        int16 m_iOffsetPressedX, m_iOffsetPressedY; // �������� ������� ������ ���������
        int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // ������� �������� ��� �������
        
        MouseHandler();
        
        bool mouseMoved( const OIS::MouseEvent &arg ); // �������� ��� �������� ����
	    bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ������� ������ ����
	    bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ���������� ������ ����
	    
	    void ResetFocusedWindows();
	    void SetCurrentFocusWindow(Window *);
    };
    
    class KeyboardHandler : public OIS::KeyListener
    {
        unsigned char m_bShiftChars; // ����� � ������� �������� ��� ����� (0 - 3)
        
        //Helper functions:
	    void detectLangShift(int keyEvent, bool bIsKeyPressed); // �������� �� ������������ ������
        wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����    
        
        public:
        
        KeyboardHandler();
        
        bool keyPressed( const OIS::KeyEvent &arg ); // �������� ��� ������� ������ �����
		bool keyReleased( const OIS::KeyEvent &arg ); // �������� ��� ���������� ������ �����
	    
	    /*
	        Called by the message and combobox widgets.  A predominantly internal function
	        that sets the current focus window for keyboard input
	    */
	    void setKeyFocus(Window * pWindow); // ������ ����� �����
    };
}