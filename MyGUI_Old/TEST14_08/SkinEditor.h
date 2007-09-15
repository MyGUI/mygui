#pragma once

#include "MyGUI_Source\\MyGUI.h"
#include "StretchControl.h"

namespace SkinEditor
{
	//=========================================================================================
	// ����� ������ ��� �������������� ������
	class SkinEditor : public MyGUI::EventCallback, public StretchControlEvent
	{
		public:

			enum {
				NONE=0,
				EDIT_IS_USE,
			};
			enum {
				EDIT_LEFT,
				EDIT_TOP,
				EDIT_RIGHT,
				EDIT_BOTTOM,
				__EDIT_COUNT
			};

			enum {
				LEVEL_MAIN,
				LEVEL_SECOND,
				LEVEL_THIRD,
				LEVEL_STRETCH_ELEMENT,
				__LEVEL_COUNT
			};

			typedef struct _tag_STATE_DATA { // ���������� �� ����� ������
				MyGUI::uint16 uPosition[__EDIT_COUNT]; // ������� � ��������
			} * LP_STATE_DATA;

			typedef struct _tag_SUB_SKIN_DATA {
				MyGUI::String strName; // �������� ��� �����
				MyGUI::uint16 uOffset[__EDIT_COUNT]; // �������� � ��������
				MyGUI::uint16 event_info; // �������
				MyGUI::uint16 style; // �����
				MyGUI::uint16 align; // ������������
				_tag_STATE_DATA stateInfo[5]; // �� ������ ���������

				Ogre::PanelOverlayElement  * viewElement; // ������� ������� � ���������
				MyGUI::uint8 level; // ������� �� ������� ����� ������� ���������

				_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA() {assert(0);}; // ����
				_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const MyGUI::String & str) : strName(str),
					event_info(0),
					style(0),
					align(0),
					viewElement(0),
					level(__LEVEL_COUNT)
				{
					memset((void*)uOffset, 0, sizeof(MyGUI::uint16) * __EDIT_COUNT); // �������
					memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // �������
				};
				_tag_SUB_SKIN_DATA::~_tag_SUB_SKIN_DATA()
				{
					if (viewElement) {
						Ogre::OverlayManager::getSingleton().destroyOverlayElement(viewElement);
						viewElement = 0;
					}
				}

			} * LP_SUB_SKIN_DATA;

			typedef struct _tag_WINDOW_DATA {
				MyGUI::String strElementName; // ��� ��������
				MyGUI::String strAddedSkin1; // �������������� �����
				MyGUI::String strAddedSkin2; // �������������� �����
				MyGUI::uint8 uAddedData1; // �������������� ������
				MyGUI::uint8 uAddedData2; // �������������� ������
				MyGUI::String strFont; // �����
				MyGUI::String strColour; // �������� �����

				MyGUI::String strMaterialName; // ��� ���������
				MyGUI::uint16 sizeTextureX; // ������ ��������
				MyGUI::uint16 sizeTextureY; // ������ ��������

				std::vector <LP_SUB_SKIN_DATA> sabSkins; // ��������� �� ��������
				_tag_WINDOW_DATA::_tag_WINDOW_DATA() {assert(0);};
				_tag_WINDOW_DATA::_tag_WINDOW_DATA(const MyGUI::String & strName) :
					strElementName(strName),
					uAddedData1(0),
					uAddedData2(0),
					sizeTextureX(0),
					sizeTextureY(0) {};
			} * LP_WINDOW_DATA;


			virtual void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // �������������� �������
			virtual void onMouseClick(MyGUI::Window * pWindow); // ������ � �������� ����� ������ ���� �� ���� �� ��������

			SkinEditor(MyGUI::EventCallback * pParent = NULL);
			~SkinEditor();
			bool createEditor(MyGUI::EventCallback * pParent = NULL); // ������� ���� �������������� ������
			void destroyEditor(); // ������� ���� �������������� ������
			void loadSkin(const MyGUI::String & strFileName); // ��������� ����
			void saveSkin(const MyGUI::String & strFileName); // ��������� ����

			SkinEditor::LP_SUB_SKIN_DATA findSkinData(const MyGUI::String & strName, bool create = true); // ���� ������ ���� ��� �� �������
			void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // ������� ��� ����
			void destroySkins(); // ������� ��� ��� �����

			void updateWindowInfo(); // ��������� ��� ���� �� ����
			void enableWindowInfo(bool bEnable); // ��������� ��� ����
			void updateSkinInfo(); // ��������� ��� ���� �� ��� �����
			void updateStateInfo(); // ��������� ��� ���� �� ��� �����
			void updateSizeElementView(); // ��������� ������ ���� ���� �����
			void enableSkinInfo(bool bEnable); // ��������� ��� �����
			void createFlagWindow(); // ������� ���� � ��������������� �������
			void createMaterialWindow(); // ������� ���� ��� ���������
			void createElementWindow(); // ������� ���� ��� ��������� ��������
			void fillFlagWindow(); // ��������� ���� �������� ��������
			bool fillMaterialWindow(); // ��������� ���� � ����������
			void fillElementWindow(); // ��������� ���� � ���������
			void fillViewElement(); // ��������� ������� �������������
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
			MyGUI::Button * m_buttonSabSkinStyle; // ����� �����

			MyGUI::ComboBox * m_comboSabSkinState; // ��������� �����

			MyGUI::Edit * m_editOffset[__EDIT_COUNT]; // �������� ����� ������ ��������
			MyGUI::Edit * m_editPosition[__EDIT_COUNT]; // ������� ���� � ����� �����

			MyGUI::Button * m_buttonSkinLoad; // �������� �����
			MyGUI::Button * m_buttonSkinSave; // ���������� �����

			std::vector <_tag_WINDOW_DATA> mWindowInfo; // ���������� � ���� �����
			std::vector <MyGUI::String> m_strMaterialName; // ��� ��������� ���������

			LP_WINDOW_DATA m_pCurrentDataWindow; // ������� ����
			LP_SUB_SKIN_DATA m_pCurrentDataSkin; // ������� ��� ����
			LP_STATE_DATA m_pCurrentDataState; // ������� ����� �����


			MyGUI::WindowFrame * m_windowStateFlags; // ���� � �������
			MyGUI::Button * m_buttonsFlagsEvent[16];
			MyGUI::Button * m_buttonsFlagsAlign[16];
			MyGUI::Button * m_buttonsFlagsStyle[16];

			MyGUI::WindowFrame * m_windowMaterial; // ���� � ����������
			bool m_bIsWindowMaterialMostShow; // ������ �� ���� ���� �������
			MyGUI::uint16 m_uTextureSizeX;
			MyGUI::uint16 m_uTextureSizeY;

			MyGUI::WindowFrame * m_windowElement; // ���� � ���������

			// ����� � ���� ���������
			StretchControl * m_textureOffsetPointer;
			// ����� � ���� �������
			StretchControl * m_elementOffsetPointer;
			// ���� ��� �������������� �������
			MyGUI::Window * m_windowElementView[__LEVEL_COUNT];

			// ����������� �� ��������� ��������� ��������
			void OnChangeLocation(StretchControl * pControl, uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY);

	};

} // namespace SkinEditor