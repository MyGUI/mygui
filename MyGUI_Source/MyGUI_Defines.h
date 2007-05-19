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
    
    enum __FADE_STATE {
		// текущий режим затемнения
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};

	enum __MESSAGE_BOX_BUTTON {
		// кнопки окна сообщения
		MBB_NONE				= FLAG_NONE,
		MBB_BUTTON1		= FLAG(0),				// первая кнопка подтверждения
		MBB_BUTTON2		= FLAG(1),				// вторая кнопка отмены
		MBB_MODAL			= FLAG(2),				// окно запускается в модальном режиме, !!! запускается только одно
	};

	enum __WINDOW_EX_STYLE { // дополниельные стили элементов скинов
		// общие флаги
		WES_NONE								= FLAG_NONE,				// простое окно
		WES_TEXT								= FLAG(0),						// элемент является насителем текста для всего элемента
		WES_CLIENT							= FLAG(1),						// элемент является клиетским окном
		WES_ACTION1							= FLAG(2),
		WES_ACTION2						= FLAG(3),
		WES_ACTION3						= FLAG(4),
		WES_ACTION4						= FLAG(5),
		WES_ACTION5						= FLAG(6),
		WES_ACTION6						= FLAG(7),
		// флаги для WindowFrame
		WES_CAPTION							= WES_ACTION1,			// элемент является заголовком
		WES_RESIZE							= WES_ACTION2,			// элемент является ресайзером
		WES_CLOSE								= WES_ACTION3,			// элемент является кнопкой закрытия
		// флаги для Button
		WES_BUTTON							= WES_ACTION1,			// элемент является кнопкой
		// флаги для Edit
		WES_EDIT								= WES_ACTION1,			// элемент является окном редактирования
		WES_EDIT_CURSOR				= WES_ACTION2,			// элемент является курсором для окна редактирования
		// флаги для VScroll
		WES_VSCROLL_UP					= WES_ACTION1,			// элемент является верхней кнопкой скролла
		WES_VSCROLL_DOWN				= WES_ACTION2,			// элемент является нижней кнопкой скролла
		WES_VSCROLL_TRACK				= WES_ACTION3,			// элемент является нижней кнопкой скролла
		// флаги для TabBar
		WES_TAB_TOP						= WES_ACTION1,			// элемент является верхним скином для растягивания
		// флаги для списка
		WES_LIST_HIDE_SCROLL		= WES_ACTION1,			// скрывать скролл , если он не нужен
		// флаги для выпадающего списка
		WES_COMBO							= WES_ACTION1,			// выпадающий список
		WES_COMBO_BUTTON			= WES_ACTION2,			// кнопка в выпадающем списке
	};

	enum __WINDOW_EVENT { // события на которое окно быдет посылать
		WE_NONE										= FLAG_NONE,						// ничего не будет посылаться
		WE_MOUSE_BUTTON						= FLAG(0),								// нажата левая кнпка мыши
		WE_MOUSE_FOCUS							= FLAG(1),								// смена активности от курсора мыши
		WE_MOUSE_MOVE							= FLAG(2),								// движение мыши после захвата
		WE_KEY_FOCUS								= FLAG(3),								// смена фокуса для ввода клавиатуры
		WE_KEY_BUTTON							= FLAG(4),								// нажата клавиша
		__WE_IS_ACTION							= WE_MOUSE_BUTTON
																  | WE_MOUSE_FOCUS
																  | WE_MOUSE_MOVE
																  | WE_KEY_FOCUS
																  | WE_KEY_BUTTON,			// !!! для внутреннего использования , не юзать
		// сообщения об опасности, посылаются всегда
		WE_WARNING_ZORDER_MAX			= FLAG(5),								// слои перекрывающихся окон подобрались к границе
		WE_WARNING_CHILD_MAX			= FLAG(6),								// колличество дитей равно шагу между слоями
		WE_WARNING_CHILD_DELETE		= FLAG(7),								// ребенок уничтожен, поправте указатели
	};

	enum __WINDOW_OTHER_EVENT {
		// события посылаемые в OtherEvents
		WOE_NONE										= FLAG_NONE,			// нет сообщения
		WOE_SCROLL_CHANGE_POS			= FLAG(0),					// изменилась позиция скролла
		WOE_FRAME_CLOSE						= FLAG(1),					// нажата кнопка закрыть фрейм
		WOE_EDIT_KEY_ACCEPT					= FLAG(2),					// нажата кнопка энтер
		WOE_EDIT_KEY_DELETE				= FLAG(3),					// нажата кнопка делит
		WOE_LIST_SELECT_CHANGE			= FLAG(4),					// изменился выделенный элемент
		WOE_LIST_SELECT_ACCEPT			= FLAG(5),					// двойной щелчек или энтер на элементе
		WOE_LIST_SELECT_NONE				= FLAG(6),					// щелчек на пустом месте
		WOE_COMBO_SELECT_ACCEPT		= FLAG(7),					// активирован элемент
		WOE_MESSAGE_PRESS_BUTTON	= FLAG(8),					// нажата кнопка в окне сообщения
	};

	enum __WINDOW_STATE {
		// состояние окна
		WS_DEACTIVATED,				// окно неактивно и недоступно
		WS_NORMAL,					// обычное состояние окна
		WS_PRESSED,					// кнопка нажата
		__WS_ACTIVATED,				// кнопка под курсором, для внутреннего использования
		__WS_SELECTED				// кнопка под курсором и нажата, для внутреннего использования
	};

	enum __WINDOW_ALIGN {
		// выравнивание окон, если хотим двигать окна, то выравниваем (WA_LEFT|WA_TOP)
		WA_NONE						= FLAG_NONE,											// окно выравнивается по центру
		WA_HCENTER					= FLAG_NONE,											// липа, при нуле он будет центрировать без растяжения
		WA_VCENTER					= FLAG_NONE,											// но центрировать не относительно границ отца
		WA_CENTER						= WA_HCENTER | WA_VCENTER,				// а относительно своего первоначального положения, если нужно другое смотри ниже
		WA_CENTER_FOTHER		= FLAG(0),													// это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		WA_LEFT							= FLAG(1),													// по левому 
		WA_RIGHT						= FLAG(2),													// по правому
		WA_HSTRETCH				= WA_LEFT | WA_RIGHT,							// растягивание по горизонтали
		WA_TOP							= FLAG(3),													// по верху 
		WA_BOTTOM					= FLAG(4),													// по низу
		WA_VSTRETCH				= WA_TOP | WA_BOTTOM,						// растягивание по вертикали
		WA_STRETCH					= WA_HSTRETCH | WA_VSTRETCH,			// растягивание по всем граням
		// выравнивание текста, у него нет растягивания
		WAT_NONE						= FLAG_NONE,											// не выравнивать то же, что WAT_LEFT | WAT_TOP
		WAT_LEFT						= FLAG(5),													// выравнивание по левому краю
		WAT_RIGHT					= FLAG(6),													// выравнивание по правому краю
		WAT_HCENTER				= WAT_LEFT | WAT_RIGHT,					// горизонтально
		WAT_TOP							= FLAG(7),													// выравнивание по верхнему краю
		WAT_BOTTOM					= FLAG(8),													// выравнивание по нижнему краю
		WAT_VCENTER				= WAT_TOP | WAT_BOTTOM,					// вертикально
		WAT_CENTER					= WAT_HCENTER | WAT_VCENTER,			// по центру окна
		// обрезка текста
		WAT_CUT_RIGHT			= FLAG(9),
		WAT_CUT_LEFT				= FLAG(10),												// по левой или по правой стороне
		WAT_ADD_DOT				= FLAG(11),												// добавлять три точки при обрезке
		WAT_CUT_RIGHT_DOT	= WAT_CUT_RIGHT | WAT_ADD_DOT,	// по правой стороне и еще с точками
		WAT_CUT_LEFT_DOT		= WAT_CUT_LEFT | WAT_ADD_DOT,		// по левой тороне и еще с точками
		__WAT_IS_CUT				= WAT_CUT_RIGHT | WAT_CUT_LEFT,	// для внутренних нужд, не юзать!  нужна ли обрезка
		WAT_MULTI_LINE			= FLAG(12),												// многострочный текст
		WAT_BREAK_WORD			= FLAG(13),												// переносить по словам
		WAT_SHIFT_TEXT			= FLAG(14),												// сдвигать ли текст при нажатии
	};

	// курсоры мыши
	const String POINTER_DEFAULT = "POINTER_DEFAULT";   // обычный указатель
    const String POINTER_RESIZE  = "POINTER_RESIZE";    // для изменения размеров
	const String POINTER_TEXT    = "POINTER_TEXT";      // для текстового ввода

	// все оверлеи класса, добавлять после OVERLAY_MOUSE и до __OVERLAY_COUNT, далее идут служебные (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE,					// оверлей для затемнения
		OVERLAY_MOUSE,				// оверлей для мыши, !!! не использовать при создании окна
		OVERLAY_POPUP,					// оверлей для всплывающего окна , меню, комбобокса, окна не перекрываются
		OVERLAY_MAIN,					// основной оверлей гуи
		OVERLAY_BACK,					// самый нижний оверлей гуи
		__OVERLAY_COUNT,			// колличество оверлеев, !!! не использовать при создании окна
		OVERLAY_OVERLAPPED,		// для создания перекрывающихся окон
		OVERLAY_CHILD,					// окно принадлежит отцу, обязательно указать отца
		OVERLAY_DESTROY_ALL		// удаление всех окон в функции destroyWindow
	};
}