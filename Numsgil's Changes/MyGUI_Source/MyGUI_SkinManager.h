#pragma once

#include "MyGUI_Defines.h"
#include <OgreFont.h>
#include <OgreString.h>

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
		Ogre::String name; // имя шрифта
		Ogre::uint8 height; // высота шрифта
		Ogre::Real spaceWidth; // ширина пробела
		Ogre::Real sizeTripleDot; // ширина трех точек
		__tag_MYGUI_FONT_INFO::__tag_MYGUI_FONT_INFO() : font(0), height(0), spaceWidth(0.0), sizeTripleDot(0.0) {}
	} * __LP_MYGUI_FONT_INFO;

	typedef struct __tag_MYGUI_POINTER_INFO { // информация об указателях
		Ogre::String strMaterialName; // материал курсора
		int8 iOffsetX; // смещение точки указателя
		int8 iOffsetY; // смещение точки указателя
		Ogre::uint8 uSizeX; // размер указателя
		Ogre::uint8 uSizeY; // размер указателя
		__tag_MYGUI_POINTER_INFO::__tag_MYGUI_POINTER_INFO() : uSizeX(0), uSizeY(0), iOffsetX(0), iOffsetY(0) {}
	} * __LP_MYGUI_POINTER_INFO;

	typedef struct __tag_MYGUI_SKIN_INFO { // информация о куске скина окна
		// у главного окна ставится этот размер, создаются дочки, а потом ставится размер указанный пользователем
		int16 posX; // позиция X
		int16 posY; // позиция Y
		int16 sizeX; // размер X
		int16 sizeY; // размер Y
		
		// скины состояния окна
		Ogre::String pStrSkinNormal; // обычный скин
		Ogre::String pStrSkinDeactived; // заблокирован
		Ogre::String pStrSkinActived; // наведен курсор
		Ogre::String pStrSkinSelected; // наведен курсор и нажат
		Ogre::String pStrSkinPressed; // нажат
		// у главного окна игногируются
		uint16 align; // выравнивание
		uint16 event_info; // сообщения посылаемые отцу
		uint16 exdata; // дополнительная информация об элементе
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0) {}
	} *__LP_MYGUI_SKIN_INFO;

	typedef struct __tag_MYGUI_WINDOW_INFO { // информация об окне
		std::vector <__LP_MYGUI_SKIN_INFO> subSkins; // дополнительные скины окна, дочки
		
		Ogre::String data1;
		Ogre::String data2;
		Ogre::String data3;
		Ogre::String data4;
		
		__LP_MYGUI_FONT_INFO fontWindow; // шрифт всего элемента
		uint32 colour; // цвет текста всего окна
		__tag_MYGUI_WINDOW_INFO::__tag_MYGUI_WINDOW_INFO() : fontWindow(0), colour(0) {}
		__tag_MYGUI_WINDOW_INFO::~__tag_MYGUI_WINDOW_INFO()
		{
			while (subSkins.size()) {
				if (subSkins.back()) delete subSkins.back();
				subSkins.pop_back();
			};
		}
	} * __LP_MYGUI_WINDOW_INFO;


	const Ogre::String SKIN_DEFAULT = "SKIN_DEFAULT"; // whatever is default for this widget
	const Ogre::String SKIN_FADE = "SKIN_FADE"; // Fade window
	const Ogre::String SKIN_WINDOWFRAME = "SKIN_WINDOWFRAME"; // простое окно
	const Ogre::String SKIN_WINDOWFRAME_C = "SKIN_WINDOWFRAME_C"; // окно с заголовком
	const Ogre::String SKIN_WINDOWFRAME_S = "SKIN_WINDOWFRAME_S"; // окно с ресайзером
	const Ogre::String SKIN_WINDOWFRAME_X = "SKIN_WINDOWFRAME_X"; // окно с кнопкой закрытия
	const Ogre::String SKIN_WINDOWFRAME_CS = "SKIN_WINDOWFRAME_CS"; // окно с заголовком и ресайзером
	const Ogre::String SKIN_WINDOWFRAME_CX= "SKIN_WINDOWFRAME_CX"; // окно с заголовком и закрытием
	const Ogre::String SKIN_WINDOWFRAME_SX = "SKIN_WINDOWFRAME_SX"; // окно с ресайзером и закрытием
	const Ogre::String SKIN_WINDOWFRAME_CSX = "SKIN_WINDOWFRAME_CSX"; // окно с заголовком, ресайзером и закрытием
	const Ogre::String SKIN_BUTTON = "SKIN_BUTTON"; // простая кнопка
	const Ogre::String SKIN_EDIT = "SKIN_EDIT"; // окно редактирования
	const Ogre::String SKIN_VSCROLL = "SKIN_VSCROLL"; // вертикальная полоса прокрутки
	const Ogre::String SKIN_TAB = "SKIN_TAB"; // панель вкладок
	const Ogre::String SKIN_TAB_BUTTON = "SKIN_TAB_BUTTON"; // кнопка для панели вкладок
	const Ogre::String SKIN_STATIC_TEXT = "SKIN_STATIC_TEXT"; // текст с многострочностью и обрезкой
	const Ogre::String SKIN_LIST_S = "SKIN_LIST_S"; // список с прокруткой
	const Ogre::String SKIN_LIST_BUTTON = "SKIN_LIST_BUTTON"; // строки для списка
	const Ogre::String SKIN_COMBO_BOX_EDIT = "SKIN_COMBO_BOX_EDIT"; // едит для комбобокса
	const Ogre::String SKIN_COMBO_BOX = "SKIN_COMBO_BOX"; // выпадающий список с окном редактирования
	const Ogre::String SKIN_DROP_LIST = "SKIN_DROP_LIST";
	const Ogre::String __SKIN_WIDGET_DEFAULT = "__SKIN_WIDGET_DEFAULT"; //default for whatever this widget is

	const Ogre::String FONT_DEFAULT = "FONT_DEFAULT";
	const Ogre::String FONT_LARGE   = "FONT_LARGE";
	
	class SkinManager
	{
	    std::vector<Ogre::String> mFontNames;
	    std::vector <__LP_MYGUI_FONT_INFO> mFonts;
		
		std::vector <__LP_MYGUI_POINTER_INFO> mPointers;
		std::vector <Ogre::String>            mPointerNames;
		
		std::vector <__LP_MYGUI_WINDOW_INFO> mSkins;
		std::vector <Ogre::String>           mSkinNames;
		
		~SkinManager()
		{		    
		    unloadResource();
        }
		
		public:
		
		static SkinManager *getSingleton()
		{
		    static SkinManager manager;
		    return &manager;
        }
        
        __LP_MYGUI_WINDOW_INFO getSkin(const Ogre::String &SkinName);        
        __LP_MYGUI_FONT_INFO getFont(const Ogre::String &FontName);        
        __LP_MYGUI_POINTER_INFO getPointer(const Ogre::String &PointerName);
        
        __LP_MYGUI_WINDOW_INFO defineNewSkin(const Ogre::String &Name);
        __LP_MYGUI_FONT_INFO defineNewFont(const Ogre::String &FontName);        
        __LP_MYGUI_POINTER_INFO defineNewPointer(const Ogre::String &PointerName);
        
        const std::vector<Ogre::String> getSkinNames();
        const std::vector<Ogre::String> getFontNames();
        const std::vector<Ogre::String> getPointerNames();
        
        size_t getSkinCount() { return mSkins.size(); }
        size_t getFontCount() { return mFonts.size(); }
        size_t getPointerCount() { return mPointers.size(); }
        
        //Returns >= 0 if the skin is defined.  It's actually returning its index
        int isSkinDefined(const Ogre::String &Name);
        int isFontDefined(const Ogre::String &Name);
        int isPointerDefined(const Ogre::String &Name);
                
		void createSkin(); // загружает все скины
		void unloadResource(); // выгружает все скины
		void clearSkins(__LP_MYGUI_SKIN_INFO pSkin); // удаляет скин
		void loadSkin(std::map<Ogre::String, uint32> & mapNameValue, const Ogre::String & strFileName); // загружает все скины окон из одного файла
	};
}