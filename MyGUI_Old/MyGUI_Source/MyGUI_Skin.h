#pragma once

#include "MyGUI_Defines.h"
#include <vector>
#include <OgreColourValue.h>

namespace MyGUI {


	// �������� ���������� �������� � �������
	enum COORD {
		_X,
		_Y,
		_CX,
		_CY,
		__COORD_COUNT
	};

	enum STATE_SKIN_OFFSETS {

		OFFSET_DEACTIVATED_X,
		OFFSET_DEACTIVATED_Y,
		OFFSET_DEACTIVATED_CX,
		OFFSET_DEACTIVATED_CY,

		OFFSET_NORMAL_X,
		OFFSET_NORMAL_Y,
		OFFSET_NORMAL_CX,
		OFFSET_NORMAL_CY,

		OFFSET_ACTIVATED_X,
		OFFSET_ACTIVATED_Y,
		OFFSET_ACTIVATED_CX,
		OFFSET_ACTIVATED_CY,

		OFFSET_PRESSED_X,
		OFFSET_PRESSED_Y,
		OFFSET_PRESSED_CX,
		OFFSET_PRESSED_CY,

		OFFSET_SELECTED_X,
		OFFSET_SELECTED_Y,
		OFFSET_SELECTED_CX,
		OFFSET_SELECTED_CY,

		__OFFSET_STATE_COUNT
	};

    enum FADE_STATES {
		NONE,
		FADE_CHANGE_STATE,
	};

	enum __SKIN_STATES {
		SKIN_STATE_DEACTIVATED, // ����������
		SKIN_STATE_NORMAL, // ������� ���������
		SKIN_STATE_ACTIVED, // ������� ������, �������
		SKIN_STATE_PRESSED, // �����
		SKIN_STATE_SELECTED, // ������� (����� � �������)
		__SKIN_STATE_COUNT // !!! �� ������������
	};

    typedef struct __tag_MYGUI_FONT_INFO { // ���������� � ������
		Ogre::Font * font; // �����
		String name; // ��� ������
		uint8 height; // ������ ������
		Real spaceWidth; // ������ �������
		Real sizeEllipses; // ������ ���� �����
		__tag_MYGUI_FONT_INFO::__tag_MYGUI_FONT_INFO() : font(0), height(0), spaceWidth(0.0), sizeEllipses(0.0) {}
	} __MYGUI_FONT_INFO, * __LP_MYGUI_FONT_INFO;

	typedef struct __tag_MYGUI_POINTER_INFO { // ���������� �� ����������
		String strMaterialName; // �������� �������
		int8 iOffsetX; // �������� ����� ���������
		int8 iOffsetY; // �������� ����� ���������
		uint8 uSizeX; // ������ ���������
		uint8 uSizeY; // ������ ���������
		__tag_MYGUI_POINTER_INFO::__tag_MYGUI_POINTER_INFO() : uSizeX(0), uSizeY(0), iOffsetX(0), iOffsetY(0) {}
	} __MYGUI_POINTER_INFO, * __LP_MYGUI_POINTER_INFO;

	typedef struct __MYGUI_SUBSKIN_INFO { // ���������� � ����� ����� ����
		// � �������� ���� �������� ���� ������, ��������� �����, � ����� �������� ������ ��������� �������������
		int16 posX; // ������� X
		int16 posY; // ������� Y
		int16 sizeX; // ������ X
		int16 sizeY; // ������ Y
		// ����� ��������� ����
		String SkinState[__SKIN_STATE_COUNT]; // ??? ������ ����� ��������
		// � �������� ���� ������������
		uint16 align; // ������������
		uint16 event_info; // ��������� ���������� ����
		uint16 exdata; // �������������� ���������� �� ��������
		// �������� ��������� � ��������� ����� ��������
		float fOffsetStateSkin[__OFFSET_STATE_COUNT/__COORD_COUNT][__COORD_COUNT];

		__MYGUI_SUBSKIN_INFO::__MYGUI_SUBSKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0)
		{
			memset((void*)fOffsetStateSkin, 0, sizeof(float) * __OFFSET_STATE_COUNT);
		}
	}__MYGUI_SUBSKIN_INFO,  *__LP_MYGUI_SUBSKIN_INFO;

    typedef struct __tag_MYGUI_SKIN_INFO { // ���������� �� ����
		std::vector <const __MYGUI_SUBSKIN_INFO *> subSkins; // �������������� ����� ����, �����
		//uint32 dataWindow; // �������������� ���������� ����� ����
		String data1, data2, data3, data4;
		String fontWindow; // ����� ����� ��������
		Ogre::ColourValue colour; // ���� ������ ����� ����
		String SkinElement; // �������� ����� ��������
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : colour(0) {}
		__tag_MYGUI_SKIN_INFO::~__tag_MYGUI_SKIN_INFO() { }
	} __MYGUI_SKIN_INFO, * __LP_MYGUI_SKIN_INFO;


	const String SKIN_DEFAULT = "SKIN_DEFAULT"; // ������ ���� ��� ��������� �����
	const String SKIN_FADE = "SKIN_FADE"; // ���� ����������
    const String SKIN_WINDOWFRAME = "SKIN_WINDOWFRAME"; // ������� ����
	const String SKIN_WINDOWFRAME_C = "SKIN_WINDOWFRAME_C"; // ���� � ����������
	const String SKIN_WINDOWFRAME_S = "SKIN_WINDOWFRAME_S"; // ���� � ����������
	const String SKIN_WINDOWFRAME_X = "SKIN_WINDOWFRAME_X"; // ���� � ������� ��������
	const String SKIN_WINDOWFRAME_CS = "SKIN_WINDOWFRAME_CS"; // ���� � ���������� � ����������
	const String SKIN_WINDOWFRAME_CX = "SKIN_WINDOWFRAME_CX"; // ���� � ���������� � ���������
	const String SKIN_WINDOWFRAME_SX = "SKIN_WINDOWFRAME_SX"; // ���� � ���������� � ���������
	const String SKIN_WINDOWFRAME_CSX = "SKIN_WINDOWFRAME_CSX"; // ���� � ����������, ���������� � ���������
	const String SKIN_BUTTON = "SKIN_BUTTON"; // ������� ������
	const String SKIN_CHECK_BOX = "SKIN_CHECK_BOX"; // ���������� ����
	const String SKIN_EDIT = "SKIN_EDIT"; // ���� ��������������
	const String SKIN_VSCROLL = "SKIN_VSCROLL"; // ������������ ������ ���������
	const String SKIN_TAB = "SKIN_TAB"; // ������ �������
	const String SKIN_TAB_BUTTON = "SKIN_TAB_BUTTON"; // ������ ��� ������ �������
	const String SKIN_STATIC_TEXT = "SKIN_STATIC_TEXT"; // ����� � ���������������� � ��������
	const String SKIN_LIST_S = "SKIN_LIST_S"; // ������ � ����������
	const String SKIN_LIST_BUTTON = "SKIN_LIST_BUTTON"; // ������ ��� ������
	const String SKIN_COMBO_BOX_EDIT = "SKIN_COMBO_BOX_EDIT"; // ���� ��� ����������
	const String SKIN_COMBO_BOX = "SKIN_COMBO_BOX"; // ���������� ������ � ����� ��������������
	const String SKIN_DROP_LIST = "SKIN_DROP_LIST"; // ���������� ������
    const String __SKIN_WIDGET_DEFAULT = "SKIN_WIDGET_DEFAULT";
	
	const String FONT_DEFAULT = "FONT_DEFAULT"; // ����� �� ���������
	const String FONT_LARGE =   "FONT_LARGE"; //???


	const String WIDGET_WINDOW = "Window";
    const String WIDGET_BUTTON = "Button";
    const String WIDGET_EDIT = "Edit";
    const String WIDGET_COMBO_BOX = "ComboBox";
    const String WIDGET_LIST_BOX = "ListBox";
    const String WIDGET_STATIC_TEXT = "StaticText";
    const String WIDGET_STATIC_IMAGE = "StaticImage";
    const String WIDGET_TAB = "Tab";
    const String WIDGET_VSCROLL = "VScroll";
    const String WIDGET_WINDOW_FRAME = "WindowFrame";


}