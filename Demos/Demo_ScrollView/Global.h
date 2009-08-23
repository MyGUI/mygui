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
#include <winsock2.h>  // Wincosk2.h должен быть подключен раньше windows.h
#include "windows.h"
#include <mmsystem.h> // для звука
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
SceneNode *g_nodeCamera1; // нод1 для камеры
SceneNode *g_nodeCamera2; // нод2 для камеры
SceneNode *g_nodeCamera3; // нод для дрожания камеры
float g_fTimeNodeCamera3 = -1.0; // время задержки дрожания камеры
#define __PSIH_CAMERA g_fTimeNodeCamera3 = 0.2
Overlay* g_pFPSOverlay = NULL; // оверлей для ФПС
OverlayContainer * g_overlayFPSInfo = NULL; // строка с ФПС
BYTE g_bIsShowFPSOverlay = TRUE; // показывать ли оверлей с инфо об ФПС
//========================================================================================
BYTE m_bIsCEGUIActive = FALSE; // активны ли гуи
Real g_axisMouseX = 0.5; // положение курсора
Real g_axisMouseY = 0.5; // положение курсора
Real g_axisMouseRotate = 0.0; // вращение камеры
BYTE g_bIsRightPressed = FALSE; // состояние кнопок мыши
BYTE g_bIsLeftPressed = FALSE; // состояние кнопок мыши
WORD g_wOldMousePosX = (WORD)-1; // поледнее значение положения курсора
WORD g_wOldMousePosY = (WORD)-1; // поледнее значение положения курсора
WORD g_wMousePressedX = (WORD)-1; // последнее нажатие мыши
WORD g_wMousePressedY = (WORD)-1; // последнее нажатие мыши
BYTE g_bIsQuadWhite = TRUE; // какого цвета квадрат
enum __INSTRUMEN_INFO {Instrument_Non=0, Instrument_UpDown=1, Instrument_Quad=2, Instrument_Sector=3};
BYTE g_bIsInstrument = Instrument_Non; // тип инструмента для земли
BYTE g_bSizeSectorX = 0; // размер сектора выделения
BYTE g_bSizeSectorY = 3; // размер сектора выделения
//========================================================================================
CTerrain terrain; // единственный объект класса земли
CGameManager game; // единственный объект класса игры
CNetwork net; // единственный экземпляр класса сети
ExampleFrameListener * g_pFrameListener = NULL; // единственный объект класса ввода
//========================================================================================
BYTE g_bUnitID = CGameManager::__UNIT_TYPE::UNIT_NON; // текущий номер юнита для выставления
BYTE g_bUnitType = CGameManager::__UNIT_TYPE::UNIT_NON; // текущий тип юнита для выставления
WORD g_wTmpSectorStartX = 0; // временное значения размера сектора для ускорения просчета
WORD g_wTmpSectorStartY = 0; // временное значения размера сектора для ускорения просчета
BYTE g_bIsMainScene = FALSE; // заставка главной сцены
BYTE g_bSizeTerrainX = 16; // размер карты
BYTE g_bSizeTerrainY = 16; // размер карты
BYTE g_bCountShop4 = 2; // колличество 4х секционных юнитов
BYTE g_bCountShop3 = 3; // колличество 3х секционных юнитов
BYTE g_bCountShop2 = 4; // колличество 2х секционных юнитов
BYTE g_bCountShop1 = 5; // колличество 1х секционных юнитов
#define __CURRENT_MAP_NAME "current.map" // имя текущей карты
//========================================================================================
// сетевые приблуды
BYTE g_bIsNetworkServer; // сервер мы или клиент
float g_fTimeLoopToNetwork = -1.0; // таймер для обновления омена сервера (-1.0 - круг стоит)
#define __TIME_LOOP_NETWORK 1.0
enum __MODE_STEP_CONNECT { // текущее состояние отношений
	MSC_NON,
	MSC_CONNECT, 
	MSC_SERVER_WAIT_INFO,
	MSC_CLIENT_WAIT_UNIT_INFO,
	MSC_SERVER_WAIT_READY,
	MSC_CLIENT_OK,
	MSC_STRIKE,
};
BYTE g_bModeStepConnect = MSC_NON; // текущий режим установки соединения
#define __MAX_NETWORK_TIMEOUT 10.0
#define __MAX_NETWORK_TIMEOUT_NON -1.0
float g_fModeStepTimeOut;  // переменная для тайм аута
enum __COMMAND_NETWORK { // комманды для сети
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
// всплывающее окно сверху
CEGUI::StaticText* g_wndInfo = NULL; // окно с информацией вверху
BYTE g_bIsShowWindowInfo = WS_NON; // показывать или скрывать
float g_fTimeOutWindowInfo = -1.0; // время задержки окна для закрытия
float g_fCurrentAlfaWindowInfo = 0.0; // текущая альфа
#define __WINDOW_INFO_HIDE -0.06
#define __WINDOW_INFO_SHOW 0.01
#define __WINDOW_INFO_POSX 0.2
#define __WINDOW_INFO_SIZEX 0.5
#define __WINDOW_INFO_SIZEY 0.05
#define __WINDOW_INFO_TIMEOUT 5
void g_setWindowInfo(CEGUI::String strWindowText) // показать текст в окне сверху
{
	if (!g_wndInfo) return;
	g_wndInfo->setText(strWindowText);
	g_bIsShowWindowInfo = WS_SHOW;
	g_fTimeOutWindowInfo = __WINDOW_INFO_TIMEOUT;
};
//========================================================================================
// всплывающее окно статуса снизу
CEGUI::StaticText* g_wndStatusInfo = NULL; // окно с информацией снизу
BYTE g_bIsShowWindowStatusInfo = WS_NON; // показывать или скрывать
float g_fTimeOutWindowStatusInfo = -1.0; // время задержки окна для закрытия
float g_fCurrentAlfaWindowStatusInfo = 0.0; // текущая альфа
#define __WINDOW_STATUS_INFO_POSX 0.2
#define __WINDOW_STATUS_INFO_POSY 0.945
#define __WINDOW_STATUS_INFO_SIZEX 0.5
#define __WINDOW_STATUS_INFO_SIZEY 0.05
void g_setWindowStatusInfo(CEGUI::String strWindowStatusText) // показать текст в окне снизу
{
	if (!g_wndStatusInfo) return;
	g_wndStatusInfo->setText(strWindowStatusText);
	g_bIsShowWindowStatusInfo = WS_SHOW;
	g_fTimeOutWindowStatusInfo = -1.0;
};
//========================================================================================
// message box
CEGUI::StaticText* g_wndMessageBox = NULL; // окно для вывода сообщения
BYTE g_bIsShowMessageBox = WS_NON; // текущее состояние окна
#define __WINDOW_MESSAGE_BOX_POSX 0.3
#define __WINDOW_MESSAGE_BOX_POSY 0.4
#define __WINDOW_MESSAGE_BOX_SIZEX 0.4
#define __WINDOW_MESSAGE_BOX_SIZEY 0.3
float g_fCurrentAlfaMessageBox = 0.0; // текущая альфа
void g_showMessageBox(CEGUI::String strWindowStatusText)  // показать окно сообщения
{
	if (!g_wndMessageBox) return;
	g_wndMessageBox->setText(strWindowStatusText);
	g_wndMessageBox->setModalState(false); // без этой строчки чередуется модальность
	g_wndMessageBox->setModalState(true);
	g_fCurrentAlfaMessageBox = 0.0;
	g_wndMessageBox->setAlpha(g_fCurrentAlfaMessageBox);
	g_wndMessageBox->show();
	g_bIsShowMessageBox = WS_SHOW;
};
//========================================================================================
// окно настроек карты
CEGUI::StaticText* g_wndOptionsMap = NULL; // окно с настройками карты
CEGUI::StaticText* g_wndOptionsMapText4xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::StaticText* g_wndOptionsMapText3xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::StaticText* g_wndOptionsMapText2xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::StaticText* g_wndOptionsMapText1xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::StaticText* g_wndOptionsMapTextSizeX = NULL; // окно с информацией о размере карты
CEGUI::StaticText* g_wndOptionsMapTextSizeY = NULL; // окно с информацией о размере карты
CEGUI::Scrollbar* g_wndOptionsMapScroll4xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::Scrollbar* g_wndOptionsMapScroll3xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::Scrollbar* g_wndOptionsMapScroll2xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::Scrollbar* g_wndOptionsMapScroll1xX = NULL; // окно с информацией о колличестве юнитов
CEGUI::Scrollbar* g_wndOptionsMapScrollSizeX = NULL; // окно с информацией о размере карты
CEGUI::Scrollbar* g_wndOptionsMapScrollSizeY = NULL; // окно с информацией о размере карты
#define __WINDOW_OPTIONS_MAP_POSX 0.75
#define __WINDOW_OPTIONS_MAP_POSY 0.36
#define __WINDOW_OPTIONS_MAP_SIZEX 0.22
#define __WINDOW_OPTIONS_MAP_SIZEY 0.35
float g_fCurrentAlfaOptionsMap = 0.0; // текущая альфа
BYTE g_bIsShowOptionsMap = WS_NON;
BYTE g_bIsOptionsMapShowUnits = FALSE; // видны ли юниты в окне настроек
void g_showOptionsMap() // показать окно настроек
{
	if (!g_wndOptionsMap) return;
	g_fCurrentAlfaOptionsMap = 0.0;
	g_wndOptionsMap->setAlpha(g_fCurrentAlfaOptionsMap);
	g_wndOptionsMap->show();
	g_bIsShowOptionsMap = WS_SHOW;
};
//========================================================================================
// окно помощи
CEGUI::StaticText* g_wndHelpWindow = NULL; // окно с помощью
#define __WINDOW_HELP_WINDOW_POSX 0.06
#define __WINDOW_HELP_WINDOW_POSY 0.1
#define __WINDOW_HELP_WINDOW_SIZEX 0.64
#define __WINDOW_HELP_WINDOW_SIZEY 0.8
#define __WINDOW_HELP_WINDOW_ALPHA 0.5
BYTE g_bIsShowHelpWindow = FALSE; // текущее состояние окна
// изменить текст окна
#define __SET_HELP_TEXT(num) g_wndHelpWindow->setText(g_strHelpStrings[num])
enum __HELP_TEXT { // индексы для текстовых строк
	HELP_MAIN,
	HELP_GAME,
	HELP_OPTIONS,
	HELP_MAX,
};
LPCSTR g_strHelpStrings[HELP_MAX] = {  // текстовые строки для помощи
	"Приветствую тебя, мой юный генерал, в этом окне я буду давать тебе информацию об управлении и режимах...\n\n    F1 - окно помощи (скрытие / открытие).\n    F2 - окно иформации об кадрах.\n    Esc - скрытие окна помощи.\n    F12 - экстренный выход из игры.\n\nПункты меню:\n\n    'Старт (классический)' - Быстрый старт.\n        размер карты: 10x10\n        юниты: 4x = 1, 3x = 2, 2x = 3, 1x = 4\n        выборочная дислокация юнитов: НЕТ\n\n    'Старт (расширенный)' - Быстрый старт.\n        размер карты: 14x14   -  загрузка случайного ландшафта\n        юниты: 4x = 2, 3x = 3, 2x = 4, 1x = 5\n        выборочная дислокация юнитов: НЕТ\n\n    'Старт' - игра с параметрами выбранными в пункте 'Настройка игры'.\n        выборочная дислокация юнитов: ДА\n\n    'Настройка игры' - Настройка карты для пункта меню 'Старт'.\n        изменяются размеры и высота карты, колличество юнитов\n        загрузка предустановленных карт\n\n    'Выход' - Бегство с поля боя.\n        меры: расстрел на месте\n\n    Всплывающие окна:\n        сверху - дополнительная информация об игре\n        снизу - описание текущего пункта меню",
	"Управление:\n\n    Курсор мыши к краям экрана сдвигает карту.\n    Удержание правой клавиши мыши и движение курсора - вращение карты.\n    Левая клавиша мыши на карте противника - выстрел.\n    Колесо мыши или табуляция - вращение юнита (при расстановке юнитов).\n\nУсловные значки:\n\n    Зеленая галочка - место уже прострелянно, промах.\n    Больничка с красным крестом - ранение вражеского юнита.\n    Круглая бомбочка - уничтожение вражеского юнита.\n\nПункты меню:\n\n    'Ваш ход' - вы первые наносите удар\n    'Ход противника' - противник первый наносит удар\n    'Создать сервер' - создает сервер и ждет подключения клиента\n    'Подключиться' - пытается подключится к серверу по IP\n    'Ответный удар' - вы наносите ответный удар\n    'Главное меню' - возврат в главное меню\n\nПримечания:\n\n    Адрес сервера пишется в строке редактирования снизу.\n    Сервер первый наносит удар.\n    В процессе игры можно посылать друг другу сообщения.",
	"Управление:\n\n    Курсор мыши к краям экрана сдвигает карту.\n    Удержание правой клавиши мыши и движение курсора - вращение карты.\n    Левая клавиша мыши - поднятие или опускание участка карты.\n    Нажатие левой клавиши мыши и движение - выравнивание участка.\n\nОкно настроек:\n\n    изменение размера карты (10 - 20)\n    изменение колличества юнитов (0 - 10)\n\nПункты меню:\n\n    'Поднять участок' или 'Опустить участок' - текущий режим для карты\n    'Следующая карта' - загрузка следующей карты из предустановленных\n    'Показать юниты' или 'Скрыть юниты' - оценка плотности расположения\n    'Главное меню' - возврат в главное меню\n\nПримечание:\n\n    Если колличество всех юнитов равно нулю, то будет создан один 4х секционный юнит...",
};
//========================================================================================
// все кнопки меню
#define __MENU_ITEM_SIZEX 0.22
#define __MENU_ITEM_SIZEY 0.045
#define __MENU_ITEM_HIDE 1.0
#define __MENU_ITEM_SHOW 0.75
#define __MENU_ITEM_COUNT_MAX 20
// наборы элементов меню для отображения и скрытия
enum __MENU_ITEM_CONTAINER { // индексы
	MIC_MAIN_MENU,
	MIC_QUAKE_START,
	MIC_SELF_STRIKE,
	MIC_NEXT_SHOT,
	MIC_OPTIONS_MENU,
	MIC_MAX,
};
unsigned __int64  g_int64MenuContainer[MIC_MAX] = {  // контейнеры с индексами элементов меню
	0x0000000504030201,
	0x000000080F0E0706,
	0x0000000000000008,
	0x0000000000000809,
	0x000000000D0C0B0A,
};
#define __BUTTONS_MENU_ANIM_COUNT 8
CEGUI::PushButton* g_abuttonsMenu[__MENU_ITEM_COUNT_MAX]; // все кнопки меню
BYTE g_baAnimButtonsMenu[__BUTTONS_MENU_ANIM_COUNT]; // текущие кнопки анимации
unsigned __int64 g_int64oldAnimButtonMenu = 0; // текущее состояние видимых кнопок для процедуры скрытия
BYTE g_bIsButtonsMenuShow = FALSE; // показывать / скрывать
void g_showMenu(BYTE bItemContainer) // показать контейнер с кнопками по индексу
{
	if (g_baAnimButtonsMenu[0] != 0) return; // анимация еще не закончилась
	if (!g_bIsButtonsMenuShow) {
		g_bIsButtonsMenuShow = TRUE;
		g_int64oldAnimButtonMenu = g_int64MenuContainer[bItemContainer];
		*(unsigned __int64*)&g_baAnimButtonsMenu[0] = g_int64oldAnimButtonMenu;
	}
}
// переменные строки и индексы для элементов меню
#define __BUTTON_TEXT_TERRAIN_UP "Поднять участок"
#define __BUTTON_TEXT_TERRAIN_DOWN "Опустить участок"
#define __BUTTON_TERRAIN_UP_DOWN 9
#define __BUTTON_TEXT_SHOW_UNITS "Показать юниты"
#define __BUTTON_TEXT_HIDE_UNITS "Скрыть юниты"
#define __BUTTON_SHOW_HIDE_UNITS 11
//========================================================================================
// Окно редактирования сообщений
CEGUI::Editbox* g_wndEditBox = NULL; // окно редактирования
BYTE g_bIsConnectToServer = FALSE; // не адрес ли мы вводим
#define __WINDOW_EDIT_BOX_POSX 0.2
#define __WINDOW_EDIT_BOX_POSY 0.89
#define __WINDOW_EDIT_BOX_SIZEX 0.5
#define __WINDOW_EDIT_BOX_SIZEY 0.05
#define __WINDOW_EDIT_BOX_ALPHA 0.5
//========================================================================================
//========================================================================================
#endif
//========================================================================================
