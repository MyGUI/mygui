#include "SkinEditor.h"
#include "BasisManager.h"
#include "MyGUI_Source\\MyGUI_LoadINI.h"
#include "MyGUI_Source\\MyGUI_AssetManager.h"
#include <OgreStringConverter.h>
#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include "StretchControl.h"

#include "debugOut.h"

namespace SkinEditor
{
	using namespace Ogre;
	using namespace MyGUI;
	using namespace std;

	//===================================================================================
	const string NO_SET = "[ no set ]";
	//===================================================================================
	const uint32 FLAG_EVENT = 0x10000;
	const uint32 FLAG_STYLE = 0x20000;
	const uint32 FLAG_ALIGN = 0x40000;
//	const uint8 ELEMENT_VIEW_OFFSET = 10; // сдвигаем элемент для восприятия
	//===================================================================================
	SkinEditor::SkinEditor(MyGUI::EventCallback * pParent) :
		m_mainWindow(0),
		m_windowElement(0),
		m_windowMaterial(0),
		m_windowStateFlags(0)
	{
		createEditor(pParent);
	}
	//===================================================================================
	SkinEditor::~SkinEditor()
	{
		destroyEditor();
	}
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

					const String & strName = m_comboBasisWindowName->getCaption();
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
					const String & strSkinName = m_comboSabSkinName->getCaption();
					for (uint16 pos=0; pos<m_comboSabSkinName->getStringCount(); pos++) {
						if (m_comboSabSkinName->getString(pos) == strSkinName) {
							find = true;
							pos = m_comboSabSkinName->getStringCount();
						}
					}

					if (!find) {
						// создаем информацию об новом сабскине
						LP_SUB_SKIN_DATA lpData = findSkinData(m_comboSabSkinName->getCaption());
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
				m_pCurrentDataWindow->strAddedSkin1 = m_comboBasisAddedSkin1->getCaption();

			} else if (pWindow == m_comboBasisAddedSkin2) {
				m_pCurrentDataWindow->strAddedSkin2 = m_comboBasisAddedSkin2->getCaption();

			} else if (pWindow == m_comboBasisFont) {
				m_pCurrentDataWindow->strFont = m_comboBasisFont->getCaption();

			} else if (pWindow == m_comboBasisColour) {
				m_pCurrentDataWindow->strColour = m_comboBasisColour->getCaption();

			// выбор материала
			} else if (pWindow == m_comboMaterialName) {
				if (m_pCurrentDataSkin) {
					if (!fillMaterialWindow()) {
						m_comboMaterialName->setString(0);
						m_pCurrentDataWindow->strMaterialName = "";
					} else m_pCurrentDataWindow->strMaterialName = m_comboMaterialName->getCaption();
				} else m_comboMaterialName->setString(0); // низя
			}

		// ввод в поле редактирования
		} else if (uEvent == WOE_EDIT_KEY_ACCEPT) {

			if (pWindow == m_editBasisData1) {
				uint32 tmp = StringConverter::parseUnsignedInt(m_editBasisData1->getCaption());
				(tmp > 255) ? m_pCurrentDataWindow->uAddedData1 = 0 : m_pCurrentDataWindow->uAddedData1 = tmp;
				m_editBasisData1->setCaption(StringConverter::toString(m_pCurrentDataWindow->uAddedData1));

			} else if (pWindow == m_editBasisData2) {
				uint32 tmp = StringConverter::parseUnsignedInt(m_editBasisData2->getCaption());
				(tmp > 255) ? m_pCurrentDataWindow->uAddedData2 = 0 : m_pCurrentDataWindow->uAddedData2 = tmp;
				m_editBasisData2->setCaption(StringConverter::toString(m_pCurrentDataWindow->uAddedData2));

			} else if (pWindow == m_comboBasisColour) {
				m_pCurrentDataWindow->strColour = m_comboBasisColour->getCaption();

			} else if (pWindow->getUserData() == EDIT_IS_USE ) { // наше

				for (uint8 index=0; index<4; index++) {
					if (pWindow == m_editOffset[index]) {
						m_pCurrentDataSkin->uOffset[index] = StringConverter::parseUnsignedInt(pWindow->getCaption());
						pWindow->setCaption(StringConverter::toString(m_pCurrentDataSkin->uOffset[index]));
					} else if (pWindow == m_editPosition[index]) {
						m_pCurrentDataState->uPosition[index] = StringConverter::parseUnsignedInt(pWindow->getCaption());
						pWindow->setCaption(StringConverter::toString(m_pCurrentDataState->uPosition[index]));
					}
				}

				// обновляем квадратик в текстуре
				m_textureOffsetPointer->setLocation(m_pCurrentDataState->uPosition[EDIT_LEFT], m_pCurrentDataState->uPosition[EDIT_TOP], m_pCurrentDataState->uPosition[EDIT_RIGHT], m_pCurrentDataState->uPosition[EDIT_BOTTOM]);
				// обновляем квадратик в просмотре
				m_elementOffsetPointer->setLocation(m_pCurrentDataSkin->uOffset[EDIT_LEFT], m_pCurrentDataSkin->uOffset[EDIT_TOP], m_pCurrentDataSkin->uOffset[EDIT_RIGHT], m_pCurrentDataSkin->uOffset[EDIT_BOTTOM]);

				// размер окна просмотра элемента
				updateSizeElementView();
			}


		// скрываем окно стилей
		} else if (uEvent == WOE_FRAME_CLOSE) {
			if (pWindow == m_windowStateFlags) m_windowStateFlags->hide();

		} else if (uEvent == WOE_EDIT_KEY_DELETE) {
			// удаляем сабскин
			if (pWindow == m_comboSabSkinName) {
				if (m_pCurrentDataSkin) {
					deleteSkinData(m_pCurrentDataSkin); // удаляет если нужно
					updateWindowInfo();
				}

			// удаляем элемент
			} else if (pWindow == m_comboBasisWindowName) {

				const String & strName = m_comboBasisWindowName->getCaption();
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
		if (pWindow == m_buttonSkinLoad) {
			loadSkin("test.mygui_skin");
			return;
		} else if (pWindow == m_buttonSkinSave) {
			saveSkin("test.mygui_skin");
			return;
		}

		if (!m_pCurrentDataWindow) return; // на всякий

		if (pWindow == m_buttonSabSkinStyle) {
			if (m_pCurrentDataSkin) {
				m_windowStateFlags->show();
				GUI::getSingleton()->upZOrder(m_windowStateFlags);
			}
		} else {
			// остальные кнопки
			pressOtherButton(pWindow);
		}
		
	}
	//===================================================================================
	bool SkinEditor::createEditor(MyGUI::EventCallback * pParent) // создает окно редактирования скинов
	{

		destroyEditor();

		m_pCurrentDataWindow = 0; // текущее окно
		m_pCurrentDataSkin = 0; // текущий саб скин
		m_pCurrentDataState = 0; // текцщий стейт скина
		m_bIsWindowMaterialMostShow = false;


		// главное окно (main window)
		m_mainWindow = GUI::getSingleton()->create<WindowFrame>(
			600, 150, 300, 530, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CX);
		m_mainWindow->setCaption("Skin editor MyGUI 1.0");
		m_mainWindow->m_pEventCallback = pParent;

		// статик текст над окном
		MyGUI::StaticText * text = m_mainWindow->create<StaticText>(10, 0, 270, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Basis window skin");

		// создаем окно с именами
		m_comboBasisWindowName = m_mainWindow->create<ComboBox>(10, 30, 270, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX);
		m_comboBasisWindowName->m_pEventCallback = this;

		text = m_mainWindow->create<StaticText>(222, 60, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Added 1");
		
		m_comboBasisAddedSkin1 = m_mainWindow->create<ComboBox>(20, 60, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
		m_comboBasisAddedSkin1->addString(NO_SET);
		m_comboBasisAddedSkin1->m_pEventCallback = this;

		text = m_mainWindow->create<StaticText>(222, 90, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Added 2");
		
		m_comboBasisAddedSkin2 = m_mainWindow->create<ComboBox>(20, 90, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
		m_comboBasisAddedSkin2->addString(NO_SET);
		m_comboBasisAddedSkin2->m_pEventCallback = this;

		m_editBasisData1 = m_mainWindow->create<Edit>(20, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editBasisData1->m_pEventCallback = this;
		m_editBasisData2 = m_mainWindow->create<Edit>(90, 120, 60, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editBasisData2->m_pEventCallback = this;

		text = m_mainWindow->create<StaticText>(155, 120, 100, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Added data");

		text = m_mainWindow->create<StaticText>(222, 150, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Font");
		
		m_comboBasisFont = m_mainWindow->create<ComboBox>(20, 150, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
		m_comboBasisFont->m_pEventCallback = this;
		for (AssetManager::FontIterator iter = AssetManager::getSingleton()->Fonts()->begin();
			 iter != AssetManager::getSingleton()->Fonts()->end(); ++iter)
			m_comboBasisFont->addString(iter->first);

		text = m_mainWindow->create<StaticText>(222, 180, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setColour(ColourValue::Green);
		text->setCaption("Colour");	
		
		m_comboBasisColour = m_mainWindow->create<ComboBox>(20, 180, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_COMBO_BOX);
		m_comboBasisColour->m_pEventCallback = this;
		m_comboBasisColour->addString("COLOUR_BLACK");
		m_comboBasisColour->addString("COLOUR_WHITE");
		m_comboBasisColour->addString("COLOUR_BLUE");
		m_comboBasisColour->addString("COLOUR_GREEN");
		m_comboBasisColour->addString("COLOUR_RED");

		// заполняем всеми доступными материалами
		text = m_mainWindow->create<StaticText>(222, 210, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("Material");
		text->setColour(ColourValue::Green);
		
		m_comboMaterialName = m_mainWindow->create<ComboBox>(20, 210, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
		m_comboMaterialName->m_pEventCallback = this;
		m_comboMaterialName->addString(NO_SET);

		Ogre::ResourceManager::ResourceMapIterator mat = Ogre::MaterialManager::getSingleton().getResourceIterator();
		while (mat.hasMoreElements()) {
			String strMaterialName = mat.getNext()->getName();
			m_comboMaterialName->addString(strMaterialName);
			m_strMaterialName.push_back(strMaterialName);
		}


		text = m_mainWindow->create<StaticText>(10, 250, 270, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("Sab skins window");
		text->setColour(ColourValue::Green);

		m_comboSabSkinName = m_mainWindow->create<ComboBox>(10, 280, 270, -1, WA_TOP|WA_LEFT, SKIN_COMBO_BOX);
		m_comboSabSkinName->m_pEventCallback = this;

		debug.add("SabSkin SelectItem", 170, 10, m_comboSabSkinName->m_pList->m_uSelectItem);
	    
		text = m_mainWindow->create<StaticText>(222, 310, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("Offset");
		text->setColour(ColourValue::Green);

		m_editOffset[0] = m_mainWindow->create<Edit>(20, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editOffset[1] = m_mainWindow->create<Edit>(71, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editOffset[2] = m_mainWindow->create<Edit>(122, 310, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editOffset[3] = m_mainWindow->create<Edit>(173, 310, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		
		m_editOffset[0]->m_pEventCallback = this;
		m_editOffset[1]->m_pEventCallback = this;
		m_editOffset[2]->m_pEventCallback = this;
		m_editOffset[3]->m_pEventCallback = this;

		m_buttonSabSkinStyle = m_mainWindow->create<Button>(20, 340, 200, -1, WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
		m_buttonSabSkinStyle->setCaption("SKIN STYLE");
		m_buttonSabSkinStyle->m_pEventCallback = this;
		m_buttonSabSkinStyle->setFont(MyGUI::FONT_DEFAULT, ColourValue::Blue);

		text = m_mainWindow->create<StaticText>(222, 370, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("State");
		text->setColour(ColourValue::Green);
		
		m_comboSabSkinState = m_mainWindow->create<ComboBox>(20, 370, 200, -1, WA_TOP|WA_LEFT, MyGUI::SKIN_DROP_LIST);
		m_comboSabSkinState->m_pEventCallback = this;
		m_comboSabSkinState->addString("WS_DEACTIVATED");
		m_comboSabSkinState->addString("WS_NORMAL");
		m_comboSabSkinState->addString("WS_PRESSED");
		m_comboSabSkinState->addString("WS_ACTIVATED");
		m_comboSabSkinState->addString("WS_SELECTED");

		text = m_mainWindow->create<StaticText>(222, 400, 60, 25, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("Position");
		text->setColour(ColourValue::Green);

		m_editPosition[0] = m_mainWindow->create<Edit>(20, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editPosition[1] = m_mainWindow->create<Edit>(71, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editPosition[2] = m_mainWindow->create<Edit>(122, 400, 45, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		m_editPosition[3] = m_mainWindow->create<Edit>(173, 400, 46, -1, WA_LEFT|WA_TOP, SKIN_EDIT);
		
		m_editPosition[0]->m_pEventCallback = this;
		m_editPosition[1]->m_pEventCallback = this;
		m_editPosition[2]->m_pEventCallback = this;
		m_editPosition[3]->m_pEventCallback = this;

		for (uint8 index=0; index<4; index++) {
			m_editOffset[index]->setUserData(EDIT_IS_USE);
			m_editPosition[index]->setUserData(EDIT_IS_USE);
		}

		m_buttonSkinLoad = m_mainWindow->create<Button>(10, 460, 130, -1, WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
		m_buttonSkinLoad->setCaption("Load");
		m_buttonSkinLoad->m_pEventCallback = this;

		m_buttonSkinSave = m_mainWindow->create<Button>(153, 460, 130, -1, WA_TOP|WA_LEFT|WAT_CENTER, SKIN_BUTTON);
		m_buttonSkinSave->setCaption("Save");
		m_buttonSkinSave->m_pEventCallback = this;

		createFlagWindow();

		// окно для материалов
		createMaterialWindow();
		
		// окно для просмотра элементов
		createElementWindow();

		updateWindowInfo();

		return true;
	}
	//===================================================================================
	void SkinEditor::destroyEditor() // удаляет окно редактирования скинов
	{
		GUI::getSingleton()->destroyWindow(m_mainWindow);
		GUI::getSingleton()->destroyWindow(m_windowElement);
		GUI::getSingleton()->destroyWindow(m_windowMaterial);
		GUI::getSingleton()->destroyWindow(m_windowStateFlags);
		destroySkins();
		// информация об окнах
		mWindowInfo.clear();
	}
	//===================================================================================
	void SkinEditor::updateWindowInfo()
	{
		enableWindowInfo(m_pCurrentDataWindow != 0);

		if (!m_pCurrentDataWindow) return; // на всякий

		if (m_pCurrentDataWindow->strAddedSkin1.empty()) m_comboBasisAddedSkin1->setCaption(NO_SET);
		else m_comboBasisAddedSkin1->setCaption(m_pCurrentDataWindow->strAddedSkin1);
		if (m_pCurrentDataWindow->strAddedSkin2.empty()) m_comboBasisAddedSkin2->setCaption(NO_SET);
		else m_comboBasisAddedSkin2->setCaption(m_pCurrentDataWindow->strAddedSkin2);
		m_comboBasisFont->setCaption(m_pCurrentDataWindow->strFont);

		m_editBasisData1->setCaption(StringConverter::toString(m_pCurrentDataWindow->uAddedData1));
		m_editBasisData2->setCaption(StringConverter::toString(m_pCurrentDataWindow->uAddedData2));
		m_comboBasisColour->setCaption(m_pCurrentDataWindow->strColour);
		m_comboMaterialName->setCaption(m_pCurrentDataWindow->strMaterialName);

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

		// заполняем окно с элементом
		fillElementWindow();

		updateSkinInfo();

	}
	//===================================================================================
	void SkinEditor::updateSkinInfo() // обновляет всю инфу об саб скине
	{
		enableSkinInfo(m_pCurrentDataSkin != 0);

		if (!m_pCurrentDataSkin) {

			m_pCurrentDataState = 0; // сотрем стейты
			m_windowStateFlags->hide();
			m_windowMaterial->hide();
			m_windowElement->hide();

		} else {

			for (uint8 pos=0; pos<4; pos++) {
				m_editOffset[pos]->setCaption(StringConverter::toString(m_pCurrentDataSkin->uOffset[pos]));
			}
			m_pCurrentDataState = &m_pCurrentDataSkin->stateInfo[0];
			fillFlagWindow(); // заполняет окна текущими значения

			// если окно должно быть показанно , то показываем
			if (m_bIsWindowMaterialMostShow) {
				m_windowMaterial->show();
				m_windowElement->show();
			}

			fillViewElement(); // обновляем оверлеи предпросмотра

			m_comboSabSkinState->setString(0); // на первый
			updateStateInfo();

			// размер окна просмотра элемента
			updateSizeElementView();

		}

	}
	//===================================================================================
	void SkinEditor::updateStateInfo() // обновляет всю инфу об стайте
	{

		if (!m_pCurrentDataState) return; // нет инфы уходим

		for (uint8 pos=0; pos<4; pos++) { // поля для чисел
			m_editPosition[pos]->setCaption(StringConverter::toString(m_pCurrentDataState->uPosition[pos]));
		}

		m_textureOffsetPointer->setLocation(m_pCurrentDataState->uPosition[EDIT_LEFT], m_pCurrentDataState->uPosition[EDIT_TOP], m_pCurrentDataState->uPosition[EDIT_RIGHT], m_pCurrentDataState->uPosition[EDIT_BOTTOM]);
		// обновляем квадратик в просмотре
		m_elementOffsetPointer->setLocation(m_pCurrentDataSkin->uOffset[EDIT_LEFT], m_pCurrentDataSkin->uOffset[EDIT_TOP], m_pCurrentDataSkin->uOffset[EDIT_RIGHT], m_pCurrentDataSkin->uOffset[EDIT_BOTTOM]);

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
		LP_SUB_SKIN_DATA data = new _tag_SUB_SKIN_DATA(strName);
//		data->viewElement = new StretchControl(m_mainWindow, this, m_pCurrentDataWindow?m_pCurrentDataWindow->strMaterialName:"BACK_EMPTY", "BACK_EMPTY", "BACK_YELLOW");
		return data;
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

		String strState[5] = {
			AssetManager::VALUE_OFFSET_DEACTIVATED,
			AssetManager::VALUE_OFFSET_NORMAL,
			AssetManager::VALUE_OFFSET_PRESSED,
			AssetManager::VALUE_OFFSET_ACTIVED,
			AssetManager::VALUE_OFFSET_SELECTED
		};
		vector <String> astrSubSkinName; // имена уже сохраненых саб скинов

		for (uint8 index=0; index<mWindowInfo.size(); index++) {
			LP_WINDOW_DATA window = &mWindowInfo[index];
			if (!window) continue;

			fp << "\n" << AssetManager::BLOCK_WINDOW_NAME << " \t" << mWindowInfo[index].strElementName << "\n{";

			if (!window->strAddedSkin1.empty() && (window->strAddedSkin1 != NO_SET)) {
				fp << "\n\t" << AssetManager::VALUE_WINDOW_ADDED_SKIN1 << " \t" << window->strAddedSkin1;
			}
			if (!window->strAddedSkin2.empty() && (window->strAddedSkin2 != NO_SET)) {
				fp << "\n\t" << AssetManager::VALUE_WINDOW_ADDED_SKIN2 << " \t" << window->strAddedSkin2;
			}

			if (window->uAddedData1) { fp << "\n\t" << AssetManager::VALUE_WINDOW_ADDED_DATA1 << " \t" << StringConverter::toString(window->uAddedData1); }
			if (window->uAddedData2) { fp << "\n\t" << AssetManager::VALUE_WINDOW_ADDED_DATA2 << " \t" << StringConverter::toString(window->uAddedData2); }

			if (!window->strFont.empty()) {fp << "\n\t" << AssetManager::VALUE_WINDOW_FONT << " \t" << window->strFont;}
			if (!window->strColour.empty()) { fp << "\n\t" << AssetManager::VALUE_WINDOW_COLOUR << " \t" << window->strColour; }
			if (!window->strMaterialName.empty()) { fp << "\n\t" << AssetManager::VALUE_WINDOW_MATERIAL << " \t\"" << window->strMaterialName << "\""; }

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
				if (pos == 0) fp << "\n\t" << AssetManager::SECTION_SUB_SKIN_MAIN;
				else fp << "\n\n\t" << AssetManager::SECTION_SUB_SKIN;
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

				if (pos == 0) fp << "\n\n\t" << AssetManager::SECTION_SUB_SKIN_MAIN << " \t";
				else fp << "\n\n\t" << AssetManager::SECTION_SUB_SKIN << " \t";
				fp << skin->strName << "\n\t{";

				fp << "\n\t\t" << AssetManager::VALUE_SKIN_OFFSET << " \t" << StringConverter::toString(skin->uOffset[0]) << "  " << StringConverter::toString(skin->uOffset[1]) << "  " << StringConverter::toString(skin->uOffset[2]) << "  " << StringConverter::toString(skin->uOffset[3]) << "\n";

				// сохраняем флаги событий
				fp << "\n\t\t" << AssetManager::VALUE_SKIN_EVENT << " \t"; 
				if (!skin->event_info) fp << "WE_NONE";
				else {
					for (AssetManager::MapValueIterator iter = AssetManager::mMapValueEvent.begin();
						iter != AssetManager::mMapValueEvent.end(); iter ++) {
						if ( iter->second & skin->event_info ) {
							if ( ( iter->second & skin->event_info ) == iter->second )
								fp << iter->first << "  ";
						}
					}
				}

				// сохраняем флаги выравнивания
				fp << "\n\t\t" << AssetManager::VALUE_SKIN_ALIGN << " \t"; 
				if (!skin->align) fp << "WA_NONE";
				else {
					for (AssetManager::MapValueIterator iter = AssetManager::mMapValueAligin.begin();
							iter != AssetManager::mMapValueAligin.end(); iter ++) {
						if ( iter->second & skin->align ) {
							if ( ( iter->second & skin->align ) == iter->second )
								fp << iter->first << "  ";
						}
					}
				}

				// сохраняем флаги стиля
				fp << "\n\t\t" << AssetManager::VALUE_SKIN_STYLE << " \t"; 
				if (!skin->style) fp << "WES_NONE";
				else {
					for (AssetManager::MapValueIterator iter = AssetManager::mMapValueStyle.begin();
						iter != AssetManager::mMapValueStyle.end(); iter ++) {
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
			AssetManager::VALUE_OFFSET_DEACTIVATED,
			AssetManager::VALUE_OFFSET_NORMAL,
			AssetManager::VALUE_OFFSET_PRESSED,
			AssetManager::VALUE_OFFSET_ACTIVED,
			AssetManager::VALUE_OFFSET_SELECTED
		};

		while (ini.seekNextBlock()) {

			if (ini.getBlockType() != AssetManager::BLOCK_WINDOW_NAME) continue;
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
				
				if (strValueName == AssetManager::VALUE_WINDOW_ADDED_SKIN1) {
					if (ini.getValue(strValue)) window->strAddedSkin1 = strValue;

				} else if (strValueName == AssetManager::VALUE_WINDOW_ADDED_SKIN2) {
					if (ini.getValue(strValue)) window->strAddedSkin2 = strValue;

				} else if (strValueName == AssetManager::VALUE_WINDOW_ADDED_DATA1) {
					if (ini.getValue(uValue)) {
						if (uValue <= 255) window->uAddedData1 = uValue;
					}

				} else if (strValueName == AssetManager::VALUE_WINDOW_ADDED_DATA2) {
					if (ini.getValue(uValue)) {
						if (uValue <= 255) window->uAddedData2 = uValue;
					}

				} else if (strValueName == AssetManager::VALUE_WINDOW_FONT) {
					if (ini.getValue(strValue)) window->strFont = strValue;

				} else if (strValueName == AssetManager::VALUE_WINDOW_COLOUR) {
					if (ini.getValue(strValue)) window->strColour = strValue;

				} else if (strValueName == AssetManager::VALUE_WINDOW_MATERIAL) {
					if (ini.getValue(strValue)) window->strMaterialName = strValue;

				} else if (strValueName == AssetManager::SECTION_SUB_SKIN) { // уже загруженные скины
					if (ini.getValue(strValue)) {
						LP_SUB_SKIN_DATA skin = findSkinData(strValue);
						if (skin) window->sabSkins.push_back(skin);
					}
				} else if (strValueName == AssetManager::SECTION_SUB_SKIN_MAIN) { // уже загруженные скины
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

					if (ini.getBlockType() != AssetManager::SECTION_SUB_SKIN) {
						if (ini.getBlockType() != AssetManager::SECTION_SUB_SKIN_MAIN) continue;
						bIsMain = true;
					} else bIsMain = false;

					LP_SUB_SKIN_DATA skin = findSkinData(ini.getBlockName());
					if (!skin) continue;

					while (ini.LoadNextValue()) {
						if (!ini.getValueName(strValueName)) continue;

						if (strValueName == AssetManager::VALUE_SKIN_OFFSET) {
							for (uint8 pos=0; pos<4; pos++) {
								if (!ini.getValue(uValue, pos)) continue;
								skin->uOffset[pos] = uValue;
							}
						} else if (strValueName == AssetManager::VALUE_SKIN_EVENT) {
							for (uint8 pos=0; pos<16; pos++) {
								if (!ini.getValue(strValue, pos)) continue;
								skin->event_info |= AssetManager::mMapValueEvent[strValue];
							}
						} else if (strValueName == AssetManager::VALUE_SKIN_ALIGN) {
							for (uint8 pos=0; pos<16; pos++) {
								if (!ini.getValue(strValue, pos)) continue;
								skin->align |= AssetManager::mMapValueAligin[strValue];
							}
						} else if (strValueName == AssetManager::VALUE_SKIN_STYLE) {
							for (uint8 pos=0; pos<16; pos++) {
								if (!ini.getValue(strValue, pos)) continue;
								skin->style |= AssetManager::mMapValueStyle[strValue];
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
		// сначала скрываем
		m_windowMaterial->show(false);
		m_bIsWindowMaterialMostShow = false;

		if (!m_pCurrentDataWindow) return false;
		m_pCurrentDataWindow->strMaterialName = "";
		m_pCurrentDataWindow->sizeTextureX = 0;
		m_pCurrentDataWindow->sizeTextureY = 0;

		if (m_comboMaterialName->getCurrentIndex() == 0) return false;
//		if (!m_pCurrentDataWindow->sabSkins.size()) return true;

		const String & strMaterialName = m_comboMaterialName->getCaption();

		uint16 sizeX, sizeY;
		if (!AssetManager::getMaterialSize(strMaterialName, sizeX, sizeY)) return false;

		const uint16 addX = m_windowMaterial->m_iSizeX-m_windowMaterial->m_pWindowClient->m_iSizeX;
		const uint16 addY = m_windowMaterial->m_iSizeY-m_windowMaterial->m_pWindowClient->m_iSizeY;

		m_windowMaterial->show(true);
		m_pCurrentDataWindow->sizeTextureX = m_uTextureSizeX = sizeX;
		m_pCurrentDataWindow->sizeTextureY = m_uTextureSizeY = sizeY;
		m_windowMaterial->size( m_uTextureSizeX+addX, m_uTextureSizeY+addY );
		m_windowMaterial->m_pWindowClient->m_overlayContainer->setMaterialName(strMaterialName);
		m_pCurrentDataWindow->strMaterialName = strMaterialName;

		m_bIsWindowMaterialMostShow = true;
		return true;
	}
	//===================================================================================
	void SkinEditor::fillElementWindow() // заполняет окно
	{
		m_windowElement->hide();

		if (!m_pCurrentDataSkin) return;

		m_windowElement->show();

	}
	//===================================================================================
	void SkinEditor::fillViewElement() // обновляем оверлеи предпросмотра
	{
		if (!m_pCurrentDataWindow) return;

		OverlayManager & manager = Ogre::OverlayManager::getSingleton();

		uint8 level;
		for (uint16 pos=0; pos<m_pCurrentDataWindow->sabSkins.size(); pos++) {

			if (pos == 0) level = LEVEL_MAIN;
			else level = LEVEL_SECOND;
			
			Ogre::PanelOverlayElement * panel = m_pCurrentDataWindow->sabSkins[pos]->viewElement;
			if ((m_pCurrentDataWindow->sabSkins[pos]->level != level) && (panel)) {
				manager.destroyOverlayElement(panel);
				panel = 0;
			}
			if (panel == 0) {
				panel = static_cast<PanelOverlayElement*>( manager.createOverlayElement("Panel", "__overlayView_" + StringConverter::toString((unsigned long)m_pCurrentDataWindow->sabSkins[pos])) ); 
				panel->setMetricsMode(GMM_PIXELS);
				panel->setPosition(10, 10);
				panel->setDimensions(20, 20);
				panel->setMaterialName("BACK_GREEN");

				m_windowElementView[level]->m_overlayContainer->addChild(panel);
				m_pCurrentDataWindow->sabSkins[pos]->level = level;
				m_pCurrentDataWindow->sabSkins[pos]->viewElement = panel;
			}
		}

	}
	//===================================================================================
	void SkinEditor::createMaterialWindow() // создает окна для материала
	{
		m_windowMaterial = GUI::getSingleton()->create<WindowFrame>(
			100, 100, 200, 200, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_C);
		m_windowMaterial->setCaption("Material view");
		m_windowMaterial->m_pEventCallback = this;
		m_windowMaterial->show(false);

		m_textureOffsetPointer = new StretchControl(m_windowMaterial, this, "BACK_EMPTY", "BACK_GREEN", "BACK_YELLOW");

	}
	//===================================================================================
	void SkinEditor::createFlagWindow()
	{
		// создаем все что связанно с окном для флагов
		m_windowStateFlags = GUI::getSingleton()->create<WindowFrame>(
			100, 100, 650, 490, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CX);
		m_windowStateFlags->setCaption("Skin flags");
		m_windowStateFlags->m_pEventCallback = this;
	    
		StaticText * text = m_windowStateFlags->create<StaticText>(10, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("EVENT");
		text->setColour(ColourValue::Green);
		text = m_windowStateFlags->create<StaticText>(220, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("ALIGN");
		text->setColour(ColourValue::Green);
		text = m_windowStateFlags->create<StaticText>(430, 0, 200, 20, WA_TOP|WA_LEFT|WAT_CENTER);
		text->setCaption("STYLE");
		text->setColour(ColourValue::Green);

		uint8 countEvents = 5;
		uint8 countAlign = 14;
		uint8 countStyle = 8;

		for (uint8 pos=0; pos<16; pos++) {
			if (pos < countEvents) {
				m_buttonsFlagsEvent[pos] = m_windowStateFlags->create<Button>(10, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
				m_buttonsFlagsEvent[pos]->m_pEventCallback = this;
			} else m_buttonsFlagsEvent[pos] = 0;

			if (pos < countAlign) {
				m_buttonsFlagsAlign[pos] = m_windowStateFlags->create<Button>(220, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
				m_buttonsFlagsAlign[pos]->m_pEventCallback = this;
			} else m_buttonsFlagsAlign[pos] = 0;

			if (pos < countStyle) {
				m_buttonsFlagsStyle[pos] = m_windowStateFlags->create<Button>(430, (pos*30)+30, 200, -1, WA_LEFT|WA_TOP, SKIN_CHECK_BOX);
				m_buttonsFlagsStyle[pos]->m_pEventCallback = this;
			} else m_buttonsFlagsStyle[pos] = 0;
		}

		#define SET_FLAG(win,num,flag) if(win){win->setCaption(#num);win->setUserData(num|flag);}

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
		SET_FLAG(m_buttonsFlagsAlign[10], WAT_ADD_ELLIPSES, FLAG_ALIGN);
		SET_FLAG(m_buttonsFlagsAlign[11], WAT_MULTI_LINE, FLAG_ALIGN);
		SET_FLAG(m_buttonsFlagsAlign[12], WAT_BREAK_WORD, FLAG_ALIGN);
		SET_FLAG(m_buttonsFlagsAlign[13], WAT_SHIFT_TEXT, FLAG_ALIGN);

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
	void SkinEditor::createElementWindow() // создает окна для материала
	{
		m_windowElement = GUI::getSingleton()->create<WindowFrame>(
			200, 100, 300, 200, MyGUI::OVERLAY_OVERLAPPED, MyGUI::SKIN_WINDOWFRAME_CS);
		m_windowElement->setCaption("Element view");
		m_windowElement->m_pEventCallback = this;
		m_windowElement->show(false);

		m_windowElement->m_pWindowClient->m_overlayContainer->setMaterialName("BACK_EMPTY");

		m_windowElementView[LEVEL_MAIN] = m_windowElement->create<Window>(0, 0, m_windowElement->m_pWindowClient->m_iSizeX, m_windowElement->m_pWindowClient->m_iSizeY, WA_STRETCH, SKIN_DEFAULT);

		for (uint8 pos=1; pos<__LEVEL_COUNT; pos++)
			m_windowElementView[pos] = m_windowElementView[pos-1]->create<Window>(0, 0, m_windowElement->m_pWindowClient->m_iSizeX, m_windowElement->m_pWindowClient->m_iSizeY, WA_STRETCH, SKIN_DEFAULT);

		m_elementOffsetPointer = new StretchControl(m_windowElementView[LEVEL_STRETCH_ELEMENT], this, "BACK_EMPTY", "BACK_GREEN", "BACK_YELLOW");

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
			m_buttonSabSkinStyle->setState(WS_DEACTIVATED);
			m_comboSabSkinState->setCaption("");
			m_comboSabSkinState->setState(WS_DEACTIVATED);
			for (uint8 pos=0; pos<4; pos++) {
				m_editOffset[pos]->setCaption("");
				m_editOffset[pos]->setState(WS_DEACTIVATED);
				m_editPosition[pos]->setCaption("");
				m_editPosition[pos]->setState(WS_DEACTIVATED);
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
			m_comboSabSkinName->setCaption("");
			m_comboSabSkinName->setState(WS_DEACTIVATED);
			m_comboMaterialName->setCaption("");
			m_comboMaterialName->setState(WS_DEACTIVATED);
			m_comboBasisColour->setCaption("");
			m_comboBasisColour->setState(WS_DEACTIVATED);
			m_comboBasisFont->setCaption("");
			m_comboBasisFont->setState(WS_DEACTIVATED);
			m_editBasisData1->setCaption("");
			m_editBasisData1->setState(WS_DEACTIVATED);
			m_editBasisData2->setCaption("");
			m_editBasisData2->setState(WS_DEACTIVATED);
			m_comboBasisAddedSkin1->setCaption("");
			m_comboBasisAddedSkin1->setState(WS_DEACTIVATED);
			m_comboBasisAddedSkin2->setCaption("");
			m_comboBasisAddedSkin2->setState(WS_DEACTIVATED);
			m_comboBasisWindowName->setCaption("");

		}

	}
	//===================================================================================
	void SkinEditor::OnChangeLocation(StretchControl * pControl, uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY)
	{
		// элемент внутри материала
		if (pControl == m_textureOffsetPointer) {
			if (m_pCurrentDataState) {
				m_pCurrentDataState->uPosition[EDIT_LEFT] = posX;
				m_pCurrentDataState->uPosition[EDIT_TOP] = posY;
				m_pCurrentDataState->uPosition[EDIT_RIGHT] = sizeX;
				m_pCurrentDataState->uPosition[EDIT_BOTTOM] = sizeY;

				updateStateInfo();
			}
		} else if (pControl == m_elementOffsetPointer) {

			if (m_pCurrentDataSkin) {
				m_pCurrentDataSkin->uOffset[EDIT_LEFT] = posX;
				m_pCurrentDataSkin->uOffset[EDIT_TOP] = posY;
				m_pCurrentDataSkin->uOffset[EDIT_RIGHT] = sizeX;
				m_pCurrentDataSkin->uOffset[EDIT_BOTTOM] = sizeY;

				updateSkinInfo();
			}

		}

	}
	//===================================================================================
	void SkinEditor::updateSizeElementView() // обновляем размер окна если нужно
	{
		// если первый элемент, то сдвигаем окно относительно размера сабскина
		/*if (m_comboSabSkinName->getCurrentIndex() == 0) {
			const uint16 addX = m_windowElement->m_iSizeX-m_windowElement->m_pWindowClient->m_iSizeX;
			const uint16 addY = m_windowElement->m_iSizeY-m_windowElement->m_pWindowClient->m_iSizeY;
			m_windowElement->size(m_pCurrentDataSkin->uOffset[EDIT_RIGHT]+addX+ELEMENT_VIEW_OFFSET+ELEMENT_VIEW_OFFSET,
				m_pCurrentDataSkin->uOffset[EDIT_BOTTOM]+addY+ELEMENT_VIEW_OFFSET+ELEMENT_VIEW_OFFSET);
		}*/
	}

} // namespace SkinEditor