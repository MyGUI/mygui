#pragma once

#include "MyGUI_Defines.h"
#include <vector>
#include <OgreColourValue.h>

namespace MyGUI {
    
    enum FADE_STATES {
		NONE,
		FADE_CHANGE_STATE,
	};

	enum __SKIN_STATES {
		SKIN_STATE_DEACTIVED, // недоступен
		SKIN_STATE_NORMAL, // обычное состояние
		SKIN_STATE_ACTIVED, // наведен курсор, активен
		SKIN_STATE_PRESSED, // нажат
		SKIN_STATE_SELECTED, // выделен (нажат и активен)
		__SKIN_STATE_COUNT // !!! не использовать
	};

    typedef struct __tag_MYGUI_FONT_INFO { // информация о шрифте
		Ogre::Font * font; // шрифт
		String name; // имя шрифта
		uint8 height; // высота шрифта
		Real spaceWidth; // ширина пробела
		Real sizeTripleDot; // ширина трех точек
		__tag_MYGUI_FONT_INFO::__tag_MYGUI_FONT_INFO() : font(0), height(0), spaceWidth(0.0), sizeTripleDot(0.0) {}
	} * __LP_MYGUI_FONT_INFO;

	typedef struct __tag_MYGUI_POINTER_INFO { // информация об указателях
		String strMaterialName; // материал курсора
		int8 iOffsetX; // смещение точки указателя
		int8 iOffsetY; // смещение точки указателя
		uint8 uSizeX; // размер указателя
		uint8 uSizeY; // размер указателя
		__tag_MYGUI_POINTER_INFO::__tag_MYGUI_POINTER_INFO() : uSizeX(0), uSizeY(0), iOffsetX(0), iOffsetY(0) {}
	} * __LP_MYGUI_POINTER_INFO;

	typedef struct __tag_MYGUI_SUBSKIN_INFO { // информация о куске скина окна
		// у главного окна ставится этот размер, создаются дочки, а потом ставится размер указанный пользователем
		int16 posX; // позиция X
		int16 posY; // позиция Y
		int16 sizeX; // размер X
		int16 sizeY; // размер Y
		// скины состояния окна
		String pStrSkinNormal; // обычный скин
		String pStrSkinDeactived; // заблокирован
		String pStrSkinActived; // наведен курсор
		String pStrSkinSelected; // наведен курсор и нажат
		String pStrSkinPressed; // нажат
		// у главного окна игногируются
		uint16 align; // выравнивание
		uint16 event_info; // сообщения посылаемые отцу
		uint16 exdata; // дополнительная информация об элементе
		__tag_MYGUI_SUBSKIN_INFO::__tag_MYGUI_SUBSKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0) {}
	} *__LP_MYGUI_SUBSKIN_INFO;

    typedef struct __tag_MYGUI_SKIN_INFO { // информация об окне
		std::vector <const __tag_MYGUI_SUBSKIN_INFO *> subSkins; // дополнительные скины окна, дочки
		//uint32 dataWindow; // дополнительная информация всего окна
		String data1, data2, data3, data4;
		String fontWindow; // шрифт всего элемента
		Ogre::ColourValue colour; // цвет текста всего окна
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : colour(0) {}
		__tag_MYGUI_SKIN_INFO::~__tag_MYGUI_SKIN_INFO() { }
	} * __LP_MYGUI_SKIN_INFO;


	const String SKIN_DEFAULT = "SKIN_DEFAULT"; // пустой скин для служебных целей
	const String SKIN_FADE = "SKIN_FADE"; // скин затемнения
    const String SKIN_WINDOWFRAME = "SKIN_WINDOWFRAME"; // простое окно
	const String SKIN_WINDOWFRAME_C = "SKIN_WINDOWFRAME_C"; // окно с заголовком
	const String SKIN_WINDOWFRAME_S = "SKIN_WINDOWFRAME_S"; // окно с ресайзером
	const String SKIN_WINDOWFRAME_X = "SKIN_WINDOWFRAME_X"; // окно с кнопкой закрытия
	const String SKIN_WINDOWFRAME_CS = "SKIN_WINDOWFRAME_CS"; // окно с заголовком и ресайзером
	const String SKIN_WINDOWFRAME_CX = "SKIN_WINDOWFRAME_CX"; // окно с заголовком и закрытием
	const String SKIN_WINDOWFRAME_SX = "SKIN_WINDOWFRAME_SX"; // окно с ресайзером и закрытием
	const String SKIN_WINDOWFRAME_CSX = "SKIN_WINDOWFRAME_CSX"; // окно с заголовком, ресайзером и закрытием
	const String SKIN_BUTTON = "SKIN_BUTTON"; // простая кнопка
	const String SKIN_CHECK_BOX = "SKIN_CHECK_BOX"; // отмечаемый флаг
	const String SKIN_EDIT = "SKIN_EDIT"; // окно редактирования
	const String SKIN_VSCROLL = "SKIN_VSCROLL"; // вертикальная полоса прокрутки
	const String SKIN_TAB = "SKIN_TAB"; // панель вкладок
	const String SKIN_TAB_BUTTON = "SKIN_TAB_BUTTON"; // кнопка для панели вкладок
	const String SKIN_STATIC_TEXT = "SKIN_STATIC_TEXT"; // текст с многострочностью и обрезкой
	const String SKIN_LIST_S = "SKIN_LIST_S"; // список с прокруткой
	const String SKIN_LIST_BUTTON = "SKIN_LIST_BUTTON"; // строки для списка
	const String SKIN_COMBO_BOX_EDIT = "SKIN_COMBO_BOX_EDIT"; // едит для комбобокса
	const String SKIN_COMBO_BOX = "SKIN_COMBO_BOX"; // выпадающий список с окном редактирования
	const String SKIN_DROP_LIST = "SKIN_DROP_LIST"; // выпадающий список
    const String __SKIN_WIDGET_DEFAULT = "SKIN_WIDGET_DEFAULT";
	
	const String FONT_DEFAULT = "FONT_DEFAULT"; // шрифт по умолчанию
	const String FONT_LARGE =   "FONT_LARGE";
	
}