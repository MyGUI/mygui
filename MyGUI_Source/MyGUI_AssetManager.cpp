#include "MyGUI_AssetManager.h"
#include "MyGUI_Skin.h"
#include "MyGUI_GUI.h"
#include <Ogre.h>
#include <OgreFontManager.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	const String AssetManager::BLOCK_WINDOW_NAME = "windowSkin";
	const String AssetManager::VALUE_WINDOW_SKIN = "skin_element";
	const String AssetManager::VALUE_WINDOW_FONT = "font";
	const String AssetManager::VALUE_WINDOW_COLOUR = "colour";
	const String AssetManager::VALUE_WINDOW_DATA1 = "data1";
	const String AssetManager::VALUE_WINDOW_DATA2 = "data2";
	const String AssetManager::VALUE_WINDOW_DATA3 = "data3";
	const String AssetManager::VALUE_WINDOW_DATA4 = "data4";

	const String AssetManager::VALUE_WINDOW_ADDED_SKIN1 = "added_skin1";
	const String AssetManager::VALUE_WINDOW_ADDED_SKIN2 = "added_skin2";
	const String AssetManager::VALUE_WINDOW_ADDED_DATA1 = "added_data1";
	const String AssetManager::VALUE_WINDOW_ADDED_DATA2 = "added_data2";
	const String AssetManager::VALUE_WINDOW_MATERIAL = "material";

	const String AssetManager::BLOCK_FONT_NAME = "Font";
	const String AssetManager::VALUE_FONT_NAME = "name";
	const String AssetManager::VALUE_FONT_SIZE = "size";

	const String AssetManager::BLOCK_POINTER_NAME = "Pointer";
	const String AssetManager::VALUE_POINTER_SIZE = "size";
	const String AssetManager::VALUE_POINTER_OFFSET = "offset";
	const String AssetManager::VALUE_POINTER_SKIN = "skin";

	const String AssetManager::SECTION_SUB_SKIN = "subSkin";
	const String AssetManager::SECTION_SUB_SKIN_MAIN = "subSkinMain";

	const String AssetManager::VALUE_SKIN_POSITION = "position";
	const String AssetManager::VALUE_SKIN_ALIGN = "align";
	const String AssetManager::VALUE_SKIN_EVENT = "event";
	const String AssetManager::VALUE_SKIN_STYLE = "style";
	const String AssetManager::VALUE_SKIN_OFFSET = "offset";

	const String AssetManager::VALUE_SKIN_DEACTIVATED = "skin_deactivated";
	const String AssetManager::VALUE_SKIN_NORMAL = "skin_normal";
	const String AssetManager::VALUE_SKIN_ACTIVED = "skin_actived";
	const String AssetManager::VALUE_SKIN_PRESSED = "skin_pressed";
	const String AssetManager::VALUE_SKIN_SELECTED = "skin_selected";

	const String AssetManager::VALUE_OFFSET_DEACTIVATED = "offset_deactivated";
	const String AssetManager::VALUE_OFFSET_NORMAL = "offset_normal";
	const String AssetManager::VALUE_OFFSET_ACTIVED = "offset_actived";
	const String AssetManager::VALUE_OFFSET_PRESSED = "offset_pressed";
	const String AssetManager::VALUE_OFFSET_SELECTED = "offset_selected";

	AssetManager::mapValue AssetManager::mMapValueEvent;
	AssetManager::mapValue AssetManager::mMapValueAligin;
	AssetManager::mapValue AssetManager::mMapValueStyle;

	void AssetManager::getResourcePath(std::vector<String> & saFilePath, const String & strMaskFileName)
	{
		saFilePath.clear();
		String strPath;
		Ogre::FileInfoListPtr pFileInfo = ResourceGroupManager::getSingleton().findResourceFileInfo(ResourceGroupManager::getSingleton().DEFAULT_RESOURCE_GROUP_NAME, strMaskFileName);
		for (FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); fi++ ) {
			strPath = fi->archive->getName();
			strPath += "\\";
			strPath += fi->filename;
			saFilePath.push_back(strPath);
		}
	}

	AssetManager *AssetManager::loadAssets() // загружает все скины
	{
		//---------------------------------------------------------------------------------------------------------------
		#define __REGISTER_VALUE_EVENT(_value) mMapValueEvent[#_value]=_value;_LOG("RESISTER_VALUE_EVENT    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_ALIGN(_value) mMapValueAligin[#_value]=_value;_LOG("RESISTER_VALUE_ALIGN    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_STYLE(_value) mMapValueStyle[#_value]=_value;_LOG("RESISTER_VALUE_STYLE    %s \t %d (0x%.8X)", #_value, _value, _value);
		//---------------------------------------------------------------------------------------------------------------
		_LOG_NEW; // с красной строки
		
		// регистрация имен дополнительных стилей
		__REGISTER_VALUE_STYLE(WES_NONE);
		__REGISTER_VALUE_STYLE(WES_TEXT);
		__REGISTER_VALUE_STYLE(WES_CLIENT);
		__REGISTER_VALUE_STYLE(WES_ACTION1);
		__REGISTER_VALUE_STYLE(WES_ACTION2);
		__REGISTER_VALUE_STYLE(WES_ACTION3);
		__REGISTER_VALUE_STYLE(WES_ACTION4);
		__REGISTER_VALUE_STYLE(WES_ACTION5);
		__REGISTER_VALUE_STYLE(WES_ACTION6);
		__REGISTER_VALUE_STYLE(WES_CAPTION);
		__REGISTER_VALUE_STYLE(WES_RESIZE);
		__REGISTER_VALUE_STYLE(WES_CLOSE);
		__REGISTER_VALUE_STYLE(WES_BUTTON);
		__REGISTER_VALUE_STYLE(WES_EDIT);
		__REGISTER_VALUE_STYLE(WES_EDIT_CURSOR);
		__REGISTER_VALUE_STYLE(WES_VSCROLL_UP);
		__REGISTER_VALUE_STYLE(WES_VSCROLL_DOWN);
		__REGISTER_VALUE_STYLE(WES_VSCROLL_TRACK);
		__REGISTER_VALUE_STYLE(WES_TAB_TOP);
		__REGISTER_VALUE_STYLE(WES_LIST_HIDE_SCROLL);
		__REGISTER_VALUE_STYLE(WES_COMBO);
		__REGISTER_VALUE_STYLE(WES_COMBO_BUTTON);
		// регистрация имен событий
		__REGISTER_VALUE_EVENT(WE_NONE);
		__REGISTER_VALUE_EVENT(WE_MOUSE_BUTTON);
		__REGISTER_VALUE_EVENT(WE_MOUSE_FOCUS);
		__REGISTER_VALUE_EVENT(WE_MOUSE_MOVE);
		__REGISTER_VALUE_EVENT(WE_KEY_FOCUS);
		__REGISTER_VALUE_EVENT(WE_KEY_BUTTON);
		// регистрация имен выравнивания
		__REGISTER_VALUE_ALIGN(WA_NONE);
		__REGISTER_VALUE_ALIGN(WA_HCENTER);
		__REGISTER_VALUE_ALIGN(WA_VCENTER);
		__REGISTER_VALUE_ALIGN(WA_CENTER);
		__REGISTER_VALUE_ALIGN(WA_CENTER_PARENT);
		__REGISTER_VALUE_ALIGN(WA_LEFT);
		__REGISTER_VALUE_ALIGN(WA_RIGHT);
		__REGISTER_VALUE_ALIGN(WA_HSTRETCH);
		__REGISTER_VALUE_ALIGN(WA_TOP);
		__REGISTER_VALUE_ALIGN(WA_BOTTOM);
		__REGISTER_VALUE_ALIGN(WA_VSTRETCH);
		__REGISTER_VALUE_ALIGN(WA_STRETCH);
		__REGISTER_VALUE_ALIGN(WAT_NONE);
		__REGISTER_VALUE_ALIGN(WAT_LEFT);
		__REGISTER_VALUE_ALIGN(WAT_RIGHT);
		__REGISTER_VALUE_ALIGN(WAT_HCENTER);
		__REGISTER_VALUE_ALIGN(WAT_TOP);
		__REGISTER_VALUE_ALIGN(WAT_BOTTOM);
		__REGISTER_VALUE_ALIGN(WAT_VCENTER);
		__REGISTER_VALUE_ALIGN(WAT_CENTER);
		__REGISTER_VALUE_ALIGN(WAT_CUT_RIGHT);
		__REGISTER_VALUE_ALIGN(WAT_CUT_LEFT);
		__REGISTER_VALUE_ALIGN(WAT_ADD_ELLIPSES);
		__REGISTER_VALUE_ALIGN(WAT_CUT_RIGHT_ELLIPSES);
		__REGISTER_VALUE_ALIGN(WAT_CUT_LEFT_ELLIPSES);
		__REGISTER_VALUE_ALIGN(WAT_MULTI_LINE);
		__REGISTER_VALUE_ALIGN(WAT_BREAK_WORD);
		__REGISTER_VALUE_ALIGN(WAT_SHIFT_TEXT);
		//---------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------
		// загрузка скинов
		vector<String> saFilePath;
		getResourcePath(saFilePath, "*.mygui_skin");
		for (size_t pos=0; pos < saFilePath.size(); pos++)
		    loadDefinitions(/*mapNameValue, */saFilePath[pos]);
		
		//---------------------------------------------------------------------------------------------------------------
		// Font checks
		__ASSERT(Fonts()->getDefinition(FONT_DEFAULT) && "No default font was defined.");
		
		//Check each font for existence
		
		_LOG_NEW;
		
		//---------------------------------------------------------------------------------------------------------------
		// Pointer checks
		if (!Pointers()->getDefinition(POINTER_DEFAULT))
		    Pointers()->defineNew(POINTER_DEFAULT);
		    
		//Check each pointer for existence
		
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
		// Skin checks
		
		__ASSERT(Skins()->getDefinition(SKIN_DEFAULT) && "No default skin defined.");
		__ASSERT(Skins()->getDefinition(SKIN_DEFAULT)->subSkins.size() > 0 &&
		    "Default skin does not have any sub skins defined.");
		
		for (SkinIterator pos = Skins()->begin(); pos != Skins()->end(); ++pos) {
            //Check that each skin exists
            
            //Check that the skins are well defined
			if(pos->second->subSkins.size() == 0)
			{
				delete pos->second;
				pos->second = new __tag_MYGUI_SKIN_INFO( *Skins()->getDefinition(SKIN_DEFAULT));
				_LOG("[ERROR] skin is not loaded \"%s\" seting to SKIN_DEFAULT", pos->first.c_str());
			} else {
				// convert pix to offset
				// перебираем все сабскины и конвертируем текстурные координаты смещения 
				for (uint16 sub=0; sub<pos->second->subSkins.size(); sub++) {
					__MYGUI_SUBSKIN_INFO * subSkin = const_cast<__MYGUI_SUBSKIN_INFO *> (pos->second->subSkins[sub]);
					uint16 sizeX, sizeY;
					if (GUI::getMaterialSize(pos->second->SkinElement, sizeX, sizeY)) {
						for (uint8 index=0; index<__SKIN_STATE_COUNT; index++) {
							if (subSkin->fOffsetStateSkin[index][_CX] != 0.0) {

								subSkin->fOffsetStateSkin[index][_X] = subSkin->fOffsetStateSkin[index][_X] / (float)sizeX;
								subSkin->fOffsetStateSkin[index][_Y] = subSkin->fOffsetStateSkin[index][_Y] / (float)sizeY;
								subSkin->fOffsetStateSkin[index][_CX] = subSkin->fOffsetStateSkin[index][_CX] / (float)sizeX;
								subSkin->fOffsetStateSkin[index][_CY] = subSkin->fOffsetStateSkin[index][_CY] / (float)sizeY;

								subSkin->fOffsetStateSkin[index][_CX] += subSkin->fOffsetStateSkin[index][_X];
								subSkin->fOffsetStateSkin[index][_CY] += subSkin->fOffsetStateSkin[index][_Y];

							} // if (subSkin->fOffsetStateSkin[index][_CX] != 0.0) {
						} // for (uint8 index; index<__SKIN_STATE_COUNT; index++) {
					} // if (getMaterialSize(pos->second->SkinElement, sizeX, sizeY)) {
				} // for (uint16 sub=0; sub<pos->second->subSkins.size(); sub++) {

			} // if(pos->second->subSkins.size() == 0)
		} // for (SkinIterator pos = Skins()->begin(); pos != Skins()->end(); ++pos) {
		_LOG_NEW;
		
		return this;
	}

	AssetManager *AssetManager::unloadAssets() // выгружает скины
	{
		_LOG_NEW;
		// шрифты
		Fonts()->clearAll();
		Pointers()->clearAll();
		SubSkins()->clearAll();
		Skins()->clearAll();		
		
		return this;
	}

    // загружает все скины окон из одного файла
	void AssetManager::loadDefinitions(const String & strFileName)
	{
		_LOG("\r\nload MyGUI source \t '%s'\r\n", strFileName.c_str());

		loadINI ini;
		if (!ini.open(strFileName.c_str())) return;

		while (ini.seekNextBlock()) {

            //Defines a new skin
			if (ini.getBlockType() == BLOCK_WINDOW_NAME) {
				
				loadSkinDefinitions(ini);

			// загрузка шрифтов окон
			} else if (ini.getBlockType() == BLOCK_FONT_NAME) {
                
				loadFontDefinitions(ini);

			// загрузка указателей
			} else if (ini.getBlockType() == BLOCK_POINTER_NAME) {

				loadPointerDefinitions(ini);
                
			// а это что за хрень
			} else { _LOG("\r\n\t[ERROR] unknow block type  -   type(%s),  name(%s)",

				ini.getBlockType().c_str(), ini.getBlockName().c_str());

			}

		}; // while (ini.seekNextBlock()) {

		ini.close();
		_LOG_NEW;
	} // void AssetManager::loadSkinDefinitions(const String & strFileName)

	void AssetManager::loadPointerDefinitions(loadINI & ini)
	{

        const String Name = ini.getBlockName();
		__LP_MYGUI_POINTER_INFO pointer = Pointers()->defineNew(Name);

		while (ini.LoadNextValue()) {
			String strValueName, strValue;
			uint32 uValue;
			if (ini.getValueName(strValueName)) {
				if (strValueName == VALUE_POINTER_SKIN) {
					if (ini.getValue(strValue)) pointer->strMaterialName = strValue;
				} else if (strValueName == VALUE_POINTER_SIZE) {
					if (ini.getValue(uValue)) {
						pointer->uSizeX = uValue;
						if (ini.getValue(uValue, 1)) pointer->uSizeY = uValue;
					}
				} else if (strValueName == VALUE_POINTER_OFFSET) {
					if (ini.getValue(uValue)) {
						pointer->iOffsetX = uValue;
						if (ini.getValue(uValue, 1)) pointer->iOffsetY = uValue;
					}
				}
			}
		};

		ResourcePtr resource = MaterialManager::getSingleton().getByName(pointer->strMaterialName);

		if (!resource.isNull()) { // есть такой материал
			_LOG("\tload pointer \t %s    '%s'  (%d, %d, %d, %d)",
			    ini.getBlockName().c_str(), pointer->strMaterialName.c_str(),
			    pointer->uSizeX, pointer->uSizeY, pointer->iOffsetX, pointer->iOffsetY);
		} else { // чего то невидно
			_LOG("\t[ERROR] load pointer \t %s", ini.getBlockName().c_str());
			Pointers()->deleteDefinition(Name);
		}				

	} // void AssetManager::loadPointerDefinitions()

	void AssetManager::loadFontDefinitions(loadINI & ini)
	{

        const String Name = ini.getBlockName();
		__LP_MYGUI_FONT_INFO font = Fonts()->defineNew(Name);

		while (ini.LoadNextValue()) {
			String strValueName, strValue;
			uint32 uValue;
			if (ini.getValueName(strValueName)) {
				if (strValueName == VALUE_FONT_NAME) {
					if (ini.getValue(strValue))
					    font->name = strValue;
				} else if (strValueName == VALUE_FONT_SIZE) {
					if (ini.getValue(uValue))
					    font->height = uValue;
				}
			}
		};

		font->font = (Font *)FontManager::getSingleton().getByName(font->name).getPointer();
		if (font->font) { // имя есть
			font->font->load();
			font->spaceWidth = font->font->getGlyphAspectRatio(0x0030/*UNICODE_ZERO*/) * (Real)font->height;
			font->sizeEllipses = (font->font->getGlyphAspectRatio( '.' ) * 3.0 ) * (Real)font->height;
			_LOG("\tload font \t %s    '%s' %d", ini.getBlockName().c_str(), font->name.c_str(), font->height);
        } else { // неудачная загрузка
			_LOG("\t[ERROR] load font \t %s", ini.getBlockName().c_str());
			Fonts()->deleteDefinition(Name);
		}
	} // void AssetManager::loadFontDefinitions(loadINI & ini)

	void AssetManager::loadSubSkinDefinitions(loadINI & ini, 
		const String & Name, __tag_MYGUI_SKIN_INFO * window, 
		unsigned int & UniqueSerialID
		)
	{

		uint32 uValue;
		float fValue;
		size_t pos;
		String strValueName, strValue;

		const String strBlockType = ini.getBlockType();

		const String enumNameOffset[__SKIN_STATE_COUNT] = {
			VALUE_OFFSET_DEACTIVATED,
			VALUE_OFFSET_NORMAL,
			VALUE_OFFSET_ACTIVED,
			VALUE_OFFSET_PRESSED,
			VALUE_OFFSET_SELECTED
		};
        
        //Invent a unique name for this sub skin if there isn't one
        const String strBlockName = (ini.getBlockName() == "") ?
            (Name + ++UniqueSerialID) :
            (ini.getBlockName());
		
		__MYGUI_SUBSKIN_INFO *skin = SubSkins()->defineNew(strBlockName);						

		while (ini.LoadNextValue()) {
			if (ini.getValueName(strValueName)) {

				if (strValueName == VALUE_SKIN_POSITION) {
					if (ini.getValue(uValue, 0)) skin->posX = (int16)uValue;
					if (ini.getValue(uValue, 1)) skin->posY = (int16)uValue;
					if (ini.getValue(uValue, 2)) skin->sizeX = (int16)uValue;
					if (ini.getValue(uValue, 3)) skin->sizeY = (int16)uValue;
				} else if (strValueName == VALUE_SKIN_ALIGN) {
					pos = 0;
					while (ini.getValue(strValue, pos)) {
						skin->align |= mMapValueAligin[strValue];
						pos ++;
					};
				} else if (strValueName == VALUE_SKIN_EVENT) {
					pos = 0;
					while (ini.getValue(strValue, pos)) {
						skin->event_info |= mMapValueEvent[strValue];
						pos ++;
					};
				} else if (strValueName == VALUE_SKIN_STYLE) {
					pos = 0;
					while (ini.getValue(strValue, pos)) {
						skin->exdata |= mMapValueStyle[strValue];
						pos ++;
					};
				} else if (strValueName == VALUE_SKIN_DEACTIVATED) {
					if (ini.getValue(strValue)) skin->SkinState[SKIN_STATE_DEACTIVATED] = strValue;
				} else if (strValueName == VALUE_SKIN_NORMAL) {
					if (ini.getValue(strValue)) skin->SkinState[SKIN_STATE_NORMAL] = strValue;
				} else if (strValueName == VALUE_SKIN_ACTIVED) {
					if (ini.getValue(strValue)) skin->SkinState[SKIN_STATE_ACTIVED] = strValue;
				} else if (strValueName == VALUE_SKIN_PRESSED) {
					if (ini.getValue(strValue)) skin->SkinState[SKIN_STATE_PRESSED] = strValue;
				} else if (strValueName == VALUE_SKIN_SELECTED) {
					if (ini.getValue(strValue)) skin->SkinState[SKIN_STATE_SELECTED] = strValue;
				} else {
					for (uint8 pos=0; pos<__SKIN_STATE_COUNT; pos++) {
						if (strValueName == enumNameOffset[pos]) {
							if (ini.getValue(fValue, 0)) { skin->fOffsetStateSkin[pos][_X] = fValue;
								if (ini.getValue(fValue, 1)) { skin->fOffsetStateSkin[pos][_Y] = fValue;
									if (ini.getValue(fValue, 2)) { skin->fOffsetStateSkin[pos][_CX] = fValue;
										if (ini.getValue(fValue, 3)) skin->fOffsetStateSkin[pos][_CY] = fValue;
									}
								}
							}
							pos = 5;
						}
					} // for (uint8 pos=0; pos<5; pos++) {

				} // if (strValueName == VALUE_SKIN_POSITION) {

			} //if (ini.getValueName(strValueName)) {
		}; // while (ini.LoadNextValue()) {

		if (strBlockType == SECTION_SUB_SKIN)
		    window->subSkins.push_back(skin);
		else if (strBlockType == SECTION_SUB_SKIN_MAIN)
		{
			if (window->subSkins.size()) { // вставляем в начало
				window->subSkins.push_back(window->subSkins[0]);
				window->subSkins[0] = skin;
			} else  window->subSkins.push_back(skin);
		} else {
			_LOG("\t\t[ERROR] unknow section type '%s'", strBlockType.c_str());
			delete skin;
			skin = 0;
		}

		if (skin) { // все хорошо
			_LOG("\t\tadd sub skin  '%s'  (%p) [%d, %d, %d, %d]   "
			     "align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  deactive(%s)  "
			     "active(%s)  normal(%s)  pressed(%s)  selected(%s)", strBlockName.c_str(),
			     skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY, skin->align,
			     skin->event_info, skin->exdata, skin->SkinState[SKIN_STATE_DEACTIVATED].c_str(),
			     skin->SkinState[SKIN_STATE_ACTIVED].c_str(), skin->SkinState[SKIN_STATE_NORMAL].c_str(),
			     skin->SkinState[SKIN_STATE_PRESSED].c_str(), skin->SkinState[SKIN_STATE_SELECTED].c_str());
			
		}
	} // void AssetManager::loadSubSkinDefinitions(loadINI & ini)

	void AssetManager::loadSkinDefinitions(loadINI & ini)
	{

		unsigned int UniqueSerialID = 0; //used to identify unnamed sub skins
		const String Name = ini.getBlockName();
		__tag_MYGUI_SKIN_INFO * window = Skins()->defineNew(Name);
		
		_LOG("\r\n\tcreate window info \t %s  (%p)", ini.getBlockName().c_str(), window);

		uint32 uValue;
		String strValueName, strValue;
		// дефолтные значения
		window->fontWindow = FONT_DEFAULT;
		window->colour = ColourValue::Black;

		while (ini.LoadNextValue()) {
			if (ini.getValueName(strValueName)) {

				if (strValueName == VALUE_WINDOW_SKIN) {
					if (ini.getValue(strValue)) {
						ResourcePtr res = Ogre::MaterialManager::getSingleton().getByName(strValue);
						if (!res.isNull()) window->SkinElement = strValue;
					}
				} else if (strValueName == VALUE_WINDOW_FONT) {
					if (ini.getValue(strValue))
					    window->fontWindow = strValue;
				} else if (strValueName == VALUE_WINDOW_COLOUR) {
				    //Custom colours: define as a 0-255 value in RGBA form.
				    //undefined elements are assumed to be 1.0 (ie: full colour)
				    ColourValue ReadColour(1,1,1);
					if (ini.getValue(strValue))
					{
					    //TODO: ignore case differences
					    if(strValue == "COLOUR_BLACK" || strValue == "Black")
					        ReadColour = ColourValue::Black;
                        if(strValue == "COLOUR_WHITE" || strValue == "White")
					        ReadColour = ColourValue::White;
					    if(strValue == "COLOUR_BLUE" || strValue == "Blue")
					        ReadColour = ColourValue::Blue;
					    if(strValue == "COLOUR_GREEN" || strValue == "Green")
					        ReadColour = ColourValue::Green;
					    if(strValue == "COLOUR_RED" || strValue == "Red")
					        ReadColour = ColourValue::Red;
					    
					    _LOG("\t[ERROR]Attempting to use undefined colour preset \t %s", strValue.c_str());
					}
					
					if(ini.getValue(uValue))
					{
					    if (ini.getValue(uValue, 0))
					        ReadColour.r = uValue / 255.0f;
                        if (ini.getValue(uValue, 1))
					        ReadColour.g = uValue / 255.0f;
					    if (ini.getValue(uValue, 2))
					        ReadColour.b = uValue / 255.0f;
					    if (ini.getValue(uValue, 3))
					        ReadColour.a = uValue / 255.0f;							
					}
					
					window->colour = ReadColour;
				} else if (strValueName == VALUE_WINDOW_DATA1) {
					if (ini.getValue(strValue))
					    window->data1 = strValue;
				} else if (strValueName == VALUE_WINDOW_DATA2) {
					if (ini.getValue(strValue))
					    window->data2 = strValue;
				} else if (strValueName == VALUE_WINDOW_DATA3) {
					if (ini.getValue(strValue))
					    window->data3 = strValue;
				} else if (strValueName == VALUE_WINDOW_DATA4) {
					if (ini.getValue(strValue))
					    window->data4 = strValue;
				}
				// уже загруженный скин просто присваиваем указатель
				else if ((strValueName == SECTION_SUB_SKIN) || (strValueName == SECTION_SUB_SKIN_MAIN))
				{
					if (ini.getValue(strValue)) //does it have a name(?)
					{
						const __MYGUI_SUBSKIN_INFO *skin = SubSkins()->getDefinition(strValue);

						if (skin)
						{
							 // вставляем в начало
							if ((window->subSkins.size()) && (strValueName == SECTION_SUB_SKIN_MAIN)) {
								window->subSkins.push_back(window->subSkins[0]);
								window->subSkins[0] = skin;
							}
							else
							    window->subSkins.push_back(skin);
							
							_LOG("\t\tset sub skin  '%s'  (%p) [%d, %d, %d, %d]   "
							     "align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  deactive(%s)  "
							     "active(%s)  normal(%s)  pressed(%s)  selected(%s)", strValue.c_str(),
							     skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY, skin->align,
							     skin->event_info, skin->exdata, skin->SkinState[SKIN_STATE_DEACTIVATED].c_str(),
							     skin->SkinState[SKIN_STATE_ACTIVED].c_str(), skin->SkinState[SKIN_STATE_NORMAL].c_str(),
							     skin->SkinState[SKIN_STATE_PRESSED].c_str(),skin->SkinState[SKIN_STATE_SELECTED].c_str());
						}
						else
						    _LOG("\t\t[ERROR] set sub skin  '%s'", strValue.c_str());

					}
				}
			}
		};
        
        //Most preliminary definitions are out of the way.
        //Lets continue with the core block definitions
		if (ini.jumpBlock(true, false)) {
			while (ini.seekNextBlock()) {

				loadSubSkinDefinitions(ini, Name, window, UniqueSerialID);

			}; // while (ini.seekNextBlock()) {
		} // if (ini.jumpBlock(true, false)) {

		if (!window->subSkins.size())
		{
			_LOG("\t\t[ERROR] load sub skin: no sub sub skins defined");
			Skins()->deleteDefinition(Name);
		}

		ini.jumpBlock(false, false); // выпрыгиваем

	} // void AssetManager::loadSkinDefinitions(loadINI & ini)

} // namespace MyGUI {