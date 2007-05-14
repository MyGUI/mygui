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
        
        bool m_bIsFocusWindowCapture; // захваченно ли окно
        Window * m_currentFocusWindow; // текущее окно которое может реагировать на мышь
        Window * m_currentWindow; // окно над которым курсор, из личных дочек гуи
        bool m_bIsActiveGUI; // активны ли GUI
		        
        public:
        int16 m_iOffsetPressedX, m_iOffsetPressedY; // смещение нажатия внутри заголовка
        int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // текущее смещение для курсора
        
        MouseHandler();
        
        bool mouseMoved( const OIS::MouseEvent &arg ); // вызывать при движении мыши
	    bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при нажатии клавиш мыши
	    bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при отпускании клавиш мыши
	    
	    void ResetFocusedWindows();
	    void SetCurrentFocusWindow(Window *);
    };
    
    class KeyboardHandler : public OIS::KeyListener
    {
        unsigned char m_bShiftChars; // сдвиг в массиве символов для ввода (0 - 3)
        
        //Helper functions:
	    void detectLangShift(int keyEvent, bool bIsKeyPressed); // проверка на переключение языков
        wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду    
        
        public:
        
        KeyboardHandler();
        
        bool keyPressed( const OIS::KeyEvent &arg ); // вызывать при нажатии кнопок клавы
		bool keyReleased( const OIS::KeyEvent &arg ); // вызывать при отпускании кнопок клавы
	    
	    /*
	        Called by the message and combobox widgets.  A predominantly internal function
	        that sets the current focus window for keyboard input
	    */
	    void setKeyFocus(Window * pWindow); // ставим фокус ввода
    };
}