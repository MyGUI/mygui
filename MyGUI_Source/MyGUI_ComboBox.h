//=========================================================================================
#ifndef __MyGUI_ComboBox_H__
#define __MyGUI_ComboBox_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class Window;
	class List;
	class GUI;

	class ComboBox : public Window {

	public:
		ComboBox(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		~ComboBox();

		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // ���������� ��� ������� ������� �����

		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed); // ������ ����� ������ ����
		void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������ �����
		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // ������ �������
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������

		inline void addString(const DisplayString & strElement){m_pList->addString(strElement);}; // �������� ������ � ������
		inline void insertString(uint16 index, const DisplayString & strElement){m_pList->insertString(index,strElement);}; // �������� ������ � ������
		inline void deleteString(uint16 index){m_pList->deleteString(index);}; // ������� ������ �� ������
		const DisplayString & getString(uint16 index); // ���������� ������
		void setString(uint16 index); // ������������� ������ �� ������ � ����
		inline uint16 getCurrentIndex() {return m_pList->m_uSelectItem;}; // ���������� ������� ���������
		inline void deleteStringAll() {m_pList->deleteStringAll();setWindowText("");}; // ������� ��� ������
		inline uint16 getStringCount() {return (uint16)m_pList->m_aString.size();}; // ���������� ����������� �����
		
		Edit * m_pEdit; // ���� ��� ������
		List * m_pList; // ����������� ������
		bool m_bIsListShow; // ����� �� ������
		bool m_bHideList; // ������ ������
	};

}
//=========================================================================================
#endif
//=========================================================================================
