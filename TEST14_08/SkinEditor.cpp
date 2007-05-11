#include "SkinEditor.h"
#include "BasisManager.h"
#include "MyGUI_Source\\MyGUI_LoadINI.h"
#include <OgreStringConverter.h>
#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>

using namespace Ogre;
using namespace MyGUI;
using namespace std;

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
const string VALUE_SKIN_ALIGN = "align";
const string VALUE_SKIN_STYLE = "style";
const string VALUE_STATE_DEACTIVED = "deactived_position";
const string VALUE_STATE_NORMAL = "normal_position";
const string VALUE_STATE_PRESSED = "pressed_position";
const string VALUE_STATE_ACTIVED = "actived_position";
const string VALUE_STATE_SELECTED = "selected_position";
//===================================================================================
const uint32 FLAG_EVENT = 0x10000;
const uint32 FLAG_STYLE = 0x20000;
const uint32 FLAG_ALIGN = 0x40000;
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
					m_comboBasisAddedSkin1->addString(strName);
					m_comboBasisAddedSkin2->addString(strName);
					mWindowInfo.push_back(_tag_WINDOW_DATA(strName));
					m_comboBasisWindowName->setString(m_comboBasisWindowName->getStringCount()-1);
					m_pCurrentDataWindow = &mWindowInfo.back();
					updateWindowInfo();
				} // if (!find) {

			}

			// выбо саб скина
		} else  if (pWindow == m_comboSabSkinName) {

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
			m_pCurrentDataWindow->strAddedSkin1 = m_comboBasisAddedSkin1->getWindowText();

		} else if (pWindow == m_comboBasisAddedSkin2) {
			m_pCurrentDataWindow->strAddedSkin2 = m_comboBasisAddedSkin2->getWindowText();

		} else if (pWindow == m_comboBasisFont) {
			m_pCurrentDataWindow->strFont = m_comboBasisFont->getWindowText();

		} else if (pWindow == m_comboBasisColour) {
			m_pCurrentDataWindow->strColour = m_comboBasisColour->getWindowText();

		// выбор материала
		} else if (pWindow == m_comboMaterialName) {
			if (m_pCurrentDataSkin) {
				if (!fillMaterialWindow()) {
					m_comboMaterialName->setString(0);
					m_pCurrentDataWindow->strMaterialName = "";
				} else m_pCurrentDataWindow->strMaterialName = m_comboMaterialName->getWindowText();
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


	// скрываем окно стилей
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
				m_comboBasisAddedSkin1->deleteString(ID+1);
				m_comboBasisAddedSkin1->setString(0);
				m_comboBasisAddedSkin2->deleteString(ID+1);
				m_comboBasisAddedSkin2->setString(0);
				
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
			GUI::getSingleton()->upZOrder(m_windowStateFlags);
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
				if (bIsFocus) m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_YELLOW");
				else m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_GREEN");
			}
		} else {
			if (bIsFocus) pWindow->m_overlayContainer->setMaterialName("BACK_YELLOW");
			else pWindow->m_overlayContainer->setMaterialName("BACK_GREEN");
		}

	}
}
//===================================================================================
void SkinEditor::onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iParentPosX, int16 iParentPosY) // уведомление о движении, но не движение
{
	if ( ! m_pCurrentDataState) return;

	if (pWindow->getUserData() == MATERIAL_OFFSET) {
		
		iParentPosX -= GUI::getSingleton()->m_iOffsetPressedX;
		iParentPosY -= GUI::getSingleton()->m_iOffsetPressedY;
		iPosX = iParentPosX - pWindow->m_iPosX;
		iPosY = iParentPosY - pWindow->m_iPosY;

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
	//GUI::getSingleton()->spawn<Window>(0,0, 400, 400, OVERLAY_OVERLAPPED, SKIN_WINDOWFRAME_CSX);
	
	m_mainWindow = GUI::getSingleton()->spawn<WindowFrame>(
	    500, 150, 300, 470, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CSX);
	m_mainWindow->setWindowText("Skin editor MyGUI 1.0");
	m_mainWindow->m_pEventCallback = this;

	// статик текст над окном
	MyGUI::StaticText * text = m_mainWindow->spawn<StaticText>(10, 0, 270, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text->setWindowText("Basis window skin");

	// создаем окно с именами
	m_comboBasisWindowName = m_mainWindow->spawn<ComboBox>(10, 30, 270, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX);
	m_comboBasisWindowName->m_pEventCallback = this;

	text = m_mainWindow->spawn<StaticText>(222, 60, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text->setWindowText("Added 1");
	
	m_comboBasisAddedSkin1 = m_mainWindow->spawn<ComboBox>(20, 60, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboBasisAddedSkin1->addString(NO_SET);
	m_comboBasisAddedSkin1->m_pEventCallback = this;

	text = m_mainWindow->spawn<StaticText>(222, 90, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text->setWindowText("Added 2");
	
	m_comboBasisAddedSkin2 = m_mainWindow->spawn<ComboBox>(20, 90, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboBasisAddedSkin2->addString(NO_SET);
	m_comboBasisAddedSkin2->m_pEventCallback = this;

	m_editBasisData1 = m_mainWindow->spawn<Edit>(20, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
    m_editBasisData1->m_pEventCallback = this;
	m_editBasisData2 = m_mainWindow->spawn<Edit>(90, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editBasisData2->m_pEventCallback = this;

	text = m_mainWindow->spawn<StaticText>(155, 120, 100, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text->setWindowText("Added data");

	text = m_mainWindow->spawn<StaticText>(222, 150, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text->setWindowText("Font");
	
	m_comboBasisFont = m_mainWindow->spawn<ComboBox>(20, 150, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboBasisFont->m_pEventCallback = this;
	for (uint8 index=0; index < MyGUI::__FONT_COUNT; index++)
	    m_comboBasisFont->addString(GUI::getSingleton()->m_strFontNames[index]);

	text = m_mainWindow->spawn<StaticText>(222, 180, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("Colour");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	
	m_comboBasisColour = m_mainWindow->spawn<ComboBox>(20, 180, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX);
	m_comboBasisColour->m_pEventCallback = this;
	m_comboBasisColour->addString("COLOUR_BLACK");
	m_comboBasisColour->addString("COLOUR_WHITE");
	m_comboBasisColour->addString("COLOUR_BLUE");
	m_comboBasisColour->addString("COLOUR_GREEN");
	m_comboBasisColour->addString("COLOUR_RED");

	// заполняем всеми доступными материалами
	text = m_mainWindow->spawn<StaticText>(222, 210, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("Material");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	
	m_comboMaterialName = m_mainWindow->spawn<ComboBox>(20, 210, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboMaterialName->m_pEventCallback = this;
	m_comboMaterialName->addString(NO_SET);

	Ogre::ResourceManager::ResourceMapIterator mat = Ogre::MaterialManager::getSingleton().getResourceIterator();
	while (mat.hasMoreElements()) {
		String strMaterialName = mat.getNext()->getName();
		m_comboMaterialName->addString(strMaterialName);
		m_strMaterialName.push_back(strMaterialName);
	}


	text = m_mainWindow->spawn<StaticText>(10, 250, 270, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("Sab skins window");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_comboSabSkinName = m_mainWindow->spawn<ComboBox>(10, 280, 270, -1, WA_TOP|WA_LEFT, SKIN_COMBO_BOX);
    m_comboSabSkinName->m_pEventCallback = this;
    
	text = m_mainWindow->spawn<StaticText>(222, 310, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("Offset");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_editOffset[0] = m_mainWindow->spawn<Edit>(20, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[1] = m_mainWindow->spawn<Edit>(71, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[2] = m_mainWindow->spawn<Edit>(122, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editOffset[3] = m_mainWindow->spawn<Edit>(173, 310, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	
	m_editOffset[0]->m_pEventCallback = this;
	m_editOffset[1]->m_pEventCallback = this;
	m_editOffset[2]->m_pEventCallback = this;
	m_editOffset[3]->m_pEventCallback = this;

	m_buttonSabSkinStyle = m_mainWindow->spawn<Button>(20, 340, 200, -1, WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
	m_buttonSabSkinStyle->setWindowText("SKIN STYLE");
	m_buttonSabSkinStyle->m_pEventCallback = this;
	m_buttonSabSkinStyle->setFont(MyGUI::FONT_DEFAULT, MyGUI::COLOUR_BLUE);

	text = m_mainWindow->spawn<StaticText>(222, 370, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("State");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	
	m_comboSabSkinState = m_mainWindow->spawn<ComboBox>(20, 370, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
	m_comboSabSkinState->m_pEventCallback = this;
	m_comboSabSkinState->addString("WS_DEACTIVE");
	m_comboSabSkinState->addString("WS_NORMAL");
	m_comboSabSkinState->addString("WS_PRESSED");
	m_comboSabSkinState->addString("WS_ACTIVED");
	m_comboSabSkinState->addString("WS_SELECTED");

	text = m_mainWindow->spawn<StaticText>(222, 400, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("Position");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	m_editPosition[0] = m_mainWindow->spawn<Edit>(20, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[1] = m_mainWindow->spawn<Edit>(71, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[2] = m_mainWindow->spawn<Edit>(122, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	m_editPosition[3] = m_mainWindow->spawn<Edit>(173, 400, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
	
	m_editPosition[0]->m_pEventCallback = this;
	m_editPosition[1]->m_pEventCallback = this;
	m_editPosition[2]->m_pEventCallback = this;
	m_editPosition[3]->m_pEventCallback = this;

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
	GUI::getSingleton()->destroyWindow(m_windowStateFlags);
	GUI::getSingleton()->destroyWindow(m_mainWindow);
	destroySkins();
	// информация об окнах
	mWindowInfo.clear();
}
//===================================================================================
void SkinEditor::updateWindowInfo()
{
	enableWindowInfo(m_pCurrentDataWindow != 0);

	if (!m_pCurrentDataWindow) return; // на всякий

	if (m_pCurrentDataWindow->strAddedSkin1.empty()) m_comboBasisAddedSkin1->setWindowText(NO_SET);
	else m_comboBasisAddedSkin1->setWindowText(m_pCurrentDataWindow->strAddedSkin1);
	if (m_pCurrentDataWindow->strAddedSkin2.empty()) m_comboBasisAddedSkin2->setWindowText(NO_SET);
	else m_comboBasisAddedSkin2->setWindowText(m_pCurrentDataWindow->strAddedSkin2);
	m_comboBasisFont->setWindowText(m_pCurrentDataWindow->strFont);

	m_editBasisData1->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData1));
	m_editBasisData2->setWindowText(StringConverter::toString(m_pCurrentDataWindow->uAddedData2));
	m_comboBasisColour->setWindowText(m_pCurrentDataWindow->strColour);
	m_comboMaterialName->setWindowText(m_pCurrentDataWindow->strMaterialName);

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

		if (!window->strAddedSkin1.empty() && (window->strAddedSkin1 != NO_SET)) {
			fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN1 << " \t" << window->strAddedSkin1;
		}
		if (!window->strAddedSkin2.empty() && (window->strAddedSkin2 != NO_SET)) {
			fp << "\n\t" << VALUE_WINDOW_ADDED_SKIN2 << " \t" << window->strAddedSkin2;
		}

		if (window->uAddedData1) { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA1 << " \t" << StringConverter::toString(window->uAddedData1); }
		if (window->uAddedData2) { fp << "\n\t" << VALUE_WINDOW_ADDED_DATA2 << " \t" << StringConverter::toString(window->uAddedData2); }

		if (!window->strFont.empty()) {fp << "\n\t" << VALUE_WINDOW_FONT << " \t" << window->strFont;}
		if (!window->strColour.empty()) { fp << "\n\t" << VALUE_WINDOW_COLOUR << " \t" << window->strColour; }
		if (!window->strMaterialName.empty()) { fp << "\n\t" << VALUE_WINDOW_MATERIAL << " \t\"" << window->strMaterialName << "\""; }

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
				for (map<String, uint16>::iterator iter = GUI::getSingleton()->m_mapFlagEvent.begin();
				    iter != GUI::getSingleton()->m_mapFlagEvent.end(); iter ++) {
					if ( iter->second & skin->event_info ) {
						if ( ( iter->second & skin->event_info ) == iter->second )
							fp << iter->first << "  ";
					}
				}
			}

			// сохраняем флаги выравнивания
			fp << "\n\t\t" << VALUE_SKIN_ALIGN << " \t"; 
			if (!skin->align) fp << "WA_NONE";
			else {
				for (map<String, uint16>::iterator iter = GUI::getSingleton()->m_mapFlagAlign.begin();
				        iter != GUI::getSingleton()->m_mapFlagAlign.end(); iter ++) {
					if ( iter->second & skin->align ) {
						if ( ( iter->second & skin->align ) == iter->second )
							fp << iter->first << "  ";
					}
				}
			}

			// сохраняем флаги стиля
			fp << "\n\t\t" << VALUE_SKIN_STYLE << " \t"; 
			if (!skin->style) fp << "WES_NONE";
			else {
				for (map<String, uint16>::iterator iter = GUI::getSingleton()->m_mapFlagStyle.begin();
				    iter != GUI::getSingleton()->m_mapFlagStyle.end(); iter ++) {
					if ( iter->second & skin->style ) {
						if ( ( iter->second & skin->style ) == iter->second )
							fp << iter->first << "  ";
					}
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

	String strState[5] = {
	    VALUE_STATE_DEACTIVED,
	    VALUE_STATE_NORMAL,
	    VALUE_STATE_PRESSED,
	    VALUE_STATE_ACTIVED,
	    VALUE_STATE_SELECTED};

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

		mWindowInfo.push_back(_tag_WINDOW_DATA(strValue));
		LP_WINDOW_DATA window = &mWindowInfo.back();
		// сбрасываем все данные
		window->strColour = "";
		window->uAddedData1 = 0;
		window->uAddedData2 = 0;

		string strValueName;
		uint32 uValue;
		while (ini.LoadNextValue()) {
			if (!ini.getValueName(strValueName)) continue;
			
			if (strValueName == VALUE_WINDOW_ADDED_SKIN1) {
				if (ini.getValue(strValue)) window->strAddedSkin1 = strValue;

			} else if (strValueName == VALUE_WINDOW_ADDED_SKIN2) {
				if (ini.getValue(strValue)) window->strAddedSkin2 = strValue;

			} else if (strValueName == VALUE_WINDOW_ADDED_DATA1) {
				if (ini.getValue(uValue)) {
					if (uValue <= 255) window->uAddedData1 = uValue;
				}

			} else if (strValueName == VALUE_WINDOW_ADDED_DATA2) {
				if (ini.getValue(uValue)) {
					if (uValue <= 255) window->uAddedData2 = uValue;
				}

			} else if (strValueName == VALUE_WINDOW_FONT) {
				if (ini.getValue(strValue)) window->strFont = strValue;

			} else if (strValueName == VALUE_WINDOW_COLOUR) {
				if (ini.getValue(strValue)) window->strColour = strValue;

			} else if (strValueName == VALUE_WINDOW_MATERIAL) {
				if (ini.getValue(strValue)) window->strMaterialName = strValue;

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
		if (ini.jumpBlock(true, false)) { // впрыгиваем в блок

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
							skin->event_info |= GUI::getSingleton()->m_mapFlagEvent[strValue];
						}
					} else if (strValueName == VALUE_SKIN_ALIGN) {
						for (uint8 pos=0; pos<16; pos++) {
							if (!ini.getValue(strValue, pos)) continue;
							skin->align |= GUI::getSingleton()->m_mapFlagAlign[strValue];
						}
					} else if (strValueName == VALUE_SKIN_STYLE) {
						for (uint8 pos=0; pos<16; pos++) {
							if (!ini.getValue(strValue, pos)) continue;
							skin->style |= GUI::getSingleton()->m_mapFlagStyle[strValue];
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

		} // if (ini.jumpBlock(true, false)) {

		ini.jumpBlock(false, false); // выпрыгиваем

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
	m_windowStateFlags = GUI::getSingleton()->spawn<WindowFrame>(
	    100, 100, 650, 490, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CX);
    m_windowStateFlags->setWindowText("Skin flags");
    m_windowStateFlags->m_pEventCallback = this;
    
	StaticText * text = m_windowStateFlags->spawn<StaticText>(10, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("EVENT");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text = m_windowStateFlags->spawn<StaticText>(220, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("ALIGN");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);
	text = m_windowStateFlags->spawn<StaticText>(430, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
	text->setWindowText("STYLE");
	text->setFont(text->m_font, MyGUI::COLOUR_GREEN);

	uint8 countEvents = 5;
	uint8 countAlign = 14;
	uint8 countStyle = 8;

	for (uint8 pos=0; pos<16; pos++) {
		if (pos < countEvents) {
			m_buttonsFlagsEvent[pos] = m_windowStateFlags->spawn<Button>(10, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
			m_buttonsFlagsEvent[pos]->m_pEventCallback = this;
		} else m_buttonsFlagsEvent[pos] = 0;

		if (pos < countAlign) {
			m_buttonsFlagsAlign[pos] = m_windowStateFlags->spawn<Button>(220, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
			m_buttonsFlagsAlign[pos]->m_pEventCallback = this;
		} else m_buttonsFlagsAlign[pos] = 0;

		if (pos < countStyle) {
			m_buttonsFlagsStyle[pos] = m_windowStateFlags->spawn<Button>(430, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
			m_buttonsFlagsStyle[pos]->m_pEventCallback = this;
		} else m_buttonsFlagsStyle[pos] = 0;
	}

	#define SET_FLAG(win,num,flag) if(win){win->setWindowText(#num);win->setUserData(num|flag);}

	SET_FLAG(m_buttonsFlagsEvent[0], WE_MOUSE_BUTTON, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[1], WE_MOUSE_FOCUS, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[2], WE_MOUSE_MOVE, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[3], WE_KEY_FOCUS, FLAG_EVENT);
	SET_FLAG(m_buttonsFlagsEvent[4], WE_KEY_BUTTON, FLAG_EVENT);

	SET_FLAG(m_buttonsFlagsAlign[0], WA_LEFT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[1], WA_RIGHT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[2], WA_TOP, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[3], WA_BOTTOM, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[4], WAT_LEFT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[5], WAT_RIGHT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[6], WAT_TOP, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[7], WAT_BOTTOM, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[8], WAT_CUT_RIGHT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[9], WAT_CUT_LEFT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[10], WAT_ADD_DOT, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[11], WAT_MULTI_LINE, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[12], WAT_BREAK_WORD, FLAG_ALIGN);
	SET_FLAG(m_buttonsFlagsAlign[13], WAT_SHIFT_TEXT_PRESSED, FLAG_ALIGN);

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
	else if (type == FLAG_ALIGN) ptr = &m_pCurrentDataSkin->align;
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
		if (m_buttonsFlagsAlign[pos]) m_buttonsFlagsAlign[pos]->setState(WS_NORMAL);
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

		if (m_pCurrentDataSkin->align & (1<<pos)) {
			// а вот теперь ищем
			uint32 find = (1<<pos) | FLAG_ALIGN;
			for (uint8 i=0; i<16; i++) {
				if (!m_buttonsFlagsAlign[i]) continue;
				if (m_buttonsFlagsAlign[i]->getUserData() == find) m_buttonsFlagsAlign[i]->setState(WS_PRESSED);
			}
		} // if (m_pCurrentDataSkin->align & (1<<pos)) {

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
	m_windowMaterial = GUI::getSingleton()->spawn<WindowFrame>(
	    100, 100, 200, 200, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C);
	m_windowMaterial->setWindowText("Material view");
	m_windowMaterial->m_pEventCallback = this;
	m_windowMaterial->show(false);

	for (uint8 pos=0; pos<MATERIAL_BORDER_COUNT; pos++) {
		m_windowMaterialOffset[pos] = m_windowMaterial->spawn<Window>(10, 10, 1, 1, WA_LEFT|WA_TOP, SKIN_DEFAULT);
		m_windowMaterialOffset[pos]->m_pEventCallback = this;
		m_windowMaterialOffset[pos]->addEvent(WE_MOUSE_FOCUS|WE_MOUSE_MOVE);
		if (pos != CENTER)
		    m_windowMaterialOffset[pos]->m_overlayContainer->setMaterialName("BACK_GREEN");
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
