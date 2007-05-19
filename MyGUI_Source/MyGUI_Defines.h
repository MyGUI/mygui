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
    
    enum __FADE_STATE {
		// ������� ����� ����������
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};

	enum __MESSAGE_BOX_BUTTON {
		// ������ ���� ���������
		MBB_NONE				= FLAG_NONE,
		MBB_BUTTON1		= FLAG(0),				// ������ ������ �������������
		MBB_BUTTON2		= FLAG(1),				// ������ ������ ������
		MBB_MODAL			= FLAG(2),				// ���� ����������� � ��������� ������, !!! ����������� ������ ����
	};

	enum __WINDOW_EX_STYLE { // ������������� ����� ��������� ������
		// ����� �����
		WES_NONE								= FLAG_NONE,				// ������� ����
		WES_TEXT								= FLAG(0),						// ������� �������� ��������� ������ ��� ����� ��������
		WES_CLIENT							= FLAG(1),						// ������� �������� ��������� �����
		WES_ACTION1							= FLAG(2),
		WES_ACTION2						= FLAG(3),
		WES_ACTION3						= FLAG(4),
		WES_ACTION4						= FLAG(5),
		WES_ACTION5						= FLAG(6),
		WES_ACTION6						= FLAG(7),
		// ����� ��� WindowFrame
		WES_CAPTION							= WES_ACTION1,			// ������� �������� ����������
		WES_RESIZE							= WES_ACTION2,			// ������� �������� ����������
		WES_CLOSE								= WES_ACTION3,			// ������� �������� ������� ��������
		// ����� ��� Button
		WES_BUTTON							= WES_ACTION1,			// ������� �������� �������
		// ����� ��� Edit
		WES_EDIT								= WES_ACTION1,			// ������� �������� ����� ��������������
		WES_EDIT_CURSOR				= WES_ACTION2,			// ������� �������� �������� ��� ���� ��������������
		// ����� ��� VScroll
		WES_VSCROLL_UP					= WES_ACTION1,			// ������� �������� ������� ������� �������
		WES_VSCROLL_DOWN				= WES_ACTION2,			// ������� �������� ������ ������� �������
		WES_VSCROLL_TRACK				= WES_ACTION3,			// ������� �������� ������ ������� �������
		// ����� ��� TabBar
		WES_TAB_TOP						= WES_ACTION1,			// ������� �������� ������� ������ ��� ������������
		// ����� ��� ������
		WES_LIST_HIDE_SCROLL		= WES_ACTION1,			// �������� ������ , ���� �� �� �����
		// ����� ��� ����������� ������
		WES_COMBO							= WES_ACTION1,			// ���������� ������
		WES_COMBO_BUTTON			= WES_ACTION2,			// ������ � ���������� ������
	};

	enum __WINDOW_EVENT { // ������� �� ������� ���� ����� ��������
		WE_NONE										= FLAG_NONE,						// ������ �� ����� ����������
		WE_MOUSE_BUTTON						= FLAG(0),								// ������ ����� ����� ����
		WE_MOUSE_FOCUS							= FLAG(1),								// ����� ���������� �� ������� ����
		WE_MOUSE_MOVE							= FLAG(2),								// �������� ���� ����� �������
		WE_KEY_FOCUS								= FLAG(3),								// ����� ������ ��� ����� ����������
		WE_KEY_BUTTON							= FLAG(4),								// ������ �������
		__WE_IS_ACTION							= WE_MOUSE_BUTTON
																  | WE_MOUSE_FOCUS
																  | WE_MOUSE_MOVE
																  | WE_KEY_FOCUS
																  | WE_KEY_BUTTON,			// !!! ��� ����������� ������������� , �� �����
		// ��������� �� ���������, ���������� ������
		WE_WARNING_ZORDER_MAX			= FLAG(5),								// ���� ��������������� ���� ����������� � �������
		WE_WARNING_CHILD_MAX			= FLAG(6),								// ����������� ����� ����� ���� ����� ������
		WE_WARNING_CHILD_DELETE		= FLAG(7),								// ������� ���������, �������� ���������
	};

	enum __WINDOW_OTHER_EVENT {
		// ������� ���������� � OtherEvents
		WOE_NONE										= FLAG_NONE,			// ��� ���������
		WOE_SCROLL_CHANGE_POS			= FLAG(0),					// ���������� ������� �������
		WOE_FRAME_CLOSE						= FLAG(1),					// ������ ������ ������� �����
		WOE_EDIT_KEY_ACCEPT					= FLAG(2),					// ������ ������ �����
		WOE_EDIT_KEY_DELETE				= FLAG(3),					// ������ ������ �����
		WOE_LIST_SELECT_CHANGE			= FLAG(4),					// ��������� ���������� �������
		WOE_LIST_SELECT_ACCEPT			= FLAG(5),					// ������� ������ ��� ����� �� ��������
		WOE_LIST_SELECT_NONE				= FLAG(6),					// ������ �� ������ �����
		WOE_COMBO_SELECT_ACCEPT		= FLAG(7),					// ����������� �������
		WOE_MESSAGE_PRESS_BUTTON	= FLAG(8),					// ������ ������ � ���� ���������
	};

	enum __WINDOW_STATE {
		// ��������� ����
		WS_DEACTIVATED,				// ���� ��������� � ����������
		WS_NORMAL,					// ������� ��������� ����
		WS_PRESSED,					// ������ ������
		__WS_ACTIVATED,				// ������ ��� ��������, ��� ����������� �������������
		__WS_SELECTED				// ������ ��� �������� � ������, ��� ����������� �������������
	};

	enum __WINDOW_ALIGN {
		// ������������ ����, ���� ����� ������� ����, �� ����������� (WA_LEFT|WA_TOP)
		WA_NONE						= FLAG_NONE,											// ���� ������������� �� ������
		WA_HCENTER					= FLAG_NONE,											// ����, ��� ���� �� ����� ������������ ��� ����������
		WA_VCENTER					= FLAG_NONE,											// �� ������������ �� ������������ ������ ����
		WA_CENTER						= WA_HCENTER | WA_VCENTER,				// � ������������ ������ ��������������� ���������, ���� ����� ������ ������ ����
		WA_CENTER_FOTHER		= FLAG(0),													// ��� ������ ����, ������ ������������ �� ������ ����, �� ��������� ������������ �������� ���������
		WA_LEFT							= FLAG(1),													// �� ������ 
		WA_RIGHT						= FLAG(2),													// �� �������
		WA_HSTRETCH				= WA_LEFT | WA_RIGHT,							// ������������ �� �����������
		WA_TOP							= FLAG(3),													// �� ����� 
		WA_BOTTOM					= FLAG(4),													// �� ����
		WA_VSTRETCH				= WA_TOP | WA_BOTTOM,						// ������������ �� ���������
		WA_STRETCH					= WA_HSTRETCH | WA_VSTRETCH,			// ������������ �� ���� ������
		// ������������ ������, � ���� ��� ������������
		WAT_NONE						= FLAG_NONE,											// �� ����������� �� ��, ��� WAT_LEFT | WAT_TOP
		WAT_LEFT						= FLAG(5),													// ������������ �� ������ ����
		WAT_RIGHT					= FLAG(6),													// ������������ �� ������� ����
		WAT_HCENTER				= WAT_LEFT | WAT_RIGHT,					// �������������
		WAT_TOP							= FLAG(7),													// ������������ �� �������� ����
		WAT_BOTTOM					= FLAG(8),													// ������������ �� ������� ����
		WAT_VCENTER				= WAT_TOP | WAT_BOTTOM,					// �����������
		WAT_CENTER					= WAT_HCENTER | WAT_VCENTER,			// �� ������ ����
		// ������� ������
		WAT_CUT_RIGHT			= FLAG(9),
		WAT_CUT_LEFT				= FLAG(10),												// �� ����� ��� �� ������ �������
		WAT_ADD_DOT				= FLAG(11),												// ��������� ��� ����� ��� �������
		WAT_CUT_RIGHT_DOT	= WAT_CUT_RIGHT | WAT_ADD_DOT,	// �� ������ ������� � ��� � �������
		WAT_CUT_LEFT_DOT		= WAT_CUT_LEFT | WAT_ADD_DOT,		// �� ����� ������ � ��� � �������
		__WAT_IS_CUT				= WAT_CUT_RIGHT | WAT_CUT_LEFT,	// ��� ���������� ����, �� �����!  ����� �� �������
		WAT_MULTI_LINE			= FLAG(12),												// ������������� �����
		WAT_BREAK_WORD			= FLAG(13),												// ���������� �� ������
		WAT_SHIFT_TEXT			= FLAG(14),												// �������� �� ����� ��� �������
	};

	// ������� ����
	const String POINTER_DEFAULT = "POINTER_DEFAULT";   // ������� ���������
    const String POINTER_RESIZE  = "POINTER_RESIZE";    // ��� ��������� ��������
	const String POINTER_TEXT    = "POINTER_TEXT";      // ��� ���������� �����

	// ��� ������� ������, ��������� ����� OVERLAY_MOUSE � �� __OVERLAY_COUNT, ����� ���� ��������� (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE,					// ������� ��� ����������
		OVERLAY_MOUSE,				// ������� ��� ����, !!! �� ������������ ��� �������� ����
		OVERLAY_POPUP,					// ������� ��� ������������ ���� , ����, ����������, ���� �� �������������
		OVERLAY_MAIN,					// �������� ������� ���
		OVERLAY_BACK,					// ����� ������ ������� ���
		__OVERLAY_COUNT,			// ����������� ��������, !!! �� ������������ ��� �������� ����
		OVERLAY_OVERLAPPED,		// ��� �������� ��������������� ����
		OVERLAY_CHILD,					// ���� ����������� ����, ����������� ������� ����
		OVERLAY_DESTROY_ALL		// �������� ���� ���� � ������� destroyWindow
	};
}