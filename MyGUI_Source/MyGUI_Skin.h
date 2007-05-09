#pragma once

#include "MyGUI_Defines.h"
#include <vector>

#define __WINDOW_DATA1(_data) ((_data->dataWindow & 0xFF000000) >> 24)
#define __WINDOW_DATA2(_data) ((_data->dataWindow & 0x00FF0000) >> 16)
#define __WINDOW_DATA3(_data) ((_data->dataWindow & 0x0000FF00) >> 8)
#define __WINDOW_DATA4(_data) (_data->dataWindow & 0x000000FF)

namespace MyGUI {

	enum __SKIN_STATES {
		SKIN_STATE_DEACTIVED, // недоступен
		SKIN_STATE_NORMAL, // обычное состояние
		SKIN_STATE_ACTIVED, // наведен курсор, активен
		SKIN_STATE_PRESSED, // нажат
		SKIN_STATE_SELECTED, // выделен (нажат и активен)
		__SKIN_STATE_COUNT // !!! не использовать
	};

	enum __FONT_COLOUR {
		COLOUR_BLACK = 0x000000,
		COLOUR_WHITE = 0xFFFFFF,
		COLOUR_BLUE = 0x0000FF,
		COLOUR_GREEN = 0x00FF00,
		COLOUR_RED = 0xFF0000,
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

	typedef struct __tag_MYGUI_SKIN_INFO { // информация о куске скина окна
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
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0) {}
	} *__LP_MYGUI_SKIN_INFO;

	typedef struct __tag_MYGUI_WINDOW_INFO { // информация об окне
		std::vector <__LP_MYGUI_SKIN_INFO> subSkins; // дополнительные скины окна, дочки
		uint32 dataWindow; // дополнительная информация всего окна
		__LP_MYGUI_FONT_INFO fontWindow; // шрифт всего элемента
		uint32 colour; // цвет текста всего окна
		__tag_MYGUI_WINDOW_INFO::__tag_MYGUI_WINDOW_INFO() : dataWindow(0), fontWindow(0), colour(0) {}
		__tag_MYGUI_WINDOW_INFO::~__tag_MYGUI_WINDOW_INFO()
		{
			while (subSkins.size()) {
				if (subSkins.back()) delete subSkins.back();
				subSkins.pop_back();
			};
		}
	} * __LP_MYGUI_WINDOW_INFO;


	enum __GUI_ENUM_SKIN_WINDOW { // внутренние имена скинов окон
		SKIN_DEFAULT, // пустой скин для служебных целей
		SKIN_FADE, // скин затемнения
		SKIN_WINDOWFRAME, // простое окно
		SKIN_WINDOWFRAME_C, // окно с заголовком
		SKIN_WINDOWFRAME_S, // окно с ресайзером
		SKIN_WINDOWFRAME_X, // окно с кнопкой закрытия
		SKIN_WINDOWFRAME_CS, // окно с заголовком и ресайзером
		SKIN_WINDOWFRAME_CX, // окно с заголовком и закрытием
		SKIN_WINDOWFRAME_SX, // окно с ресайзером и закрытием
		SKIN_WINDOWFRAME_CSX, // окно с заголовком, ресайзером и закрытием
		SKIN_BUTTON, // простая кнопка
		SKIN_CHECK_BOX, // отмечаемый флаг
		SKIN_EDIT, // окно редактирования
		SKIN_VSCROLL, // вертикальная полоса прокрутки
		SKIN_TAB, // панель вкладок
		SKIN_TAB_BUTTON, // кнопка для панели вкладок
		SKIN_STATIC_TEXT, // текст с многострочностью и обрезкой
		SKIN_LIST_S, // список с прокруткой
		SKIN_LIST_BUTTON, // строки для списка
		SKIN_COMBO_BOX_EDIT, // едит для комбобокса
		SKIN_COMBO_BOX, // выпадающий список с окном редактирования
		SKIN_DROP_LIST, // выпадающий список
		__SKIN_COUNT, // колличество окон, !!!не использовать (значения вставлять до этого)
		__SKIN_WIDGET_DEFAULT
	};

	enum __GUI_ENUM_FONT { // внутренние имена шрифтов
		FONT_DEFAULT, // шрифт по умолчанию
		FONT_LARGE,
		__FONT_COUNT // колличество шрифтов, !!!не использовать (значения вставлять до этого)
	};

}