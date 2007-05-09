/**************************************************
This file defines the prototypes and typedefs
that most of the files need, but we do it in a way
that removes issues of too many dependencies in 
include files
***************************************************/
#pragma once

#include "MyGUI_Logging.h"
#include <OgreString.h>
#include <OgreUTFString.h>

namespace MyGUI
{
    const Ogre::uint16 ITEM_NON_SELECT = 0xFFFF; // ������� �� �������
	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))
	
    typedef int int32;
    typedef short int16;
    typedef char int8;
    using Ogre::uint8;
    using Ogre::uint16;
    using Ogre::uint32;
    using Ogre::Real;
    using Ogre::String;
    
    //=========================================================================================
    //Constants used for placing text
    const int __GUI_FONT_HOFFSET = 2;
    const int __GUI_FONT_SIZE_HOFFSET = __GUI_FONT_HOFFSET / 2;

    const int __GUI_BUTTON_SHIFT_TEXT_PRESSED = 1;
    const int __GUI_TIME_DOUBLE_CLICK = 250;

    const float __GUI_POPUP_FADE_ALPHA = 0.5f;
    //==================================================================================================
    // ������ ������� ��� ���� ���������� ����� ������
    #define __GUI_ZORDER_FADE 649
    // ������ ������� ��� ����
    #define __GUI_ZORDER_MOUSE 648
    // ������ ������� ��� ����������� ����
    #define __GUI_ZORDER_POPUP 640
    // ������ ������� ��� ���� ���������� ���������
    #define __GUI_ZORDER_POPUP_FADE 639
    // ������ �������� �������
    #define __GUI_ZORDER_MAIN 630
    //=========================================================================================
    // ������ ������ ������� ��� ��������������� ����
    #define __GUI_ZORDER_OVERLAPPED 7
    // ��� ��� ���������� ������ ������� ��������������� ����
    #define __GUI_ZORDER_OVERLAPPED_STEP 10
    // ������ ��� ������� ���������� �������������� �� ������������ �����
    #define __GUI_ZORDER_OVERLAPPED_WARNING 600
    //=========================================================================================
    // ������ ������� �������
    #define __GUI_ZORDER_BACK 0
    
    //=========================================================================================
    //=========================================================================================
    class Button;
    class ComboBox;
    class Edit;
    class List;
    class Message;
    class StaticText;
    class Tab;
    class VScroll;
    class Window;
    class WindowFrame;
    class EventCallback;
    class GUI;
    
    enum __FADE_STATE { // ������� ����� ����������
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};
	enum __MESSAGE_BOX_BUTTON { // ������ ���� ���������
		MBB_NONE = FLAG_NONE,
		MBB_BUTTON1 = FLAG(0), // ������ ������ �������������
		MBB_BUTTON2 = FLAG(1), // ������ ������ ������
		MBB_MODAL = FLAG(2), // ���� ����������� � ��������� ������, !!! ����������� ������ ����
	};
	enum __WINDOW_EX_STYLE { // ������������� ����� ��������� ������
		// ����� �����
		WES_NONE = FLAG_NONE, // ������� ����
		WES_TEXT = FLAG(0), // ������� �������� ��������� ������ ��� ����� ��������
		WES_CLIENT = FLAG(1), // ������� �������� ��������� �����
		WES_ACTION1 = FLAG(2),
		WES_ACTION2 = FLAG(3),
		WES_ACTION3 = FLAG(4),
		WES_ACTION4 = FLAG(5),
		WES_ACTION5 = FLAG(6),
		WES_ACTION6 = FLAG(7),
		// ����� ��� WindowFrame
		WES_CAPTION = 0x0010, // ������� �������� ����������
		WES_RESIZE = 0x0020, // ������� �������� ����������
		WES_CLOSE = 0x0040, // ������� �������� ������� ��������
		// ����� ��� Button
		WES_BUTTON = 0x0010, // ������� �������� �������
		// ����� ��� Edit
		WES_EDIT = 0x0010, // ������� �������� ����� ��������������
		WES_EDIT_CURSOR = 0x0020, // ������� �������� �������� ��� ���� ��������������
		// ����� ��� VScroll
		WES_VSCROLL_UP = 0x0010, // ������� �������� ������� ������� �������
		WES_VSCROLL_DOWN = 0x0020, // ������� �������� ������ ������� �������
		WES_VSCROLL_TRACK = 0x0040, // ������� �������� ������ ������� �������
		// ����� ��� TabBar
		WES_TAB_TOP = 0x0010, // ������� �������� ������� ������ ��� ������������
		// ����� ��� ������
		WES_LIST_HIDE_SCROLL = 0x0010, // �������� ������ , ���� �� �� �����
		// ����� ��� ����������� ������
		WES_COMBO = 0x0010, // ���������� ������
		WES_COMBO_BUTTON = 0x0020, // ������ � ���������� ������
	};

	enum __WINDOW_EVENT { // ������� �� ������� ���� ����� ��������
		WE_NONE = FLAG_NONE, // ������ �� ����� ����������
		WE_MOUSE_BUTTON = FLAG(0), // ������ ����� ����� ����
		WE_MOUSE_FOCUS = FLAG(1), // ����� ���������� �� ������� ����
		WE_MOUSE_MOVE = FLAG(2), // �������� ���� ����� �������
		WE_KEY_FOCUS = FLAG(3), // ����� ������ ��� ����� ����������
		WE_KEY_BUTTON = FLAG(4), // ������ �������
		__WE_IS_ACTION = 0xFF, // !! ��� ����������� ������������� , �� �����  |   ���� �� ������� �� ������� ���� � ����������
		// ��������� �� ���������, ���������� ������
		WE_WARNING_ZORDER_MAX = 0x1000, // ���� ��������������� ���� ����������� � �������
		WE_WARNING_CHILD_MAX = 0x2000, // ����������� ����� ����� ���� ����� ������
		WE_WARNING_CHILD_DELETE = 0x4000, // ������� ���������, �������� ���������
	};
	enum __WINDOW_OTHER_EVENT { // ������� ���������� � OtherEvents
		WOE_NONE = 0x00000, // ��� ���������
		WOE_SCROLL_CHANGE_POS = 0x0001, // ���������� ������� �������
		WOE_FRAME_CLOSE = 0x0002, // ������ ������ ������� �����
		WOE_EDIT_KEY_ACCEPT = 0x0004, // ������ ������ �����
		WOE_EDIT_KEY_DELETE = 0x0008, // ������ ������ �����
		WOE_LIST_SELECT_CHANGE = 0x0010, // ��������� ���������� �������
		WOE_LIST_SELECT_ACCEPT = 0x0020, // ������� ������ ��� ����� �� ��������
		WOE_LIST_SELECT_NONE = 0x0040, // ������ �� ������ �����
		WOE_COMBO_SELECT_ACCEPT = 0x0080, // ����������� �������
		WOE_MESSAGE_PRESS_BUTTON = 0x0100, // ������ ������ � ���� ���������
	};
	enum __WINDOW_STATE { // ��������� ����
		WS_DEACTIVE, // ���� ��������� � ����������
		WS_NORMAL, // ������� ��������� ����
		WS_PRESSED, // ������ ������
		__WS_ACTIVED, // ������ ��� ��������, ��� ����������� �������������
		__WS_SELECTED // ������ ��� �������� � ������, ��� ����������� �������������
	};
	enum __WINDOW_ALIGN {
		// ������������ ����, ���� ����� ������� ����, �� ����������� (WA_LEFT|WA_TOP)
		WA_NONE=0x00, //                                 ���� ������������� �� ������
		WA_HCENTER=0x00, //                           ����, ��� ���� �� ����� ������������ ��� ����������
		WA_VCENTER=0x00, //                            �� ������������ �� ������������ ������ ����
		WA_CENTER=0x00, //                              � ������������ ������ ��������������� ���������, ���� ����� ������ ������ ����
		WA_CENTER_FOTHER=0x1000, //           ��� ������ ����, ������ ������������ �� ������ ����, �� ��������� ������������ �������� ���������
		WA_LEFT=0x01, WA_RIGHT=0x02, //     �� ������ ��� �� �������
		WA_HSTRETCH=0x03,  //                        ������������ �� �����������
		WA_TOP=0x04, WA_BOTTOM=0x08, //  �� ����� ��� �� ����
		WA_VSTRETCH=0x0C,  //                        ������������ �� ���������
		WA_STRETCH=0x0F, //                           ������������ �� ���� ������
		// ������������ ������, � ���� ��� ������������
		WAT_NONE=0x00, // �� ����������� �� ��, ��� WAT_LEFT|WAT_TOP
		WAT_LEFT=0x10, WAT_RIGHT=0x20, WAT_HCENTER=0x30, // �������������
		WAT_TOP=0x40, WAT_BOTTOM=0x80, WAT_VCENTER=0xC0, // �����������
		WAT_CENTER=0xF0, // �� ������ ����
		// ������� ������
		WAT_CUT_RIGHT=0x0100, WAT_CUT_LEFT=0x0200, // �� ����� ��� �� ������ �������
		WAT_ADD_DOT=0x0400, // ��������� ��� ����� ��� �������
		WAT_CUT_RIGHT_DOT=0x0500, WAT_CUT_LEFT_DOT=0x0600, // �� ����� ��� �� ������ ������� � ��� � �������
		__WAT_IS_CUT=0x0300, // ��� ���������� ����, �� �����!  ����� �� �������
		WAT_MULTI_LINE = 0x0800, // ������������� �����
		WAT_BREAK_WORD = 0x2000, // ���������� �� ������
		WAT_SHIFT_TEXT_PRESSED = 0x8000, // �������� �� ����� ��� �������
	};

	// ������� ����
	enum __MYGUI_POINTER {
		POINTER_DEFAULT, // ������� ���������
		POINTER_RESIZE, // ��� ��������� ��������
		POINTER_TEXT, // ��� ���������� �����
		__POINTER_COUNT
	};

	// ��� ������� ������, ��������� ����� OVERLAY_MOUSE � �� __OVERLAY_COUNT, ����� ���� ��������� (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE, // ������� ��� ����������
		OVERLAY_MOUSE, // ������� ��� ����, !!! �� ������������ ��� �������� ����
		OVERLAY_POPUP, // ������� ��� ������������ ���� , ����, ����������, ���� �� �������������
		OVERLAY_MAIN, // �������� ������� ���
		OVERLAY_BACK, // ����� ������ ������� ���
		__OVERLAY_COUNT, // ����������� ��������, !!! �� ������������ ��� �������� ����
		OVERLAY_OVERLAPPED, // ��� �������� ��������������� ����
		OVERLAY_CHILD, // ���� ����������� ����, ����������� ������� ����
		OVERLAY_DESTROY_ALL // �������� ���� ���� � ������� destroyWindow
	};
}