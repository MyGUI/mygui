//=========================================================================================
#ifndef __MyGUI_Window_H__
#define __MyGUI_Window_H__
//=========================================================================================
using namespace Ogre;
using namespace std;
//=========================================================================================
namespace MyGUI {

	class WindowCallback;
	class GUI;
	class Edit;
	class Button;
	class VScroll;
	class HScrollBar;
	class Tab;
	class StaticText;
	class List;
	class ComboBox;

	class Window : public EventCallback { // базовый класс окон гуи

	public:
		Window(__LP_MYGUI_SKIN_INFO lpSkin, GUI *gui, uint8 uOverlay, Window *pWindowFother);
		virtual ~Window();

		virtual void _OnMouseChangeFocus(bool bIsFocus); // вызывается при смене активности от курсора
		virtual void _OnMouseMove(int16 iPosX, int16 iPosY); // вызывается при движении окна
		virtual void _OnMouseButtonPressed(bool bIsLeftButtonPressed); // вызывается при нажатии клавиши
		virtual void _OnMouseButtonClick(bool bIsDouble); // вызывается при нажатии клавиши и отпускании на том же элементе
		virtual void _OnKeyButtonPressed(int keyEvent, wchar_t cText); // вызывается при нажатии клавиши клавы
//		virtual void _OnKeyButtonClick(int keyEvent); // вызывается при отпускании клавиши клавы
		virtual void _OnKeyChangeFocus(bool bIsFocus); // вызывается при смене активности ввода
		virtual void _OnUpZOrder() {}; // вызывается при активации окна

		MyGUI::Window * createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback = 0); // обыкновенное окно
		MyGUI::Window * createWindowReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin, EventCallback * pEventCallback = 0) { // обыкновенное окно
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createWindow(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		};
		MyGUI::WindowFrame * createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0); // окно с рамкой
		MyGUI::WindowFrame * createWindowFrameReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0) { // окно с рамкой
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createWindowFrame(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		};
		MyGUI::Button * createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0); // создает кнопку
		MyGUI::Button * createButtonReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0) { // создает кнопку
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createButton(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		}
		MyGUI::Edit * createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0); // создает окно редактирования
		MyGUI::Edit * createEditReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0) { // создает окно редактирования
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createEdit(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::VScroll * createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0); // создает полосу прокрутки
		MyGUI::VScroll * createVScrollReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0) { // создает полосу прокрутки
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createVScroll(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::Tab * createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0); // создает панель вкладок
		MyGUI::Tab * createTabReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0) { // создает панель вкладок
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createTab(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::StaticText * createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0); // создает статический текст
		MyGUI::StaticText * createStaticTextReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint16 uAligin, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0) { // создает статический текст
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createStaticText(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, strWindowText, uAligin, uSkin, pEventCallback);
		}
		MyGUI::List * createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0); // создает список
		MyGUI::List * createListReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0) { // создает список
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createList(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}
		MyGUI::ComboBox * createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint16 uAligin, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0); // создает выпадающий список
		MyGUI::ComboBox * createComboBoxReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint16 uAligin, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0) { // создает выпадающий список
			if (fSizeX != -1.0) fSizeX *= m_GUI->m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_GUI->m_uHeight;
			return createComboBox(fPosX*m_GUI->m_uWidth, fPosY*m_GUI->m_uHeight, fSizeX, fSizeY, uAligin, uSkin, pEventCallback);
		}

		void show(bool bIsShow); // скрыть показать окно
		void move(int16 iPosX, int16 iPosY); // передвинуть окно
		virtual void size(int16 iSizeX, int16 iSizeY); // изменяем размер окна
		bool check(int16 iPosX, int16 iPosY, bool bCapture = false); // проверка окна на позицию курсора
		void setState(uint8 uState); // состояние окна
		uint8 getState(); // состояние окна
		void showFocus(bool bIsFocus); // активирование окна
		virtual void setWindowText(const DisplayString & strText); // устанавливает текст окна
		const DisplayString & getWindowText(); // возвращает строку окна
		virtual void aliginWindowText(); // выполняет выравнивание текста
		void aliginWindow(int16 rNewSizeX, int16 rNewSizeY); // выравнивает окно относительно отца
		void setFont(__LP_MYGUI_FONT_INFO lpFont, uint32 colour); // устанавливает шрифт для элемента
		void setFont(uint8 uFont, uint32 colour); // устанавливает шрифт для элемента

		void addEvent(uint16 addEvent) {m_uEventCallback |= addEvent;}; // добавляет событие на которое надо реагировать

		inline void setUserString(const DisplayString & strUserString) {m_strUserString = strUserString;}; // ставит строку пользователя
		inline DisplayString & getUserString() {return m_strUserString;}; // возвращает строку пользователя
		inline void setUserData(uint32 uUserData) {m_uUserData = uUserData;}; // ставит данные пользователя
		inline uint32 getUserData() {return m_uUserData;}; // возвращает данные пользователя


		Overlay* m_overlay; // оверлей этого окна
		PanelOverlayElement * m_overlayContainer; // оверлей элемента
		OverlayContainer* m_overlayCaption; // оверлей текста
		vector<MyGUI::Window*>m_aWindowChild; // дети окна
		Window * m_pWindowFother; // отец окна
		Window * m_pWindowText; // элемент скина содержащий текст всего элемента (по дефолту this)
		Window * m_pWindowClient; // элемент скина является клиенским окном всего элемента (по дефолту this)
		int16 m_iPosX, m_iPosY, m_iSizeX, m_iSizeY; // размеры окна
		int16 m_iOffsetAliginX, m_iOffsetAliginY; // смещение, используется только при выравнивании по центру без растяжения

		GUI *m_GUI; // указатль на гуи
		uint8 m_uState; // статус окна
		EventCallback *m_pEventCallback; // указатель на класс для вызова функций
		uint16 m_uEventCallback; // флаги для посылки событий
		uint16 m_uAligin; // выравнивание окна и текста
		int16 m_sizeTextX; // размер текста окна
		int16 m_sizeTextY; // размер текста окна
		int16 m_sizeCutTextX; // видимый размер текста
		int16 m_sizeCutTextY; // видимый размер текста
		DisplayString m_strWindowText; // текст окна для буфера обрезки, когда текст не обрезан эта строка пуста
		bool m_bIsOverlapped; // окно перекрывающееся
		bool m_bIsTextShiftPressed; // сдвинут ли текст вниз

		__LP_MYGUI_FONT_INFO m_font; // шрифт окна
		uint32 m_fontColour; // цвет текста
		String * m_paStrSkins[__SKIN_STATE_COUNT]; // скины состояний
		uint16 m_uExData; // дополнительная информация об элементе

		DisplayString m_strUserString; // строка для дополнительной информации пользователя
		uint32 m_uUserData; // дополнительная информация пользователя

	};

}
//=========================================================================================
#endif
//=========================================================================================
