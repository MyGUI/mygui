#pragma once

#include "MyGUI_Window.h"

namespace MyGUI {

	class List : public Window {
        List(const __MYGUI_SUBSKIN_INFO *lpSkin, const String & strMaterialElement, uint8 uOverlay, Window *pWindowParent);
	public:
	    ~List();
		static List *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        Window *parent, uint16 uAlign, uint16 uOverlay, const String &Skin = SKIN_LIST_S);

		void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText); // нажата клавиша
		void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса ввода
		void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseClick(MyGUI::Window * pWindow);
		void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события
		void onMouseDoubleClick(MyGUI::Window * pWindow); // двойной щелчек мыши

		void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна

		void addString(const DisplayString & strElement, bool bIsRedraw = true); // добавить строку в список
		void insertString(uint16 index, const DisplayString & strElement, bool bIsRedraw = true); // вставить строку в список
		void deleteString(uint16 index, bool bIsRedraw = true); // удалить строку из списка
		void deleteStringAll(bool bIsRedraw = true); // удаляет все строки
		const DisplayString & getString(uint16 index); // возвращает строку
		void setFont(const String &lpFont, Ogre::ColourValue colour); // устанавливает шрифт для всего списка
		inline uint16 getStringCount() {return (uint16)m_aString.size();}; // возвращает колличество строк

		void redrawStrings(); // перерисовывает все строки
		void recalcScroll(); // пересчет полосы прокрутки

		VScroll * m_scroll; // скролл окна
		String m_SkinButton; // скин для кнопок списка
		uint8 m_uSizeYButton; // высота строки
		uint8 m_uSizeXScroll; // ширина скрола
		uint16 m_uCurrentFillSize; // место до куда созданны окна
		size_t m_uStartWindow; // откуда ничинаются кнопки - 0 или 1 (есть скролл)
		uint8 m_uCountVisible; // сколько видимых строк

		std::vector <DisplayString * > m_aString; // список строк
		uint16 m_uOffsetDrawString; // элемент первой видимой строки
		uint16 m_uSelectItem; // выделенный элемент
		Window *pWindowSelect; // текущая выделенная строчка

        bool m_bIsFocus; // в фокусе ли элемент
		bool m_bIsVisibleScroll; // виден ли скролл
		bool m_bIsOneClickActived; // акцепт одним кликом мыши

	};
}