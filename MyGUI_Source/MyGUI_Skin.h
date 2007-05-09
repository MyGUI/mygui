#pragma once

#include "MyGUI_Defines.h"
#include <vector>

#define __WINDOW_DATA1(_data) ((_data->dataWindow & 0xFF000000) >> 24)
#define __WINDOW_DATA2(_data) ((_data->dataWindow & 0x00FF0000) >> 16)
#define __WINDOW_DATA3(_data) ((_data->dataWindow & 0x0000FF00) >> 8)
#define __WINDOW_DATA4(_data) (_data->dataWindow & 0x000000FF)

namespace MyGUI {

	enum __SKIN_STATES {
		SKIN_STATE_DEACTIVED, // ����������
		SKIN_STATE_NORMAL, // ������� ���������
		SKIN_STATE_ACTIVED, // ������� ������, �������
		SKIN_STATE_PRESSED, // �����
		SKIN_STATE_SELECTED, // ������� (����� � �������)
		__SKIN_STATE_COUNT // !!! �� ������������
	};

	enum __FONT_COLOUR {
		COLOUR_BLACK = 0x000000,
		COLOUR_WHITE = 0xFFFFFF,
		COLOUR_BLUE = 0x0000FF,
		COLOUR_GREEN = 0x00FF00,
		COLOUR_RED = 0xFF0000,
	};

	typedef struct __tag_MYGUI_FONT_INFO { // ���������� � ������
		Ogre::Font * font; // �����
		String name; // ��� ������
		uint8 height; // ������ ������
		Real spaceWidth; // ������ �������
		Real sizeTripleDot; // ������ ���� �����
		__tag_MYGUI_FONT_INFO::__tag_MYGUI_FONT_INFO() : font(0), height(0), spaceWidth(0.0), sizeTripleDot(0.0) {}
	} * __LP_MYGUI_FONT_INFO;

	typedef struct __tag_MYGUI_POINTER_INFO { // ���������� �� ����������
		String strMaterialName; // �������� �������
		int8 iOffsetX; // �������� ����� ���������
		int8 iOffsetY; // �������� ����� ���������
		uint8 uSizeX; // ������ ���������
		uint8 uSizeY; // ������ ���������
		__tag_MYGUI_POINTER_INFO::__tag_MYGUI_POINTER_INFO() : uSizeX(0), uSizeY(0), iOffsetX(0), iOffsetY(0) {}
	} * __LP_MYGUI_POINTER_INFO;

	typedef struct __tag_MYGUI_SKIN_INFO { // ���������� � ����� ����� ����
		// � �������� ���� �������� ���� ������, ��������� �����, � ����� �������� ������ ��������� �������������
		int16 posX; // ������� X
		int16 posY; // ������� Y
		int16 sizeX; // ������ X
		int16 sizeY; // ������ Y
		// ����� ��������� ����
		String pStrSkinNormal; // ������� ����
		String pStrSkinDeactived; // ������������
		String pStrSkinActived; // ������� ������
		String pStrSkinSelected; // ������� ������ � �����
		String pStrSkinPressed; // �����
		// � �������� ���� ������������
		uint16 align; // ������������
		uint16 event_info; // ��������� ���������� ����
		uint16 exdata; // �������������� ���������� �� ��������
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0) {}
	} *__LP_MYGUI_SKIN_INFO;

	typedef struct __tag_MYGUI_WINDOW_INFO { // ���������� �� ����
		std::vector <__LP_MYGUI_SKIN_INFO> subSkins; // �������������� ����� ����, �����
		uint32 dataWindow; // �������������� ���������� ����� ����
		__LP_MYGUI_FONT_INFO fontWindow; // ����� ����� ��������
		uint32 colour; // ���� ������ ����� ����
		__tag_MYGUI_WINDOW_INFO::__tag_MYGUI_WINDOW_INFO() : dataWindow(0), fontWindow(0), colour(0) {}
		__tag_MYGUI_WINDOW_INFO::~__tag_MYGUI_WINDOW_INFO()
		{
			while (subSkins.size()) {
				if (subSkins.back()) delete subSkins.back();
				subSkins.pop_back();
			};
		}
	} * __LP_MYGUI_WINDOW_INFO;


	enum __GUI_ENUM_SKIN_WINDOW { // ���������� ����� ������ ����
		SKIN_DEFAULT, // ������ ���� ��� ��������� �����
		SKIN_FADE, // ���� ����������
		SKIN_WINDOWFRAME, // ������� ����
		SKIN_WINDOWFRAME_C, // ���� � ����������
		SKIN_WINDOWFRAME_S, // ���� � ����������
		SKIN_WINDOWFRAME_X, // ���� � ������� ��������
		SKIN_WINDOWFRAME_CS, // ���� � ���������� � ����������
		SKIN_WINDOWFRAME_CX, // ���� � ���������� � ���������
		SKIN_WINDOWFRAME_SX, // ���� � ���������� � ���������
		SKIN_WINDOWFRAME_CSX, // ���� � ����������, ���������� � ���������
		SKIN_BUTTON, // ������� ������
		SKIN_CHECK_BOX, // ���������� ����
		SKIN_EDIT, // ���� ��������������
		SKIN_VSCROLL, // ������������ ������ ���������
		SKIN_TAB, // ������ �������
		SKIN_TAB_BUTTON, // ������ ��� ������ �������
		SKIN_STATIC_TEXT, // ����� � ���������������� � ��������
		SKIN_LIST_S, // ������ � ����������
		SKIN_LIST_BUTTON, // ������ ��� ������
		SKIN_COMBO_BOX_EDIT, // ���� ��� ����������
		SKIN_COMBO_BOX, // ���������� ������ � ����� ��������������
		SKIN_DROP_LIST, // ���������� ������
		__SKIN_COUNT, // ����������� ����, !!!�� ������������ (�������� ��������� �� �����)
		__SKIN_WIDGET_DEFAULT
	};

	enum __GUI_ENUM_FONT { // ���������� ����� �������
		FONT_DEFAULT, // ����� �� ���������
		FONT_LARGE,
		__FONT_COUNT // ����������� �������, !!!�� ������������ (�������� ��������� �� �����)
	};

}