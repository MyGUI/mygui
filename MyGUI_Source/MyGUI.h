//=========================================================================================
// MyGUI версия 1.3.0
// автор:  my.name  (my.work(a)inbox.ru)
// офф. русский сайт Ogre   www.Ogre3D.org.ru
//=========================================================================================
//=========================================================================================
#ifndef __MyGUI_H__
#define __MyGUI_H__
//=========================================================================================
#include "Ogre.h"
#include "OgreFontManager.h"
#include "OIS\\OIS.h"
//=========================================================================================
//=========================================================================================
typedef int int32;
typedef short int16;
typedef char int8;
//=========================================================================================
#define __GUI_FONT_HOFFSET 2
#define __GUI_FONT_SIZE_HOFFSET (__GUI_FONT_HOFFSET << 1)

#define __GUI_BUTTON_SHIFT_TEXT_PRESSED 1
#define __GUI_TIME_DOUBLE_CLICK 250

#define __GUI_POPUP_FADE_ALPHA 0.5
//==================================================================================================
// высота оверлея для окна затемнения всего экрана
#define __GUI_ZORDER_FADE 649
// высота оверлея для мыши
#define __GUI_ZORDER_MOUSE 648
// высота оверлея для всплывающих окон
#define __GUI_ZORDER_POPUP 640
// высота оверлея для окна затемнения сообщения
#define __GUI_ZORDER_POPUP_FADE 639
// высота главного оверлея
#define __GUI_ZORDER_MAIN 630
//=========================================================================================
// высота начала оверлея для перекрывающихся окон
#define __GUI_ZORDER_OVERLAPPED 7
// шаг для следующего уровня оверлея перекрывающихся окон
#define __GUI_ZORDER_OVERLAPPED_STEP 10
// высота при которой посылается предупреждение об заполнености слоев
#define __GUI_ZORDER_OVERLAPPED_WARNING 600
//=========================================================================================
// высота нижнего оверлея
#define __GUI_ZORDER_BACK 0
//=========================================================================================
//=========================================================================================
#ifdef _DEBUG
	namespace MyGUI {extern void _LOG( const char *format, ... );} // выводит строку лога в файл
	#define _LOG_NEW _LOG("\r\n")
//	extern void _OUT(char* fmt, ... );
	#define __ASSERT(mygui_exp) assert(mygui_exp)
#else
	#define _LOG
	#define _LOG_NEW _LOG("\r\n")
//	#define _OUT
	#define __ASSERT(mygui_exp)
#endif
//=========================================================================================
#include "loadINI.h"
#include "MyGUI_Skin.h"
#include "MyGUI_GUI.h"
#include "MyGUI_Window.h"
#include "MyGUI_WindowFrame.h"
#include "MyGUI_Button.h"
#include "MyGUI_Edit.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_Tab.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_List.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_Message.h"
//=========================================================================================
#endif
//=========================================================================================
