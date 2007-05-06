#pragma once

#include "MyGUI_EventCallBack.h"
#include "MyGUI_SkinManager.h"
#include <OgreOverlayElement.h>

namespace MyGUI {

	class Window : public EventCallback { // базовый класс окон гуи
	public:
	    Window(__LP_MYGUI_SKIN_INFO lpSkin, uint8 uOverlay, Window *pWindowParent);
		virtual ~Window();
	public:
		virtual void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // вызывается при движении окна
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // вызывается при нажатии клавиши
		virtual void _OnMouseButtonClick(bool bIsDouble); // вызывается при нажатии клавиши и отпускании на том же элементе
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы
		virtual void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		virtual void _OnUpZOrder() {}; // вызывается при активации окна

        template<class Widget> Widget *spawn(const Coord &Pos, const Coord &Size,
            const uint16 uAlign = 0, const Ogre::String &uSkin = __SKIN_WIDGET_DEFAULT)
        {
            if(uSkin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(Pos, Size, this, uAlign, 0);
            else
                return Widget::create(Pos, Size, this, uAlign, 0, uSkin);
        }
        
        template<class Widget> Widget *spawnReal(const Ogre::Vector2 &Pos, const Ogre::Vector2 &Size,
            const uint16 uAlign = 0, const uint8 uSkin = __SKIN_WIDGET_DEFAULT)
        {
            OrderedPos NewPos  = Pos  * Ogre::Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight);
            OrderedPos NewSize = Size * Ogre::Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight);
            
            return spawn<Widget>(NewPos, NewSize, uAlign, uSkin);
        }

		static Window * Window::create(const Coord &Pos, const Coord &Size,
	        Window *parent, const uint16 uAlign, const uint8 uOverlay, const Ogre::String &uSkin = SKIN_WINDOWFRAME);
		
		void show(bool bIsShow); // скрыть показать окно
		void move(int16 iPosX, int16 iPosY); // передвинуть окно
		virtual void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // проверка окна на позицию курсора
		void setState(uint8 uState); // состояние окна
		void showFocus(bool bIsFocus); // активирование окна
		virtual Window *setWindowText(const Ogre::DisplayString & strText); // устанавливает текст окна
		Ogre::DisplayString getWindowText(); // возвращает строку окна
		virtual void alignWindowText(); // выполняет выравнивание текста
		void alignWindow(int16 rNewSizeX, int16 rNewSizeY); // выравнивает окно относительно отца
		Window *setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour); // устанавливает шрифт для элемента
		Window *setFont(const Ogre::String &uFont, uint32 colour); // устанавливает шрифт для элемента

		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // добавляет событие на которое надо реагировать

		inline void setUserString(const Ogre::DisplayString & strUserString) {m_strUserString = strUserString;}; // ставит строку пользователя
		inline Ogre::DisplayString & getUserString() {return m_strUserString;}; // возвращает строку пользователя
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ставит данные пользователя
		inline uint32 getUserData() {return m_uUserData;}; // возвращает данные пользователя


		Ogre::Overlay* m_overlay; // оверлей этого окна
		Ogre::OverlayContainer* m_overlayContainer; // оверлей элемента
		Ogre::OverlayContainer* m_overlayCaption; // оверлей текста
		std::vector<Window*>m_aWindowChild; // дети окна
		Window * m_pWindowParent; // отец окна
		Window * m_pWindowText; // элемент скина содержащий текст всего элемента (по дефолту this)
		Window * m_pWindowClient; // элемент скина является клиенским окном всего элемента (по дефолту this)
		int16 m_iPosX, m_iPosY, m_iSizeX, m_iSizeY; // размеры окна
		int16 m_iOffsetAlignX, m_iOffsetAlignY; // смещение, используется только при выравнивании по центру без растяжения

		uint8 m_uState; // статус окна
		EventCallback *m_pEventCallback; // указатель на класс для вызова функций
		uint16 m_uEventCallback; // флаги для посылки событий
		uint16 m_uAlign; // выравнивание окна и текста
		int16 m_sizeTextX; // размер текста окна
		int16 m_sizeTextY; // размер текста окна
		int16 m_sizeCutTextX; // видимый размер текста
		int16 m_sizeCutTextY; // видимый размер текста
		Ogre::DisplayString m_strWindowText; // текст окна для буфера обрезки, когда текст не обрезан эта строка пуста
		bool m_bIsOverlapped; // окно перекрывающееся
		bool m_bIsTextShiftPressed; // сдвинут ли текст вниз

		__LP_MYGUI_FONT_INFO m_font; // шрифт окна
		uint32 m_fontColour; // цвет текста
		Ogre::String * m_paStrSkins[__SKIN_STATE_COUNT]; // скины состояний
		
		uint16 m_uExData; // дополнительная информация об элементе

		Ogre::DisplayString m_strUserString; // строка для дополнительной информации пользователя
		uint32 m_uUserData; // дополнительная информация пользователя

	};

}