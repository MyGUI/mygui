#pragma once

#include "MyGUI_Source\\MyGUI.h"

using namespace Ogre;
using namespace std;

// класс панели для редактирования скинов
class SkinEditor : public MyGUI::EventCallback
{
	public:

		bool createEditor(); // создает окно редактирования скинов
		void destroyEditor(); // удаляет окно редактирования скинов

		typedef struct _tag_STATE_DATA { // информация об одном стейте
			size_t uPosition[4]; // позиция в элементе
		} * LP_STATE_DATA;

		typedef struct _tag_SUB_SKIN_DATA {
			Ogre::String strName; // название саб скина
			MyGUI::uint16 uOffset[4]; // смещение в текстуре
			MyGUI::uint16 event_info; // события
			MyGUI::uint16 style; // стиль
			MyGUI::uint16 aligin; // выравнивание
			_tag_STATE_DATA stateInfo[5]; // на каждое состояние
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const String & str) : strName(str)
			{
				memset((void*)uOffset, 0, sizeof(MyGUI::uint16) * 4); // очищаем
				memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // очищаем
				event_info = 0;
				style = 0;
				aligin = 0;
			};
		} * LP_SUB_SKIN_DATA;

		typedef struct _tag_WINDOW_DATA {
			Ogre::String uAddedSkin1; // дополнительные скины
			Ogre::String uAddedSkin2; // дополнительные скины
			Ogre::String uAddedData1; // дополнительные данные
			Ogre::String uAddedData2; // дополнительные данные
			Ogre::String uFont; // номер шрифта
			Ogre::String strColour; // название цвета
			MyGUI::uint16 uMaterial; // материал на весь элемент
			std::vector <LP_SUB_SKIN_DATA> sabSkins; // указатели на сабскины
			_tag_WINDOW_DATA::_tag_WINDOW_DATA() : uAddedSkin1(0), uAddedSkin2(0), uAddedData1(0), uAddedData2(0), uFont(0), uMaterial(0) {};
		} * LP_WINDOW_DATA;


		virtual void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // дополнительные события
		virtual void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

		SkinEditor::LP_SUB_SKIN_DATA findSkinData(const Ogre::String & strName, bool create = true); // ищет данные если нет то создает
		void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // удаляет саб скин
		void destroySkins(); // удаляет все саб скины
		void loadSkin(); // сохраняет скин
		void saveSkin(); // загружает скин

		void updateWindowInfo(); // обновляет всю инфу об окне
		void updateSkinInfo(); // обновляет всю инфу об саб скине
		void updateStateInfo(); // обновляет всю инфу об саб скине
		void createFlagWindow(); // создает окно с дополнительными флагами
		void createMaterialWindow(); // создает окна для материала
		void fillFlagWindow(); // заполняет окна текущими значения
		void fillMaterialWindow(); // заполняем окно с материалом
		void pressOtherButton(MyGUI::Window * pWindow); // сверяем с кнопками флагов

        MyGUI::WindowFrame * m_mainWindow; // главное окно инструментов
		MyGUI::ComboBox * m_comboBasisWindowName; // главный скин окна

		MyGUI::ComboBox * m_comboBasisAddedSkin1; // присоединяемый скин 1
		MyGUI::ComboBox * m_comboBasisAddedSkin2; // присоединяемый скин 2
		MyGUI::Edit * m_editBasisData1; // дополнительные данные 1
		MyGUI::Edit * m_editBasisData2; // дополнительные данные 1
		MyGUI::ComboBox * m_comboBasisFont; // шрифт окна
		MyGUI::ComboBox * m_comboBasisColour; // цвет окна
		MyGUI::ComboBox * m_comboMaterialName; // материал для данного скина

		MyGUI::ComboBox * m_comboSabSkinName; // саб скины окна
		MyGUI::Button * m_buttonSabSkinCreate; // добавить саб скин
		MyGUI::Button * m_buttonSabSkinDelete; // удалить саб скин
		MyGUI::Button * m_buttonSabSkinStyle; // стили скина

		MyGUI::ComboBox * m_comboSabSkinState; // состояние скина

		enum {EDIT_NONE=0, EDIT_IS_USE};
		MyGUI::Edit * m_editOffset[4]; // смещения куска внутри текстуры
		MyGUI::Edit * m_editPosition[4]; // позиция окна в общем скине

		std::vector <_tag_WINDOW_DATA> mWindowInfo; // информация о всех окнах
		std::vector <String> m_strMaterialName; // все доступные материалы

		LP_WINDOW_DATA m_pCurrentDataWindow; // текущее окно
		LP_SUB_SKIN_DATA m_pCurrentDataSkin; // текущий саб скин
		LP_STATE_DATA m_pCurrentDataState; // текцщий стейт скина

		MyGUI::WindowFrame * m_windowStateFlags; // окно с флагами
		MyGUI::Button * m_buttonsFlagsEvent[16];
		MyGUI::Button * m_buttonsFlagsAligin[16];
		MyGUI::Button * m_buttonsFlagsStyle[16];

		MyGUI::WindowFrame * m_windowMaterial; // окно с материалом
		MyGUI::Window * m_windowMaterialLeft; // левая граница
		MyGUI::Window * m_windowMaterialTop; // верхняя граница
		MyGUI::Window * m_windowMaterialRight; // правая граница
		MyGUI::Window * m_windowMaterialBottom; // нижняя граница
};