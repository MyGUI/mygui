#include "MyGUI_AssetManager.h"
#include "MyGUI_Skin.h"
#include "MyGUI_GUI.h"
#include "MyGUI_LoadINI.h"
#include <Ogre.h>
#include <OgreFontManager.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	void getResourcePath(vector<String> & saFilePath, const String & strMaskFileName)
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
		std::map<String, uint32> mapNameValue;
		//---------------------------------------------------------------------------------------------------------------
		#define __REGISTER_VALUE(_value) mapNameValue[#_value] = _value;_LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_EVENT(_value) mapNameValue[#_value] = _value;GUI::getSingleton()->m_mapFlagEvent[#_value] = _value;_LOG("RESISTER_VALUE_EVENT    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_ALIGN(_value) mapNameValue[#_value] = _value;GUI::getSingleton()->m_mapFlagAlign[#_value] = _value;_LOG("RESISTER_VALUE_ALIGN    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_STYLE(_value) mapNameValue[#_value] = _value;GUI::getSingleton()->m_mapFlagStyle[#_value] = _value;_LOG("RESISTER_VALUE_STYLE    %s \t %d (0x%.8X)", #_value, _value, _value);
		//---------------------------------------------------------------------------------------------------------------
		_LOG_NEW; // с красной строки
		
		// регистраци€ имен дополнительных стилей
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
		// регистраци€ имен событий
		__REGISTER_VALUE_EVENT(WE_NONE);
		__REGISTER_VALUE_EVENT(WE_MOUSE_BUTTON);
		__REGISTER_VALUE_EVENT(WE_MOUSE_FOCUS);
		__REGISTER_VALUE_EVENT(WE_MOUSE_MOVE);
		__REGISTER_VALUE_EVENT(WE_KEY_FOCUS);
		__REGISTER_VALUE_EVENT(WE_KEY_BUTTON);
		// регистраци€ имен выравнивани€
		__REGISTER_VALUE_ALIGN(WA_NONE);
		__REGISTER_VALUE_ALIGN(WA_HCENTER);
		__REGISTER_VALUE_ALIGN(WA_VCENTER);
		__REGISTER_VALUE_ALIGN(WA_CENTER);
		__REGISTER_VALUE_ALIGN(WA_CENTER_FOTHER);
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
		__REGISTER_VALUE_ALIGN(WAT_ADD_DOT);
		__REGISTER_VALUE_ALIGN(WAT_CUT_RIGHT_DOT);
		__REGISTER_VALUE_ALIGN(WAT_CUT_LEFT_DOT);
		__REGISTER_VALUE_ALIGN(WAT_MULTI_LINE);
		__REGISTER_VALUE_ALIGN(WAT_BREAK_WORD);
		__REGISTER_VALUE_ALIGN(WAT_SHIFT_TEXT_PRESSED);
		//---------------------------------------------------------------------------------------------------------------
		//---------------------------------------------------------------------------------------------------------------
		// загрузка скинов
		vector<String> saFilePath;
		getResourcePath(saFilePath, "*.mygui_skin");
		for (size_t pos=0; pos < saFilePath.size(); pos++)
		    loadSkinDefinitions(mapNameValue, saFilePath[pos]);
		
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
		
		for (SkinIterator pos = Skins()->begin(); pos != Skins()->end(); ++pos)
        {
            //Check that each skin exists
            
            //Check that the skins are well defined
			if(pos->second->subSkins.size() == 0)
			{
				delete pos->second;
				pos->second = new __tag_MYGUI_SKIN_INFO( *Skins()->getDefinition(SKIN_DEFAULT));
				_LOG("[ERROR] skin is not loaded \"%s\" seting to SKIN_DEFAULT", pos->first.c_str());
			}
		}
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
	void AssetManager::loadSkinDefinitions(std::map<String, uint32> & mapNameValue, const String & strFileName)
	{
		_LOG("\r\nload MyGUI source \t '%s'\r\n", strFileName.c_str());

		#define BLOCK_WINDOW_NAME "windowSkin"
		#define VALUE_WINDOW_FONT "font"
		#define VALUE_WINDOW_COLOUR "colour"
		#define VALUE_WINDOW_DATA1 "data1"
		#define VALUE_WINDOW_DATA2 "data2"
		#define VALUE_WINDOW_DATA3 "data3"
		#define VALUE_WINDOW_DATA4 "data4"
		#define SECTION_SUB_SKIN "subSkin"
		#define SECTION_SUB_SKIN_MAIN "subSkinMain"
		#define VALUE_SKIN_POSITION "position"
		#define VALUE_SKIN_ALIGN "align"
		#define VALUE_SKIN_EVENT "event"
		#define VALUE_SKIN_STYLE "style"
		#define VALUE_SKIN_DEACTIVE "skin_deactive"
		#define VALUE_SKIN_NORMAL "skin_normal"
		#define VALUE_SKIN_ACTIVE "skin_active"
		#define VALUE_SKIN_PRESSED "skin_pressed"
		#define VALUE_SKIN_SELECTED "skin_selected"

		#define BLOCK_FONT_NAME "Font"
		#define VALUE_FONT_NAME "name"
		#define VALUE_FONT_SIZE "size"

		#define BLOCK_POINTER_NAME "Pointer"
		#define VALUE_POINTER_SIZE "size"
		#define VALUE_POINTER_OFFSET "offset"
		#define VALUE_POINTER_SKIN "skin"

		map <String, __LP_MYGUI_SUBSKIN_INFO> mapLoadingSkin; // именна именнованых уже загруженных скинов

		loadINI ini;
		if (!ini.open(strFileName.c_str())) return;

		while (ini.seekNextBlock()) {

            //Defines a new skin
			if (ini.getBlockType() == BLOCK_WINDOW_NAME) {
				
				unsigned int UniqueSerialID = 0; //used to identify unnamed sub skins
				const String Name = ini.getBlockName();
				__tag_MYGUI_SKIN_INFO * window = Skins()->defineNew(Name);
				
				_LOG("\r\n\tcreate window info \t %s  (%p)", ini.getBlockName().c_str(), window);

				uint32 uValue;
				size_t pos;
				String strValueName, strValue;
				// дефолтные значени€
				window->fontWindow = FONT_DEFAULT;
				window->colour = ColourValue::Black;

				while (ini.LoadNextValue()) {
					if (ini.getValueName(strValueName)) {
						if (strValueName == VALUE_WINDOW_FONT) {
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
								const __tag_MYGUI_SUBSKIN_INFO *skin = SubSkins()->getDefinition(strValue);

								if (skin)
								{
									 // вставл€ем в начало
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
									     skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(),
									     skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(),
									     skin->pStrSkinPressed.c_str(),skin->pStrSkinSelected.c_str());
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
					    const String strBlockType = ini.getBlockType();
                        
                        //Invent a unique name for this sub skin if there isn't one
                        const String strBlockName = (ini.getBlockName() == "") ?
                            (Name + ++UniqueSerialID) :
                            (ini.getBlockName());
						
						__tag_MYGUI_SUBSKIN_INFO *skin = SubSkins()->defineNew(strBlockName);						

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
										skin->align |= (uint32)mapNameValue[strValue];
										pos ++;
									};
								} else if (strValueName == VALUE_SKIN_EVENT) {
									pos = 0;
									while (ini.getValue(strValue, pos)) {
										skin->event_info |= (uint32)mapNameValue[strValue];
										pos ++;
									};
								} else if (strValueName == VALUE_SKIN_STYLE) {
									pos = 0;
									while (ini.getValue(strValue, pos)) {
										skin->exdata |= (uint32)mapNameValue[strValue];
										pos ++;
									};
								} else if (strValueName == VALUE_SKIN_DEACTIVE) {
									if (ini.getValue(strValue)) skin->pStrSkinDeactived = strValue;
								} else if (strValueName == VALUE_SKIN_NORMAL) {
									if (ini.getValue(strValue)) skin->pStrSkinNormal = strValue;
								} else if (strValueName == VALUE_SKIN_ACTIVE) {
									if (ini.getValue(strValue)) skin->pStrSkinActived = strValue;
								} else if (strValueName == VALUE_SKIN_PRESSED) {
									if (ini.getValue(strValue)) skin->pStrSkinPressed = strValue;
								} else if (strValueName == VALUE_SKIN_SELECTED) {
									if (ini.getValue(strValue)) skin->pStrSkinSelected = strValue;
								}
							}
						};

						if (strBlockType == SECTION_SUB_SKIN)
						    window->subSkins.push_back(skin);
						else if (strBlockType == SECTION_SUB_SKIN_MAIN)
						{
							if (window->subSkins.size()) { // вставл€ем в начало
								window->subSkins.push_back(window->subSkins[0]);
								window->subSkins[0] = skin;
							} else  window->subSkins.push_back(skin);
						}else {
							_LOG("\t\t[ERROR] unknow section type '%s'", strBlockType.c_str());
							delete skin;
							skin = 0;
						}

						if (skin) { // все хорошо
							_LOG("\t\tadd sub skin  '%s'  (%p) [%d, %d, %d, %d]   "
							     "align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  deactive(%s)  "
							     "active(%s)  normal(%s)  pressed(%s)  selected(%s)", strBlockName.c_str(),
							     skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY, skin->align,
							     skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(),
							     skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(),
							     skin->pStrSkinPressed.c_str(), skin->pStrSkinSelected.c_str());
							
							if (!strBlockName.empty()) { // у этого блока есть им€, сохран€ем
								//TODO: port
								mapLoadingSkin[strBlockName] = skin;
								_LOG("\t\t  store sub skin  '%s'  (%p)", strBlockName.c_str(), skin);
							}
						}

					};
				}

				if (!window->subSkins.size())
				{
					_LOG("\t\t[ERROR] load sub skin: no sub sub skins defined");
					Skins()->deleteDefinition(Name);
				}

				ini.jumpBlock(false, false); // выпрыгиваем

			// загрузка шрифтов окон -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_FONT_NAME) {
                
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
				if (font->font) { // им€ есть
					font->font->load();
					font->spaceWidth = font->font->getGlyphAspectRatio(0x0030/*UNICODE_ZERO*/) * (Real)font->height;
					font->sizeTripleDot = (font->font->getGlyphAspectRatio( '.' ) * 3.0 ) * (Real)font->height;
					_LOG("\tload font \t %s    '%s' %d", ini.getBlockName().c_str(), font->name.c_str(), font->height);
                } else { // неудачна€ загрузка
					_LOG("\t[ERROR] load font \t %s", ini.getBlockName().c_str());
					Fonts()->deleteDefinition(Name);
				}

			// загрузка указателей -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_POINTER_NAME) {
                
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
			// а это что за хрень -------------------------------------------------------------------------------------------------
			} else { _LOG("\r\n\t[ERROR] unknow block type  -   type(%s),  name(%s)",
			    ini.getBlockType().c_str(), ini.getBlockName().c_str()); }
		};

		ini.close();
		_LOG_NEW;
	}
}