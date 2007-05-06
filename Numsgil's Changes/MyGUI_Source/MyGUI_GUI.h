#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Coord.h"
#include "MyGUI_OIS.h"
#include "MyGUI_EventCallBack.h"
#include <OgreTimer.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>

namespace MyGUI {

	class GUI : public OIS::MouseListener, public OIS::KeyListener
	{
        GUI ();
		~GUI();
	
	public:
	    static GUI *getSingleton()
	    {
	        static GUI gui;
	        return &gui;	    
	    }
		
		GUI *Initialize(uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0);
    
        //Animation
		uint8 m_bFadeState; // ���� �������� ����������
		    //Fade
		    Real m_fCurrentFadeAlpha; // ������� ����� ����������
		    uint8 m_uFadeID; // ������������� ����������
		    EventCallback *m_pEventCallbackFade; // ��������� �� ����� ��� ������ ����� ���������
		void eventUpdateAnimation(Real fTimeLastFrame); // ��������� �������� ���
        Window * m_currentFadeWindow; // ���� �������������� ����������
        
		//Mouse
		private:
		bool m_bIsActiveGUI; // ������� �� GUI
		
		public:
		//override OIS implementations
		bool mouseMoved( const OIS::MouseEvent &arg ); // �������� ��� �������� ����
		bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ������� ������ ����
		bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ���������� ������ ����
		
		void createMousePointer();
		void setMousePointer(const Ogre::String uTypePointer);
		void showMousePointer(bool bIsShow)
		{
		    if (bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
            else
                m_overlayGUI[OVERLAY_MOUSE]->hide();
        };
        
        Ogre::String uPointerType; //current pointer type
		
		//Keyboard
		private:
		unsigned char m_bShiftChars; // ����� � ������� �������� ��� ����� (0 - 3)
		
		public:
		//override OIS implementations
		bool keyPressed( const OIS::KeyEvent &arg ); // �������� ��� ������� ������ �����
		bool keyReleased( const OIS::KeyEvent &arg ); // �������� ��� ���������� ������ �����
		
		void setKeyFocus(Window * pWindow); // ������ ����� �����
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // �������� �����
		
		void detectLangShift(int keyEvent, bool bIsKeyPressed); // �������� �� ������������ ������
		wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����
		
        //GUI Builder
		template<typename Widget> Widget *spawn(const Coord &Pos, const Coord &Size,
            const uint8 uOverlay, const Ogre::String &uSkin = __SKIN_WIDGET_DEFAULT)
        {
            if(uSkin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(Pos, Size, NULL, 0, uOverlay);
            else
                return Widget::create(Pos, Size, NULL, 0, uOverlay, uSkin);
        }
        
        template<typename Widget> Widget *spawnReal(const Ogre::Vector2 &Pos, const Ogre::Vector2 &Size,
            const uint8 uOverlay, const uint8 uSkin = __SKIN_WIDGET_DEFAULT)
        {
            Coord NewPos, NewSize;
            
            NewPos  = Coord(Pos *  Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight));
            NewSize = Coord(Size * Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight));
            
            return spawn<Widget>(NewPos, NewSize, uOverlay, uSkin);
        }
		
		
		Message * createMessage(const Ogre::DisplayString & strCaption,
		                        const Ogre::DisplayString & strMessage,
		                        uint16 uID, bool bIsModal,
		                        const Ogre::DisplayString & strButton1 = "Ok",
		                        const Ogre::DisplayString & strButton2 = "");

		
		Window * m_currentWindow; // ���� ��� ������� ������, �� ������ ����� ���
		Window * m_currentFocusWindow; // ������� ���� ������� ����� ����������� �� ����
		Window * m_currentEditWindow; // ������� ���� ������� ����� ����������� �� �����
				
		Window * getTopWindow();
		void destroyWindow(Window * pWindow); //destroys a specific window
		
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); //destroys all windows on an overlay level (?)
		
		//Z Order
		void upZOrder(Window *pWindow); // ������� ���� �� ����� �����
		uint16 m_uMaxZOrder; // ������� ������������ ����� ����
        
        //Overlay
		Ogre::Overlay * createOverlay(Ogre::String strName, uint16 zOrder, bool bIsShow = true);
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // ������� ���
		Ogre::OverlayContainer* m_overlayContainerMouse; // ��������� ��� �������
        
        public:
		void eventWindowResize(uint16 uWidth, uint16 uHeight); // ��������� ������ �������� ����
		uint16 m_uWidth, m_uHeight; // ������� ������

		std::vector <Window*> m_aWindowChild; // ���� ���
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // ������ � ����� � ��������������� ���� � ������
		
		EventCallback *m_pEventCallback; // ������� ��������� �� ����� ��� ������ ������� � ������� ��������������
		bool m_bIsFocusWindowCapture; // ���������� �� ����
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // �������� ������� ������ ���������
	};

}