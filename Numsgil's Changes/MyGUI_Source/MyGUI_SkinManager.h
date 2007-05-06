#pragma once

#include "MyGUI_Defines.h"
#include <OgreFont.h>
#include <OgreString.h>

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
		Ogre::String name; // ��� ������
		Ogre::uint8 height; // ������ ������
		Ogre::Real spaceWidth; // ������ �������
		Ogre::Real sizeTripleDot; // ������ ���� �����
		__tag_MYGUI_FONT_INFO::__tag_MYGUI_FONT_INFO() : font(0), height(0), spaceWidth(0.0), sizeTripleDot(0.0) {}
	} * __LP_MYGUI_FONT_INFO;

	typedef struct __tag_MYGUI_POINTER_INFO { // ���������� �� ����������
		Ogre::String strMaterialName; // �������� �������
		int8 iOffsetX; // �������� ����� ���������
		int8 iOffsetY; // �������� ����� ���������
		Ogre::uint8 uSizeX; // ������ ���������
		Ogre::uint8 uSizeY; // ������ ���������
		__tag_MYGUI_POINTER_INFO::__tag_MYGUI_POINTER_INFO() : uSizeX(0), uSizeY(0), iOffsetX(0), iOffsetY(0) {}
	} * __LP_MYGUI_POINTER_INFO;

	typedef struct __tag_MYGUI_SKIN_INFO { // ���������� � ����� ����� ����
		// � �������� ���� �������� ���� ������, ��������� �����, � ����� �������� ������ ��������� �������������
		int16 posX; // ������� X
		int16 posY; // ������� Y
		int16 sizeX; // ������ X
		int16 sizeY; // ������ Y
		
		// ����� ��������� ����
		Ogre::String pStrSkinNormal; // ������� ����
		Ogre::String pStrSkinDeactived; // ������������
		Ogre::String pStrSkinActived; // ������� ������
		Ogre::String pStrSkinSelected; // ������� ������ � �����
		Ogre::String pStrSkinPressed; // �����
		// � �������� ���� ������������
		uint16 align; // ������������
		uint16 event_info; // ��������� ���������� ����
		uint16 exdata; // �������������� ���������� �� ��������
		__tag_MYGUI_SKIN_INFO::__tag_MYGUI_SKIN_INFO() : posX(0), posY(0), sizeX(0), sizeY(0), align(0), event_info(0), exdata(0) {}
	} *__LP_MYGUI_SKIN_INFO;

	typedef struct __tag_MYGUI_WINDOW_INFO { // ���������� �� ����
		std::vector <__LP_MYGUI_SKIN_INFO> subSkins; // �������������� ����� ����, �����
		
		Ogre::String data1;
		Ogre::String data2;
		Ogre::String data3;
		Ogre::String data4;
		
		__LP_MYGUI_FONT_INFO fontWindow; // ����� ����� ��������
		uint32 colour; // ���� ������ ����� ����
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
	const Ogre::String SKIN_WINDOWFRAME = "SKIN_WINDOWFRAME"; // ������� ����
	const Ogre::String SKIN_WINDOWFRAME_C = "SKIN_WINDOWFRAME_C"; // ���� � ����������
	const Ogre::String SKIN_WINDOWFRAME_S = "SKIN_WINDOWFRAME_S"; // ���� � ����������
	const Ogre::String SKIN_WINDOWFRAME_X = "SKIN_WINDOWFRAME_X"; // ���� � ������� ��������
	const Ogre::String SKIN_WINDOWFRAME_CS = "SKIN_WINDOWFRAME_CS"; // ���� � ���������� � ����������
	const Ogre::String SKIN_WINDOWFRAME_CX= "SKIN_WINDOWFRAME_CX"; // ���� � ���������� � ���������
	const Ogre::String SKIN_WINDOWFRAME_SX = "SKIN_WINDOWFRAME_SX"; // ���� � ���������� � ���������
	const Ogre::String SKIN_WINDOWFRAME_CSX = "SKIN_WINDOWFRAME_CSX"; // ���� � ����������, ���������� � ���������
	const Ogre::String SKIN_BUTTON = "SKIN_BUTTON"; // ������� ������
	const Ogre::String SKIN_EDIT = "SKIN_EDIT"; // ���� ��������������
	const Ogre::String SKIN_VSCROLL = "SKIN_VSCROLL"; // ������������ ������ ���������
	const Ogre::String SKIN_TAB = "SKIN_TAB"; // ������ �������
	const Ogre::String SKIN_TAB_BUTTON = "SKIN_TAB_BUTTON"; // ������ ��� ������ �������
	const Ogre::String SKIN_STATIC_TEXT = "SKIN_STATIC_TEXT"; // ����� � ���������������� � ��������
	const Ogre::String SKIN_LIST_S = "SKIN_LIST_S"; // ������ � ����������
	const Ogre::String SKIN_LIST_BUTTON = "SKIN_LIST_BUTTON"; // ������ ��� ������
	const Ogre::String SKIN_COMBO_BOX_EDIT = "SKIN_COMBO_BOX_EDIT"; // ���� ��� ����������
	const Ogre::String SKIN_COMBO_BOX = "SKIN_COMBO_BOX"; // ���������� ������ � ����� ��������������
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
                
		void createSkin(); // ��������� ��� �����
		void unloadResource(); // ��������� ��� �����
		void clearSkins(__LP_MYGUI_SKIN_INFO pSkin); // ������� ����
		void loadSkin(std::map<Ogre::String, uint32> & mapNameValue, const Ogre::String & strFileName); // ��������� ��� ����� ���� �� ������ �����
	};
}