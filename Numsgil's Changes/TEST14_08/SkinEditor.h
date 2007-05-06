#pragma once

#include "MyGUI_Source\\MyGUI.h"

using namespace Ogre;
using namespace std;

// ����� ������ ��� �������������� ������
class SkinEditor : public MyGUI::EventCallback
{
	public:

		bool createEditor(); // ������� ���� �������������� ������
		void destroyEditor(); // ������� ���� �������������� ������

		typedef struct _tag_STATE_DATA { // ���������� �� ����� ������
			size_t uPosition[4]; // ������� � ��������
		} * LP_STATE_DATA;

		typedef struct _tag_SUB_SKIN_DATA {
			Ogre::String strName; // �������� ��� �����
			MyGUI::uint16 uOffset[4]; // �������� � ��������
			MyGUI::uint16 event_info; // �������
			MyGUI::uint16 style; // �����
			MyGUI::uint16 aligin; // ������������
			_tag_STATE_DATA stateInfo[5]; // �� ������ ���������
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const String & str) : strName(str)
			{
				memset((void*)uOffset, 0, sizeof(MyGUI::uint16) * 4); // �������
				memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // �������
				event_info = 0;
				style = 0;
				aligin = 0;
			};
		} * LP_SUB_SKIN_DATA;

		typedef struct _tag_WINDOW_DATA {
			Ogre::String uAddedSkin1; // �������������� �����
			Ogre::String uAddedSkin2; // �������������� �����
			Ogre::String uAddedData1; // �������������� ������
			Ogre::String uAddedData2; // �������������� ������
			Ogre::String uFont; // ����� ������
			Ogre::String strColour; // �������� �����
			MyGUI::uint16 uMaterial; // �������� �� ���� �������
			std::vector <LP_SUB_SKIN_DATA> sabSkins; // ��������� �� ��������
			_tag_WINDOW_DATA::_tag_WINDOW_DATA() : uAddedSkin1(0), uAddedSkin2(0), uAddedData1(0), uAddedData2(0), uFont(0), uMaterial(0) {};
		} * LP_WINDOW_DATA;


		virtual void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // �������������� �������
		virtual void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

		SkinEditor::LP_SUB_SKIN_DATA findSkinData(const Ogre::String & strName, bool create = true); // ���� ������ ���� ��� �� �������
		void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // ������� ��� ����
		void destroySkins(); // ������� ��� ��� �����
		void loadSkin(); // ��������� ����
		void saveSkin(); // ��������� ����

		void updateWindowInfo(); // ��������� ��� ���� �� ����
		void updateSkinInfo(); // ��������� ��� ���� �� ��� �����
		void updateStateInfo(); // ��������� ��� ���� �� ��� �����
		void createFlagWindow(); // ������� ���� � ��������������� �������
		void createMaterialWindow(); // ������� ���� ��� ���������
		void fillFlagWindow(); // ��������� ���� �������� ��������
		void fillMaterialWindow(); // ��������� ���� � ����������
		void pressOtherButton(MyGUI::Window * pWindow); // ������� � �������� ������

        MyGUI::WindowFrame * m_mainWindow; // ������� ���� ������������
		MyGUI::ComboBox * m_comboBasisWindowName; // ������� ���� ����

		MyGUI::ComboBox * m_comboBasisAddedSkin1; // �������������� ���� 1
		MyGUI::ComboBox * m_comboBasisAddedSkin2; // �������������� ���� 2
		MyGUI::Edit * m_editBasisData1; // �������������� ������ 1
		MyGUI::Edit * m_editBasisData2; // �������������� ������ 1
		MyGUI::ComboBox * m_comboBasisFont; // ����� ����
		MyGUI::ComboBox * m_comboBasisColour; // ���� ����
		MyGUI::ComboBox * m_comboMaterialName; // �������� ��� ������� �����

		MyGUI::ComboBox * m_comboSabSkinName; // ��� ����� ����
		MyGUI::Button * m_buttonSabSkinCreate; // �������� ��� ����
		MyGUI::Button * m_buttonSabSkinDelete; // ������� ��� ����
		MyGUI::Button * m_buttonSabSkinStyle; // ����� �����

		MyGUI::ComboBox * m_comboSabSkinState; // ��������� �����

		enum {EDIT_NONE=0, EDIT_IS_USE};
		MyGUI::Edit * m_editOffset[4]; // �������� ����� ������ ��������
		MyGUI::Edit * m_editPosition[4]; // ������� ���� � ����� �����

		std::vector <_tag_WINDOW_DATA> mWindowInfo; // ���������� � ���� �����
		std::vector <String> m_strMaterialName; // ��� ��������� ���������

		LP_WINDOW_DATA m_pCurrentDataWindow; // ������� ����
		LP_SUB_SKIN_DATA m_pCurrentDataSkin; // ������� ��� ����
		LP_STATE_DATA m_pCurrentDataState; // ������� ����� �����

		MyGUI::WindowFrame * m_windowStateFlags; // ���� � �������
		MyGUI::Button * m_buttonsFlagsEvent[16];
		MyGUI::Button * m_buttonsFlagsAligin[16];
		MyGUI::Button * m_buttonsFlagsStyle[16];

		MyGUI::WindowFrame * m_windowMaterial; // ���� � ����������
		MyGUI::Window * m_windowMaterialLeft; // ����� �������
		MyGUI::Window * m_windowMaterialTop; // ������� �������
		MyGUI::Window * m_windowMaterialRight; // ������ �������
		MyGUI::Window * m_windowMaterialBottom; // ������ �������
};