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
    const Ogre::uint16 ITEM_NON_SELECT = 0xFFFF; // элемент не выделен
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
    
    enum __FADE_STATE { // текущий режим затемнения
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};
	enum __MESSAGE_BOX_BUTTON { // кнопки окна сообщения
		MBB_NONE = FLAG_NONE,
		MBB_BUTTON1 = FLAG(0), // первая кнопка подтверждения
		MBB_BUTTON2 = FLAG(1), // вторая кнопка отмены
		MBB_MODAL = FLAG(2), // окно запускается в модальном режиме, !!! запускается только одно
	};
	enum __WINDOW_EX_STYLE { // дополниельные стили элементов скинов
		// общие флаги
		WES_NONE = FLAG_NONE, // простое окно
		WES_TEXT = FLAG(0), // элемент является насителем текста для всего элемента
		WES_CLIENT = FLAG(1), // элемент является клиетским окном
		WES_ACTION1 = FLAG(2),
		WES_ACTION2 = FLAG(3),
		WES_ACTION3 = FLAG(4),
		WES_ACTION4 = FLAG(5),
		WES_ACTION5 = FLAG(6),
		WES_ACTION6 = FLAG(7),
		// флаги для WindowFrame
		WES_CAPTION = 0x0010, // элемент является заголовком
		WES_RESIZE = 0x0020, // элемент является ресайзером
		WES_CLOSE = 0x0040, // элемент является кнопкой закрытия
		// флаги для Button
		WES_BUTTON = 0x0010, // элемент является кнопкой
		// флаги для Edit
		WES_EDIT = 0x0010, // элемент является окном редактирования
		WES_EDIT_CURSOR = 0x0020, // элемент является курсором для окна редактирования
		// флаги для VScroll
		WES_VSCROLL_UP = 0x0010, // элемент является верхней кнопкой скролла
		WES_VSCROLL_DOWN = 0x0020, // элемент является нижней кнопкой скролла
		WES_VSCROLL_TRACK = 0x0040, // элемент является нижней кнопкой скролла
		// флаги для TabBar
		WES_TAB_TOP = 0x0010, // элемент является верхним скином для растягивания
		// флаги для списка
		WES_LIST_HIDE_SCROLL = 0x0010, // скрывать скролл , если он не нужен
		// флаги для выпадающего списка
		WES_COMBO = 0x0010, // выпадающий список
		WES_COMBO_BUTTON = 0x0020, // кнопка в выпадающем списке
	};

	enum __WINDOW_EVENT { // события на которое окно быдет посылать
		WE_NONE = FLAG_NONE, // ничего не будет посылаться
		WE_MOUSE_BUTTON = FLAG(0), // нажата левая кнпка мыши
		WE_MOUSE_FOCUS = FLAG(1), // смена активности от курсора мыши
		WE_MOUSE_MOVE = FLAG(2), // движение мыши после захвата
		WE_KEY_FOCUS = FLAG(3), // смена фокуса для ввода клавиатуры
		WE_KEY_BUTTON = FLAG(4), // нажата клавиша
		__WE_IS_ACTION = 0xFF, // !! для внутреннего использования , не юзать  |   есть ли реакция на события мыши и клавиатуры
		// сообщения об опасности, посылаются всегда
		WE_WARNING_ZORDER_MAX = 0x1000, // слои перекрывающихся окон подобрались к границе
		WE_WARNING_CHILD_MAX = 0x2000, // колличество дитей равно шагу между слоями
		WE_WARNING_CHILD_DELETE = 0x4000, // ребенок уничтожен, поправте указатели
	};
	enum __WINDOW_OTHER_EVENT { // события посылаемые в OtherEvents
		WOE_NONE = 0x00000, // нет сообщения
		WOE_SCROLL_CHANGE_POS = 0x0001, // изменилась позиция скролла
		WOE_FRAME_CLOSE = 0x0002, // нажата кнопка закрыть фрейм
		WOE_EDIT_KEY_ACCEPT = 0x0004, // нажата кнопка энтер
		WOE_EDIT_KEY_DELETE = 0x0008, // нажата кнопка делит
		WOE_LIST_SELECT_CHANGE = 0x0010, // изменился выделенный элемент
		WOE_LIST_SELECT_ACCEPT = 0x0020, // двойной щелчек или энтер на элементе
		WOE_LIST_SELECT_NONE = 0x0040, // щелчек на пустом месте
		WOE_COMBO_SELECT_ACCEPT = 0x0080, // активирован элемент
		WOE_MESSAGE_PRESS_BUTTON = 0x0100, // нажата кнопка в окне сообщения
	};
	enum __WINDOW_STATE { // состояние окна
		WS_DEACTIVE, // окно неактивно и недоступно
		WS_NORMAL, // обычное состояние окна
		WS_PRESSED, // кнопка нажата
		__WS_ACTIVED, // кнопка под курсором, для внутреннего использования
		__WS_SELECTED // кнопка под курсором и нажата, для внутреннего использования
	};
	enum __WINDOW_ALIGN {
		// выравнивание окон, если хотим двигать окна, то выравниваем (WA_LEFT|WA_TOP)
		WA_NONE=0x00, //                                 окно выравнивается по центру
		WA_HCENTER=0x00, //                           липа, при нуле он будет центрировать без растяжения
		WA_VCENTER=0x00, //                            но центрировать не относительно границ отца
		WA_CENTER=0x00, //                              а относительно своего первоначального положения, если нужно другое смотри ниже
		WA_CENTER_FOTHER=0x1000, //           это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		WA_LEFT=0x01, WA_RIGHT=0x02, //     по левому или по правому
		WA_HSTRETCH=0x03,  //                        растягивание по горизонтали
		WA_TOP=0x04, WA_BOTTOM=0x08, //  по верху или по низу
		WA_VSTRETCH=0x0C,  //                        растягивание по вертикали
		WA_STRETCH=0x0F, //                           растягивание по всем граням
		// выравнивание текста, у него нет растягивания
		WAT_NONE=0x00, // не выравнивать то же, что WAT_LEFT|WAT_TOP
		WAT_LEFT=0x10, WAT_RIGHT=0x20, WAT_HCENTER=0x30, // горизонтально
		WAT_TOP=0x40, WAT_BOTTOM=0x80, WAT_VCENTER=0xC0, // вертикально
		WAT_CENTER=0xF0, // по центру окна
		// обрезка текста
		WAT_CUT_RIGHT=0x0100, WAT_CUT_LEFT=0x0200, // по левой или по правой стороне
		WAT_ADD_DOT=0x0400, // добавлять три точки при обрезке
		WAT_CUT_RIGHT_DOT=0x0500, WAT_CUT_LEFT_DOT=0x0600, // по левой или по правой стороне и еще с точками
		__WAT_IS_CUT=0x0300, // для внутренних нужд, не юзать!  нужна ли обрезка
		WAT_MULTI_LINE = 0x0800, // многострочный текст
		WAT_BREAK_WORD = 0x2000, // переносить по словам
		WAT_SHIFT_TEXT_PRESSED = 0x8000, // сдвигать ли текст при нажатии
	};

	// курсоры мыши
	enum __MYGUI_POINTER {
		POINTER_DEFAULT, // обычный указатель
		POINTER_RESIZE, // для изменения размеров
		POINTER_TEXT, // для текстового ввода
		__POINTER_COUNT
	};

	// все оверлеи класса, добавлять после OVERLAY_MOUSE и до __OVERLAY_COUNT, далее идут служебные (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE, // оверлей для затемнения
		OVERLAY_MOUSE, // оверлей для мыши, !!! не использовать при создании окна
		OVERLAY_POPUP, // оверлей для всплывающего окна , меню, комбобокса, окна не перекрываются
		OVERLAY_MAIN, // основной оверлей гуи
		OVERLAY_BACK, // самый нижний оверлей гуи
		__OVERLAY_COUNT, // колличество оверлеев, !!! не использовать при создании окна
		OVERLAY_OVERLAPPED, // для создания перекрывающихся окон
		OVERLAY_CHILD, // окно принадлежит отцу, обязательно указать отца
		OVERLAY_DESTROY_ALL // удаление всех окон в функции destroyWindow
	};
}