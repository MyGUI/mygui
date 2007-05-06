//=========================================================================================
#ifndef __MyGUI_List_H__
#define __MyGUI_List_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class GUI;
	class VScroll;

	class List : public Window {

	public:
		List(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		~List();

		void _OnKeyChangeFocus(bool bIsFocus); // ���������� ��� ����� ���������� �����
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������ �����
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseClick(MyGUI::Window * pWindow);
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������
		void onMouseDoubleClick(MyGUI::Window * pWindow); // ������� ������ ����

		void size(int16 iSizeX, int16 iSizeY); // �������� ������ ����

		void addString(const DisplayString & strElement, bool bIsRedraw = true); // �������� ������ � ������
		void insertString(uint16 index, const DisplayString & strElement, bool bIsRedraw = true); // �������� ������ � ������
		void deleteString(uint16 index, bool bIsRedraw = true); // ������� ������ �� ������
		void deleteStringAll(bool bIsRedraw = true); // ������� ��� ������
		const DisplayString & getString(uint16 index); // ���������� ������
		void setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour); // ������������� ����� ��� ����� ������
		inline uint16 getStringCount() {return (uint16)m_aString.size();}; // ���������� ����������� �����

		void redrawStrings(); // �������������� ��� ������
		void recalcScroll(); // �������� ������ ���������

		VScroll * m_scroll; // ������ ����
		uint8 m_uSkinButton; // ���� ��� ������ ������
		uint8 m_uSizeYButton; // ������ ������
		uint8 m_uSizeXScroll; // ������ ������
		uint16 m_uCurrentFillSize; // ����� �� ���� �������� ����
		uint8 m_uStartWindow; // ������ ���������� ������ - 0 ��� 1 (���� ������)
		uint8 m_uCountVisible; // ������� ������� �����

		vector <DisplayString * > m_aString; // ������ �����
		uint16 m_uOffsetDrawString; // ������� ������ ������� ������
		uint16 m_uSelectItem; // ���������� �������
		Window *pWindowSelect; // ������� ���������� �������

		bool m_bIsFocus; // � ������ �� �������
		bool m_bIsVisibleScroll; // ����� �� ������
		bool m_bIsOneClickActived; // ������ ����� ������ ����

	};

}
//=========================================================================================
#endif
//=========================================================================================
