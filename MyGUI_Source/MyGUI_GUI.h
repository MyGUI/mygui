#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_OIS.h"
#include "MyGUI_Skin.h"
#include <OgreTimer.h>
#include <map>
#include <OgreOverlayElement.h>
#include <OgreOverlay.h>

namespace MyGUI {

    using Ogre::DisplayString;

	class GUI : public OIS::KeyListener, public OIS::MouseListener {

	protected:

		GUI () { }// ������������ ����������� ��������
		~GUI() { }
		
	public:
	    static GUI *getSingleton()
	    {
	        static GUI Instance;
	        
	        return &Instance;
        }
        
        GUI *Initialize (uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0); // ������������ ����������� ��������
        void Shutdown();

        //Mouse
		    bool mouseMoved( const OIS::MouseEvent &arg ); // �������� ��� �������� ����
		    bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ������� ������ ����
		    bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ���������� ������ ����
		        //Somehow related to input (Mouse)
		        bool m_bIsActiveGUI; // ������� �� GUI

		//Keyboard
		    bool keyPressed( const OIS::KeyEvent &arg ); // �������� ��� ������� ������ �����
		    bool keyReleased( const OIS::KeyEvent &arg ); // �������� ��� ���������� ������ �����
		    /*
		        Called by the message and combobox widgets.  A predominantly internal function
		        that sets the current focus window (that active window that receives key input)
		    */
		    void setKeyFocus(Window * pWindow); // ������ ����� �����
		    //Helper functions:
		        void detectLangShift(int keyEvent, bool bIsKeyPressed); // �������� �� ������������ ������
		        /*
		            Non public.  Called by the keyboard functions.  Converts a key code into a character
		        */
		        wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

		//Sizing
		    unsigned int m_uWidth, m_uHeight; // ������� ������
		        /*
		            Public interface, must be called by user when they resize their window.
		        */
		        void eventWindowResize(const unsigned int uWidth, const unsigned int uHeight); // ��������� ������ �������� ����
		
		//Animation
		
		    /*
		        Public interface: Must be called by the user every frame if they want to properly have animation working.
		        (Ie: at the moment this is just teh fade screen.)
		    */    
		    void eventUpdateAnimation(Real fTimeLastFrame); // ��������� �������� ���

		/*
		    Unknown scope, most likely private.  Called when the GUI is initialized.
		*/
		void createMousePointer(); // ������� ������� ��� ���� � �������
		
		/*
		    Public interface: allows user and widgets to change the current cursor
		    graphic.
		*/
		void setMousePointer(const String &TypePointer); // �������� ���������
		
		/*
		    Public interface: allows users to show or hide the mouse graphic
		    by turning on/off the associated overlay layer.
		*/
		void showMousePointer(bool bIsShow = true) { // ������ �������� ������
		     if(bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
             else
		        m_overlayGUI[OVERLAY_MOUSE]->hide();
        }
        
        /*
            Public interface.  Sets the fade state.
                bIsFade turns on/off the fade screen.
                uFadeID appears to have something to do with callback events (?)
                pEventCallback is the EventCallback to call when the fade state changes (?).
                    Only one is allowed.  Newer callbacks replace older ones                
        */
		void fadeScreen(bool bIsFade, FADE_STATES uFadeID = NONE, EventCallback *pEventCallback = 0); // �������� �����
		
		//GUI Builder
		    /*
		        GUI Builder functions.  Very public: these are the heart and soul of the GUI library.
		    */
		    template<typename Widget> Widget *spawn(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
		        uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
		    {
	            if(Skin == __SKIN_WIDGET_DEFAULT)
	                return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay);
	            else
	                return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay, Skin);
	        }
    		    
		    template<typename Widget> Widget *spawnReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
		        uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
		    {
		        return spawn<Widget>(PosX *  GUI::getSingleton()->m_uWidth, PosY *  GUI::getSingleton()->m_uHeight,
                                 SizeX * GUI::getSingleton()->m_uWidth, SIzeY * GUI::getSingleton()->m_uHeight,
                                 uOverlay, uSkin);		
		    }

		    MyGUI::Message * createMessage(const DisplayString & strCaption,
		        const DisplayString & strMessage, uint16 uID, bool bIsModal,
		        const DisplayString & strButton1 = "Ok", const DisplayString & strButton2 = ""); // ���� ���������
        /*
            Never called, so I'm not sure what the intended use is.
        */
		Window * getTopWindow(); // ���������� ����� ������� ���� �� ���������������
		
		/*
		    Destroys a specific window and handles any cleanup associated with it.
		    Question: why can we destroy a window in two ways?  (Ie: destroyWindow(Window) and delete Window)
		*/
		void destroyWindow(MyGUI::Window * pWindow); // ���������� ���� � ������� �� ���� ������� (�� ������ �� �������)
		
		/*
		    Destroys an entire layer of Windows.
		*/		
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // ���������� ��� ���� �� �������		
		
		/*
		    Called to set up the default overlays, as well as to set up new overlay layers as needed to handle
		    overlapping windows.  Does not need to be a public interface if the Window class's use can be abstracted
		*/
		Ogre::Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // ������� �������

		/*
		    Moves a window up in its ZOrder (To the top of the stack?)
		*/
		void upZOrder(Window *pWindow);
		
		/*
		    These are both only referenced by Window class, so should be moved as private members of window class
		*/
		void getLengthText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // ���������� ������ ������
		void getCutText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign); // ���������� ���������� ������ ������ ��������

		uint16 m_uMaxZOrder; // ������� ������������ ����� ����
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // ������� ���
		Ogre::OverlayContainer* m_overlayContainerMouse; // ��������� ��� �������
        
        
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // ������� �������� ��� �������
		unsigned char m_bShiftChars; // ����� � ������� �������� ��� ����� (0 - 3)

		std::vector <Window*> mChildWindows; // ���� ���
		size_t m_uOverlappedStart, m_uOverlappedEnd; // ������ � ����� � ��������������� ���� � ������
		Window * m_currentWindow; // ���� ��� ������� ������, �� ������ ����� ���
		Window * m_currentFocusWindow; // ������� ���� ������� ����� ����������� �� ����
		Window * m_currentEditWindow; // ������� ���� ������� ����� ����������� �� �����
		Window * m_currentFadeWindow; // ���� �������������� ����������
		EventCallback *m_pEventCallback; // ������� ��������� �� ����� ��� ������ ������� � ������� ��������������
		bool m_bIsFocusWindowCapture; // ���������� �� ����
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // �������� ������� ������ ���������

		Ogre::Timer m_time; // ������ ��� �������� ������
		uint8 m_bFadeState; // ���� �������� ����������
		Real m_fCurrentFadeAlpha; // ������� ����� ����������
		FADE_STATES m_uFadeID; // ������������� ����������
		EventCallback *m_pEventCallbackFade; // ��������� �� ����� ��� ������ ����� ���������

		std::map <String, uint16> m_mapFlagEvent; // ����� ������ ��� ��������
		std::map <String, uint16> m_mapFlagAlign; // ����� ������ ��� ��������
		std::map <String, uint16> m_mapFlagStyle; // ����� ������ ��� ��������

	};

}