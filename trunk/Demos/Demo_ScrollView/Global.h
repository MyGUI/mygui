//=========================================================================================
#ifndef __Global_H__
#define __Global_H__
//=========================================================================================
#include <OgreNoMemoryMacros.h> //mem probs without this next one
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIFontManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIStaticText.h>
#include <CEGUI/elements/CEGUIRadioButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/elements/CEGUIEditbox.h>
#include <OgreMemoryMacros.h> //regular mem handler
//========================================================================================
#include <winsock2.h>  // Wincosk2.h ������ ���� ��������� ������ windows.h
#include "windows.h"
#include <mmsystem.h> // ��� �����
#include "Ogre.h"
#include "dubugOverlay.h"
#include "Network.h"
#include "ExampleApplication.h"
#include "Terrain.h"
#include "GameManager.h"
//========================================================================================
CEGUI::MouseButton convertOgreButtonToCegui(int buttonID)
{
	switch (buttonID) {
		case MouseEvent::BUTTON0_MASK:return CEGUI::LeftButton;
		case MouseEvent::BUTTON1_MASK:return CEGUI::RightButton;
		case MouseEvent::BUTTON2_MASK:return CEGUI::MiddleButton;
		case MouseEvent::BUTTON3_MASK:return CEGUI::X1Button;
		default:return CEGUI::LeftButton;
	}
}
//========================================================================================
#define __CAMERA_SPEED_Z 60000.0
#define __CAMERA_SPEED_XY 600.0
#define __CAMERA_SPEED_ROTATE 600.0
SceneNode *g_nodeCamera1; // ���1 ��� ������
SceneNode *g_nodeCamera2; // ���2 ��� ������
SceneNode *g_nodeCamera3; // ��� ��� �������� ������
float g_fTimeNodeCamera3 = -1.0; // ����� �������� �������� ������
#define __PSIH_CAMERA g_fTimeNodeCamera3 = 0.2
Overlay* g_pFPSOverlay = NULL; // ������� ��� ���
OverlayContainer * g_overlayFPSInfo = NULL; // ������ � ���
BYTE g_bIsShowFPSOverlay = TRUE; // ���������� �� ������� � ���� �� ���
//========================================================================================
BYTE m_bIsCEGUIActive = FALSE; // ������� �� ���
Real g_axisMouseX = 0.5; // ��������� �������
Real g_axisMouseY = 0.5; // ��������� �������
Real g_axisMouseRotate = 0.0; // �������� ������
BYTE g_bIsRightPressed = FALSE; // ��������� ������ ����
BYTE g_bIsLeftPressed = FALSE; // ��������� ������ ����
WORD g_wOldMousePosX = (WORD)-1; // �������� �������� ��������� �������
WORD g_wOldMousePosY = (WORD)-1; // �������� �������� ��������� �������
WORD g_wMousePressedX = (WORD)-1; // ��������� ������� ����
WORD g_wMousePressedY = (WORD)-1; // ��������� ������� ����
BYTE g_bIsQuadWhite = TRUE; // ������ ����� �������
enum __INSTRUMEN_INFO {Instrument_Non=0, Instrument_UpDown=1, Instrument_Quad=2, Instrument_Sector=3};
BYTE g_bIsInstrument = Instrument_Non; // ��� ����������� ��� �����
BYTE g_bSizeSectorX = 0; // ������ ������� ���������
BYTE g_bSizeSectorY = 3; // ������ ������� ���������
//========================================================================================
CTerrain terrain; // ������������ ������ ������ �����
CGameManager game; // ������������ ������ ������ ����
CNetwork net; // ������������ ��������� ������ ����
ExampleFrameListener * g_pFrameListener = NULL; // ������������ ������ ������ �����
//========================================================================================
BYTE g_bUnitID = CGameManager::__UNIT_TYPE::UNIT_NON; // ������� ����� ����� ��� �����������
BYTE g_bUnitType = CGameManager::__UNIT_TYPE::UNIT_NON; // ������� ��� ����� ��� �����������
WORD g_wTmpSectorStartX = 0; // ��������� �������� ������� ������� ��� ��������� ��������
WORD g_wTmpSectorStartY = 0; // ��������� �������� ������� ������� ��� ��������� ��������
BYTE g_bIsMainScene = FALSE; // �������� ������� �����
BYTE g_bSizeTerrainX = 16; // ������ �����
BYTE g_bSizeTerrainY = 16; // ������ �����
BYTE g_bCountShop4 = 2; // ����������� 4� ���������� ������
BYTE g_bCountShop3 = 3; // ����������� 3� ���������� ������
BYTE g_bCountShop2 = 4; // ����������� 2� ���������� ������
BYTE g_bCountShop1 = 5; // ����������� 1� ���������� ������
#define __CURRENT_MAP_NAME "current.map" // ��� ������� �����
//========================================================================================
// ������� ��������
BYTE g_bIsNetworkServer; // ������ �� ��� ������
float g_fTimeLoopToNetwork = -1.0; // ������ ��� ���������� ����� ������� (-1.0 - ���� �����)
#define __TIME_LOOP_NETWORK 1.0
enum __MODE_STEP_CONNECT { // ������� ��������� ���������
	MSC_NON,
	MSC_CONNECT, 
	MSC_SERVER_WAIT_INFO,
	MSC_CLIENT_WAIT_UNIT_INFO,
	MSC_SERVER_WAIT_READY,
	MSC_CLIENT_OK,
	MSC_STRIKE,
};
BYTE g_bModeStepConnect = MSC_NON; // ������� ����� ��������� ����������
#define __MAX_NETWORK_TIMEOUT 10.0
#define __MAX_NETWORK_TIMEOUT_NON -1.0
float g_fModeStepTimeOut;  // ���������� ��� ���� ����
enum __COMMAND_NETWORK { // �������� ��� ����
	CN_CONNECT_TO_SERVER,
	CN_POLE_INFO,
	CN_UNIT_INFO,
	CN_CLIENT_OK,
	CN_SERVER_OK,
	CN_STRIKE,
};
//========================================================================================
//========================================================================================
enum __WINDOW_STATUS {WS_NON, WS_SHOW, WS_HIDE};
//========================================================================================
// ����������� ���� ������
CEGUI::StaticText* g_wndInfo = NULL; // ���� � ����������� ������
BYTE g_bIsShowWindowInfo = WS_NON; // ���������� ��� ��������
float g_fTimeOutWindowInfo = -1.0; // ����� �������� ���� ��� ��������
float g_fCurrentAlfaWindowInfo = 0.0; // ������� �����
#define __WINDOW_INFO_HIDE -0.06
#define __WINDOW_INFO_SHOW 0.01
#define __WINDOW_INFO_POSX 0.2
#define __WINDOW_INFO_SIZEX 0.5
#define __WINDOW_INFO_SIZEY 0.05
#define __WINDOW_INFO_TIMEOUT 5
void g_setWindowInfo(CEGUI::String strWindowText) // �������� ����� � ���� ������
{
	if (!g_wndInfo) return;
	g_wndInfo->setText(strWindowText);
	g_bIsShowWindowInfo = WS_SHOW;
	g_fTimeOutWindowInfo = __WINDOW_INFO_TIMEOUT;
};
//========================================================================================
// ����������� ���� ������� �����
CEGUI::StaticText* g_wndStatusInfo = NULL; // ���� � ����������� �����
BYTE g_bIsShowWindowStatusInfo = WS_NON; // ���������� ��� ��������
float g_fTimeOutWindowStatusInfo = -1.0; // ����� �������� ���� ��� ��������
float g_fCurrentAlfaWindowStatusInfo = 0.0; // ������� �����
#define __WINDOW_STATUS_INFO_POSX 0.2
#define __WINDOW_STATUS_INFO_POSY 0.945
#define __WINDOW_STATUS_INFO_SIZEX 0.5
#define __WINDOW_STATUS_INFO_SIZEY 0.05
void g_setWindowStatusInfo(CEGUI::String strWindowStatusText) // �������� ����� � ���� �����
{
	if (!g_wndStatusInfo) return;
	g_wndStatusInfo->setText(strWindowStatusText);
	g_bIsShowWindowStatusInfo = WS_SHOW;
	g_fTimeOutWindowStatusInfo = -1.0;
};
//========================================================================================
// message box
CEGUI::StaticText* g_wndMessageBox = NULL; // ���� ��� ������ ���������
BYTE g_bIsShowMessageBox = WS_NON; // ������� ��������� ����
#define __WINDOW_MESSAGE_BOX_POSX 0.3
#define __WINDOW_MESSAGE_BOX_POSY 0.4
#define __WINDOW_MESSAGE_BOX_SIZEX 0.4
#define __WINDOW_MESSAGE_BOX_SIZEY 0.3
float g_fCurrentAlfaMessageBox = 0.0; // ������� �����
void g_showMessageBox(CEGUI::String strWindowStatusText)  // �������� ���� ���������
{
	if (!g_wndMessageBox) return;
	g_wndMessageBox->setText(strWindowStatusText);
	g_wndMessageBox->setModalState(false); // ��� ���� ������� ���������� �����������
	g_wndMessageBox->setModalState(true);
	g_fCurrentAlfaMessageBox = 0.0;
	g_wndMessageBox->setAlpha(g_fCurrentAlfaMessageBox);
	g_wndMessageBox->show();
	g_bIsShowMessageBox = WS_SHOW;
};
//========================================================================================
// ���� �������� �����
CEGUI::StaticText* g_wndOptionsMap = NULL; // ���� � ����������� �����
CEGUI::StaticText* g_wndOptionsMapText4xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::StaticText* g_wndOptionsMapText3xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::StaticText* g_wndOptionsMapText2xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::StaticText* g_wndOptionsMapText1xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::StaticText* g_wndOptionsMapTextSizeX = NULL; // ���� � ����������� � ������� �����
CEGUI::StaticText* g_wndOptionsMapTextSizeY = NULL; // ���� � ����������� � ������� �����
CEGUI::Scrollbar* g_wndOptionsMapScroll4xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::Scrollbar* g_wndOptionsMapScroll3xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::Scrollbar* g_wndOptionsMapScroll2xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::Scrollbar* g_wndOptionsMapScroll1xX = NULL; // ���� � ����������� � ����������� ������
CEGUI::Scrollbar* g_wndOptionsMapScrollSizeX = NULL; // ���� � ����������� � ������� �����
CEGUI::Scrollbar* g_wndOptionsMapScrollSizeY = NULL; // ���� � ����������� � ������� �����
#define __WINDOW_OPTIONS_MAP_POSX 0.75
#define __WINDOW_OPTIONS_MAP_POSY 0.36
#define __WINDOW_OPTIONS_MAP_SIZEX 0.22
#define __WINDOW_OPTIONS_MAP_SIZEY 0.35
float g_fCurrentAlfaOptionsMap = 0.0; // ������� �����
BYTE g_bIsShowOptionsMap = WS_NON;
BYTE g_bIsOptionsMapShowUnits = FALSE; // ����� �� ����� � ���� ��������
void g_showOptionsMap() // �������� ���� ��������
{
	if (!g_wndOptionsMap) return;
	g_fCurrentAlfaOptionsMap = 0.0;
	g_wndOptionsMap->setAlpha(g_fCurrentAlfaOptionsMap);
	g_wndOptionsMap->show();
	g_bIsShowOptionsMap = WS_SHOW;
};
//========================================================================================
// ���� ������
CEGUI::StaticText* g_wndHelpWindow = NULL; // ���� � �������
#define __WINDOW_HELP_WINDOW_POSX 0.06
#define __WINDOW_HELP_WINDOW_POSY 0.1
#define __WINDOW_HELP_WINDOW_SIZEX 0.64
#define __WINDOW_HELP_WINDOW_SIZEY 0.8
#define __WINDOW_HELP_WINDOW_ALPHA 0.5
BYTE g_bIsShowHelpWindow = FALSE; // ������� ��������� ����
// �������� ����� ����
#define __SET_HELP_TEXT(num) g_wndHelpWindow->setText(g_strHelpStrings[num])
enum __HELP_TEXT { // ������� ��� ��������� �����
	HELP_MAIN,
	HELP_GAME,
	HELP_OPTIONS,
	HELP_MAX,
};
LPCSTR g_strHelpStrings[HELP_MAX] = {  // ��������� ������ ��� ������
	"����������� ����, ��� ���� �������, � ���� ���� � ���� ������ ���� ���������� �� ���������� � �������...\n\n    F1 - ���� ������ (������� / ��������).\n    F2 - ���� ��������� �� ������.\n    Esc - ������� ���� ������.\n    F12 - ���������� ����� �� ����.\n\n������ ����:\n\n    '����� (������������)' - ������� �����.\n        ������ �����: 10x10\n        �����: 4x = 1, 3x = 2, 2x = 3, 1x = 4\n        ���������� ���������� ������: ���\n\n    '����� (�����������)' - ������� �����.\n        ������ �����: 14x14   -  �������� ���������� ���������\n        �����: 4x = 2, 3x = 3, 2x = 4, 1x = 5\n        ���������� ���������� ������: ���\n\n    '�����' - ���� � ����������� ���������� � ������ '��������� ����'.\n        ���������� ���������� ������: ��\n\n    '��������� ����' - ��������� ����� ��� ������ ���� '�����'.\n        ���������� ������� � ������ �����, ����������� ������\n        �������� ����������������� ����\n\n    '�����' - ������� � ���� ���.\n        ����: �������� �� �����\n\n    ����������� ����:\n        ������ - �������������� ���������� �� ����\n        ����� - �������� �������� ������ ����",
	"����������:\n\n    ������ ���� � ����� ������ �������� �����.\n    ��������� ������ ������� ���� � �������� ������� - �������� �����.\n    ����� ������� ���� �� ����� ���������� - �������.\n    ������ ���� ��� ��������� - �������� ����� (��� ����������� ������).\n\n�������� ������:\n\n    ������� ������� - ����� ��� ������������, ������.\n    ��������� � ������� ������� - ������� ���������� �����.\n    ������� �������� - ����������� ���������� �����.\n\n������ ����:\n\n    '��� ���' - �� ������ �������� ����\n    '��� ����������' - ��������� ������ ������� ����\n    '������� ������' - ������� ������ � ���� ����������� �������\n    '������������' - �������� ����������� � ������� �� IP\n    '�������� ����' - �� �������� �������� ����\n    '������� ����' - ������� � ������� ����\n\n����������:\n\n    ����� ������� ������� � ������ �������������� �����.\n    ������ ������ ������� ����.\n    � �������� ���� ����� �������� ���� ����� ���������.",
	"����������:\n\n    ������ ���� � ����� ������ �������� �����.\n    ��������� ������ ������� ���� � �������� ������� - �������� �����.\n    ����� ������� ���� - �������� ��� ��������� ������� �����.\n    ������� ����� ������� ���� � �������� - ������������ �������.\n\n���� ��������:\n\n    ��������� ������� ����� (10 - 20)\n    ��������� ����������� ������ (0 - 10)\n\n������ ����:\n\n    '������� �������' ��� '�������� �������' - ������� ����� ��� �����\n    '��������� �����' - �������� ��������� ����� �� �����������������\n    '�������� �����' ��� '������ �����' - ������ ��������� ������������\n    '������� ����' - ������� � ������� ����\n\n����������:\n\n    ���� ����������� ���� ������ ����� ����, �� ����� ������ ���� 4� ���������� ����...",
};
//========================================================================================
// ��� ������ ����
#define __MENU_ITEM_SIZEX 0.22
#define __MENU_ITEM_SIZEY 0.045
#define __MENU_ITEM_HIDE 1.0
#define __MENU_ITEM_SHOW 0.75
#define __MENU_ITEM_COUNT_MAX 20
// ������ ��������� ���� ��� ����������� � �������
enum __MENU_ITEM_CONTAINER { // �������
	MIC_MAIN_MENU,
	MIC_QUAKE_START,
	MIC_SELF_STRIKE,
	MIC_NEXT_SHOT,
	MIC_OPTIONS_MENU,
	MIC_MAX,
};
unsigned __int64  g_int64MenuContainer[MIC_MAX] = {  // ���������� � ��������� ��������� ����
	0x0000000504030201,
	0x000000080F0E0706,
	0x0000000000000008,
	0x0000000000000809,
	0x000000000D0C0B0A,
};
#define __BUTTONS_MENU_ANIM_COUNT 8
CEGUI::PushButton* g_abuttonsMenu[__MENU_ITEM_COUNT_MAX]; // ��� ������ ����
BYTE g_baAnimButtonsMenu[__BUTTONS_MENU_ANIM_COUNT]; // ������� ������ ��������
unsigned __int64 g_int64oldAnimButtonMenu = 0; // ������� ��������� ������� ������ ��� ��������� �������
BYTE g_bIsButtonsMenuShow = FALSE; // ���������� / ��������
void g_showMenu(BYTE bItemContainer) // �������� ��������� � �������� �� �������
{
	if (g_baAnimButtonsMenu[0] != 0) return; // �������� ��� �� �����������
	if (!g_bIsButtonsMenuShow) {
		g_bIsButtonsMenuShow = TRUE;
		g_int64oldAnimButtonMenu = g_int64MenuContainer[bItemContainer];
		*(unsigned __int64*)&g_baAnimButtonsMenu[0] = g_int64oldAnimButtonMenu;
	}
}
// ���������� ������ � ������� ��� ��������� ����
#define __BUTTON_TEXT_TERRAIN_UP "������� �������"
#define __BUTTON_TEXT_TERRAIN_DOWN "�������� �������"
#define __BUTTON_TERRAIN_UP_DOWN 9
#define __BUTTON_TEXT_SHOW_UNITS "�������� �����"
#define __BUTTON_TEXT_HIDE_UNITS "������ �����"
#define __BUTTON_SHOW_HIDE_UNITS 11
//========================================================================================
// ���� �������������� ���������
CEGUI::Editbox* g_wndEditBox = NULL; // ���� ��������������
BYTE g_bIsConnectToServer = FALSE; // �� ����� �� �� ������
#define __WINDOW_EDIT_BOX_POSX 0.2
#define __WINDOW_EDIT_BOX_POSY 0.89
#define __WINDOW_EDIT_BOX_SIZEX 0.5
#define __WINDOW_EDIT_BOX_SIZEY 0.05
#define __WINDOW_EDIT_BOX_ALPHA 0.5
//========================================================================================
//========================================================================================
#endif
//========================================================================================
