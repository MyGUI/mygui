#include "SkinEditor.h"
#include "BasisManager.h"
#include "MyGUI_Source\\MyGUI.h"

using namespace Ogre;
using namespace MyGUI;

extern BasisManager basis;

const string BLOCK_WINDOW_NAME = "windowSkin";
const string SECTION_SUB_SKIN = "subSkin";
const string SECTION_SUB_SKIN_MAIN = "subSkinMain";
const string VALUE_WINDOW_ADDED_SKIN1 = "added_skin1";
const string VALUE_WINDOW_ADDED_SKIN2 = "added_skin2";
const string VALUE_WINDOW_ADDED_DATA1 = "added_data1";
const string VALUE_WINDOW_ADDED_DATA2 = "added_data2";
const string VALUE_WINDOW_FONT = "font";
const string VALUE_WINDOW_COLOUR = "colour";
const string VALUE_WINDOW_MATERIAL = "material";
const string VALUE_SKIN_OFFSET = "offset";
const string VALUE_SKIN_EVENT = "event";
const string VALUE_SKIN_ALIGIN = "aligin";
const string VALUE_SKIN_STYLE = "style";
const string VALUE_STATE_DEACTIVED = "deactived_position";
const string VALUE_STATE_NORMAL = "normal_position";
const string VALUE_STATE_PRESSED = "pressed_position";
const string VALUE_STATE_ACTIVED = "actived_position";
const string VALUE_STATE_SELECTED = "selected_position";

const MyGUI::uint32 FLAG_EVENT = 0x10000;
const MyGUI::uint32 FLAG_STYLE = 0x20000;
const MyGUI::uint32 FLAG_ALIGIN = 0x40000;
//===================================================================================
//===================================================================================
void SkinEditor::onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data) // дополнительные события
{
	if (!m_pCurrentDataWindow) return; // на всякий

	// выбор в комбо боксе
	if (uEvent == WOE_COMBO_SELECT_ACCEPT) {
		
		// главный комбобокс выбора окна
		if (pWindow == m_comboBasisWindowName) {
			m_pCurrentDataWindow = &mWindowInfo[data];
			updateWindowInfo();

		// выбок саб скина
		} else if (pWindow == m_comboSabSkinName) {
			if (m_pCurrentDataWindow) {
				m_pCurrentDataSkin = m_pCurrentDataWindow->sabSkins[data];
				updateSkinInfo();
			}

		// выбор состояния
		} else if (pWindow == m_comboSabSkinState) {
			if (m_pCurrentDataSkin) {
				m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[data];
				updateStateInfo();
			}

		} else if (pWindow == m_comboBasisAddedSkin1) {
			m_pCurrentDataWindow->uAddedSkin1 = data;

		} else if (pWindow == m_comboBasisAddedSkin2) {
			m_pCurrentDataWindow->uAddedSkin2 = data;

		} else if (pWindow == m_comboBasisFont) {
			m_pCurrentDataWindow->uFont = data;

		} else if (pWindow == m_comboBasisColour) {
			m_pCurrentDataWindow->strColour = m_comboBasisColour->getWindowText();

		// выбор материала
		} else if (pWindow == m_comboMaterialName) {
			if (m_pCurrentDataSkin) {
				m_pCurrentDataWindow->uMaterial = data;
				fillMaterialWindow();
			} else m_comboMaterialName->setString(0); // низя
		}

	// ввод в поле редактирования
	} else if (uEvent == WOE_EDIT_KEY_ACCEPT) {

		if (pWindow == m_editBasisData1) {
			m_pCurrentDataWindow->uAddedData1 = m_editBasisData1->getWindowText();
			//m_editBasisData1->setWindowText(m_pCurrentDataWindow->uAddedData1);

		} else if (pWindow == m_editBasisData2) {
			
			m_pCurrentDataWindow->uAddedData1 = m_editBasisData1->getWindowText();

		} else if (pWindow == m_comboBasisColour) {
			m_pCurrentDataWindow->strColour = m_comboBasisColour->getWindowText();

		} else if (pWindow->getUserData() == EDIT_IS_USE ) { // наше

			for (unsigned int index=0; index<4; index++) {
				if (pWindow == m_editOffset[index]) {
					if (m_pCurrentDataSkin) {
						m_pCurrentDataSkin->uOffset[index] = StringConverter::parseUnsignedInt(pWindow->getWindowText());
						pWindow->setWindowText(StringConverter::toString(m_pCurrentDataSkin->uOffset[index]));
					}
				} else if (pWindow == m_editPosition[index]) {
					if (m_pCurrentDataState) {
						m_pCurrentDataState->uPosition[index] = StringConverter::parseUnsignedInt(pWindow->getWindowText());
						pWindow->setWindowText(StringConverter::toString(m_pCurrentDataState->uPosition[index]));
					}
				}
			}

		}

	// показываем окно стилей
	} else if ((uEvent == WOE_FRAME_CLOSE) && (pWindow == m_windowStateFlags)) {
		m_windowStateFlags->show(false);
	}

}
//===================================================================================
void SkinEditor::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
{
	if (!m_pCurrentDataWindow) return; // на всякий

	if (pWindow == m_buttonSabSkinCreate) {
		LP_SUB_SKIN_DATA lpData = findSkinData(m_comboSabSkinName->getWindowText());
		if (lpData) {
			m_comboSabSkinName->addString(lpData->strName);
			m_comboSabSkinName->setString(m_comboSabSkinName->getStringCount()-1);
			m_pCurrentDataWindow->sabSkins.push_back(lpData);
			if (!m_pCurrentDataSkin) m_pCurrentDataState = &lpData->stateInfo[0]; // первый раз, надо открыть доступ
			m_pCurrentDataSkin = lpData;
			updateSkinInfo();
		}
	} else if (pWindow == m_buttonSabSkinDelete) {
		deleteSkinData(m_pCurrentDataSkin); // удаляет если нужно
		updateWindowInfo();
	} else if (pWindow == m_buttonSabSkinStyle) {
		if (m_pCurrentDataSkin) {
			m_windowStateFlags->show(true);
			basis.mGUI->upZOrder(m_windowStateFlags);
		}
	} else {
		// остальные кнопки
		pressOtherButton(pWindow);
	}
	
}
//===================================================================================
bool SkinEditor::createEditor() // создает окно редактирования скинов
{
	m_pCurrentDataWindow = 0; // текущее окно
	m_pCurrentDataSkin = 0; // текущий саб скин
	m_pCurrentDataState = 0; // текцщий стейт скина

	// информация об окнах
	
	// главное окно
	m_mainWindow = basis.mGUI->spawn<WindowFrame>(Coord(500, 150), Coord(300, 520), MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C);
	m_mainWindow->setWindowText("Skin editor MyGUI 1.0");

	// статик текст над окном
	MyGUI::StaticText * text = m_mainWindow->spawn<StaticText>(Coord(10, 0), Coord(270, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Basis window skin");

	// создаем окно с именами
	m_comboBasisWindowName = m_mainWindow->spawn<ComboBox>(Coord(10, 30), Coord(270, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);

	text = m_mainWindow->spawn<StaticText>(Coord(222, 60), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Added 1");
	m_comboBasisAddedSkin1 = m_mainWindow->spawn<ComboBox>(Coord(20, 60), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboBasisAddedSkin1->addString("[ no set ]");

	text = m_mainWindow->spawn<StaticText>(Coord(222, 90), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Added 2");
	    
	m_comboBasisAddedSkin2 = m_mainWindow->spawn<ComboBox>(Coord(20, 90), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboBasisAddedSkin2->addString("[ no set ]");

	// заполняем список
	m_comboBasisWindowName->addString(SkinManager::getSingleton()->getSkinNames()[0]);
	for (size_t index=1; index < SkinManager::getSingleton()->getSkinCount(); ++index) {
	    const String SkinName = SkinManager::getSingleton()->getSkinNames()[index];
	    
	    m_comboBasisWindowName->addString(SkinName);
		m_comboBasisAddedSkin1->addString(SkinName);
		m_comboBasisAddedSkin2->addString(SkinName);
	}

	m_editBasisData1 = m_mainWindow->spawn<Edit>(Coord(20, 120), Coord(60, -1), WA_LEFT|WA_TOP, SKIN_EDIT);

	m_editBasisData2 = m_mainWindow->spawn<Edit>(Coord(90, 120), Coord(60, -1), WA_LEFT|WA_TOP, SKIN_EDIT);

	text = m_mainWindow->spawn<StaticText>(Coord(155, 120), Coord(100, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Added data");

	text = m_mainWindow->spawn<StaticText>(Coord(222, 150), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Font");
	    
	m_comboBasisFont = m_mainWindow->spawn<ComboBox>(Coord(20, 150), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	for (size_t index = 0; index < SkinManager::getSingleton()->getFontCount(); ++index)
	    m_comboBasisFont->addString(SkinManager::getSingleton()->getFontNames()[index]);

	text = m_mainWindow->spawn<StaticText>(Coord(222, 180), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Colour");
	    
	m_comboBasisColour = m_mainWindow->spawn<ComboBox>(Coord(20, 180), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX);
	m_comboBasisColour->addString("COLOUR_BLACK");
	m_comboBasisColour->addString("COLOUR_WHITE");
	m_comboBasisColour->addString("COLOUR_BLUE");
	m_comboBasisColour->addString("COLOUR_GREEN");
	m_comboBasisColour->addString("COLOUR_RED");

	// заполняем всеми доступными материалами
	text = m_mainWindow->spawn<StaticText>(Coord(222, 210), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Material");
	    
	m_comboMaterialName = m_mainWindow->spawn<ComboBox>(Coord(20, 210), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboMaterialName->addString("[ no set ]");

	Ogre::ResourceManager::ResourceMapIterator mat = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (mat.hasMoreElements()) {
		String strMaterialName = mat.getNext()->getName();
		m_comboMaterialName->addString(strMaterialName);
		m_strMaterialName.push_back(strMaterialName);
	}


	text = m_mainWindow->spawn<StaticText>(Coord(10, 250), Coord(270, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Sab skins window");

	m_comboSabSkinName = m_mainWindow->spawn<ComboBox>(Coord(10, 280), Coord(270, -1), WA_TOP|WA_LEFT, SKIN_COMBO_BOX);

	m_buttonSabSkinCreate = m_mainWindow->spawn<Button>(Coord(10, 310), Coord(130, 20), WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
	m_buttonSabSkinCreate->setFont(MyGUI::FONT_DEFAULT, MyGUI::COLOUR_RED)
	                     ->setWindowText("create skin");

	m_buttonSabSkinDelete = m_mainWindow->spawn<Button>(Coord(148, 310), Coord(130, 20), WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
	m_buttonSabSkinDelete->setFont(MyGUI::FONT_DEFAULT, MyGUI::COLOUR_RED)
	                     ->setWindowText("delete skin");

	text = m_mainWindow->spawn<StaticText>(Coord(222, 340), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Offset");

	m_editOffset[0] = m_mainWindow->spawn<Edit>(Coord(20, 340), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[1] = m_mainWindow->spawn<Edit>(Coord(71, 340), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[2] = m_mainWindow->spawn<Edit>(Coord(122, 340), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[3] = m_mainWindow->spawn<Edit>(Coord(173, 340), Coord(46, -1), WA_LEFT|WA_TOP, SKIN_EDIT);

	m_buttonSabSkinStyle = m_mainWindow->spawn<Button>(Coord(20, 370), Coord(200, -1), WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
	m_buttonSabSkinStyle->setFont(MyGUI::FONT_DEFAULT, MyGUI::COLOUR_BLUE)
	                    ->setWindowText("SKIN STYLE");

	text = m_mainWindow->spawn<StaticText>(Coord(222, 410), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("State");
	    
	m_comboSabSkinState = m_mainWindow->spawn<ComboBox>(Coord(20, 410), Coord(200, -1), WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboSabSkinState->addString("WS_DEACTIVE");
	m_comboSabSkinState->addString("WS_NORMAL");
	m_comboSabSkinState->addString("WS_PRESSED");
	m_comboSabSkinState->addString("WS_ACTIVED");
	m_comboSabSkinState->addString("WS_SELECTED");

	text = m_mainWindow->spawn<StaticText>(Coord(222, 440), Coord(60, 25), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("Position");

	m_editPosition[0] = m_mainWindow->spawn<Edit>(Coord(20, 440), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[1] = m_mainWindow->spawn<Edit>(Coord(71, 440), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[2] = m_mainWindow->spawn<Edit>(Coord(122, 440), Coord(45, -1), WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[3] = m_mainWindow->spawn<Edit>(Coord(173, 440), Coord(46, -1), WA_LEFT|WA_TOP, SKIN_EDIT);

	for (unsigned int index=0; index < 4; index++) {
		m_editOffset[index]->setUserData(EDIT_IS_USE);
		m_editPosition[index]->setUserData(EDIT_IS_USE);
	}

	createFlagWindow();

	// окно для материалов
	createMaterialWindow();
	
	// обновляем
	m_comboBasisWindowName->setString(0);
	m_pCurrentDataWindow = &mWindowInfo[0];

	updateWindowInfo();

	return true;
}
//===================================================================================
void SkinEditor::destroyEditor() // удаляет окно редактирования скинов
{
	basis.mGUI->destroyWindow(m_windowStateFlags);
	basis.mGUI->destroyWindow(m_mainWindow);
	destroySkins();
	// информация об окнах
	mWindowInfo.clear();
}
//===================================================================================
void SkinEditor::updateWindowInfo()
{
	if (!m_pCurrentDataWindow) return; // на всякий

	// загружаем новые значения
	//TODO: port
	//m_comboBasisAddedSkin1->setString(m_pCurrentDataWindow->uAddedSkin1);
	//m_comboBasisAddedSkin2->setString(m_pCurrentDataWindow->uAddedSkin2);
	
	m_editBasisData1->setWindowText(m_pCurrentDataWindow->uAddedData1);
	m_editBasisData2->setWindowText(m_pCurrentDataWindow->uAddedData2);
	
	//TODO: port
	//m_comboBasisFont->setString(m_pCurrentDataWindow->uFont);
	
	m_comboBasisColour->setWindowText(m_pCurrentDataWindow->strColour);
	m_comboMaterialName->setString(m_pCurrentDataWindow->uMaterial);

	// очищаем поля
	m_comboSabSkinName->deleteStringAll();

	// заполняем информацией об сабскинах
	for (size_t pos = 0; pos < m_pCurrentDataWindow->sabSkins.size(); pos++) {
		if (m_pCurrentDataWindow->sabSkins[pos]) {
			m_comboSabSkinName->addString(m_pCurrentDataWindow->sabSkins[pos]->strName.c_str());
		}
	}

	if (m_pCurrentDataWindow->sabSkins.size()) {
		// саб скин
		m_pCurrentDataSkin = m_pCurrentDataWindow->sabSkins[0];
		m_comboSabSkinName->setString(0);
		// стейт
		m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[0];
		m_comboSabSkinState->setString(0);
	} else {
		m_pCurrentDataSkin = 0;
	}
	
	fillMaterialWindow(); // заполняем окно с материалом

	updateSkinInfo();

}
//===================================================================================
void SkinEditor::updateSkinInfo() // обновляет всю инфу об саб скине
{
	if (!m_pCurrentDataSkin) {
		m_pCurrentDataState = 0; // сотрем стейты
		for (unsigned int pos=0; pos<4; pos++) m_editOffset[pos]->setWindowText("0");
		m_windowStateFlags->show(false);
	} else {
		for (unsigned int pos=0; pos<4; pos++) m_editOffset[pos]->setWindowText(StringConverter::toString(m_pCurrentDataSkin->uOffset[pos]));
		m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[0];
		fillFlagWindow(); // заполняет окна текущими значения
	}

	m_comboSabSkinState->setString(0); // на первый

	updateStateInfo();
}
//===================================================================================
void SkinEditor::updateStateInfo() // обновляет всю инфу об саб скине
{
	// сначала все очистим
	for (unsigned int pos=0; pos<4; pos++) { // поля для чисел
		m_editPosition[pos]->setWindowText("0");
	}

	if (!m_pCurrentDataState) return; // нет инфы уходим

	for (unsigned int pos=0; pos<4; pos++) { // поля для чисел
		m_editPosition[pos]->setWindowText(StringConverter::toString(m_pCurrentDataState->uPosition[pos]));
	}

}
//===================================================================================
void SkinEditor::deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin) // удаляет саб скин
{
	if (!lpDataSkin) return;
	if (!m_pCurrentDataWindow) return;

	size_t size = m_pCurrentDataWindow->sabSkins.size();

	for (size_t pos=0; pos<size; pos++) {
		if (m_pCurrentDataWindow->sabSkins[pos] == lpDataSkin) {
			if (size != 1) m_pCurrentDataWindow->sabSkins[pos] = m_pCurrentDataWindow->sabSkins[size-1];
			m_pCurrentDataWindow->sabSkins.pop_back();
			pos = size; // выходим
		}
	}

	// если не найдем еще такую, то удаляем
	if ( !findSkinData(lpDataSkin->strName, false) ) {
		delete lpDataSkin;
	}
}
//===================================================================================
SkinEditor::LP_SUB_SKIN_DATA SkinEditor::findSkinData(const String & strName, bool create) // ищет данные если нет то создает
{
	if (strName.empty()) return 0;

	for (size_t index=0; index < SkinManager::getSingleton()->getSkinCount(); index++) {
		for (size_t pos=0; pos<mWindowInfo[index].sabSkins.size(); pos++) {
			if (!mWindowInfo[index].sabSkins[pos]) continue;
			if (mWindowInfo[index].sabSkins[pos]->strName == strName) return mWindowInfo[index].sabSkins[pos];
		}
	}

	if (!create) return 0; // создавать не надо
	// не нашли значит создадим
	return new _tag_SUB_SKIN_DATA(strName);
}
//===================================================================================
void SkinEditor::destroySkins()
{

	for (size_t index=0; index < SkinManager::getSingleton()->getSkinCount(); index++) {
		m_pCurrentDataWindow = &mWindowInfo[index];
		while (m_pCurrentDataWindow->sabSkins.size()) {
			deleteSkinData(m_pCurrentDataWindow->sabSkins[0]);
		}
	}

	m_pCurrentDataWindow = 0; // текущее окно
	m_pCurrentDataSkin = 0; // текущий саб скин
	m_pCurrentDataState = 0; // текцщий стейт скина

}
//===================================================================================
void SkinEditor::saveSkin() // сохраняет скин
{
	std::ofstream fp;
	fp.open("test.mygui_skin", ios_base::out | ios_base::trunc);

	String strState[5] = {VALUE_STATE_DEACTIVED, VALUE_STATE_NORMAL, VALUE_STATE_PRESSED, VALUE_STATE_ACTIVED, VALUE_STATE_SELECTED};
	vector <String> astrSubSkinName; // имена уже сохоаненых саб скинов

	for (size_t index=0; index < SkinManager::getSingleton()->getSkinCount(); index++) {
		LP_WINDOW_DATA window = &mWindowInfo[index];
		if (!window) continue;

		fp << "\n" << BLOCK_WINDOW_NAME << " \t" << SkinManager::getSingleton()->getSkinNames()[index] << "\n{";

		if (window->uAddedSkin1 != "") { fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN1 << " \t" << window->uAddedSkin1; }
		if (window->uAddedSkin2 != "") { fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN2 << " \t" << window->uAddedSkin2; }

		if (window->uAddedData1 != "") { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA1 << " \t" << window->uAddedData1; }
		if (window->uAddedData2 != "") { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA2 << " \t" << window->uAddedData2; }

		if (window->uFont != "") {fp << "\n\t" << VALUE_WINDOW_FONT << " \t" << window->uFont;	}
		if (!window->strColour.empty()) { fp << "\n\t" << VALUE_WINDOW_COLOUR << " \t" << window->strColour; }
		if (window->uMaterial) { fp << "\n\t" << VALUE_WINDOW_MATERIAL <<
		    " \t\"" << m_strMaterialName[window->uMaterial-1] << "\""; }

		// сначала проходим и сохраняем только именна если уже было сохранение
		for (size_t pos=0; pos<window->sabSkins.size(); pos++) {

			LP_SUB_SKIN_DATA skin = window->sabSkins[pos];
			// ищем такой же блок
			bool find = false;
			for (size_t search = 0; search<astrSubSkinName.size(); search++) {
				if (astrSubSkinName[search] == skin->strName) {
					find = true;
					search = astrSubSkinName.size();
				}
			}
			if (!find) continue;

			// сохраняем только описание
			if (pos == 0) fp << "\n\t" << SECTION_SUB_SKIN_MAIN;
			else fp << "\n\n\t" << SECTION_SUB_SKIN;
			fp << " \t" << skin->strName;


		} // for (uint8 pos=0; pos<window->sabSkins.size(); pos++) {

		// а теперь сохраняем полностью блоки
		for (size_t pos=0; pos<window->sabSkins.size(); pos++) {

			LP_SUB_SKIN_DATA skin = window->sabSkins[pos];

			// ищем такой же блок
			bool find = false;
			for (size_t search=0; search<astrSubSkinName.size(); search++) {
				if (astrSubSkinName[search] == skin->strName) {
					find = true;
					search = astrSubSkinName.size();
				}
			}
			if (find) continue; // уже значит сохраняли

			if (pos == 0) fp << "\n\n\t" << SECTION_SUB_SKIN_MAIN << " \t";
			else fp << "\n\n\t" << SECTION_SUB_SKIN << " \t";
			fp << skin->strName << "\n\t{";

			fp << "\n\t\t" << VALUE_SKIN_OFFSET << " \t" << StringConverter::toString(skin->uOffset[0]) << "  " << StringConverter::toString(skin->uOffset[1]) << "  " << StringConverter::toString(skin->uOffset[2]) << "  " << StringConverter::toString(skin->uOffset[3]) << "\n";

			// сохраняем флаги событий
			fp << "\n\t\t" << VALUE_SKIN_EVENT << " \t"; 
			if (!skin->event_info) fp << "WE_NONE";
			else {
			    
			    //TODO: port	
				//for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagEvent.begin(); iter != basis.mGUI->m_mapFlagEvent.end(); iter ++) {
				//	if ( ((*iter).second & skin->event_info) != (*iter).second ) continue;
				//	fp << (*iter).first << "  ";
				//}
			
			}

			// сохраняем флаги выравнивания
			fp << "\n\t\t" << VALUE_SKIN_ALIGIN << " \t"; 
			if (!skin->aligin) fp << "WA_NONE";
			else {
				
				//TODO: port
				//for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagAligin.begin(); iter != basis.mGUI->m_mapFlagAligin.end(); iter ++) {
				//	if ( ((*iter).second & skin->aligin) != (*iter).second ) continue;
				//	fp << (*iter).first << "  ";
				//}
				
			}

			// сохраняем флаги стиля
			fp << "\n\t\t" << VALUE_SKIN_STYLE << " \t"; 
			if (!skin->style) fp << "WES_NONE";
			else {
				
				//TODO:
				//for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagStyle.begin(); iter != basis.mGUI->m_mapFlagStyle.end(); iter ++) {
				//	if ( ((*iter).second & skin->style) != (*iter).second ) continue;
				//	fp << (*iter).first << "  ";
				//}
			}

			fp << "\n";

			for (unsigned int iState=0; iState<5; iState++) {
				if ((!skin->stateInfo[iState].uPosition[2]) || (!skin->stateInfo[iState].uPosition[3])) continue;
				fp << "\n\t\t" << strState[iState] << " \t" << StringConverter::toString(skin->stateInfo[iState].uPosition[0]) << "  " << StringConverter::toString(skin->stateInfo[iState].uPosition[1]) << "  " << StringConverter::toString(skin->stateInfo[iState].uPosition[2]) << "  " << StringConverter::toString(skin->stateInfo[iState].uPosition[3]);
			}

			fp << "\n\t}";

			astrSubSkinName.push_back(skin->strName); // запоминаем что мы его уже сохранили

		} // for (uint8 pos=0; pos<window->sabSkins.size(); pos++) {
		
		fp << "\n}\n";
	} // for (uint8 index=0; index<MyGUI::__SKIN_COUNT; index++) {
	
	fp.close();
}
//===================================================================================
void SkinEditor::loadSkin() // загружает скин
{
    SkinManager::getSingleton()->unloadResource();
    SkinManager::getSingleton()->createSkin();
	
	/*String strState[5] = {VALUE_STATE_DEACTIVED, VALUE_STATE_NORMAL, VALUE_STATE_PRESSED, VALUE_STATE_ACTIVED, VALUE_STATE_SELECTED};

	while (ini.seekNextBlock()) {

		if (ini.getBlockType() != BLOCK_WINDOW_NAME) continue;
		string strValue = ini.getBlockName();

		uint16 ID = (uint16)-1;
		for (uint16 pos=0; pos<basis.mGUI->m_strSkinNames.size(); pos++) {
			if (basis.mGUI->m_strSkinNames[pos] == strValue) {
				ID = pos;
				pos = (uint16)basis.mGUI->m_strSkinNames.size(); // выходим
			}
		}
		if (ID >= mWindowInfo.size()) continue; // враг не пройдет

		LP_WINDOW_DATA window = &mWindowInfo[ID];
		// сбрасываем все данные
//		while (window->sabSkins.size()) deleteSkinData(window->sabSkins[0]);
		window->strColour = "";
		window->uAddedData1 = 0;
		window->uAddedData2 = 0;
		window->uAddedSkin1 = 0;
		window->uAddedSkin2 = 0;
		window->uFont = 0;
		window->uMaterial = 0;

		string strValueName;
		uint32 uValue;
		while (ini.LoadNextValue()) {
			if (!ini.getValueName(strValueName)) continue;
			
			if (strValueName == VALUE_WINDOW_ADDED_SKIN1) {
				if (ini.getValue(strValue)) {
					for (uint16 pos=0; pos<basis.mGUI->m_strSkinNames.size(); pos++) {
						if (strValue == basis.mGUI->m_strSkinNames[pos].c_str()) {
							window->uAddedSkin1 = pos;
							pos = (uint16)basis.mGUI->m_strSkinNames.size(); // выходим
						}
					}
				}
			} else if (strValueName == VALUE_WINDOW_ADDED_SKIN2) {
				if (ini.getValue(strValue)) {
					for (uint16 pos=0; pos<basis.mGUI->m_strSkinNames.size(); pos++) {
						if (strValue == basis.mGUI->m_strSkinNames[pos]) {
							window->uAddedSkin2 = pos;
							pos = (uint16)basis.mGUI->m_strSkinNames.size(); // выходим
						}
					}
				}
			} else if (strValueName == VALUE_WINDOW_ADDED_DATA1) {
				if (ini.getValue(uValue)) {
					if (uValue <= 255) window->uAddedData1 = uValue;
				}
			} else if (strValueName == VALUE_WINDOW_ADDED_DATA2) {
				if (ini.getValue(uValue)) {
					if (uValue <= 255) window->uAddedData2 = uValue;
				}
			} else if (strValueName == VALUE_WINDOW_FONT) {
				if (ini.getValue(uValue)) {
					if (uValue < __FONT_COUNT) window->uFont = uValue;
				}
			} else if (strValueName == VALUE_WINDOW_COLOUR) {
				if (ini.getValue(strValue)) window->strColour = strValue;
			} else if (strValueName == VALUE_WINDOW_MATERIAL) {
				if (ini.getValue(strValue)) {
					for (uint16 pos=0; pos<m_strMaterialName.size(); pos++) { // ищем такой материал
						if (m_strMaterialName[pos] == strValue) {
							window->uMaterial = pos+1;
							pos = (uint16)m_strMaterialName.size();
						}
					}
				}

			} else if (strValueName == SECTION_SUB_SKIN) { // уже загруженные скины
				if (ini.getValue(strValue)) {
					LP_SUB_SKIN_DATA skin = findSkinData(strValue);
					if (skin) window->sabSkins.push_back(skin);
				}
			} else if (strValueName == SECTION_SUB_SKIN_MAIN) { // уже загруженные скины
				if (ini.getValue(strValue)) {
					LP_SUB_SKIN_DATA skin = findSkinData(strValue);
					if (skin) { // вставляем в начало
						window->sabSkins.push_back(0);
						window->sabSkins[window->sabSkins.size()-1] = window->sabSkins[0];
						window->sabSkins[0] = skin;
					}
				}
			}

		}; // while (ini.LoadNextValue()) {


		// теперь проверяем внутрение блоки
		if (ini.jampBlock(true, false)) { // впрыгиваем в блок

			bool bIsMain;
			while (ini.seekNextBlock()) {

				if (ini.getBlockType() != SECTION_SUB_SKIN) {
					if (ini.getBlockType() != SECTION_SUB_SKIN_MAIN) continue;
					bIsMain = true;
				} else bIsMain = false;

				LP_SUB_SKIN_DATA skin = findSkinData(ini.getBlockName());
				if (!skin) continue;

				while (ini.LoadNextValue()) {
					if (!ini.getValueName(strValueName)) continue;

					if (strValueName == VALUE_SKIN_OFFSET) {
						for (uint8 pos=0; pos<4; pos++) {
							if (!ini.getValue(uValue, pos)) continue;
							skin->uOffset[pos] = uValue;
						}
					} else if (strValueName == VALUE_SKIN_EVENT) {
						for (uint8 pos=0; pos<16; pos++) {
							if (!ini.getValue(strValue, pos)) continue;
							skin->event_info |= basis.mGUI->m_mapFlagEvent[strValue];
						}
					} else if (strValueName == VALUE_SKIN_ALIGIN) {
						for (uint8 pos=0; pos<16; pos++) {
							if (!ini.getValue(strValue, pos)) continue;
							skin->aligin |= basis.mGUI->m_mapFlagAligin[strValue];
						}
					} else if (strValueName == VALUE_SKIN_STYLE) {
						for (uint8 pos=0; pos<16; pos++) {
							if (!ini.getValue(strValue, pos)) continue;
							skin->style |= basis.mGUI->m_mapFlagStyle[strValue];
						}

					} else {
						for (uint8 index=0; index<5; index++) {

							if (strValueName == strState[index]) {
								for (uint8 pos=0; pos<4; pos++) {
									if (!ini.getValue(uValue, pos)) continue;
									skin->stateInfo[index].uPosition[pos] = uValue;
								}
								index = 5; // выходим
							}

						} // for (uint8 index=0; index<5; index++) {

					}

				}; // while (ini.LoadNextValue()) {

				window->sabSkins.push_back(skin);
				if (bIsMain) { // вставляем в начало
					window->sabSkins[window->sabSkins.size()-1] = window->sabSkins[0];
					window->sabSkins[0] = skin;
				}

			}; // while (ini.seekNextBlock()) {

		} // if (ini.jampBlock(true, false)) {

		ini.jampBlock(false, false); // выпрыгиваем

	}; // while (ini.seekNextBlock()) {
	*/

	if (mWindowInfo.size()) {
		m_comboBasisWindowName->setString(0);
		m_pCurrentDataWindow = &mWindowInfo[0];
		updateWindowInfo();
	}
}
//===================================================================================
void SkinEditor::createFlagWindow()
{
	// создаем все что связанно с окном для флагов
	m_windowStateFlags = basis.mGUI->spawn<WindowFrame>(Coord(100, 100), Coord(650, 490), MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CX);
    m_windowStateFlags->setWindowText("Skin flags");
    
	StaticText * text = m_windowStateFlags->spawn<StaticText>(Coord(10, 0), Coord(200, 20), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("EVENT");
	text = m_windowStateFlags->spawn<StaticText>(Coord(220, 0), Coord(200, 20), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("ALIGIN");
	text = m_windowStateFlags->spawn<StaticText>(Coord(430, 0), Coord(200, 20), WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN)
	    ->setWindowText("STYLE");
    
    //??
	MyGUI::uint8 countEvents = 5;
	MyGUI::uint8 countAligin = 14;
	MyGUI::uint8 countStyle = 8;

	for (unsigned int pos=0; pos<16; pos++) {
		if (pos < countEvents) {
			m_buttonsFlagsEvent[pos] = m_windowStateFlags->spawn<Button>(Coord(10, (pos*30)+30), Coord(200, -1), WA_LEFT|WA_TOP, "SKIN_CHECK_BOX");
		} else m_buttonsFlagsEvent[pos] = 0;

		if (pos < countAligin) {
			m_buttonsFlagsAligin[pos] = m_windowStateFlags->spawn<Button>(Coord(220, (pos*30)+30), Coord(200, -1), WA_LEFT|WA_TOP, "SKIN_CHECK_BOX");
		} else m_buttonsFlagsAligin[pos] = 0;

		if (pos < countStyle) {
			m_buttonsFlagsStyle[pos] = m_windowStateFlags->spawn<Button>(Coord(430, (pos*30)+30), Coord(200, -1), WA_LEFT|WA_TOP, "SKIN_CHECK_BOX");
		} else m_buttonsFlagsStyle[pos] = 0;
	}

	#define SET_FLAG(win,num,flag) if(win){win->setWindowText(#num);win->setUserData(num|flag);}

	SET_FLAG(m_buttonsFlagsEvent[0], WE_MOUSE_BUTTON, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[1], WE_MOUSE_FOCUS, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[2], WE_MOUSE_MOVE, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[3], WE_KEY_FOCUS, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[4], WE_KEY_BUTTON, FLAG_EVENT);

	SET_FLAG(m_buttonsFlagsAligin[0], WA_LEFT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[1], WA_RIGHT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[2], WA_TOP, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[3], WA_BOTTOM, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[4], WAT_LEFT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[5], WAT_RIGHT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[6], WAT_TOP, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[7], WAT_BOTTOM, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[8], WAT_CUT_RIGHT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[9], WAT_CUT_LEFT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[10], WAT_ADD_DOT, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[11], WAT_MULTI_LINE, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[12], WAT_BREAK_WORD, FLAG_ALIGIN);
	SET_FLAG(m_buttonsFlagsAligin[13], WAT_SHIFT_TEXT_PRESSED, FLAG_ALIGIN);

	SET_FLAG(m_buttonsFlagsStyle[0], WES_TEXT, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[1], WES_CLIENT, FLAG_STYLE);
	
    //TODO: port
	//SET_FLAG(m_buttonsFlagsStyle[2], WES_ACTION1, FLAG_STYLE);
	//SET_FLAG(m_buttonsFlagsStyle[3], WES_ACTION2, FLAG_STYLE);
	//SET_FLAG(m_buttonsFlagsStyle[4], WES_ACTION3, FLAG_STYLE);
	//SET_FLAG(m_buttonsFlagsStyle[5], WES_ACTION4, FLAG_STYLE);
	//SET_FLAG(m_buttonsFlagsStyle[6], WES_ACTION5, FLAG_STYLE);
	//SET_FLAG(m_buttonsFlagsStyle[7], WES_ACTION6, FLAG_STYLE);

	m_windowStateFlags->show(false);

}
//===================================================================================
void SkinEditor::pressOtherButton(MyGUI::Window * pWindow) // сверяем с кнопками флагов
{
	if (!m_pCurrentDataSkin) return;

	MyGUI::uint32 type = pWindow->getUserData() & 0xF0000;
	MyGUI::uint16 * ptr;

	if (type == FLAG_EVENT) ptr = &m_pCurrentDataSkin->event_info;
	else if (type == FLAG_STYLE) ptr = &m_pCurrentDataSkin->style;
	else if (type == FLAG_ALIGIN) ptr = &m_pCurrentDataSkin->aligin;
	else return;

	MyGUI::uint16 flag = pWindow->getUserData() & 0xFFFF;

	//TODO: port
	/*if (pWindow->getState() == WS_PRESSED) {
		pWindow->setState(WS_NORMAL);
		(*ptr) &= ~(flag);
	} else {
		pWindow->setState(WS_PRESSED);
		(*ptr) |= flag; 
	}*/
}
//===================================================================================
void SkinEditor::fillFlagWindow() // заполняет окна текущими значения
{
	if (!m_pCurrentDataSkin) return;

	// сначала очищаем все поля
	for (unsigned int pos=0; pos<16; pos++) {
		if (m_buttonsFlagsEvent[pos]) m_buttonsFlagsEvent[pos]->setState(WS_NORMAL);
		if (m_buttonsFlagsAligin[pos]) m_buttonsFlagsAligin[pos]->setState(WS_NORMAL);
		if (m_buttonsFlagsStyle[pos]) m_buttonsFlagsStyle[pos]->setState(WS_NORMAL);
	}

	// теперь будем искать совпадения и отмечать
	for (unsigned int pos=0; pos<16; pos++) {

		if (m_pCurrentDataSkin->event_info & (1<<pos)) {
			// а вот теперь ищем
			MyGUI::uint32 find = (1<<pos) | FLAG_EVENT;
			for (unsigned int i=0; i<16; i++) {
				if (!m_buttonsFlagsEvent[i]) continue;
				if (m_buttonsFlagsEvent[i]->getUserData() == find) m_buttonsFlagsEvent[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->event_info & (1<<pos)) {

		if (m_pCurrentDataSkin->aligin & (1<<pos)) {
			// а вот теперь ищем
			MyGUI::uint32 find = (1<<pos) | FLAG_ALIGIN;
			for (unsigned int i=0; i<16; i++) {
				if (!m_buttonsFlagsAligin[i]) continue;
				if (m_buttonsFlagsAligin[i]->getUserData() == find) m_buttonsFlagsAligin[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->aligin & (1<<pos)) {

		if (m_pCurrentDataSkin->style & (1<<pos)) {
			// а вот теперь ищем
			MyGUI::uint32 find = (1<<pos) | FLAG_STYLE;
			for (unsigned int i=0; i<16; i++) {
				if (!m_buttonsFlagsStyle[i]) continue;
				if (m_buttonsFlagsStyle[i]->getUserData() == find) m_buttonsFlagsStyle[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->style & (1<<pos)) {

	} // for (uint8 pos=0; pos<16; pos++) {

}
//===================================================================================
void SkinEditor::fillMaterialWindow() // заполняем окно с материалом
{

		if (m_comboMaterialName->getCurrentIndex() == 0) {
			m_windowMaterial->show(false);
			return;
		}

		const String & strMaterialName = m_comboMaterialName->getWindowText();

		MaterialPtr mat = MaterialManager::getSingleton().getByName(strMaterialName);
		if (mat.isNull()) {
			m_windowMaterial->show(false);
			return;
		}

		// обязательно загружаем
		mat->load();



		// warning
		const String & textName = mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getTextureName();

		TexturePtr tex = (TexturePtr)TextureManager::getSingleton().getByName(textName);

		if (tex.isNull()) {
			m_windowMaterial->show(false);
			return;
		}

		const unsigned int addX = m_windowMaterial->m_iSizeX-m_windowMaterial->m_pWindowClient->m_iSizeX;
		const unsigned int addY = m_windowMaterial->m_iSizeY-m_windowMaterial->m_pWindowClient->m_iSizeY;

		m_windowMaterial->show(true);
		m_windowMaterial->size( (MyGUI::int16 ) tex->getWidth()+addX, (MyGUI::int16 ) tex->getHeight()+addY );
		m_windowMaterial->m_pWindowClient->m_overlayContainer->setMaterialName(strMaterialName);

}
//===================================================================================
void SkinEditor::createMaterialWindow() // создает окна для материала
{
	m_windowMaterial = basis.mGUI->spawn<WindowFrame>(Coord(100, 100), Coord(200, 200), MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C);
	m_windowMaterial->setWindowText("Material view");
	m_windowMaterial->show(false);

	m_windowMaterialLeft = m_windowMaterial->spawn<Window>(Coord(10, 10), Coord(1, 100), WA_LEFT|WA_TOP, SKIN_DEFAULT);
//	m_windowMaterialLeft->m_pWindowClient->m_overlayContainer->setMaterialName("BACK_GREEN");
}
//===================================================================================
