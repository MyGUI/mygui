//===================================================================================
#include "SkinEditor.h"
#include "BasisManager.h"
#include "MyGUI_Source\\loadINI.h"
//===================================================================================
using namespace Ogre;
using namespace MyGUI;
//===================================================================================
extern BasisManager basis;
//===================================================================================
const string NO_SET = "[ no set ]";
//===================================================================================
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
//===================================================================================
const uint32 FLAG_EVENT = 0x10000;
const uint32 FLAG_STYLE = 0x20000;
const uint32 FLAG_ALIGIN = 0x40000;
//===================================================================================
//===================================================================================
void SkinEditor::onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) // дополнительные события
{
	// выбор в комбо боксе
	if (uEvent == WOE_COMBO_SELECT_ACCEPT) {

		if (pWindow == m_comboBasisWindowName) {

			if (data != MyGUI::ITEM_NON_SELECT) {
				m_pCurrentDataWindow = &mWindowInfo[data];
				updateWindowInfo();
			} else {

				const String & strName = m_comboBasisWindowName->getWindowText();
				bool find = false;
				for (uint16 pos=0; pos<mWindowInfo.size(); pos++) {
					if (mWindowInfo[pos].strElementName == strName) {
						find = true;
						pos = (uint16)mWindowInfo.size(); // выходим
					}
				}

				if (!find) {
					m_comboBasisWindowName->addString(strName);
					mWindowInfo.push_back(_tag_WINDOW_DATA(strName));
					m_comboBasisWindowName->setString(m_comboBasisWindowName->getStringCount()-1);
					m_pCurrentDataWindow = &mWindowInfo.back();
					updateWindowInfo();
				} // if (!find) {

			}

			return;
		} // if (pWindow == m_comboBasisWindowName) {

	} // if (uEvent == WOE_COMBO_SELECT_ACCEPT) {


	// выбор в комбо боксе
	if (uEvent == WOE_COMBO_SELECT_ACCEPT) {
		
		// выбо саб скина
		if (pWindow == m_comboSabSkinName) {

			if (data != MyGUI::ITEM_NON_SELECT) {
				m_pCurrentDataSkin = m_pCurrentDataWindow->sabSkins[data];
				updateSkinInfo();
			} else {
				// нажат ентер с новым именем

				bool find = false;
				const String & strSkinName = m_comboSabSkinName->getWindowText();
				for (uint16 pos=0; pos<m_comboSabSkinName->getStringCount(); pos++) {
					if (m_comboSabSkinName->getString(pos) == strSkinName) {
						find = true;
						pos = m_comboSabSkinName->getStringCount();
					}
				}

				if (!find) {
					LP_SUB_SKIN_DATA lpData = findSkinData(m_comboSabSkinName->getWindowText());
					if (lpData) {
						m_comboSabSkinName->addString(lpData->strName);
						m_comboSabSkinName->setString(m_comboSabSkinName->getStringCount()-1);
						m_pCurrentDataWindow->sabSkins.push_back(lpData);
						if (!m_pCurrentDataSkin) m_pCurrentDataState = &lpData->stateInfo[0]; // первый раз, надо открыть доступ
						m_pCurrentDataSkin = lpData;
						updateSkinInfo();
					}
				} // if (!find) {

			} // if (data != MyGUI::ITEM_NON_SELECT) {

		// выбор состояния
		} else if (pWindow == m_comboSabSkinState) {
			m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[data];
			updateStateInfo();

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
				if (!fillMaterialWindow()) m_comboMaterialName->setString(0);
			} else m_comboMaterialName->setString(0); // низя
		}

	// ввод в поле редактирования
	} else if (uEvent == WOE_EDIT_KEY_ACCEPT) {

		if (pWindow == m_editBasisData1) {
			uint32 tmp = StringConverter::parseUnsignedInt(m_editBasisData1->getWindowText());
			(tmp > 255) ? m_pCurrentDataWindow->uAddedData1 = 0 : m_pCurrentDataWindow->uAddedData1 = tmp;
			m_editBasisData1->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData1));

		} else if (pWindow == m_editBasisData2) {
			uint32 tmp = StringConverter::parseUnsignedInt(m_editBasisData2->getWindowText());
			(tmp > 255) ? m_pCurrentDataWindow->uAddedData2 = 0 : m_pCurrentDataWindow->uAddedData2 = tmp;
			m_editBasisData2->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData2));

		} else if (pWindow == m_comboBasisColour) {
			m_pCurrentDataWindow->strColour = m_comboBasisColour->getWindowText();

		} else if (pWindow->getUserData() == EDIT_IS_USE ) { // наше

			for (uint8 index=0; index<4; index++) {
				if (pWindow == m_editOffset[index]) {
					m_pCurrentDataSkin->uOffset[index] = StringConverter::parseUnsignedInt(pWindow->getWindowText());
					pWindow->setWindowText(StringConverter::toString(m_pCurrentDataSkin->uOffset[index]));
				} else if (pWindow == m_editPosition[index]) {
					m_pCurrentDataState->uPosition[index] = StringConverter::parseUnsignedInt(pWindow->getWindowText());
					pWindow->setWindowText(StringConverter::toString(m_pCurrentDataState->uPosition[index]));
				}
			}

			// обновляем квадратик
			setMaterialOffset(m_pCurrentDataState->uPosition[0], m_pCurrentDataState->uPosition[1], m_pCurrentDataState->uPosition[2], m_pCurrentDataState->uPosition[3]);
		}


	// показываем окно стилей
	} else if (uEvent == WOE_FRAME_CLOSE) {
		if (pWindow == m_windowStateFlags) m_windowStateFlags->show(false);

	} else if (uEvent == WOE_EDIT_KEY_DELETE) {
		// удаляем сабскин
		if (pWindow == m_comboSabSkinName) {
			if (m_pCurrentDataSkin) {
				deleteSkinData(m_pCurrentDataSkin); // удаляет если нужно
				updateWindowInfo();
			}

		// удаляем элемент
		} else if (pWindow == m_comboBasisWindowName) {

			const String & strName = m_comboBasisWindowName->getWindowText();
			uint16 ID = (uint16)-1;
			for (uint16 pos=0; pos<mWindowInfo.size(); pos++) {
				if (mWindowInfo[pos].strElementName == strName) {
					ID = pos;
					pos = (uint16)mWindowInfo.size(); // выходим
				}
			}

			if (ID < mWindowInfo.size()) {
				m_pCurrentDataWindow = &mWindowInfo[ID];
				while (m_pCurrentDataWindow->sabSkins.size()) {
					deleteSkinData(m_pCurrentDataWindow->sabSkins[0]);
				}
				mWindowInfo[ID] = mWindowInfo[mWindowInfo.size()-1];
				mWindowInfo.pop_back();
				m_comboBasisWindowName->deleteString(ID);
				
				if (mWindowInfo.size() > 0) m_pCurrentDataWindow = &mWindowInfo[0];
				else m_pCurrentDataWindow = 0;
				updateWindowInfo();

			} 


		}

	}

}
//===================================================================================
void SkinEditor::onMouseClick(MyGUI::Window * pWindow) // нажата и отпущена левая кнопка мыши на этом же элементе
{
	if (!m_pCurrentDataWindow) return; // на всякий

	if (pWindow == m_buttonSabSkinStyle) {
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
void SkinEditor::onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) // смена фокуса
{
	if (pWindow->getUserData() == MATERIAL_OFFSET) {
		if (pWindow == m_windowMaterialOffset[CENTER]) {
			for (uint8 pos=0; pos<4; pos++) {
				if (bIsFocus) m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_YELLOY");
				else m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_GREEN");
			}
		} else {
			if (bIsFocus) pWindow->m_overlayContainer->setMaterialName("BACK_YELLOY");
			else pWindow->m_overlayContainer->setMaterialName("BACK_GREEN");
		}

	}
}
//===================================================================================
void SkinEditor::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iFotherPosX, int16 iFotherPosY) // уведомление о движении, но не движение
{
	if ( ! m_pCurrentDataState) return;

	if (pWindow->getUserData() == MATERIAL_OFFSET) {
		
		iFotherPosX -= basis.mGUI->m_iOffsetPressedX;
		iFotherPosY -= basis.mGUI->m_iOffsetPressedY;
		iPosX = iFotherPosX - pWindow->m_iPosX;
		iPosY = iFotherPosY - pWindow->m_iPosY;

		if (pWindow == m_windowMaterialOffset[CENTER]) {
			if (((int16)m_pCurrentDataState->uPosition[LEFT] + iPosX) < 0) iPosX = 0 - (int16)m_pCurrentDataState->uPosition[LEFT];
			else if ((m_pCurrentDataState->uPosition[LEFT] + iPosX) > (m_uTextureSizeX - m_pCurrentDataState->uPosition[RIGHT])) iPosX = m_uTextureSizeX - m_pCurrentDataState->uPosition[RIGHT] - m_pCurrentDataState->uPosition[LEFT];
			m_pCurrentDataState->uPosition[LEFT] += iPosX;

			if (((int16)m_pCurrentDataState->uPosition[TOP] + iPosY) < 0) iPosY = 0 - (int16)m_pCurrentDataState->uPosition[TOP];
			else if ((m_pCurrentDataState->uPosition[TOP] + iPosY) > (m_uTextureSizeY - m_pCurrentDataState->uPosition[BOTTOM])) iPosY = m_uTextureSizeY - m_pCurrentDataState->uPosition[BOTTOM] - m_pCurrentDataState->uPosition[TOP];
			m_pCurrentDataState->uPosition[TOP] += iPosY;

		} else if (pWindow == m_windowMaterialOffset[RIGHT]) {
			if (((int16)m_pCurrentDataState->uPosition[RIGHT] + iPosX) < 2) iPosX = 2 - (int16)m_pCurrentDataState->uPosition[RIGHT];
			else if ((m_pCurrentDataState->uPosition[RIGHT] + iPosX) > (m_uTextureSizeX - m_pCurrentDataState->uPosition[LEFT])) iPosX = m_uTextureSizeX - m_pCurrentDataState->uPosition[RIGHT] - m_pCurrentDataState->uPosition[LEFT];
			m_pCurrentDataState->uPosition[RIGHT] += iPosX;

		} else if (pWindow == m_windowMaterialOffset[BOTTOM]) {
			if (((int16)m_pCurrentDataState->uPosition[BOTTOM] + iPosY) < 2) iPosY = 2 - (int16)m_pCurrentDataState->uPosition[BOTTOM];
			else if ((m_pCurrentDataState->uPosition[BOTTOM] + iPosY) > (m_uTextureSizeY - m_pCurrentDataState->uPosition[TOP])) iPosY = m_uTextureSizeY - m_pCurrentDataState->uPosition[BOTTOM] - m_pCurrentDataState->uPosition[TOP];
			m_pCurrentDataState->uPosition[BOTTOM] += iPosY;

		} else if (pWindow == m_windowMaterialOffset[LEFT]) {
			if (((int16)m_pCurrentDataState->uPosition[LEFT] + iPosX) < 0) iPosX = 0 - (int16)m_pCurrentDataState->uPosition[LEFT];
			else if (((int16)m_pCurrentDataState->uPosition[RIGHT] - iPosX) < 2) iPosX = (int16)m_pCurrentDataState->uPosition[RIGHT] - 2;
			m_pCurrentDataState->uPosition[LEFT] += iPosX;
			m_pCurrentDataState->uPosition[RIGHT] -= iPosX;

		} else if (pWindow == m_windowMaterialOffset[TOP]) {
			if (((int16)m_pCurrentDataState->uPosition[TOP] + iPosY) < 0) iPosY = 0 - (int16)m_pCurrentDataState->uPosition[TOP];
			else if (((int16)m_pCurrentDataState->uPosition[BOTTOM] - iPosY) < 2) iPosY = (int16)m_pCurrentDataState->uPosition[BOTTOM] - 2;
			m_pCurrentDataState->uPosition[TOP] += iPosY;
			m_pCurrentDataState->uPosition[BOTTOM] -= iPosY;

		} else return;

		updateStateInfo();


	}
}
//===================================================================================
bool SkinEditor::createEditor() // создает окно редактирования скинов
{
	destroyEditor();

	m_pCurrentDataWindow = 0; // текущее окно
	m_pCurrentDataSkin = 0; // текущий саб скин
	m_pCurrentDataState = 0; // текцщий стейт скина


	// главное окно
	m_mainWindow = basis.mGUI->createWindowFrame(500, 150, 300, 470, "Skin editor MyGUI 1.0", MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C, this);

	// статик текст над окном
	MyGUI::StaticText * text = m_mainWindow->createStaticText(10, 0, 270, 25, "Basis window skin", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	// создаем окно с именами
	m_comboBasisWindowName = m_mainWindow->createComboBox(10, 30, 270, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX, this);

	text = m_mainWindow->createStaticText(222, 60, 60, 25, "Added 1", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboBasisAddedSkin1 = m_mainWindow->createComboBox(20, 60, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST, this);
	m_comboBasisAddedSkin1->addString(NO_SET);

	text = m_mainWindow->createStaticText(222, 90, 60, 25, "Added 2", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboBasisAddedSkin2 = m_mainWindow->createComboBox(20, 90, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST, this);
	m_comboBasisAddedSkin2->addString(NO_SET);

	m_editBasisData1 = m_mainWindow->createEdit(20, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);

	m_editBasisData2 = m_mainWindow->createEdit(90, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);

	text = m_mainWindow->createStaticText(155, 120, 100, 25, "Added data", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	text = m_mainWindow->createStaticText(222, 150, 60, 25, "Font", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboBasisFont = m_mainWindow->createComboBox(20, 150, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST, this);
	for (uint8 index=0; index<MyGUI::__FONT_COUNT; index++) m_comboBasisFont->addString(basis.mGUI->m_strFontNames[index]);

	text = m_mainWindow->createStaticText(222, 180, 60, 25, "Colour", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboBasisColour = m_mainWindow->createComboBox(20, 180, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX, this);
	m_comboBasisColour->addString("COLOUR_BLACK");
	m_comboBasisColour->addString("COLOUR_WHITE");
	m_comboBasisColour->addString("COLOUR_BLUE");
	m_comboBasisColour->addString("COLOUR_GREEN");
	m_comboBasisColour->addString("COLOUR_RED");

	// заполняем всеми доступными материалами
	text = m_mainWindow->createStaticText(222, 210, 60, 25, "Material", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboMaterialName = m_mainWindow->createComboBox(20, 210, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST, this);
	m_comboMaterialName->addString(NO_SET);

	Ogre::ResourceManager::ResourceMapIterator mat = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (mat.hasMoreElements()) {
		String strMaterialName = mat.getNext()->getName();
		m_comboMaterialName->addString(strMaterialName);
		m_strMaterialName.push_back(strMaterialName);
	}


	text = m_mainWindow->createStaticText(10, 250, 270, 25, "Sab skins window", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_comboSabSkinName = m_mainWindow->createComboBox(10, 280, 270, -1, WA_TOP|WA_LEFT, SKIN_COMBO_BOX, this);

	text = m_mainWindow->createStaticText(222, 310, 60, 25, "Offset", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_editOffset[0] = m_mainWindow->createEdit(20, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editOffset[1] = m_mainWindow->createEdit(71, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editOffset[2] = m_mainWindow->createEdit(122, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editOffset[3] = m_mainWindow->createEdit(173, 310, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);

	m_buttonSabSkinStyle = m_mainWindow->createButton(20, 340, 200, -1, "SKIN STYLE", WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON, this);
	m_buttonSabSkinStyle->setFont(MyGUI::FONT_DEFAULT, MyGUI::COLOUR_BLUE);

	text = m_mainWindow->createStaticText(222, 370, 60, 25, "State", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	m_comboSabSkinState = m_mainWindow->createComboBox(20, 370, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST, this);
	m_comboSabSkinState->addString("WS_DEACTIVE");
	m_comboSabSkinState->addString("WS_NORMAL");
	m_comboSabSkinState->addString("WS_PRESSED");
	m_comboSabSkinState->addString("WS_ACTIVED");
	m_comboSabSkinState->addString("WS_SELECTED");

	text = m_mainWindow->createStaticText(222, 400, 60, 25, "Position", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_editPosition[0] = m_mainWindow->createEdit(20, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editPosition[1] = m_mainWindow->createEdit(71, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editPosition[2] = m_mainWindow->createEdit(122, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);
	m_editPosition[3] = m_mainWindow->createEdit(173, 400, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT, this);

	for (uint8 index=0; index<4; index++) {
		m_editOffset[index]->setUserData(EDIT_IS_USE);
		m_editPosition[index]->setUserData(EDIT_IS_USE);
	}

	createFlagWindow();

	// окно для материалов
	createMaterialWindow();
	
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
	enableWindowInfo(m_pCurrentDataWindow != 0);

	if (!m_pCurrentDataWindow) return; // на всякий

	// загружаем новые значения
	m_comboBasisAddedSkin1->setString(m_pCurrentDataWindow->uAddedSkin1);
	m_comboBasisAddedSkin2->setString(m_pCurrentDataWindow->uAddedSkin2);
	m_editBasisData1->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData1));
	m_editBasisData2->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData2));
	m_comboBasisFont->setString(m_pCurrentDataWindow->uFont);
	m_comboBasisColour->setWindowText(m_pCurrentDataWindow->strColour);
	m_comboMaterialName->setString(m_pCurrentDataWindow->uMaterial);

	// очищаем поля
	m_comboSabSkinName->deleteStringAll();

	// заполняем информацией об сабскинах
	for (uint8 pos=0; pos<m_pCurrentDataWindow->sabSkins.size(); pos++) {
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
	
	// заполняем окно с материалом
	if (!fillMaterialWindow()) m_comboMaterialName->setString(0);

	updateSkinInfo();

}
//===================================================================================
void SkinEditor::updateSkinInfo() // обновляет всю инфу об саб скине
{
	enableSkinInfo(m_pCurrentDataSkin != 0);

	if (!m_pCurrentDataSkin) {

		m_pCurrentDataState = 0; // сотрем стейты
		m_windowStateFlags->show(false);

	} else {

		for (uint8 pos=0; pos<4; pos++) m_editOffset[pos]->setWindowText(StringConverter::toString(m_pCurrentDataSkin->uOffset[pos]));
		m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[0];
		fillFlagWindow(); // заполняет окна текущими значения

		m_comboSabSkinState->setString(0); // на первый
		updateStateInfo();

	}

}
//===================================================================================
void SkinEditor::updateStateInfo() // обновляет всю инфу об стайте
{

	if (!m_pCurrentDataState) return; // нет инфы уходим

	for (uint8 pos=0; pos<4; pos++) { // поля для чисел
		m_editPosition[pos]->setWindowText(StringConverter::toString(m_pCurrentDataState->uPosition[pos]));
	}

	setMaterialOffset(m_pCurrentDataState->uPosition[0], m_pCurrentDataState->uPosition[1], m_pCurrentDataState->uPosition[2], m_pCurrentDataState->uPosition[3]);

}
//===================================================================================
void SkinEditor::deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin) // удаляет саб скин
{
	if (!lpDataSkin) return;
	if (!m_pCurrentDataWindow) return;

	uint16 size = (uint16)m_pCurrentDataWindow->sabSkins.size();

	for (uint8 pos=0; pos<size; pos++) {
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

	for (uint8 index=0; index<mWindowInfo.size(); index++) {
		for (uint8 pos=0; pos<mWindowInfo[index].sabSkins.size(); pos++) {
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

	for (uint8 index=0; index<mWindowInfo.size(); index++) {
		m_pCurrentDataWindow = &mWindowInfo[index];
		while (m_pCurrentDataWindow->sabSkins.size()) {
			deleteSkinData(m_pCurrentDataWindow->sabSkins[0]);
		}
	}

	mWindowInfo.clear();

	m_pCurrentDataWindow = 0; // текущее окно
	m_pCurrentDataSkin = 0; // текущий саб скин
	m_pCurrentDataState = 0; // текцщий стейт скина

}
//===================================================================================
void SkinEditor::saveSkin(const String & strFileName) // сохраняет скин
{
	std::ofstream fp;
	fp.open(strFileName.c_str(), ios_base::out | ios_base::trunc);

	String strState[5] = {VALUE_STATE_DEACTIVED, VALUE_STATE_NORMAL, VALUE_STATE_PRESSED, VALUE_STATE_ACTIVED, VALUE_STATE_SELECTED};
	vector <String> astrSubSkinName; // имена уже сохраненых саб скинов

	for (uint8 index=0; index<mWindowInfo.size(); index++) {
		LP_WINDOW_DATA window = &mWindowInfo[index];
		if (!window) continue;

		fp << "\n" << BLOCK_WINDOW_NAME << " \t" << mWindowInfo[index].strElementName << "\n{";

		if (window->uAddedSkin1) { fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN1 << " \t" << basis.mGUI->m_strSkinNames[window->uAddedSkin1]; }
		if (window->uAddedSkin2) { fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN2 << " \t" << basis.mGUI->m_strSkinNames[window->uAddedSkin2]; }

		if (window->uAddedData1) { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA1 << " \t" << StringConverter::toString(window->uAddedData1); }
		if (window->uAddedData2) { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA2 << " \t" << StringConverter::toString(window->uAddedData2); }

		if (window->uFont) {fp << "\n\t" << VALUE_WINDOW_FONT << " \t" << basis.mGUI->m_strFontNames[window->uFont];	}
		if (!window->strColour.empty()) { fp << "\n\t" << VALUE_WINDOW_COLOUR << " \t" << window->strColour; }
		if (window->uMaterial) { fp << "\n\t" << VALUE_WINDOW_MATERIAL << " \t\"" << m_strMaterialName[window->uMaterial-1] << "\""; }

		// сначала проходим и сохраняем только именна если уже было сохранение
		for (uint8 pos=0; pos<window->sabSkins.size(); pos++) {

			LP_SUB_SKIN_DATA skin = window->sabSkins[pos];
			// ищем такой же блок
			bool find = false;
			for (uint16 search=0; search<astrSubSkinName.size(); search++) {
				if (astrSubSkinName[search] == skin->strName) {
					find = true;
					search = (uint16)astrSubSkinName.size();
				}
			}
			if (!find) continue;

			// сохраняем только описание
			if (pos == 0) fp << "\n\t" << SECTION_SUB_SKIN_MAIN;
			else fp << "\n\n\t" << SECTION_SUB_SKIN;
			fp << " \t" << skin->strName;


		} // for (uint8 pos=0; pos<window->sabSkins.size(); pos++) {

		// а теперь сохраняем полностью блоки
		for (uint8 pos=0; pos<window->sabSkins.size(); pos++) {

			LP_SUB_SKIN_DATA skin = window->sabSkins[pos];

			// ищем такой же блок
			bool find = false;
			for (uint16 search=0; search<astrSubSkinName.size(); search++) {
				if (astrSubSkinName[search] == skin->strName) {
					find = true;
					search = (uint16)astrSubSkinName.size();
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
				for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagEvent.begin(); iter != basis.mGUI->m_mapFlagEvent.end(); iter ++) {
					if ( ((*iter).second & skin->event_info) != (*iter).second ) continue;
					fp << (*iter).first << "  ";
				}
			}

			// сохраняем флаги выравнивания
			fp << "\n\t\t" << VALUE_SKIN_ALIGIN << " \t"; 
			if (!skin->aligin) fp << "WA_NONE";
			else {
				for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagAligin.begin(); iter != basis.mGUI->m_mapFlagAligin.end(); iter ++) {
					if ( ((*iter).second & skin->aligin) != (*iter).second ) continue;
					fp << (*iter).first << "  ";
				}
			}

			// сохраняем флаги стиля
			fp << "\n\t\t" << VALUE_SKIN_STYLE << " \t"; 
			if (!skin->style) fp << "WES_NONE";
			else {
				for (map<String, uint16>::iterator iter = basis.mGUI->m_mapFlagStyle.begin(); iter != basis.mGUI->m_mapFlagStyle.end(); iter ++) {
					if ( ((*iter).second & skin->style) != (*iter).second ) continue;
					fp << (*iter).first << "  ";
				}
			}

			fp << "\n";

			for (uint8 iState=0; iState<5; iState++) {
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
void SkinEditor::loadSkin(const String & strFileName) // загружает скин
{
	destroySkins();

	m_comboBasisWindowName->deleteStringAll();
	m_comboBasisAddedSkin1->deleteStringAll();
	m_comboBasisAddedSkin2->deleteStringAll();
	m_comboBasisAddedSkin1->addString(NO_SET);
	m_comboBasisAddedSkin2->addString(NO_SET);

	loadINI ini;
	if (!ini.open(strFileName.c_str())) return;

	String strState[5] = {VALUE_STATE_DEACTIVED, VALUE_STATE_NORMAL, VALUE_STATE_PRESSED, VALUE_STATE_ACTIVED, VALUE_STATE_SELECTED};

	while (ini.seekNextBlock()) {

		if (ini.getBlockType() != BLOCK_WINDOW_NAME) continue;
		string strValue = ini.getBlockName();

		bool find = false;
		for (uint16 pos=0; pos<mWindowInfo.size(); pos++) {
			if (mWindowInfo[pos].strElementName == strValue) {
				find = true;
				pos = (uint16)mWindowInfo.size(); // выходим
			}
		}
		if (find) continue; // такой скин уже есть
//		if (ID >= mWindowInfo.size()) continue; // враг не пройдет

		mWindowInfo.push_back(_tag_WINDOW_DATA(strValue));
		LP_WINDOW_DATA window = &mWindowInfo.back();
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

	ini.close();

	if (mWindowInfo.size()) {
		for (uint16 pos=0; pos<mWindowInfo.size(); pos++) {
			m_comboBasisWindowName->addString(mWindowInfo[pos].strElementName);
			m_comboBasisAddedSkin1->addString(mWindowInfo[pos].strElementName);
			m_comboBasisAddedSkin2->addString(mWindowInfo[pos].strElementName);
		}
		m_comboBasisWindowName->setString(0);
		m_pCurrentDataWindow = &mWindowInfo[0];
		updateWindowInfo();
	}
}
//===================================================================================
void SkinEditor::createFlagWindow()
{
	// создаем все что связанно с окном для флагов
	m_windowStateFlags = basis.mGUI->createWindowFrame(100, 100, 650, 490, "Skin flags", MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CX, this);

	StaticText * text = m_windowStateFlags->createStaticText(10, 0, 200, 20, "EVENT", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text = m_windowStateFlags->createStaticText(220, 0, 200, 20, "ALIGIN", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text = m_windowStateFlags->createStaticText(430, 0, 200, 20, "STYLE", WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	uint8 countEvents = 5;
	uint8 countAligin = 14;
	uint8 countStyle = 8;

	for (uint8 pos=0; pos<16; pos++) {
		if (pos < countEvents) {
			m_buttonsFlagsEvent[pos] = m_windowStateFlags->createButton(10, (pos*30)+30, 200, -1, "", WA_LEFT|WA_TOP, SKIN_CHECK_BOX, this);
		} else m_buttonsFlagsEvent[pos] = 0;

		if (pos < countAligin) {
			m_buttonsFlagsAligin[pos] = m_windowStateFlags->createButton(220, (pos*30)+30, 200, -1, "", WA_LEFT|WA_TOP, SKIN_CHECK_BOX, this);
		} else m_buttonsFlagsAligin[pos] = 0;

		if (pos < countStyle) {
			m_buttonsFlagsStyle[pos] = m_windowStateFlags->createButton(430, (pos*30)+30, 200, -1, "", WA_LEFT|WA_TOP, SKIN_CHECK_BOX, this);
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
	SET_FLAG(m_buttonsFlagsStyle[2], WES_ACTION1, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[3], WES_ACTION2, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[4], WES_ACTION3, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[5], WES_ACTION4, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[6], WES_ACTION5, FLAG_STYLE);
	SET_FLAG(m_buttonsFlagsStyle[7], WES_ACTION6, FLAG_STYLE);

	m_windowStateFlags->show(false);

}
//===================================================================================
void SkinEditor::pressOtherButton(MyGUI::Window * pWindow) // сверяем с кнопками флагов
{
	if (!m_pCurrentDataSkin) return;

	uint32 type = pWindow->getUserData() & 0xF0000;
	uint16 * ptr;

	if (type == FLAG_EVENT) ptr = &m_pCurrentDataSkin->event_info;
	else if (type == FLAG_STYLE) ptr = &m_pCurrentDataSkin->style;
	else if (type == FLAG_ALIGIN) ptr = &m_pCurrentDataSkin->aligin;
	else return;

	uint16 flag = pWindow->getUserData() & 0xFFFF;

	if (pWindow->getState() == WS_PRESSED) {
		pWindow->setState(WS_NORMAL);
		(*ptr) &= ~(flag);
	} else {
		pWindow->setState(WS_PRESSED);
		(*ptr) |= flag; 
	}
}
//===================================================================================
void SkinEditor::fillFlagWindow() // заполняет окна текущими значения
{
	if (!m_pCurrentDataSkin) return;

	// сначала очищаем все поля
	for (uint8 pos=0; pos<16; pos++) {
		if (m_buttonsFlagsEvent[pos]) m_buttonsFlagsEvent[pos]->setState(WS_NORMAL);
		if (m_buttonsFlagsAligin[pos]) m_buttonsFlagsAligin[pos]->setState(WS_NORMAL);
		if (m_buttonsFlagsStyle[pos]) m_buttonsFlagsStyle[pos]->setState(WS_NORMAL);
	}

	// теперь будем искать совпадения и отмечать
	for (uint8 pos=0; pos<16; pos++) {

		if (m_pCurrentDataSkin->event_info & (1<<pos)) {
			// а вот теперь ищем
			uint32 find = (1<<pos) | FLAG_EVENT;
			for (uint8 i=0; i<16; i++) {
				if (!m_buttonsFlagsEvent[i]) continue;
				if (m_buttonsFlagsEvent[i]->getUserData() == find) m_buttonsFlagsEvent[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->event_info & (1<<pos)) {

		if (m_pCurrentDataSkin->aligin & (1<<pos)) {
			// а вот теперь ищем
			uint32 find = (1<<pos) | FLAG_ALIGIN;
			for (uint8 i=0; i<16; i++) {
				if (!m_buttonsFlagsAligin[i]) continue;
				if (m_buttonsFlagsAligin[i]->getUserData() == find) m_buttonsFlagsAligin[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->aligin & (1<<pos)) {

		if (m_pCurrentDataSkin->style & (1<<pos)) {
			// а вот теперь ищем
			uint32 find = (1<<pos) | FLAG_STYLE;
			for (uint8 i=0; i<16; i++) {
				if (!m_buttonsFlagsStyle[i]) continue;
				if (m_buttonsFlagsStyle[i]->getUserData() == find) m_buttonsFlagsStyle[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->style & (1<<pos)) {

	} // for (uint8 pos=0; pos<16; pos++) {

}
//===================================================================================
bool SkinEditor::fillMaterialWindow() // заполняем окно с материалом
{

	if (!m_pCurrentDataWindow) return false;
	m_pCurrentDataWindow->strMaterialName = "";
	m_pCurrentDataWindow->sizeTextureX = 0;
	m_pCurrentDataWindow->sizeTextureY = 0;

	// сначала скрываем
	m_windowMaterial->show(false);

	if (m_comboMaterialName->getCurrentIndex() == 0) return false;

	const String & strMaterialName = m_comboMaterialName->getWindowText();

	MaterialPtr mat = MaterialManager::getSingleton().getByName(strMaterialName);
	if (mat.isNull()) return false;

	// обязательно загружаем
	mat->load();

	// только так, иначе при пустых викидывает
	Material::TechniqueIterator iTechnique = mat->getTechniqueIterator();
	if ( ! iTechnique.hasMoreElements() ) return false;

	Pass * pass = iTechnique.getNext()->getPass(0);
	if (!pass) return false;

	Pass::TextureUnitStateIterator iUnit = pass->getTextureUnitStateIterator();
	if ( ! iUnit.hasMoreElements()) return false;

	const String & textName = iUnit.getNext()->getTextureName();

	TexturePtr tex = (TexturePtr)TextureManager::getSingleton().getByName(textName);
	if (tex.isNull()) return false;

	const uint16 addX = m_windowMaterial->m_iSizeX-m_windowMaterial->m_pWindowClient->m_iSizeX;
	const uint16 addY = m_windowMaterial->m_iSizeY-m_windowMaterial->m_pWindowClient->m_iSizeY;

	m_windowMaterial->show(true);
	m_pCurrentDataWindow->sizeTextureX = m_uTextureSizeX = (uint16)tex->getWidth();
	m_pCurrentDataWindow->sizeTextureY = m_uTextureSizeY = (uint16)tex->getHeight();
	m_windowMaterial->size( m_uTextureSizeX+addX, m_uTextureSizeY+addY );
	m_windowMaterial->m_pWindowClient->m_overlayContainer->setMaterialName(strMaterialName);
	m_pCurrentDataWindow->strMaterialName = strMaterialName;

	return true;
}
//===================================================================================
void SkinEditor::createMaterialWindow() // создает окна для материала
{
	m_windowMaterial = basis.mGUI->createWindowFrame(100, 100, 200, 200, "Material view", MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C, this);
	m_windowMaterial->show(false);

	for (uint8 pos=0; pos<MATERIAL_BORDER_COUNT; pos++) {
		m_windowMaterialOffset[pos] = m_windowMaterial->createWindow(10, 10, 1, 1, WA_LEFT|WA_TOP, SKIN_DEFAULT, this);
		m_windowMaterialOffset[pos]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		if (pos != CENTER) m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_GREEN");
		m_windowMaterialOffset[pos]->setUserData(MATERIAL_OFFSET);
	}

}
//===================================================================================
void SkinEditor::setMaterialOffset(uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY) // сдвигаем рамку
{
	if ( (!sizeX) || (!sizeY) || (m_windowMaterial->m_pWindowClient->m_iSizeX < (posX+sizeX)) || (m_windowMaterial->m_pWindowClient->m_iSizeY < (posY+sizeY)) ) {
		for (uint8 pos=0; pos<MATERIAL_BORDER_COUNT; pos++) m_windowMaterialOffset[pos]->show(false);
		return;
	}

	for (uint8 pos=0; pos<MATERIAL_BORDER_COUNT; pos++) m_windowMaterialOffset[pos]->show(true);

	sizeX --;
	sizeY --;
	m_windowMaterialOffset[LEFT]->move(posX, posY);
	m_windowMaterialOffset[LEFT]->size(1, sizeY);
	m_windowMaterialOffset[TOP]->move(posX+1, posY);
	m_windowMaterialOffset[TOP]->size(sizeX, 1);

	m_windowMaterialOffset[RIGHT]->move(sizeX+posX, posY+1);
	m_windowMaterialOffset[RIGHT]->size(1, sizeY);
	m_windowMaterialOffset[BOTTOM]->move(posX, sizeY+posY);
	m_windowMaterialOffset[BOTTOM]->size(sizeX, 1);

	m_windowMaterialOffset[CENTER]->move(posX+1, posY+1);
	m_windowMaterialOffset[CENTER]->size(sizeX-1, sizeY-1);
}
//===================================================================================
void SkinEditor::enableSkinInfo(bool bEnable) // блокирует
{
	static bool _bIsEnabled = true;
	if (bEnable == _bIsEnabled) return;
	_bIsEnabled = bEnable;

	// разблокируем поля
	if (bEnable) {
		m_buttonSabSkinStyle->setState(WS_NORMAL);
		m_comboSabSkinState->setState(WS_NORMAL);
		for (uint8 pos=0; pos<4; pos++) {
			m_editOffset[pos]->setState(WS_NORMAL);
			m_editPosition[pos]->setState(WS_NORMAL);
		}

	// заблокируем
	} else {
		m_buttonSabSkinStyle->setState(WS_DEACTIVE);
		m_comboSabSkinState->setWindowText("");
		m_comboSabSkinState->setState(WS_DEACTIVE);
		for (uint8 pos=0; pos<4; pos++) {
			m_editOffset[pos]->setWindowText("");
			m_editOffset[pos]->setState(WS_DEACTIVE);
			m_editPosition[pos]->setWindowText("");
			m_editPosition[pos]->setState(WS_DEACTIVE);
		}
	}
}
//===================================================================================
void SkinEditor::enableWindowInfo(bool bEnable) // блокирует все окна
{
	static bool _bIsEnabled = true;
	if (bEnable == _bIsEnabled) return;
	_bIsEnabled = bEnable;

	// разблокируем поля
	if (bEnable) {

		m_comboSabSkinName->setState(WS_NORMAL);
		m_comboMaterialName->setState(WS_NORMAL);
		m_comboBasisColour->setState(WS_NORMAL);
		m_comboBasisFont->setState(WS_NORMAL);
		m_editBasisData1->setState(WS_NORMAL);
		m_editBasisData2->setState(WS_NORMAL);
		m_comboBasisAddedSkin1->setState(WS_NORMAL);
		m_comboBasisAddedSkin2->setState(WS_NORMAL);

		// заблокируем
	} else {

		enableSkinInfo(false);
		m_comboSabSkinName->setWindowText("");
		m_comboSabSkinName->setState(WS_DEACTIVE);
		m_comboMaterialName->setWindowText("");
		m_comboMaterialName->setState(WS_DEACTIVE);
		m_comboBasisColour->setWindowText("");
		m_comboBasisColour->setState(WS_DEACTIVE);
		m_comboBasisFont->setWindowText("");
		m_comboBasisFont->setState(WS_DEACTIVE);
		m_editBasisData1->setWindowText("");
		m_editBasisData1->setState(WS_DEACTIVE);
		m_editBasisData2->setWindowText("");
		m_editBasisData2->setState(WS_DEACTIVE);
		m_comboBasisAddedSkin1->setWindowText("");
		m_comboBasisAddedSkin1->setState(WS_DEACTIVE);
		m_comboBasisAddedSkin2->setWindowText("");
		m_comboBasisAddedSkin2->setState(WS_DEACTIVE);
		m_comboBasisWindowName->setWindowText("");

	}

}
//===================================================================================
