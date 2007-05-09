#include "MyGUI_Skin.h"
#include "MyGUI_LoadINI.h"
#include "MyGUI_GUI.h"

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

	void GUI::createSkin() // загружает все скины
	{
		//---------------------------------------------------------------------------------------------------------------
		m_fontInfo.resize(__FONT_COUNT); // размер под все шрифты
		m_pointerInfo.resize(__POINTER_COUNT); // размер под все скины курсоров
		m_windowInfo.resize(__SKIN_COUNT); // размер под все скины окон

		m_strSkinNames.resize(__SKIN_COUNT); // размер под все имена скинов
		m_strFontNames.resize(__FONT_COUNT); // размер под все имена шрифтов
		//---------------------------------------------------------------------------------------------------------------
		std::map<String, uint32> mapNameValue;
		//---------------------------------------------------------------------------------------------------------------
		#define __REGISTER_VALUE(_value) mapNameValue[#_value] = _value;_LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_EVENT(_value) mapNameValue[#_value] = _value;m_mapFlagEvent[#_value] = _value;_LOG("RESISTER_VALUE_EVENT    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_ALIGN(_value) mapNameValue[#_value] = _value;m_mapFlagAlign[#_value] = _value;_LOG("RESISTER_VALUE_ALIGN    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_VALUE_STYLE(_value) mapNameValue[#_value] = _value;m_mapFlagStyle[#_value] = _value;_LOG("RESISTER_VALUE_STYLE    %s \t %d (0x%.8X)", #_value, _value, _value);
		#define __REGISTER_SKIN_VALUE(_value) mapNameValue[#_value] = _value;_LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);m_strSkinNames[_value]=#_value;
		#define __REGISTER_FONT_VALUE(_value) mapNameValue[#_value] = _value;_LOG("RESISTER_VALUE    %s \t %d (0x%.8X)", #_value, _value, _value);m_strFontNames[_value]=#_value;
		//---------------------------------------------------------------------------------------------------------------
		_LOG_NEW; // с красной строки
		//---------------------------------------------------------------------------------------------------------------
		// регистрация имен скинов
		__REGISTER_SKIN_VALUE(SKIN_DEFAULT);
		__REGISTER_SKIN_VALUE(SKIN_FADE);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_C);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_S);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_X);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_CS);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_CX);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_SX);
		__REGISTER_SKIN_VALUE(SKIN_WINDOWFRAME_CSX);
		__REGISTER_SKIN_VALUE(SKIN_BUTTON);
		__REGISTER_SKIN_VALUE(SKIN_CHECK_BOX);
		__REGISTER_SKIN_VALUE(SKIN_EDIT);
		__REGISTER_SKIN_VALUE(SKIN_VSCROLL);
		__REGISTER_SKIN_VALUE(SKIN_TAB);
		__REGISTER_SKIN_VALUE(SKIN_TAB_BUTTON);
		__REGISTER_SKIN_VALUE(SKIN_STATIC_TEXT);
		__REGISTER_SKIN_VALUE(SKIN_LIST_S);
		__REGISTER_SKIN_VALUE(SKIN_LIST_BUTTON);
		__REGISTER_SKIN_VALUE(SKIN_COMBO_BOX_EDIT);
		__REGISTER_SKIN_VALUE(SKIN_COMBO_BOX);
		__REGISTER_SKIN_VALUE(SKIN_DROP_LIST);
		//---------------------------------------------------------------------------------------------------------------
		// регистрация имен шрифтов
		__REGISTER_FONT_VALUE(FONT_DEFAULT);
		__REGISTER_FONT_VALUE(FONT_LARGE);
		// регистрация имен указателей
		__REGISTER_VALUE(POINTER_DEFAULT);
		__REGISTER_VALUE(POINTER_RESIZE);
		__REGISTER_VALUE(POINTER_TEXT);
		// регистрация имен цветов
		__REGISTER_VALUE(COLOUR_BLACK);
		__REGISTER_VALUE(COLOUR_WHITE);
		__REGISTER_VALUE(COLOUR_BLUE);
		__REGISTER_VALUE(COLOUR_GREEN);
		__REGISTER_VALUE(COLOUR_RED);
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
		for (uint16 pos=0; pos<saFilePath.size(); pos++) loadSkin(mapNameValue, saFilePath[pos]);
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот шрифтов
		__ASSERT(m_fontInfo[FONT_DEFAULT]);
		for (uint8 pos=0; pos<__FONT_COUNT; pos++) {
			if (pos == FONT_DEFAULT) continue;
			if (!m_fontInfo[pos]) {
				m_fontInfo[pos] = m_fontInfo[FONT_DEFAULT];
				_LOG("[ERROR] font is not load (%d)   set FONT_DEFAULT", pos);
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот курсоров
		if (!m_pointerInfo[POINTER_DEFAULT]) m_pointerInfo[POINTER_DEFAULT] = new __tag_MYGUI_POINTER_INFO;
		for (uint8 pos=0; pos<__POINTER_COUNT; pos++) {
			if (pos == POINTER_DEFAULT) continue;
			if (!m_pointerInfo[pos]) {
				m_pointerInfo[pos] = m_pointerInfo[POINTER_DEFAULT];
				_LOG("[ERROR] pointer is not load (%d)   set POINTER_DEFAULT", pos);
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
		// проверка и заполнение пустот скинов
		__ASSERT(m_windowInfo[SKIN_DEFAULT]);
		__ASSERT(m_windowInfo[SKIN_DEFAULT]->subSkins.size());
		for (uint8 pos=0; pos<__SKIN_COUNT; pos++) {
			if (!m_windowInfo[pos]) {
				m_windowInfo[pos] = m_windowInfo[SKIN_DEFAULT];
				_LOG("[ERROR] skin is not load   %s   [%d]   set SKIN_DEFAULT", m_strSkinNames[pos].c_str(), pos);
			} else {
				if (!m_windowInfo[pos]->subSkins.size()) {
					delete m_windowInfo[pos]; // какой то пустой скин
					m_windowInfo[pos] = m_windowInfo[SKIN_DEFAULT];
					_LOG("[ERROR] skin is not load   %s   [%d]   set SKIN_DEFAULT", m_strSkinNames[pos].c_str(), pos);
				}
			}
		}
		_LOG_NEW;
		//---------------------------------------------------------------------------------------------------------------
	}

	void GUI::unloadResource() // выгружает скины
	{
		_LOG_NEW;
		// шрифты
		for (uint16 pos=0; pos<m_fontInfo.size(); pos++) { // все
			if (m_fontInfo[FONT_DEFAULT] != m_fontInfo[pos]) { // удаляем если не похож на дефолтный
				_LOG("delete font (%p) [%d]", m_fontInfo[pos], pos);
				delete m_fontInfo[pos];
			}
		}
		_LOG("delete font (%p) [%d]", m_fontInfo[FONT_DEFAULT], FONT_DEFAULT);
		delete m_fontInfo[FONT_DEFAULT]; // удаляем дефолтный
		m_fontInfo.clear();

		_LOG_NEW;
		// указатели
		for (uint16 pos=0; pos<m_pointerInfo.size(); pos++) { // все
			if (m_pointerInfo[POINTER_DEFAULT] != m_pointerInfo[pos]) { // удаляем если не похож на дефолтный
				_LOG("delete pointer (%p) [%d]", m_pointerInfo[pos], pos);
				delete m_pointerInfo[pos];
			}
		}
		_LOG("delete pointer (%p) [%d]", m_pointerInfo[POINTER_DEFAULT], POINTER_DEFAULT);
		delete m_pointerInfo[POINTER_DEFAULT]; // удаляем дефолтный
		m_pointerInfo.clear();

		_LOG_NEW;
		// скины
		for (uint16 pos=0; pos<m_windowInfo.size(); pos++) {
			__LP_MYGUI_WINDOW_INFO pSkinWindow = m_windowInfo[pos]; // указатели на окна скинов
			if (!pSkinWindow) continue;

			if (pSkinWindow == m_windowInfo[SKIN_DEFAULT]) { // дефолтный скин, убираем все упоминания
				for (uint16 find_pos=0; find_pos<m_windowInfo.size(); find_pos++) {
					if (pSkinWindow == m_windowInfo[find_pos]) 	m_windowInfo[find_pos] = 0;
				}
			}

			_LOG("\r\ndelete window info %s (%p) [%d]", m_strSkinNames[pos].c_str(), pSkinWindow, pos);

			for (uint16 sub=0; sub<pSkinWindow->subSkins.size(); sub++) { // детишки
				clearSkins(pSkinWindow->subSkins[sub]);
				pSkinWindow->subSkins[sub] = 0;
			}

			delete pSkinWindow; // удаляем если не похож на дефолтный
			m_windowInfo[pos] = 0;

		}

		m_windowInfo.clear();

	}

	void GUI::clearSkins(__LP_MYGUI_SKIN_INFO pSkin) // удаляет скин
	{
		if (!pSkin) return;
		_LOG("\tdelete sub skin (%p)", pSkin);
		delete pSkin; // удаляем скин
		// ищем тот же скин и присваиваем ему 0
		for (uint16 pos=0; pos<__SKIN_COUNT; pos++) { // все
			__LP_MYGUI_WINDOW_INFO pSkinWindow = m_windowInfo[pos]; // указатели на окна скинов
			if (!pSkinWindow) continue;
			for (uint8 sub=0; sub<pSkinWindow->subSkins.size(); sub++) {
				if (pSkinWindow->subSkins[sub] == pSkin) pSkinWindow->subSkins[sub] = 0;
			}
		}
	}

	void GUI::loadSkin(std::map<String, uint32> & mapNameValue, const String & strFileName) // загружает все скины окон из одного файла
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

		map <String, __LP_MYGUI_SKIN_INFO> mapLoadingSkin; // именна именнованых уже загруженных скинов

		loadINI ini;
		if (!ini.open(strFileName.c_str())) return;

		while (ini.seekNextBlock()) {

			// загрузка скинов окон -------------------------------------------------------------------------------------------------
			if (ini.getBlockType() == BLOCK_WINDOW_NAME) {
				
				uint32 ID = (uint32)mapNameValue[ini.getBlockName()];
				if (ID >= __SKIN_COUNT) {_LOG("\t\t[ERROR] load skin  (ID >= __SKIN_COUNT)");continue;}

				__LP_MYGUI_WINDOW_INFO window = new __tag_MYGUI_WINDOW_INFO;
				_LOG("\r\n\tcreate window info \t %s  (%p) [%d]", ini.getBlockName().c_str(), window, ID);

				uint32 uValue, pos;
				String strValueName, strValue;
				// дефолтные значения
				window->fontWindow = m_fontInfo[FONT_DEFAULT];
				window->colour = COLOUR_BLACK;

				while (ini.LoadNextValue()) {
					if (ini.getValueName(strValueName)) {
						if (strValueName == VALUE_WINDOW_FONT) {
							if (ini.getValue(strValue)) window->fontWindow = m_fontInfo[mapNameValue[strValue]];
						} else if (strValueName == VALUE_WINDOW_COLOUR) {
							if (!ini.getValue(uValue = 0)) {if (ini.getValue(strValue)) uValue = (uint32)mapNameValue[strValue];}
							window->colour = uValue;
						} else if (strValueName == VALUE_WINDOW_DATA1) {
							if (!ini.getValue(uValue = 0)) {if (ini.getValue(strValue)) uValue = (uint32)mapNameValue[strValue];}
							window->dataWindow += ((uValue & 0x000000FF) << 24);
						} else if (strValueName == VALUE_WINDOW_DATA2) {
							if (!ini.getValue(uValue = 0)) {if (ini.getValue(strValue)) uValue = (uint32)mapNameValue[strValue];}
							window->dataWindow += ((uValue & 0x000000FF) << 16);
						} else if (strValueName == VALUE_WINDOW_DATA3) {
							if (!ini.getValue(uValue = 0)) {if (ini.getValue(strValue)) uValue = (uint32)mapNameValue[strValue];}
							window->dataWindow += ((uValue & 0x000000FF) << 8);
						} else if (strValueName == VALUE_WINDOW_DATA4) {
							if (!ini.getValue(uValue = 0)) {if (ini.getValue(strValue)) uValue = (uint32)mapNameValue[strValue];}
							window->dataWindow += (uValue & 0x000000FF);
						} else if ((strValueName == SECTION_SUB_SKIN) || (strValueName == SECTION_SUB_SKIN_MAIN)) { // уже загруженный скин просто присваиваем указатель

							if (ini.getValue(strValue)) {
								__LP_MYGUI_SKIN_INFO skin = 0;
								try {skin = mapLoadingSkin[strValue];} catch (...) {}

								if (skin) {
									if ((window->subSkins.size()) && (strValueName == SECTION_SUB_SKIN_MAIN)) { // вставляем в начало
										window->subSkins.push_back(window->subSkins[0]);
										window->subSkins[0] = skin;
									} else  window->subSkins.push_back(skin);
									_LOG("\t\tset sub skin  '%s'  (%p) [%d, %d, %d, %d]   align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  deactive(%s)  active(%s)  normal(%s)  pressed(%s)  selected(%s)", strValue.c_str(), skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY, skin->align, skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(), skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(), skin->pStrSkinPressed.c_str(), skin->pStrSkinSelected.c_str());
								} else  _LOG("\t\t[ERROR] set sub skin  '%s'", strValue.c_str());

							}
						}
					}
				};

				if (ini.jumpBlock(true, false)) {
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
							_LOG("\t\tadd sub skin  '%s'  (%p) [%d, %d, %d, %d]   align(0x%.8X)  event(0x%.8X)   style(0x%.8X)  deactive(%s)  active(%s)  normal(%s)  pressed(%s)  selected(%s)", strBlockName.c_str(), skin, skin->posX, skin->posY, skin->sizeX, skin->sizeY, skin->align, skin->event_info, skin->exdata, skin->pStrSkinDeactived.c_str(), skin->pStrSkinActived.c_str(), skin->pStrSkinNormal.c_str(), skin->pStrSkinPressed.c_str(), skin->pStrSkinSelected.c_str());
							if (!strBlockName.empty()) { // у этого блока есть имя, сохраняем
								mapLoadingSkin[strBlockName] = skin;
								_LOG("\t\t  store sub skin  '%s'  (%p)", strBlockName.c_str(), skin);
							}
						}

					};
				}

				if (window->subSkins.size()) {
					if (m_windowInfo[ID]) {__ASSERT(!m_windowInfo[ID]);} // элемент уже есть, низя
					m_windowInfo[ID] = window;
				} else { // неполучилось
					_LOG("\t\t[ERROR] load sub skin");
					delete window;
				}

				ini.jumpBlock(false, false); // выпрыгиваем

			// загрузка шрифтов окон -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_FONT_NAME) {

				uint32 ID = (uint32)mapNameValue[ini.getBlockName()];
				if (ID >= __FONT_COUNT) {_LOG("\t\t[ERROR] load font  (ID >= __FONT_COUNT)");continue;}

				__LP_MYGUI_FONT_INFO font = new __tag_MYGUI_FONT_INFO;

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
					m_fontInfo[ID] = font;
					font = 0;
				} else { // неудачная загрузка
					_LOG("\t[ERROR] load font \t %s", ini.getBlockName().c_str());
					delete font;
				}

			// загрузка указателей -------------------------------------------------------------------------------------------------
			} else if (ini.getBlockType() == BLOCK_POINTER_NAME) {

				uint32 ID = (uint32)mapNameValue[ini.getBlockName()];
				if (ID >= __POINTER_COUNT) {_LOG("\t\t[ERROR] load pointer  (ID >= __POINTER_COUNT)");continue;}

				__LP_MYGUI_POINTER_INFO pointer = new __tag_MYGUI_POINTER_INFO;

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
					_LOG("\tload pointer \t %s    '%s'  (%d, %d, %d, %d)", ini.getBlockName().c_str(), pointer->strMaterialName.c_str(), pointer->uSizeX, pointer->uSizeY, pointer->iOffsetX, pointer->iOffsetY);
					m_pointerInfo[ID] = pointer;
					pointer = 0;
				} else { // чего то невидно
					_LOG("\t[ERROR] load pointer \t %s", ini.getBlockName().c_str());
					delete pointer;
				}

				
			// а это что за хрень -------------------------------------------------------------------------------------------------
			} else { _LOG("\r\n\t[ERROR] unknow block type  -   type(%s),  name(%s)", ini.getBlockType().c_str(), ini.getBlockName().c_str()); }


		};

		ini.close();
		_LOG_NEW;
	}

}
//=========================================================================================
