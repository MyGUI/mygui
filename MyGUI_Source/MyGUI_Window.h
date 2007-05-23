#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_EventCallback.h"
#include "MyGUI_Skin.h"
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

namespace MyGUI {

    using Ogre::DisplayString;

	class Window : public EventCallback { // ������� ����� ���� ���
    private:
	    void getLengthText(const __tag_MYGUI_FONT_INFO *font, 
	        int16 &sizeX, int16 &sizeY,
	        const DisplayString & strSource); // ���������� ������ ������
		void getCutText(const __tag_MYGUI_FONT_INFO *font,
		    int16 &sizeX, int16 &sizeY,
		    DisplayString & strDest,
		    const DisplayString & strSource,
		    uint16 uAlign); // ���������� ���������� ������ ������ ��������
    public:
	    Window(const __MYGUI_SUBSKIN_INFO * lpSkin, uint8 uOverlay, Window *pWindowParent);		
        virtual ~Window();
	    
	    static Window *Window::create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_WINDOWFRAME);
	    
	    template <typename Widget> Widget *spawn(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
            uint16 uAlign, const String &Skin = __SKIN_WIDGET_DEFAULT)
        {
            if(Skin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(PosX, PosY, SizeX, SizeY, this, uAlign, 0);
            else
                return Widget::create(PosX, PosY, SizeX, SizeY, this, uAlign, 0, Skin);
        }
        
        template <typename Widget> Widget *spawnReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
            uint16 uAlign, const String &Skin = __SKIN_WIDGET_DEFAULT)
        {
            uint16 NewPosX  = PosX  * GUI::getSingleton()->getWidth();
            uint16 NewPosY  = PosY  * GUI::getSingleton()->getHeight();
            uint16 NewSizeX = SizeX * GUI::getSingleton()->getWidth();
            uint16 NewSizeX = SizeX * GUI::getSingleton()->getHeight();
                
            return spawn<Widget>(NewPosX, NewPosY, NewSizX, NewSizeY, uAlign, uSkin);
        }
        
        /*
            Event handlers.  Question: could these be integrated with the EventCallback class to unify
            event callback listeners into a single class?
        */
        virtual void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // ���������� ��� �������� ����
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		virtual void _OnMouseButtonClick(bool bIsDouble); // ���������� ��� ������� ������� � ���������� �� ��� �� ��������
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����
//		virtual void _OnKeyButtonClick(int keyEvent); // ���������� ��� ���������� ������� �����
		virtual void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		virtual void _OnUpZOrder() {}; // ���������� ��� ��������� ����
		
		void show(bool ShowWindow = true);
//		void show() //Show the window
//		{   show(true); }
		inline void hide() //hide the window
		{   show(false); }
		
		//??
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // �������� ���� �� ������� �������
		
		//??
		void setState(uint8 uState); // ��������� ����
		uint8 getState(); // ��������� ����
		
		void showFocus(bool bIsFocus); // ������������� ����
	    
	    
	    /*
	        Relating to captions, their fonts and colours, etc.
        */
            private:
            //largely redundant with the get/set Caption functions in overlays
		    DisplayString m_strWindowText; // ����� ���� ��� ������ �������, ����� ����� �� ������� ��� ������ �����
		    
		    public:
	        //??
		    Window *alignWindow(int16 rNewSizeX, int16 rNewSizeY); // ����������� ���� ������������ ����
    		
    		//Fonts
		    Window *setFont(const Ogre::String &Font, Ogre::ColourValue colour);
		    Window *setFont(const String &Font) { setFont(Font, m_fontColour); return this; }
		    const __tag_MYGUI_FONT_INFO *getFont() const;
		    
		    //Colours
		    Window *setColour(Ogre::ColourValue colour);
		    Ogre::ColourValue getColour() const { return m_fontColour; }
		    
		    //The Caption itself
		    virtual Window *setCaption(const DisplayString & strText); //Places text in the window in whatever way the window defines it
		    const DisplayString & getCaption(); // ���������� ������ ����
            virtual Window * alignCaption(); // ��������� ������������ ������
    		
		    private:
		    void Window::bootFont(); //sets up the overlays for the font
		    Ogre::ColourValue m_fontColour; // ���� ������
		    String m_font; // ����� ����
		    		    
		    public:
		    Ogre::OverlayContainer* m_overlayCaption; // ������� ������	
		    //A window for the caption on this window		    
		    void setWindowText(Window *win) { m_pWindowText = win; } //preferred method of manipulating
		    Window * m_pWindowText;   // ������� ����� ���������� ����� ����� �������� (�� ������� this)
		
		public:
		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // ��������� ������� �� ������� ���� �����������
        
        //User data is a catch-all extra variable that different widgets use for different things
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ������ ������ ������������
		inline uint32 getUserData() {return m_uUserData;}; // ���������� ������ ������������

		//Somehow related to its representation in Ogre
		Ogre::Overlay* m_overlay; // ������� ����� ����
		Ogre::PanelOverlayElement * m_overlayContainer; // ������� ��������
				
		/*
		    Child windows of this window.  Each window is actually made up of a main window
		    and anywhere from 0 to many child windows, which define things such as frames and other
		    miscellaneous graphics.
		*/
		std::vector<MyGUI::Window*> mChildWindows;
		typedef std::vector<MyGUI::Window*>::iterator ChildWindowsIterator;	
		
		Window * m_pWindowParent; // A link back to this window's parent.  NULL if this is a root object
		
		//??
		// In this window daughters are added (default this)
		Window * m_pWindowClient; // ������� ����� �������� ��������� ����� ����� �������� (�� ������� this)
		
		void move(int16 iPosX, int16 iPosY); // ����������� ����
		virtual void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		
		EventCallback *m_pEventCallback; // pointer to a callback function that handles this window's events
		uint16 m_uEventCallback;         // flags for callback events (that is, what events do we want to respond to?)
		
		int m_iPosX, m_iPosY,
		    m_iSizeX, m_iSizeY; // ������� ����
		
		//??
		int m_iOffsetAlignX, m_iOffsetAlignY; // [ displacement, is used only at alignment on the center without a stretching ]
        uint16 m_uAlign; // ������������ ���� � ������
        
		__WINDOW_STATE m_uState; // ������ ����
				
		int16 m_sizeTextX; // ������ ������ ����
		int16 m_sizeTextY; // ������ ������ ����
		int16 m_sizeCutTextX; // ������� ������ ������
		int16 m_sizeCutTextY; // ������� ������ ������
		
		bool m_bIsOverlapped; //Is this window being overlapped by another window somewhere
		
		//??
		protected:
		void shiftText(bool IsShiftText);
		
		private:
		const __MYGUI_SUBSKIN_INFO * m_lpSkinInfo; // ��������� �� ���� ����
		
		public:
		void setSkinState(__SKIN_STATES);
		
		uint16 m_uExData; // �������������� ���������� �� ��������
		uint32 m_uUserData; // �������������� ���������� ������������

	}; // class Window : public EventCallback { // ������� ����� ���� ���

} // namespace MyGUI {