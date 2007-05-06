#pragma once
#include "MyGUI_Coord.h"
#include "OgreOverlayElement.h"

namespace MyGUI {

	class List : public Window {
    protected:
        List(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
		~List();
	public:
		void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы

		void onKeyButton(Window * pWindow, int keyEvent, char cText); // нажата клавиша
		void onKeyFocus(Window * pWindow, bool bIsFocus); // смена фокуса ввода
		void onMouseFocus(Window * pWindow, bool bIsFocus); // смена фокуса
		void onMouseClick(Window * pWindow);
		void onOtherEvent(Window * pWindow, uint16 uEvent, uint32 data); // дополнительные события
		void onMouseDoubleClick(Window * pWindow); // двойной щелчек мыши

        //returns a reference to the current object to allow for chaining (addString()->addString()->addString()...)
		List *addString(const Ogre::DisplayString & strElement, bool bIsRedraw = true); // добавить строку в список
		List *insertString(uint16 index, const Ogre::DisplayString & strElement, bool bIsRedraw = true); // вставить строку в список
		List *deleteString(uint16 index, bool bIsRedraw = true); // удалить строку из списка
		List *deleteStringAll(bool bIsRedraw = true); // удаляет все строки
		Ogre::DisplayString getString(Ogre::uint16 index); // возвращает строку
		List *setFont(__LP_MYGUI_FONT_INFO lpFont, Ogre::uint32 colour); // устанавливает шрифт для всего списка
		size_t getStringCount() const { return m_aString.size(); }

		std::vector <Ogre::DisplayString * > m_aString; // список строк
        void size(const Coord &Size);
        bool m_bIsOneClickActived; // акцепт одним кликом мыши
        Ogre::uint8 m_uSizeXScroll; // ширина скрола
        VScroll * m_scroll; // скролл окна
		Ogre::uint8 m_uStartWindow; // откуда ничинаются кнопки - 0 или 1 (есть скролл)
        
    public:
		void redrawStrings(); // перерисовывает все строки
		void recalcScroll(); // пересчет полосы прокрутки

		
		Ogre::String m_uSkinButton; // скин для кнопок списка
		Ogre::uint8 m_uSizeYButton; // высота строки
		
		Ogre::uint16 m_uCurrentFillSize; // место до куда созданны окна
		
		Ogre::uint8 m_uCountVisible; // сколько видимых строк


		Ogre::uint16 m_uOffsetDrawString; // элемент первой видимой строки
		Ogre::uint16 m_uSelectItem; // выделенный элемент
		Window *pWindowSelect; // текущая выделенная строчка
		
		size_t getCurrentIndex() const { return m_uSelectItem; }
		
		bool m_bIsFocus; // в фокусе ли элемент
		bool m_bIsVisibleScroll; // виден ли скролл
		
		static List * List::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_LIST_S);
	};
}