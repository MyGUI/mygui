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

	class GUI {

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

		bool eventMouseMove( const OIS::MouseEvent &arg ); // �������� ��� �������� ����
		bool eventMousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ������� ������ ����
		bool eventMouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // �������� ��� ���������� ������ ����

		bool eventKeyPressed( const OIS::KeyEvent &arg ); // �������� ��� ������� ������ �����
		bool eventKeyReleased( const OIS::KeyEvent &arg ); // �������� ��� ���������� ������ �����

		void eventWindowResize(uint16 uWidth, uint16 uHeight); // ��������� ������ �������� ����
		void eventUpdateAnimation(Real fTimeLastFrame); // ��������� �������� ���

		void detectLangShift(int keyEvent, bool bIsKeyPressed); // �������� �� ������������ ������
		wchar_t getKeyChar(int keyEvent); // ���������� ������ �� ��� ���� ����

		void createMousePointer(); // ������� ������� ��� ���� � �������
		void setMousePointer(uint8 uTypePointer); // �������� ���������
		void showMousePointer(bool bIsShow) {
		    if (bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
            else
                m_overlayGUI[OVERLAY_MOUSE]->hide();
        } // ������ �������� ������
		void setKeyFocus(Window * pWindow); // ������ ����� �����
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // �������� �����
		void createSkin(); // ��������� ��� �����
		void unloadResource(); // ��������� ��� �����
		void clearSkins(__LP_MYGUI_SKIN_INFO pSkin); // ������� ����
		void loadSkin(std::map<String, uint32> & mapNameValue, const String & strFileName); // ��������� ��� ����� ���� �� ������ �����
		
		template<typename Widget> Widget *spawn(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
		    uint8 uOverlay, uint8 uSkin = __SKIN_WIDGET_DEFAULT)
		{
	        if(uSkin == __SKIN_WIDGET_DEFAULT)
	            return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay);
	        else
	            return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay, uSkin);
	    }
		    
		template<typename Widget> Widget *spawnReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
		    uint8 uOverlay, uint8 uSkin = __SKIN_WIDGET_DEFAULT)
		{
		    return spawn<Widget>(PosX *  GUI::getSingleton()->m_uWidth, PosY *  GUI::getSingleton()->m_uHeight,
                             SizeX * GUI::getSingleton()->m_uWidth, SIzeY * GUI::getSingleton()->m_uHeight,
                             uOverlay, uSkin);		
		}

		MyGUI::Message * createMessage(const DisplayString & strCaption,
		    const DisplayString & strMessage, uint16 uID, bool bIsModal,
		    const DisplayString & strButton1 = "Ok", const DisplayString & strButton2 = ""); // ���� ���������

		MyGUI::Window * getTopWindow(); // ���������� ����� ������� ���� �� ���������������
		void destroyWindow(MyGUI::Window * pWindow); // ���������� ���� � ������� �� ���� ������� (�� ������ �� �������)
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // ���������� ��� ���� �� �������
		Ogre::Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // ������� �������

		void upZOrder(Window *pWindow); // ������� ���� �� ����� �����
		void getLenghtText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // ���������� ������ ������
		void getCutText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign); // ���������� ���������� ������ ������ ��������

		uint16 m_uMaxZOrder; // ������� ������������ ����� ����
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // ������� ���
		Ogre::OverlayContainer* m_overlayContainerMouse; // ��������� ��� �������

		bool m_bIsActiveGUI; // ������� �� GUI
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // ������� �������� ��� �������
		unsigned char m_bShiftChars; // ����� � ������� �������� ��� ����� (0 - 3)

		uint16 m_uWidth, m_uHeight; // ������� ������

		std::vector <Window*> m_aWindowChild; // ���� ���
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // ������ � ����� � ��������������� ���� � ������
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
		uint8 m_uFadeID; // ������������� ����������
		EventCallback *m_pEventCallbackFade; // ��������� �� ����� ��� ������ ����� ���������

		std::vector <__LP_MYGUI_FONT_INFO> m_fontInfo; // ������ ���� �������
		std::vector <__LP_MYGUI_WINDOW_INFO> m_windowInfo; // ������ ���� ���� ������
		std::vector <__LP_MYGUI_POINTER_INFO> m_pointerInfo; // ������ ���������� �� �������

		std::vector <String> m_strSkinNames; // ����� ������
		std::vector <String> m_strFontNames; // ����� �������

		std::map <String, uint16> m_mapFlagEvent; // ����� ������ ��� ��������
		std::map <String, uint16> m_mapFlagAlign; // ����� ������ ��� ��������
		std::map <String, uint16> m_mapFlagStyle; // ����� ������ ��� ��������

	};

}