#pragma once

#include "MyGUI_Source\\MyGUI.h"
#include "StretchControl.h"

namespace SkinEditor
{
	//=========================================================================================
	// класс панели для редактирования скинов
	class SkinEditor : public MyGUI::EventCallback, public StretchControlEvent
	{
		public:

			enum {
				NONE=0,
				EDIT_IS_USE,
			};
			enum {
				EDIT_LEFT,
				EDIT_TOP,
				EDIT_RIGHT,
				EDIT_BOTTOM,
				__EDIT_COUNT
			};

			enum {
				LEVEL_MAIN,
				LEVEL_SECOND,
				LEVEL_THIRD,
				LEVEL_STRETCH_ELEMENT,
				__LEVEL_COUNT
			};

			typedef struct _tag_STATE_DATA { // информация об одном стейте
				MyGUI::uint16 uPosition[__EDIT_COUNT]; // позиция в элементе
			} * LP_STATE_DATA;

			typedef struct _tag_SUB_SKIN_DATA {
				MyGUI::String strName; // название саб скина
				MyGUI::uint16 uOffset[__EDIT_COUNT]; // смещение в текстуре
				MyGUI::uint16 event_info; // события
				MyGUI::uint16 style; // стиль
				MyGUI::uint16 align; // выравнивание
				_tag_STATE_DATA stateInfo[5]; // на каждое состояние

				Ogre::PanelOverlayElement  * viewElement; // видимый элемент в просмотре
				MyGUI::uint8 level; // уровень на котором висит елемент просмотра

				_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA() {assert(0);}; // низя
				_tag_SUB_SKIN_DATA::_tag_SUB_SKIN_DATA(const MyGUI::String & str) : strName(str),
					event_info(0),
					style(0),
					align(0),
					viewElement(0),
					level(__LEVEL_COUNT)
				{
					memset((void*)uOffset, 0, sizeof(MyGUI::uint16) * __EDIT_COUNT); // очищаем
					memset((void*)stateInfo, 0, sizeof(_tag_STATE_DATA) * 5); // очищаем
				};
				_tag_SUB_SKIN_DATA::~_tag_SUB_SKIN_DATA()
				{
					if (viewElement) {
						Ogre::OverlayManager::getSingleton().destroyOverlayElement(viewElement);
						viewElement = 0;
					}
				}

			} * LP_SUB_SKIN_DATA;

			typedef struct _tag_WINDOW_DATA {
				MyGUI::String strElementName; // имя элемента
				MyGUI::String strAddedSkin1; // дополнительные скины
				MyGUI::String strAddedSkin2; // дополнительные скины
				MyGUI::uint8 uAddedData1; // дополнительные данные
				MyGUI::uint8 uAddedData2; // дополнительные данные
				MyGUI::String strFont; // шрифт
				MyGUI::String strColour; // название цвета

				MyGUI::String strMaterialName; // имя материала
				MyGUI::uint16 sizeTextureX; // размер текстуры
				MyGUI::uint16 sizeTextureY; // размер текстуры

				std::vector <LP_SUB_SKIN_DATA> sabSkins; // указатели на сабскины
				_tag_WINDOW_DATA::_tag_WINDOW_DATA() {assert(0);};
				_tag_WINDOW_DATA::_tag_WINDOW_DATA(const MyGUI::String & strName) :
					strElementName(strName),
					uAddedData1(0),
					uAddedData2(0),
					sizeTextureX(0),
					sizeTextureY(0) {};
			} * LP_WINDOW_DATA;


			virtual void onOtherEvent(MyGUI::Window * pWindow, MyGUI::uint16 uEvent, MyGUI::uint32 data); // дополнительные события
			virtual void onMouseClick(MyGUI::Window * pWindow); // нажата и отпущена левая кнопка мыши на этом же элементе

			SkinEditor(MyGUI::EventCallback * pParent = NULL);
			~SkinEditor();
			bool createEditor(MyGUI::EventCallback * pParent = NULL); // создает окно редактирования скинов
			void destroyEditor(); // удаляет окно редактирования скинов
			void loadSkin(const MyGUI::String & strFileName); // сохраняет скин
			void saveSkin(const MyGUI::String & strFileName); // загружает скин

			SkinEditor::LP_SUB_SKIN_DATA findSkinData(const MyGUI::String & strName, bool create = true); // ищет данные если нет то создает
			void deleteSkinData(SkinEditor::LP_SUB_SKIN_DATA lpDataSkin); // удаляет саб скин
			void destroySkins(); // удаляет все саб скины

			void updateWindowInfo(); // обновляет всю инфу об окне
			void enableWindowInfo(bool bEnable); // блокирует все окна
			void updateSkinInfo(); // обновляет всю инфу об саб скине
			void updateStateInfo(); // обновляет всю инфу об саб скине
			void updateSizeElementView(); // обновляем размер окна если нужно
			void enableSkinInfo(bool bEnable); // блокирует саб скины
			void createFlagWindow(); // создает окно с дополнительными флагами
			void createMaterialWindow(); // создает окна для материала
			void createElementWindow(); // создает окно для просмотра елемента
			void fillFlagWindow(); // заполняет окна текущими значения
			bool fillMaterialWindow(); // заполняем окно с материалом
			void fillElementWindow(); // заполняем окно с элементом
			void fillViewElement(); // обновляем оверлеи предпросмотра
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
			MyGUI::Button * m_buttonSabSkinStyle; // стили скина

			MyGUI::ComboBox * m_comboSabSkinState; // состояние скина

			MyGUI::Edit * m_editOffset[__EDIT_COUNT]; // смещения куска внутри текстуры
			MyGUI::Edit * m_editPosition[__EDIT_COUNT]; // позиция окна в общем скине

			MyGUI::Button * m_buttonSkinLoad; // загрузка скина
			MyGUI::Button * m_buttonSkinSave; // сохранение скина

			std::vector <_tag_WINDOW_DATA> mWindowInfo; // информация о всех окнах
			std::vector <MyGUI::String> m_strMaterialName; // все доступные материалы

			LP_WINDOW_DATA m_pCurrentDataWindow; // текущее окно
			LP_SUB_SKIN_DATA m_pCurrentDataSkin; // текущий саб скин
			LP_STATE_DATA m_pCurrentDataState; // текцщий стейт скина


			MyGUI::WindowFrame * m_windowStateFlags; // окно с флагами
			MyGUI::Button * m_buttonsFlagsEvent[16];
			MyGUI::Button * m_buttonsFlagsAlign[16];
			MyGUI::Button * m_buttonsFlagsStyle[16];

			MyGUI::WindowFrame * m_windowMaterial; // окно с материалом
			bool m_bIsWindowMaterialMostShow; // должно ли окно быть видимым
			MyGUI::uint16 m_uTextureSizeX;
			MyGUI::uint16 m_uTextureSizeY;

			MyGUI::WindowFrame * m_windowElement; // окно с элементом

			// рамка в окне материала
			StretchControl * m_textureOffsetPointer;
			// рамка в окне вьювера
			StretchControl * m_elementOffsetPointer;
			// окна для многоэтажности вьювера
			MyGUI::Window * m_windowElementView[__LEVEL_COUNT];

			// уведомление об изменении положения контрола
			void OnChangeLocation(StretchControl * pControl, uint16 posX, uint16 posY, uint16 sizeX, uint16 sizeY);

	};

} // namespace SkinEditor