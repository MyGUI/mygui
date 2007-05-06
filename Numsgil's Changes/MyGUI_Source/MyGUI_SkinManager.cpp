#include "MyGUI_SkinManager.h"
#include "MyGUI_LoadINI.h"
#include "MyGUI_GUI.h"
#include <OgreArchive.h>
#include <OgreResourceManager.h>
#include <OgreFontManager.h>
#include <OgreMaterialManager.h>

using namespace Ogre;
using namespace std;

namespace MyGUI {

	void getResourcePath(vector<String> & saFilePath, const String & strMaskFileName)
	{
		saFilePath.clear();
		String strPath;
		FileInfoListPtr pFileInfo = ResourceGroupManager::getSingleton().findResourceFileInfo(ResourceGroupManager::getSingleton().DEFAULT_RESOURCE_GROUP_NAME, strMaskFileName);
		for (FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); fi++ ) {
			strPath = fi->archive->getName();
			strPath += "\\";
			strPath += fi->filename;
			saFilePath.push_back(strPath);
		}
	}

	void SkinManager::createSkin() // загружает все скины
	{
		std::map<String, uint32> mapNameValue;
		
		#define __REGISTER_VALUE(_value) mapNameValue[#_value] = _value; _LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_SKIN_VALUE(_value) mapNameValue[#_value] = _value; _LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);g_strSkinNames[_value]=#_value;
		
		_LOG_NEW; // с красной строки
		
		// регистрация имен цветов
		__REGISTER_VALUE(COLOUR_BLACK);
		__REGISTER_VALUE(COLOUR_WHITE);
		__REGISTER_VALUE(COLOUR_BLUE);
		__REGISTER_VALUE(COLOUR_GREEN);
		__REGISTER_VALUE(COLOUR_RED);
		// регистрация имен дополнительных стилей
		__REGISTER_VALUE(WES_NONE);
		__REGISTER_VALUE(WES_TEXT);
		__REGISTER_VALUE(WES_CLIENT);
		__REGISTER_VALUE(WES_CAPTION);
		__REGISTER_VALUE(WES_RESIZE);
		__REGISTER_VALUE(WES_CLOSE);
		__REGISTER_VALUE(WES_BUTTON);
		__REGISTER_VALUE(WES_EDIT);
		__REGISTER_VALUE(WES_EDIT_CURSOR);
		__REGISTER_VALUE(WES_VSCROLL_UP);
		__REGISTER_VALUE(WES_VSCROLL_DOWN);
		__REGISTER_VALUE(WES_VSCROLL_TRACK);
		__REGISTER_VALUE(WES_TAB_TOP);
		__REGISTER_VALUE(WES_LIST_HIDE_SCROLL);
		__REGISTER_VALUE(WES_COMBO);
		__REGISTER_VALUE(WES_COMBO_BUTTON);
		// регистрация имен событий
		__REGISTER_VALUE(WE_NONE);
		__REGISTER_VALUE(WE_MOUSE_BUTTON);
		__REGISTER_VALUE(WE_MOUSE_FOCUS);
		__REGISTER_VALUE(WE_MOUSE_MOVE);
		__REGISTER_VALUE(WE_KEY_FOCUS);
		__REGISTER_VALUE(WE_KEY_BUTTON);
		// регистрация имен выравнивания
		__REGISTER_VALUE(WA_NONE);
		__REGISTER_VALUE(WA_HCENTER);
		__REGISTER_VALUE(WA_VCENTER);
		__REGISTER_VALUE(WA_CENTER);
		__REGISTER_VALUE(WA_CENTER_Parent);
		__REGISTER_VALUE(WA_LEFT);
		__REGISTER_VALUE(WA_RIGHT);
		__REGISTER_VALUE(WA_HSTRETCH);
		__REGISTER_VALUE(WA_TOP);
		__REGISTER_VALUE(WA_BOTTOM);
		__REGISTER_VALUE(WA_VSTRETCH);
		__REGISTER_VALUE(WA_STRETCH);
		__REGISTER_VALUE(WAT_NONE);
		__REGISTER_VALUE(WAT_LEFT);
		__REGISTER_VALUE(WAT_RIGHT);
		__REGISTER_VALUE(WAT_HCENTER);
		__REGISTER_VALUE(WAT_TOP);
		__REGISTER_VALUE(WAT_BOTTOM);
		__REGISTER_VALUE(WAT_VCENTER);
		__REGISTER_VALUE(WAT_CENTER);
		__REGISTER_VALUE(WAT_CUT_RIGHT);
		__REGISTER_VALUE(WAT_CUT_LEFT);
		__REGISTER_VALUE(WAT_ADD_DOT);
		__REGISTER_VALUE(WAT_CUT_RIGHT_DOT);
		__REGISTER_VALUE(WAT_CUT_LEFT_DOT);
		__REGISTER_VALUE(WAT_MULTI_LINE);
		__REGISTER_VALUE(WAT_BREAK_WORD);
		__REGISTER_VALUE(WAT_SHIFT_TEXT_PRESSED);
		
		//Find and load GUI definition files
		vector<String> saFilePath;
		getResourcePath(saFilePath, "*.mygui_skin");
		for (uint16 pos=0; pos<saFilePath.size(); pos++)
		    loadSkin(mapNameValue, saFilePath[pos]);
		
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот шрифтов
		getFont(FONT_DEFAULT); //assert that the default font exists
		for (size_t pos = 0; pos < getFontCount(); pos++) {
			if (mFontNames[pos] == FONT_DEFAULT)
			    continue;
			if (!mFonts[pos]) {
				mFonts[pos] = getFont(FONT_DEFAULT);
				_LOG("[ERROR] font is not load (%d)   set FONT_DEFAULT", pos);
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот курсоров
		if (isPointerDefined(POINTER_DEFAULT) >= 0)
		    defineNewPointer(POINTER_DEFAULT);
		
		for (size_t pos=0; pos < getPointerCount(); pos++) {
		    if(mPointers[pos] == getPointer(POINTER_DEFAULT))
		        continue;
			if (!mPointers[pos]) {
				mPointers[pos] = getPointer(POINTER_DEFAULT);
				_LOG("[ERROR] pointer is not load (%d)   set POINTER_DEFAULT", pos);
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот скинов
		__ASSERT(getSkin(SKIN_DEFAULT));
		__ASSERT(getSkin(SKIN_DEFAULT)->subSkins.size());
		for (size_t pos=0; pos < getSkinCount(); pos++) {
			if (!mSkins[pos]) {
			    mSkins[pos] = getSkin(SKIN_DEFAULT);
				_LOG("[ERROR] skin is not load   %s   [%d]   set SKIN_DEFAULT", mSkinNames[pos].c_str(), pos);
			} else {
				if (!mSkins[pos]->subSkins.size()) {
					delete mSkins[pos]; // какой то пустой скин
					mSkins[pos] = getSkin(SKIN_DEFAULT);
					_LOG("[ERROR] skin is not load   %s   [%d]   set SKIN_DEFAULT", mSkinNames[pos].c_str(), pos);
				}
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
	}

	void SkinManager::unloadResource() // выгружает скины
	{
		_LOG_NEW;
		// шрифты
		for (size_t pos=0; pos < getFontCount(); pos++) { // все
			if (getFont(FONT_DEFAULT) != mFonts[pos]) { // удаляем если не похож на дефолтный
				_LOG("delete font (%p) [%d]", mFonts[pos], pos);
				delete mFonts[pos];
				mFonts[pos] = getFont(FONT_DEFAULT);
			}
		}
		
		_LOG("delete font (%p) [%d]", getFont(FONT_DEFAULT), FONT_DEFAULT);
		delete getFont(FONT_DEFAULT); // удаляем дефолтный
		mFonts.clear();

		_LOG_NEW;
		// указатели
		for (size_t pos=0; pos < mPointers.size(); pos++) { // все
			if (getPointer(POINTER_DEFAULT) != mPointers[pos]) { // удаляем если не похож на дефолтный
				_LOG("delete pointer (%p) [%d]", mPointers[pos], pos);
				delete mPointers[pos];
			}
		}
		_LOG("delete pointer (%p) [%d]", getPointer(POINTER_DEFAULT), POINTER_DEFAULT);
		delete getPointer(POINTER_DEFAULT); // удаляем дефолтный
		mPointers.clear();

		_LOG_NEW;
		// скины
		for (size_t pos=0; pos < mSkins.size(); pos++) {
			__LP_MYGUI_WINDOW_INFO pSkinWindow = mSkins[pos]; // указатели на окна скинов
			if (!pSkinWindow)
			    continue;

			if (pSkinWindow == getSkin(SKIN_DEFAULT)) { // дефолтный скин, убираем все упоминания
				for (size_t find_pos = 0; find_pos < mSkins.size(); find_pos++) {
					if (pSkinWindow == mSkins[find_pos])
					 	mSkins[find_pos] = 0;
				}
			}

			_LOG("\r\ndelete window info %s (%p) [%d]", mSkinNames[pos].c_str(), pSkinWindow, pos);

			for (size_t sub=0; sub < pSkinWindow->subSkins.size(); sub++) { // детишки
				clearSkins(pSkinWindow->subSkins[sub]);
				pSkinWindow->subSkins[sub] = 0;
			}

			delete pSkinWindow; // удаляем если не похож на дефолтный
			mSkins[pos] = 0;

		}

		mSkins.clear();

	}

	void SkinManager::clearSkins(__LP_MYGUI_SKIN_INFO pSkin) // удаляет скин
	{
		if (!pSkin) return;
		_LOG("\tdelete sub skin (%p)", pSkin);
		delete pSkin; // удаляем скин
		// ищем тот же скин и присваиваем ему 0
		for (size_t pos=0; pos < mSkins.size(); pos++) { // все
			__LP_MYGUI_WINDOW_INFO pSkinWindow = mSkins[pos]; // указатели на окна скинов
			if (!pSkinWindow) continue;
			for (uint8 sub=0; sub<pSkinWindow->subSkins.size(); sub++) {
				if (pSkinWindow->subSkins[sub] == pSkin) pSkinWindow->subSkins[sub] = 0;
			}
		}
	}
	
	void SkinManager::loadSkin(std::map<String, uint32> & mapNameValue, const String & strFileName) // загружает все скины окон из одного файла
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
		#define VALUE_SKIN_ALIGN "aligin"
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

		map <String, __LP_MYGUI_SKIN_INFO> mapLoadingSkin; // именна именнованых уже загруженных скинов

		loadINI ini;
		if (!ini.open(strFileName.c_str())) return;

		while (ini.seekNextBlock()) {

			// загрузка скинов окон -------------------------------------------------------------------------------------------------
			if (ini.getBlockType() == BLOCK_WINDOW_NAME) {
				__LP_MYGUI_WINDOW_INFO window = defineNewSkin(ini.getBlockName());
				_LOG("\r\n\tcreate window info \t %s  (%p)", ini.getBlockName().c_str(), window);

				uint32 uValue;
				size_t pos;
				String strValueName, strValue;
				// дефолтные значения
				window->fontWindow = getFont(FONT_DEFAULT);
				window->colour = COLOUR_BLACK;

				while (ini.LoadNextValue()) {
					if (ini.getValueName(strValueName)) {
						if (strValueName == VALUE_WINDOW_FONT) {
							if (ini.getValue(strValue)) window->fontWindow = getFont(strValue);
						} else if (strValueName == VALUE_WINDOW_COLOUR) {
							if (!ini.getValue(uValue = 0)) {
							    if (ini.getValue(strValue))
							        uValue = (uint32)mapNameValue[strValue];}
							window->colour = uValue;
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
						// уже загруженный скин просто присваиваем указатель
						} else if ((strValueName == SECTION_SUB_SKIN) || (strValueName == SECTION_SUB_SKIN_MAIN)) {

							if (ini.getValue(strValue)) {
								__LP_MYGUI_SKIN_INFO skin = 0;
								try {skin = mapLoadingSkin[strValue];} catch (...) {}

								if (skin) {
									 // вставляем в начало
									if ((window->subSkins.size()) && (strValueName == SECTION_SUB_SKIN_MAIN)) {
										window->subSkins.push_back(window->subSkins[0]);
										window->subSkins[0] = skin;
									} else  window->subSkins.push_back(skin);
									_LOG("\t\tset sub skin  '%s'  (%p) [%d, %d, %d, %d]   "
									     "align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  "
									     "deactive(%s)  active(%s)  normal(%s)  pressed(%s)"
									     "  selected(%s)", strValue.c_str(), skin, skin->posX,
									     skin->posY, skin->sizeX, skin->sizeY, skin->align,
									     skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(),
									     skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(),
									     skin->pStrSkinPressed.c_str(), skin->pStrSkinSelected.c_str());
								} else 
								    _LOG("\t\t[ERROR] set sub skin  '%s'", strValue.c_str());

							}
						}
					}
				};

				if (ini.jampBlock(true, false)) {
					while (ini.seekNextBlock()) {

						__LP_MYGUI_SKIN_INFO skin = new __tag_MYGUI_SKIN_INFO;

						String strBlockType = ini.getBlockType();
						String strBlockName = ini.getBlockName();

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
										skin->align |= mapNameValue[strValue];
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

						if (strBlockType == SECTION_SUB_SKIN) window->subSkins.push_back(skin);
						else if (strBlockType == SECTION_SUB_SKIN_MAIN) {
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
							_LOG("\t\tadd sub skin  '%s'  (%p) [%d, %d, %d, %d]   align(0x%.8X)  event(0x%.8X):"
							     "style(0x%.8X)  deactive(%s)  active(%s)  normal(%s)  pressed(%s)  selected(%s)",
							     strBlockName.c_str(), skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY,
							     skin->align, skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(),
							     skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(),
							     skin->pStrSkinPressed.c_str(), skin->pStrSkinSelected.c_str());
							if (!strBlockName.empty()) { // у этого блока есть имя, сохраняем
								mapLoadingSkin[strBlockName] = skin;
								_LOG("\t\t  store sub skin  '%s'  (%p)", strBlockName.c_str(), skin);
							}
						}

					};
				}

				//The skin was successfully defined, with subskins and everything
				if (window->subSkins.size()) {
				    
				} else { // неполучилось
					assert(!"Skin unsuccessfully created.  Not handled in code yet.");
					_LOG("\t\t[ERROR] load sub skin");
					delete window;
					//To do: we'll need to remove this from the SkinManager's lists as well
				}

				ini.jampBlock(false, false); // выпрыгиваем

			// загрузка шрифтов окон -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_FONT_NAME) {

				__LP_MYGUI_FONT_INFO font = defineNewFont(ini.getBlockName());

				while (ini.LoadNextValue()) {
					String strValueName, strValue;
					uint32 uValue;
					if (ini.getValueName(strValueName)) {
						if (strValueName == VALUE_FONT_NAME) {
							if (ini.getValue(strValue)) font->name = strValue;
						} else if (strValueName == VALUE_FONT_SIZE) {
							if (ini.getValue(uValue)) font->height = uValue;
						}
					}
				};

				font->font = (Font *)FontManager::getSingleton().getByName(font->name).getPointer();
				if (font->font) { // имя есть
					font->font->load();
					font->spaceWidth = font->font->getGlyphAspectRatio(0x0030/*UNICODE_ZERO*/) * (Real)font->height;
					font->sizeTripleDot = (font->font->getGlyphAspectRatio( '.' ) * 3.0 ) * (Real)font->height;
					_LOG("\tload font \t %s    '%s' %d", ini.getBlockName().c_str(), font->name.c_str(), font->height);
				} else { // неудачная загрузка
					_LOG("\t[ERROR] load font \t %s", ini.getBlockName().c_str());
					delete font;
					//To do: we'll need to remove this from the SkinManager's lists as well
				}

			// загрузка указателей -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_POINTER_NAME) {

				__LP_MYGUI_POINTER_INFO pointer = defineNewPointer(ini.getBlockName());

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
					delete pointer;
					//To do: we'll need to remove this from the SkinManager's lists as well
				}
				
			// а это что за хрень -------------------------------------------------------------------------------------------------
			} else { _LOG("\r\n\t[ERROR] unknow block type  -   type(%s),  name(%s)",
			              ini.getBlockType().c_str(), ini.getBlockName().c_str()); }


		};

		ini.close();
		_LOG_NEW;
	}
	        	
	__LP_MYGUI_WINDOW_INFO SkinManager::getSkin(const Ogre::String &SkinName)
    {
        for(size_t x = 0; x < mSkinNames.size(); ++x)
            if(!strcmp(mSkinNames[x].c_str(), SkinName.c_str()))
                return mSkins[x];
        //skin undefined
        __ASSERT(!"Attempting to use an undefined skin.");
        return NULL;
    }
    
    __LP_MYGUI_FONT_INFO SkinManager::getFont(const Ogre::String &FontName)
    {
        for(size_t x = 0; x < mFontNames.size(); ++x)
            if(!strcmp(mFontNames[x].c_str(), FontName.c_str()))
                return mFonts[x];
        //skin undefined
        __ASSERT(!"Attempting to use an undefined font.");
        return NULL;    
    }
    __LP_MYGUI_POINTER_INFO SkinManager::getPointer(const Ogre::String &PointerName)
    {
        for(size_t x = 0; x < mPointerNames.size(); ++x)
            if(!strcmp(mPointerNames[x].c_str(), PointerName.c_str()))
                return mPointers[x];
        //skin undefined
        __ASSERT(!"Attempting to use an undefined mouse pointer.");
        return NULL;    
    }
    
    __LP_MYGUI_WINDOW_INFO SkinManager::defineNewSkin(const Ogre::String &Name)
	{
	    //Check to see if this is already a defined name
	    if(isSkinDefined(Name) >= 0)
	        return getSkin(Name);
	    
	    mSkinNames.push_back(Name);
	    mSkins.push_back(new __tag_MYGUI_WINDOW_INFO);
	    return mSkins.back();
	}
	
    __LP_MYGUI_FONT_INFO SkinManager::defineNewFont(const Ogre::String &FontName)    
    {
	    //Check to see if this is already a defined name
	    if(isFontDefined(FontName) >= 0)
	        return getFont(FontName);
	    
	    mFontNames.push_back(FontName);
	    mFonts.push_back(new __tag_MYGUI_FONT_INFO);
	    return mFonts.back();
	}
	
    __LP_MYGUI_POINTER_INFO SkinManager::defineNewPointer(const Ogre::String &PointerName)
    {
	    //Check to see if this is already a defined name
	    if(isPointerDefined(PointerName) >= 0)
	        return getPointer(PointerName);
	    
	    mPointerNames.push_back(PointerName);
	    mPointers.push_back(new __tag_MYGUI_POINTER_INFO);
	    return mPointers.back();
	}
    
    const std::vector<Ogre::String> SkinManager::getSkinNames()
    {
        return mSkinNames;
    }
    
    const std::vector<Ogre::String> SkinManager::getFontNames()
    {
        return mFontNames;
    }
    
    const std::vector<Ogre::String> SkinManager::getPointerNames()
    {
        return mPointerNames;
    }
    
    int SkinManager::isSkinDefined(const Ogre::String &Name)
    {
        for(size_t iter = 0; iter < this->mSkinNames.size(); ++iter)
	    {
	        if(!strcmp(mSkinNames[iter].c_str(), Name.c_str()))
	            return (int)iter;
	    }
	    
	    return -1;
    }
    
    int SkinManager::isFontDefined(const Ogre::String &Name)
    {
        for(size_t iter = 0; iter < this->mFontNames.size(); ++iter)
	    {
	        if(!strcmp(mFontNames[iter].c_str(), Name.c_str()))
	            return (int)iter;
	    }
	    
	    return -1;
    }
    
    int SkinManager::isPointerDefined(const Ogre::String &Name)
    {
        for(size_t iter = 0; iter < this->mPointerNames.size(); ++iter)
	    {
	        if(!strcmp(mPointerNames[iter].c_str(), Name.c_str()))
	            return (int)iter;
	    }
	    
	    return -1;
    }   
}