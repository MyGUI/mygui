//=========================================================================================
#ifndef __SkinEditor_H__
#define __SkinEditor_H__
//=========================================================================================
#include "MyGUI_Source\\MyGUI.h"
//=========================================================================================
using namespace Ogre;
using namespace MyGUI;
//=========================================================================================
// класс панели для редактирования скинов
class SkinEditor : public MyGUI::EventCallback
{
	public:

		enum {
			NONE=0,
			EDIT_IS_USE,
			MATERIAL_OFFSET,
		};
		enum {
			LEFT = 0,
			TOP = 1,
			RIGHT = 2,
			BOTTOM = 3,
			CENTER = 4,
			MATERIAL_BORDER_COUNT
		};

		typedef struct _tag_STATE_DATA { // информация об одном стейте
			uint16 uPosition[4]; // позиция в элементе
		} * LP_STATE_DATA;

		typedef struct _tag_SUB_SKIN_DATA {
			String strName; // название саб скина
			uint16 uOffset[4]; // смещение в текстуре
			uint16 event_info; // события
			uint16 style; // стиль
			uint16 aligin; // выравнивание
			_tag_STATE_DATA stateInfo[5]; // на каждое состояние
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA() {assert(0);}; // низя
			_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const String & str) : strName(str), event_info(0), style(0), aligin(0)
			{
				memset((void*)uOffset, 0, sizeof(uint16) * 4); // очищаем
				memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // очищаем
			};
		} * LP_SUB_SKIN_DATA;

		typedef struct _tag_WINDOW_DATA {
			String strElementName; // имя элемента
			uint8 uAddedSkin1; // дополнительные скины
			uint8 uAddedSkin2; // дополнительные скины
			uint8 uAddedData1; // дополнительные данные
			uint8 uAddedData2; // дополнительные данные
			uint8 uFont; // номер шрифта
			String strColour; // название цвета
			uint16 uMaterial; // материал на весь элемент

			String strMaterialName; // имя материала
			uint16 sizeTextureX; // размер текстуры
			uint16 sizeTextureY; // размер текстуры

			vector <LP_SUB_SKIN_DATA> sabSkins; // указатели на сабскины
			_tag_WINDOW_DATA::_tag_WINDOW_DATA() {assert(0);};
			_tag_WINDOW_DATA::_tag_WINDOW_DATA(const String & strName) : strElementName(strName), uAddedSkin1(0), uAddedSkin2(0), uAddedData1(0), uAddedData2(0), uFont(0), uMaterial(0), sizeTextureX(0), sizeTextureY(0) {};
		} * LP_WINDOW_DATA;


		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события
		virtual void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе
		virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY, int16 iFotherPosX, int16 iFotherPosY); // уведомление о движении, но не движение

		bool createEditor(); // создает окно редактирования скинов
		void destroyEditor(); // удаляет окно редактирования скинов
		void loadSkin(const String & strFileName); // сохраняет скин
		void saveSkin(const String & strFileName); // загружает скин
		void resizeWindow(); // изменились размеры окна

		SkinEditor::LP_SUB_SKIN_DATA findSkinData(const String & strName, bool create = true); // ищет данные если нет то создает
		void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // удаляет саб скин
		void destroySkins(); // удаляет все саб скины

		void updateWindowInfo(); // обновляет всю инфу об окне
		void updateSkinInfo(); // обновляет всю инфу об саб скине
		void updateStateInfo(); // обновляет всю инфу об саб скине
		void createFlagWindow(); // создает окно с дополнительными флагами
		void createMaterialWindow(); // создает окна для материала
		void fillFlagWindow(); // заполняет окна текущими значения
		void fillMaterialWindow(); // заполняем окно с материалом
		void pressOtherButton(MyGUI::Window * pWindow); // сверяем с кнопками флагов
		void setMaterialOffset(uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY); // сдвигаем рамку


//		Edit * m_editInfo; // окно информации

		WindowFrame * m_mainWindow; // главное окно инструментов
		ComboBox * m_comboBasisWindowName; // главный скин окна

		ComboBox * m_comboBasisAddedSkin1; // присоединяемый скин 1
		ComboBox * m_comboBasisAddedSkin2; // присоединяемый скин 2
		Edit * m_editBasisData1; // дополнительные данные 1
		Edit * m_editBasisData2; // дополнительные данные 1
		ComboBox * m_comboBasisFont; // шрифт окна
		ComboBox * m_comboBasisColour; // цвет окна
		ComboBox * m_comboMaterialName; // материал для данного скина

		ComboBox * m_comboSabSkinName; // саб скины окна
//		Button * m_buttonSabSkinCreate; // добавить саб скин
//		Button * m_buttonSabSkinDelete; // удалить саб скин
		Button * m_buttonSabSkinStyle; // стили скина

		ComboBox * m_comboSabSkinState; // состояние скина

		Edit * m_editOffset[4]; // смещения куска внутри текстуры
		Edit * m_editPosition[4]; // позиция окна в общем скине

		vector <_tag_WINDOW_DATA> mWindowInfo; // информация о всех окнах
		vector <String> m_strMaterialName; // все доступные материалы

		LP_WINDOW_DATA m_pCurrentDataWindow; // текущее окно
		LP_SUB_SKIN_DATA m_pCurrentDataSkin; // текущий саб скин
		LP_STATE_DATA m_pCurrentDataState; // текцщий стейт скина


		WindowFrame * m_windowStateFlags; // окно с флагами
		Button * m_buttonsFlagsEvent[16];
		Button * m_buttonsFlagsAligin[16];
		Button * m_buttonsFlagsStyle[16];

		WindowFrame * m_windowMaterial; // окно с материалом
		Window * m_windowMaterialOffset[MATERIAL_BORDER_COUNT]; // граница
		uint16 m_uTextureSizeX;
		uint16 m_uTextureSizeY;

};
//=========================================================================================
#endif
//=========================================================================================
