//=========================================================================================
#ifndef __SkinEditor_H__
#define __SkinEditor_H__
//=========================================================================================
#include "MyGUI_Source\\MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace MyGUI;
//=========================================================================================
// ����� ������ ��� �������������� ������
class SkinEditor : public MyGUI::EventCallback
{
	public:

		enum {
			NONE=0,
			EDIT_IS_USE,
			MATERIAL_OFFSET,
		};
		enum {
			LEFT = 0,
			TOP = 1,
			RIGHT = 2,
			BOTTOM = 3,
			CENTER = 4,
			MATERIAL_BORDER_COUNT
		};

		typedef struct _tag_STATE_DATA { // ���������� �� ����� ������
			uint16 uPosition[4]; // ������� � ��������
		} * LP_STATE_DATA;

		typedef struct _tag_SUB_SKIN_DATA {
			String strName; // �������� ��� �����
			uint16 uOffset[4]; // �������� � ��������
			uint16 event_info; // �������
			uint16 style; // �����
			uint16 aligin; // ������������
			_tag_STATE_DATA stateInfo[5]; // �� ������ ���������
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA() {assert(0);}; // ����
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const String & str) : strName(str), event_info(0), style(0), aligin(0)
			{
				memset((void*)uOffset, 0, sizeof(uint16) * 4); // �������
				memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // �������
			};
		} * LP_SUB_SKIN_DATA;

		typedef struct _tag_WINDOW_DATA {
			String strElementName; // ��� ��������
			uint8 uAddedSkin1; // �������������� �����
			uint8 uAddedSkin2; // �������������� �����
			uint8 uAddedData1; // �������������� ������
			uint8 uAddedData2; // �������������� ������
			uint8 uFont; // ����� ������
			String strColour; // �������� �����
			uint16 uMaterial; // �������� �� ���� �������

			String strMaterialName; // ��� ���������
			uint16 sizeTextureX; // ������ ��������
			uint16 sizeTextureY; // ������ ��������

			vector <LP_SUB_SKIN_DATA> sabSkins; // ��������� �� ��������
			_tag_WINDOW_DATA::_tag_WINDOW_DATA() {assert(0);};
			_tag_WINDOW_DATA::_tag_WINDOW_DATA(const String & strName) : strElementName(strName), uAddedSkin1(0), uAddedSkin2(0), uAddedData1(0), uAddedData2(0), uFont(0), uMaterial(0), sizeTextureX(0), sizeTextureY(0) {};
		} * LP_WINDOW_DATA;


		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // �������������� �������
		virtual void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������
		virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // ����� ������
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iFotherPosX, int16 iFotherPosY); // ����������� � ��������, �� �� ��������

		bool createEditor(); // ������� ���� �������������� ������
		void destroyEditor(); // ������� ���� �������������� ������
		void loadSkin(const String & strFileName); // ��������� ����
		void saveSkin(const String & strFileName); // ��������� ����
		void resizeWindow(); // ���������� ������� ����

		SkinEditor::LP_SUB_SKIN_DATA findSkinData(const String & strName, bool create = true); // ���� ������ ���� ��� �� �������
		void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // ������� ��� ����
		void destroySkins(); // ������� ��� ��� �����

		void updateWindowInfo(); // ��������� ��� ���� �� ����
		void updateSkinInfo(); // ��������� ��� ���� �� ��� �����
		void updateStateInfo(); // ��������� ��� ���� �� ��� �����
		void createFlagWindow(); // ������� ���� � ��������������� �������
		void createMaterialWindow(); // ������� ���� ��� ���������
		void fillFlagWindow(); // ��������� ���� �������� ��������
		void fillMaterialWindow(); // ��������� ���� � ����������
		void pressOtherButton(MyGUI::Window * pWindow); // ������� � �������� ������
		void setMaterialOffset(uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY); // �������� �����


//		Edit * m_editInfo; // ���� ����������

		WindowFrame * m_mainWindow; // ������� ���� ������������
		ComboBox * m_comboBasisWindowName; // ������� ���� ����

		ComboBox * m_comboBasisAddedSkin1; // �������������� ���� 1
		ComboBox * m_comboBasisAddedSkin2; // �������������� ���� 2
		Edit * m_editBasisData1; // �������������� ������ 1
		Edit * m_editBasisData2; // �������������� ������ 1
		ComboBox * m_comboBasisFont; // ����� ����
		ComboBox * m_comboBasisColour; // ���� ����
		ComboBox * m_comboMaterialName; // �������� ��� ������� �����

		ComboBox * m_comboSabSkinName; // ��� ����� ����
//		Button * m_buttonSabSkinCreate; // �������� ��� ����
//		Button * m_buttonSabSkinDelete; // ������� ��� ����
		Button * m_buttonSabSkinStyle; // ����� �����

		ComboBox * m_comboSabSkinState; // ��������� �����

		Edit * m_editOffset[4]; // �������� ����� ������ ��������
		Edit * m_editPosition[4]; // ������� ���� � ����� �����

		vector <_tag_WINDOW_DATA> mWindowInfo; // ���������� � ���� �����
		vector <String> m_strMaterialName; // ��� ��������� ���������

		LP_WINDOW_DATA m_pCurrentDataWindow; // ������� ����
		LP_SUB_SKIN_DATA m_pCurrentDataSkin; // ������� ��� ����
		LP_STATE_DATA m_pCurrentDataState; // ������� ����� �����


		WindowFrame * m_windowStateFlags; // ���� � �������
		Button * m_buttonsFlagsEvent[16];
		Button * m_buttonsFlagsAligin[16];
		Button * m_buttonsFlagsStyle[16];

		WindowFrame * m_windowMaterial; // ���� � ����������
		Window * m_windowMaterialOffset[MATERIAL_BORDER_COUNT]; // �������
		uint16 m_uTextureSizeX;
		uint16 m_uTextureSizeY;

};
//=========================================================================================
#endif
//=========================================================================================
