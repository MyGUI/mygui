//=========================================================================================
#ifndef __MyGUI_Window_H__
#define __MyGUI_Window_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class WindowCallback;
	class GUI;
	class Edit;
	class Button;
	class VScroll;
	class HScrollBar;
	class Tab;
	class StaticText;
	class List;
	class ComboBox;

	class Window : public EventCallback { // ������� ����� ���� ���

	public:
		Window(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		virtual ~Window();

		virtual void _OnMouseChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �� �������
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // ���������� ��� �������� ����
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // ���������� ��� ������� �������
		virtual void _OnMouseButtonClick(bool bIsDouble); // ���������� ��� ������� ������� � ���������� �� ��� �� ��������
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����
//		virtual void _OnKeyButtonClick(int keyEvent); // ���������� ��� ���������� ������� �����
		virtual void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		virtual void _OnUpZOrder() {}; // ���������� ��� ��������� ����

		MyGUI::Window * createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback = 0); // ������������ ����
		MyGUI::Window * createWindowReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback = 0) { // ������������ ����
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createWindow(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		};
		MyGUI::WindowFrame * createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0); // ���� � ������
		MyGUI::WindowFrame * createWindowFrameReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0) { // ���� � ������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createWindowFrame(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		};
		MyGUI::Button * createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0); // ������� ������
		MyGUI::Button * createButtonReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0) { // ������� ������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createButton(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		}
		MyGUI::Edit * createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0); // ������� ���� ��������������
		MyGUI::Edit * createEditReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0) { // ������� ���� ��������������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createEdit(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::VScroll * createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0); // ������� ������ ���������
		MyGUI::VScroll * createVScrollReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0) { // ������� ������ ���������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createVScroll(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::Tab * createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0); // ������� ������ �������
		MyGUI::Tab * createTabReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0) { // ������� ������ �������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createTab(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::StaticText * createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0); // ������� ����������� �����
		MyGUI::StaticText * createStaticTextReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0) { // ������� ����������� �����
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createStaticText(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		}
		MyGUI::List * createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0); // ������� ������
		MyGUI::List * createListReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0) { // ������� ������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createList(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::ComboBox * createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0); // ������� ���������� ������
		MyGUI::ComboBox * createComboBoxReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0) { // ������� ���������� ������
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createComboBox(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}

		void show(bool bIsShow); // ������ �������� ����
		void move(int16 iPosX, int16 iPosY); // ����������� ����
		virtual void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // �������� ���� �� ������� �������
		void setState(uint8 uState); // ��������� ����
		uint8 getState(); // ��������� ����
		void showFocus(bool bIsFocus); // ������������� ����
		virtual void setWindowText(const DisplayString & strText); // ������������� ����� ����
		const DisplayString & getWindowText(); // ���������� ������ ����
		virtual void aliginWindowText(); // ��������� ������������ ������
		void aliginWindow(int16 rNewSizeX, int16 rNewSizeY); // ����������� ���� ������������ ����
		void setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour); // ������������� ����� ��� ��������
		void setFont(uint8 uFont, uint32 colour); // ������������� ����� ��� ��������

		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // ��������� ������� �� ������� ���� �����������

		inline void setUserString(const DisplayString & strUserString) {m_strUserString = strUserString;}; // ������ ������ ������������
		inline DisplayString & getUserString() {return m_strUserString;}; // ���������� ������ ������������
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ������ ������ ������������
		inline uint32 getUserData() {return m_uUserData;}; // ���������� ������ ������������


		Overlay* m_overlay; // ������� ����� ����
		PanelOverlayElement * m_overlayContainer; // ������� ��������
		OverlayContainer* m_overlayCaption; // ������� ������
		vector<MyGUI::Window*>m_aWindowChild; // ���� ����
		Window * m_pWindowFother; // ���� ����
		Window * m_pWindowText; // ������� ����� ���������� ����� ����� �������� (�� ������� this)
		Window * m_pWindowClient; // ������� ����� �������� ��������� ����� ����� �������� (�� ������� this)
		int16 m_iPosX, m_iPosY, m_iSizeX, m_iSizeY; // ������� ����
		int16 m_iOffsetAliginX, m_iOffsetAliginY; // ��������, ������������ ������ ��� ������������ �� ������ ��� ����������

		GUI *m_GUI; // �������� �� ���
		uint8 m_uState; // ������ ����
		EventCallback *m_pEventCallback; // ��������� �� ����� ��� ������ �������
		uint16 m_uEventCallback; // ����� ��� ������� �������
		uint16 m_uAligin; // ������������ ���� � ������
		int16 m_sizeTextX; // ������ ������ ����
		int16 m_sizeTextY; // ������ ������ ����
		int16 m_sizeCutTextX; // ������� ������ ������
		int16 m_sizeCutTextY; // ������� ������ ������
		DisplayString m_strWindowText; // ����� ���� ��� ������ �������, ����� ����� �� ������� ��� ������ �����
		bool m_bIsOverlapped; // ���� ���������������
		bool m_bIsTextShiftPressed; // ������� �� ����� ����

		__LP_MYGUI_FONT_INFO m_font; // ����� ����
		uint32 m_fontColour; // ���� ������
		String * m_paStrSkins[__SKIN_STATE_COUNT]; // ����� ���������
		uint16 m_uExData; // �������������� ���������� �� ��������

		DisplayString m_strUserString; // ������ ��� �������������� ���������� ������������
		uint32 m_uUserData; // �������������� ���������� ������������

	};

}
//=========================================================================================
#endif
//=========================================================================================
