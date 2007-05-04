//==================================================================================================
#ifndef __MyGUI_GUI_H__
#define __MyGUI_GUI_H__
//==================================================================================================
#include "MyGUI.h"
//==================================================================================================
using namespace Ogre;
using namespace std;

namespace MyGUI {

	class Window;
	class WindowFrame;
	class Button;
	class Edit;
	class VScroll;
	class HScrollBar;
	class Tab;
	class StaticText;
	class List;
	class ComboBox;
	class Message;

	enum __FADE_STATE { // ������� ����� ����������
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};
	enum __MESSAGE_BOX_BUTTON { // ������ ���� ���������
		MBB_NONE = 0x00,
		MBB_BUTTON1 = 0x01, // ������ ������ �������������
		MBB_BUTTON2 = 0x02, // ������ ������ ������
		MBB_MODAL = 0x10, // ���� ����������� � ��������� ������, !!! ����������� ������ ����
	};
	enum __WINDOW_EX_STYLE { // ������������� ����� ��������� ������
		// ����� �����
		WES_NONE = 0x0000, // ������� ����
		WES_TEXT = 0x0001, // ������� �������� ��������� ������ ��� ����� ��������
		WES_CLIENT = 0x0002, // ������� �������� ��������� �����
		WES_ACTION1 = 0x0004,
		WES_ACTION2 = 0x0008,
		WES_ACTION3 = 0x0010,
		WES_ACTION4 = 0x0020,
		WES_ACTION5 = 0x0040,
		WES_ACTION6 = 0x0080,
		// ����� ��� WindowFrame
		WES_CAPTION = 0x0010, // ������� �������� ����������
		WES_RESIZE = 0x0020, // ������� �������� ����������
		WES_CLOSE = 0x0040, // ������� �������� ������� ��������
		// ����� ��� Button
		WES_BUTTON = 0x0010, // ������� �������� �������
		// ����� ��� Edit
		WES_EDIT = 0x0010, // ������� �������� ����� ��������������
		WES_EDIT_CURSOR = 0x0020, // ������� �������� �������� ��� ���� ��������������
		// ����� ��� VScroll
		WES_VSCROLL_UP = 0x0010, // ������� �������� ������� ������� �������
		WES_VSCROLL_DOWN = 0x0020, // ������� �������� ������ ������� �������
		WES_VSCROLL_TRACK = 0x0040, // ������� �������� ������ ������� �������
		// ����� ��� TabBar
		WES_TAB_TOP = 0x0010, // ������� �������� ������� ������ ��� ������������
		// ����� ��� ������
		WES_LIST_HIDE_SCROLL = 0x0010, // �������� ������ , ���� �� �� �����
		// ����� ��� ����������� ������
		WES_COMBO = 0x0010, // ���������� ������
		WES_COMBO_BUTTON = 0x0020, // ������ � ���������� ������
	};

	enum __WINDOW_EVENT { // ������� �� ������� ���� ����� ��������
		WE_NONE = 0x00, // ������ �� ����� ����������
		WE_MOUSE_BUTTON=0x01, // ������ ����� ����� ����
		WE_MOUSE_FOCUS=0x02, // ����� ���������� �� ������� ����
		WE_MOUSE_MOVE=0x04, // �������� ���� ����� �������
		WE_KEY_FOCUS=0x08, // ����� ������ ��� ����� ����������
		WE_KEY_BUTTON=0x10, // ������ �������
		__WE_IS_ACTION = 0xFF, // !! ��� ����������� ������������� , �� �����  |   ���� �� ������� �� ������� ���� � ����������
		// ��������� �� ���������, ���������� ������
		WE_WARNING_ZORDER_MAX = 0x1000, // ���� ��������������� ���� ����������� � �������
		WE_WARNING_CHILD_MAX = 0x2000, // ����������� ����� ����� ���� ����� ������
		WE_WARNING_CHILD_DELETE = 0x4000, // ������� ���������, �������� ���������
	};
	enum __WINDOW_OTHER_EVENT { // ������� ���������� � OtherEvents
		WOE_NONE = 0x00000, // ��� ���������
		WOE_SCROLL_CHANGE_POS = 0x0001, // ���������� ������� �������
		WOE_FRAME_CLOSE = 0x0002, // ������ ������ ������� �����
		WOE_EDIT_KEY_ACCEPT = 0x0004, // ������ ������ �����
		WOE_LIST_SELECT_CHANGE = 0x0008, // ��������� ���������� �������
		WOE_LIST_SELECT_ACCEPT = 0x0010, // ������� ������ ��� ����� �� ��������
		WOE_LIST_SELECT_NONE = 0x0020, // ������ �� ������ �����
		WOE_COMBO_SELECT_ACCEPT = 0x0040, // ����������� �������
		WOE_MESSAGE_PRESS_BUTTON = 0x0080, // ������ ������ � ���� ���������
	};
	enum __WINDOW_STATE { // ��������� ����
		WS_DEACTIVE, // ���� ��������� � ����������
		WS_NORMAL, // ������� ��������� ����
		WS_PRESSED, // ������ ������
		__WS_ACTIVED, // ������ ��� ��������, ��� ����������� �������������
		__WS_SELECTED // ������ ��� �������� � ������, ��� ����������� �������������
	};
	enum __WINDOW_ALIGIN {
		// ������������ ����, ���� ����� ������� ����, �� ����������� (WA_LEFT|WA_TOP)
		WA_NONE=0x00, //                                 ���� ������������� �� ������
		WA_HCENTER=0x00, //                           ����, ��� ���� �� ����� ������������ ��� ����������
		WA_VCENTER=0x00, //                            �� ������������ �� ������������ ������ ����
		WA_CENTER=0x00, //                              � ������������ ������ ��������������� ���������, ���� ����� ������ ������ ����
		WA_CENTER_FOTHER=0x1000, //           ��� ������ ����, ������ ������������ �� ������ ����, �� ��������� ������������ �������� ���������
		WA_LEFT=0x01, WA_RIGHT=0x02, //     �� ������ ��� �� �������
		WA_HSTRETCH=0x03,  //                        ������������ �� �����������
		WA_TOP=0x04, WA_BOTTOM=0x08, //  �� ����� ��� �� ����
		WA_VSTRETCH=0x0C,  //                        ������������ �� ���������
		WA_STRETCH=0x0F, //                           ������������ �� ���� ������
		// ������������ ������, � ���� ��� ������������
		WAT_NONE=0x00, // �� ����������� �� ��, ��� WAT_LEFT|WAT_TOP
		WAT_LEFT=0x10, WAT_RIGHT=0x20, WAT_HCENTER=0x30, // �������������
		WAT_TOP=0x40, WAT_BOTTOM=0x80, WAT_VCENTER=0xC0, // �����������
		WAT_CENTER=0xF0, // �� ������ ����
		// ������� ������
		WAT_CUT_RIGHT=0x0100, WAT_CUT_LEFT=0x0200, // �� ����� ��� �� ������ �������
		WAT_ADD_DOT=0x0400, // ��������� ��� ����� ��� �������
		WAT_CUT_RIGHT_DOT=0x0500, WAT_CUT_LEFT_DOT=0x0600, // �� ����� ��� �� ������ ������� � ��� � �������
		__WAT_IS_CUT=0x0300, // ��� ���������� ����, �� �����!  ����� �� �������
		WAT_MULTI_LINE = 0x0800, // ������������� �����
		WAT_BREAK_WORD = 0x2000, // ���������� �� ������
		WAT_SHIFT_TEXT_PRESSED = 0x8000, // �������� �� ����� ��� �������
	};

	// ������� ����
	enum __MYGUI_POINTER {
		POINTER_DEFAULT, // ������� ���������
		POINTER_RESIZE, // ��� ��������� ��������
		POINTER_TEXT, // ��� ���������� �����
		__POINTER_COUNT
	};

	// ��� ������� ������, ��������� ����� OVERLAY_MOUSE � �� __OVERLAY_COUNT, ����� ���� ��������� (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE, // ������� ��� ����������
		OVERLAY_MOUSE, // ������� ��� ����, !!! �� ������������ ��� �������� ����
		OVERLAY_POPUP, // ������� ��� ������������ ���� , ����, ����������, ���� �� �������������
		OVERLAY_MAIN, // �������� ������� ���
		OVERLAY_BACK, // ����� ������ ������� ���
		__OVERLAY_COUNT, // ����������� ��������, !!! �� ������������ ��� �������� ����
		OVERLAY_OVERLAPPED, // ��� �������� ��������������� ����
		OVERLAY_CHILD, // ���� ����������� ����, ����������� ������� ����
		OVERLAY_DESTROY_ALL // �������� ���� ���� � ������� destroyWindow
	};

	class EventCallback // ������� ����� ��� �������
	{
	public:
		virtual void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) {}; // ������ ����� ������ ����
		virtual void onMouseClick(MyGUI::Window * pWindow) {}; // ������ � �������� ����� ������ ���� �� ���� �� ��������
		virtual void onMouseDoubleClick(MyGUI::Window * pWindow) {}; // ������� ������ ����
		virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // ����� ������
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) {}; // ����������� � ��������, �� �� ��������

		virtual void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // ����� ������ �����
		virtual void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) {}; // ������ �������

		virtual void onWarningEvent(MyGUI::Window * pWindow, uint16 uEvent) {}; // ������� ��������������� ������
		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) {}; // �������������� �������
		virtual void onFadeEnd(bool bIsFade, uint8 fadeID) {}; // ����������� ����������
	};

	class GUI {

	public:

		GUI (uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0); // ������������ ����������� ��������
		~GUI();

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
		void showMousePointer(bool bIsShow) {if (bIsShow) m_overlayGUI[OVERLAY_MOUSE]->show();else m_overlayGUI[OVERLAY_MOUSE]->hide();}; // ������ �������� ������
		void setKeyFocus(Window * pWindow); // ������ ����� �����
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // �������� �����
		void createSkin(); // ��������� ��� �����
		void unloadResource(); // ��������� ��� �����
		void clearSkins(__LP_MYGUI_SKIN_INFO pSkin); // ������� ����
		void loadSkin(std::map<String, uint32> & mapNameValue, const String & strFileName); // ��������� ��� ����� ���� �� ������ �����

		MyGUI::Window * createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback = 0); // ������������ ����
		MyGUI::Window * createWindowReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback = 0) { // ������������ ����
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createWindow(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		};
		MyGUI::WindowFrame * createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay = OVERLAY_OVERLAPPED, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0); // ���� � ������
		MyGUI::WindowFrame * createWindowFrameReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay = OVERLAY_OVERLAPPED, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0) { // ���� � ������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createWindowFrame(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		};
		MyGUI::Button * createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0); // ������� ������
		MyGUI::Button * createButtonReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0) { // ������� ������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createButton(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Edit * createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0); // ������� ���� ��������������
		MyGUI::Edit * createEditReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0) { // ������� ���� ��������������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createEdit(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::VScroll * createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0); // ������� ������ ���������
		MyGUI::VScroll * createVScrollReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0) { // ������� ������ ���������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createVScroll(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Tab * createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0); // ������� ������ �������
		MyGUI::Tab * createTabReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0) { // ������� ������ �������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createTab(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::StaticText * createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0); // ������� ����������� �����
		MyGUI::StaticText * createStaticTextReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0) { // ������� ����������� �����
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createStaticText(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::List * createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0); // ������� ������
		MyGUI::List * createListReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0) { // ������� ������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createList(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::ComboBox * createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0); // ������� ���������� ������
		MyGUI::ComboBox * createComboBoxReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0) { // ������� ���������� ������
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createComboBox(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Message * createMessage(const DisplayString & strCaption, const DisplayString & strMessage, uint16 uID, bool bIsModal, const DisplayString & strButton1 = "Ok", const DisplayString & strButton2 = ""); // ���� ���������

		MyGUI::Window * getTopWindow(); // ���������� ����� ������� ���� �� ���������������
		void destroyWindow(MyGUI::Window * pWindow); // ���������� ���� � ������� �� ���� ������� (�� ������ �� �������)
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // ���������� ��� ���� �� �������
		Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // ������� �������

		void upZOrder(Window *pWindow); // ������� ���� �� ����� �����
		void getLenghtText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // ���������� ������ ������
		void getCutText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAligin); // ���������� ���������� ������ ������ ��������

		uint16 m_uMaxZOrder; // ������� ������������ ����� ����
		Overlay* m_overlayGUI[__OVERLAY_COUNT]; // ������� ���
		OverlayContainer* m_overlayContainerMouse; // ��������� ��� �������

		bool m_bIsActiveGUI; // ������� �� GUI
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // ������� �������� ��� �������
		unsigned char m_bShiftChars; // ����� � ������� �������� ��� ����� (0 - 3)

		uint16 m_uWidth, m_uHeight; // ������� ������

		vector <Window*> m_aWindowChild; // ���� ���
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // ������ � ����� � ��������������� ���� � ������
		Window * m_currentWindow; // ���� ��� ������� ������, �� ������ ����� ���
		Window * m_currentFocusWindow; // ������� ���� ������� ����� ����������� �� ����
		Window * m_currentEditWindow; // ������� ���� ������� ����� ����������� �� �����
		Window * m_currentFadeWindow; // ���� �������������� ����������
		EventCallback *m_pEventCallback; // ������� ��������� �� ����� ��� ������ ������� � ������� ��������������
		bool m_bIsFocusWindowCapture; // ���������� �� ����
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // �������� ������� ������ ���������

		Timer m_time; // ������ ��� �������� ������
		uint8 m_bFadeState; // ���� �������� ����������
		Real m_fCurrentFadeAlpha; // ������� ����� ����������
		uint8 m_uFadeID; // ������������� ����������
		EventCallback *m_pEventCallbackFade; // ��������� �� ����� ��� ������ ����� ���������

		vector <__LP_MYGUI_FONT_INFO> m_fontInfo; // ������ ���� �������
		vector <__LP_MYGUI_WINDOW_INFO> m_windowInfo; // ������ ���� ���� ������
		vector <__LP_MYGUI_POINTER_INFO> m_pointerInfo; // ������ ���������� �� �������

		vector <String> m_strSkinNames; // ����� ������
		vector <String> m_strFontNames; // ����� �������

		map <String, uint16> m_mapFlagEvent; // ����� ������ ��� ��������
		map <String, uint16> m_mapFlagAligin; // ����� ������ ��� ��������
		map <String, uint16> m_mapFlagStyle; // ����� ������ ��� ��������

	};

}
//==================================================================================================
#endif
//==================================================================================================
