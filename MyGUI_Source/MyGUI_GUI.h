//==================================================================================================
#ifndef __MyGUI_GUI_H__
#define __MyGUI_GUI_H__
//==================================================================================================
#include "MyGUI.h"
//==================================================================================================
using namespace Ogre;
using namespace std;

namespace MyGUI {

	class Window;
	class WindowFrame;
	class Button;
	class Edit;
	class VScroll;
	class HScrollBar;
	class Tab;
	class StaticText;
	class List;
	class ComboBox;
	class Message;

	enum __FADE_STATE { // текущий режим затемнения
		FADE_END = 0,
		FADE_DOWN,
		FADE_UP
	};
	enum __MESSAGE_BOX_BUTTON { // кнопки окна сообщения
		MBB_NONE = 0x00,
		MBB_BUTTON1 = 0x01, // первая кнопка подтверждения
		MBB_BUTTON2 = 0x02, // вторая кнопка отмены
		MBB_MODAL = 0x10, // окно запускается в модальном режиме, !!! запускается только одно
	};
	enum __WINDOW_EX_STYLE { // дополниельные стили элементов скинов
		// общие флаги
		WES_NONE = 0x0000, // простое окно
		WES_TEXT = 0x0001, // элемент является насителем текста для всего элемента
		WES_CLIENT = 0x0002, // элемент является клиетским окном
		WES_ACTION1 = 0x0004,
		WES_ACTION2 = 0x0008,
		WES_ACTION3 = 0x0010,
		WES_ACTION4 = 0x0020,
		WES_ACTION5 = 0x0040,
		WES_ACTION6 = 0x0080,
		// флаги для WindowFrame
		WES_CAPTION = 0x0010, // элемент является заголовком
		WES_RESIZE = 0x0020, // элемент является ресайзером
		WES_CLOSE = 0x0040, // элемент является кнопкой закрытия
		// флаги для Button
		WES_BUTTON = 0x0010, // элемент является кнопкой
		// флаги для Edit
		WES_EDIT = 0x0010, // элемент является окном редактирования
		WES_EDIT_CURSOR = 0x0020, // элемент является курсором для окна редактирования
		// флаги для VScroll
		WES_VSCROLL_UP = 0x0010, // элемент является верхней кнопкой скролла
		WES_VSCROLL_DOWN = 0x0020, // элемент является нижней кнопкой скролла
		WES_VSCROLL_TRACK = 0x0040, // элемент является нижней кнопкой скролла
		// флаги для TabBar
		WES_TAB_TOP = 0x0010, // элемент является верхним скином для растягивания
		// флаги для списка
		WES_LIST_HIDE_SCROLL = 0x0010, // скрывать скролл , если он не нужен
		// флаги для выпадающего списка
		WES_COMBO = 0x0010, // выпадающий список
		WES_COMBO_BUTTON = 0x0020, // кнопка в выпадающем списке
	};

	enum __WINDOW_EVENT { // события на которое окно быдет посылать
		WE_NONE = 0x00, // ничего не будет посылаться
		WE_MOUSE_BUTTON=0x01, // нажата левая кнпка мыши
		WE_MOUSE_FOCUS=0x02, // смена активности от курсора мыши
		WE_MOUSE_MOVE=0x04, // движение мыши после захвата
		WE_KEY_FOCUS=0x08, // смена фокуса для ввода клавиатуры
		WE_KEY_BUTTON=0x10, // нажата клавиша
		__WE_IS_ACTION = 0xFF, // !! для внутреннего использования , не юзать  |   есть ли реакция на события мыши и клавиатуры
		// сообщения об опасности, посылаются всегда
		WE_WARNING_ZORDER_MAX = 0x1000, // слои перекрывающихся окон подобрались к границе
		WE_WARNING_CHILD_MAX = 0x2000, // колличество дитей равно шагу между слоями
		WE_WARNING_CHILD_DELETE = 0x4000, // ребенок уничтожен, поправте указатели
	};
	enum __WINDOW_OTHER_EVENT { // события посылаемые в OtherEvents
		WOE_NONE = 0x00000, // нет сообщения
		WOE_SCROLL_CHANGE_POS = 0x0001, // изменилась позиция скролла
		WOE_FRAME_CLOSE = 0x0002, // нажата кнопка закрыть фрейм
		WOE_EDIT_KEY_ACCEPT = 0x0004, // нажата кнопка энтер
		WOE_LIST_SELECT_CHANGE = 0x0008, // изменился выделенный элемент
		WOE_LIST_SELECT_ACCEPT = 0x0010, // двойной щелчек или энтер на элементе
		WOE_LIST_SELECT_NONE = 0x0020, // щелчек на пустом месте
		WOE_COMBO_SELECT_ACCEPT = 0x0040, // активирован элемент
		WOE_MESSAGE_PRESS_BUTTON = 0x0080, // нажата кнопка в окне сообщения
	};
	enum __WINDOW_STATE { // состояние окна
		WS_DEACTIVE, // окно неактивно и недоступно
		WS_NORMAL, // обычное состояние окна
		WS_PRESSED, // кнопка нажата
		__WS_ACTIVED, // кнопка под курсором, для внутреннего использования
		__WS_SELECTED // кнопка под курсором и нажата, для внутреннего использования
	};
	enum __WINDOW_ALIGIN {
		// выравнивание окон, если хотим двигать окна, то выравниваем (WA_LEFT|WA_TOP)
		WA_NONE=0x00, //                                 окно выравнивается по центру
		WA_HCENTER=0x00, //                           липа, при нуле он будет центрировать без растяжения
		WA_VCENTER=0x00, //                            но центрировать не относительно границ отца
		WA_CENTER=0x00, //                              а относительно своего первоначального положения, если нужно другое смотри ниже
		WA_CENTER_FOTHER=0x1000, //           это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		WA_LEFT=0x01, WA_RIGHT=0x02, //     по левому или по правому
		WA_HSTRETCH=0x03,  //                        растягивание по горизонтали
		WA_TOP=0x04, WA_BOTTOM=0x08, //  по верху или по низу
		WA_VSTRETCH=0x0C,  //                        растягивание по вертикали
		WA_STRETCH=0x0F, //                           растягивание по всем граням
		// выравнивание текста, у него нет растягивания
		WAT_NONE=0x00, // не выравнивать то же, что WAT_LEFT|WAT_TOP
		WAT_LEFT=0x10, WAT_RIGHT=0x20, WAT_HCENTER=0x30, // горизонтально
		WAT_TOP=0x40, WAT_BOTTOM=0x80, WAT_VCENTER=0xC0, // вертикально
		WAT_CENTER=0xF0, // по центру окна
		// обрезка текста
		WAT_CUT_RIGHT=0x0100, WAT_CUT_LEFT=0x0200, // по левой или по правой стороне
		WAT_ADD_DOT=0x0400, // добавлять три точки при обрезке
		WAT_CUT_RIGHT_DOT=0x0500, WAT_CUT_LEFT_DOT=0x0600, // по левой или по правой стороне и еще с точками
		__WAT_IS_CUT=0x0300, // для внутренних нужд, не юзать!  нужна ли обрезка
		WAT_MULTI_LINE = 0x0800, // многострочный текст
		WAT_BREAK_WORD = 0x2000, // переносить по словам
		WAT_SHIFT_TEXT_PRESSED = 0x8000, // сдвигать ли текст при нажатии
	};

	// курсоры мыши
	enum __MYGUI_POINTER {
		POINTER_DEFAULT, // обычный указатель
		POINTER_RESIZE, // для изменения размеров
		POINTER_TEXT, // для текстового ввода
		__POINTER_COUNT
	};

	// все оверлеи класса, добавлять после OVERLAY_MOUSE и до __OVERLAY_COUNT, далее идут служебные (OVERLAY_OVERLAPPED, OVERLAY_CHILD)
	enum __MYGUI_OVERLAYS {
		OVERLAY_FADE, // оверлей для затемнения
		OVERLAY_MOUSE, // оверлей для мыши, !!! не использовать при создании окна
		OVERLAY_POPUP, // оверлей для всплывающего окна , меню, комбобокса, окна не перекрываются
		OVERLAY_MAIN, // основной оверлей гуи
		OVERLAY_BACK, // самый нижний оверлей гуи
		__OVERLAY_COUNT, // колличество оверлеев, !!! не использовать при создании окна
		OVERLAY_OVERLAPPED, // для создания перекрывающихся окон
		OVERLAY_CHILD, // окно принадлежит отцу, обязательно указать отца
		OVERLAY_DESTROY_ALL // удаление всех окон в функции destroyWindow
	};

	class EventCallback // базовый класс для вызовов
	{
	public:
		virtual void onMouseButton(MyGUI::Window * pWindow, bool bIsLeftButtonPressed) {}; // нажата левая кнопка мыши
		virtual void onMouseClick(MyGUI::Window * pWindow) {}; // нажата и отпущена левая кнопка мыши на этом же элементе
		virtual void onMouseDoubleClick(MyGUI::Window * pWindow) {}; // двойной щелчек мыши
		virtual void onMouseFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // смена фокуса
		virtual void onMouseMove(MyGUI::Window * pWindow, int16 iPosX, int16 iPosY) {}; // уведомление о движении, но не движение

		virtual void onKeyFocus(MyGUI::Window * pWindow, bool bIsFocus) {}; // смена фокуса ввода
		virtual void onKeyButton(MyGUI::Window * pWindow, int keyEvent, wchar_t cText) {}; // нажата клавиша

		virtual void onWarningEvent(MyGUI::Window * pWindow, uint16 uEvent) {}; // события предепреждающие ошибки
		virtual void onOtherEvent(MyGUI::Window * pWindow, uint16 uEvent, uint32 data) {}; // дополнительные события
		virtual void onFadeEnd(bool bIsFade, uint8 fadeID) {}; // закончилось затемнение
	};

	class GUI {

	public:

		GUI (uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0); // обязательная регистрация размеров
		~GUI();

		bool eventMouseMove( const OIS::MouseEvent &arg ); // вызывать при движении мыши
		bool eventMousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при нажатии клавиш мыши
		bool eventMouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при отпускании клавиш мыши

		bool eventKeyPressed( const OIS::KeyEvent &arg ); // вызывать при нажатии кнопок клавы
		bool eventKeyReleased( const OIS::KeyEvent &arg ); // вызывать при отпускании кнопок клавы

		void eventWindowResize(uint16 uWidth, uint16 uHeight); // изменился размер главного окна
		void eventUpdateAnimation(Real fTimeLastFrame); // обновляет анимацию гуи

		void detectLangShift(int keyEvent, bool bIsKeyPressed); // проверка на переключение языков
		wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду

		void createMousePointer(); // создает оверлей для мыши и курсоры
		void setMousePointer(uint8 uTypePointer); // изменить указатель
		void showMousePointer(bool bIsShow) {if (bIsShow) m_overlayGUI[OVERLAY_MOUSE]->show();else m_overlayGUI[OVERLAY_MOUSE]->hide();}; // скрыть показать курсор
		void setKeyFocus(Window * pWindow); // ставим фокус ввода
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // затеняем экран
		void createSkin(); // загружает все скины
		void unloadResource(); // выгружает все скины
		void clearSkins(__LP_MYGUI_SKIN_INFO pSkin); // удаляет скин
		void loadSkin(std::map<String, uint32> & mapNameValue, const String & strFileName); // загружает все скины окон из одного файла

		MyGUI::Window * createWindow(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback = 0); // обыкновенное окно
		MyGUI::Window * createWindowReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin, EventCallback * pEventCallback = 0) { // обыкновенное окно
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createWindow(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		};
		MyGUI::WindowFrame * createWindowFrame(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay = OVERLAY_OVERLAPPED, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0); // окно с рамкой
		MyGUI::WindowFrame * createWindowFrameReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay = OVERLAY_OVERLAPPED, uint8 uSkin = SKIN_WINDOWFRAME_CSX, EventCallback * pEventCallback = 0) { // окно с рамкой
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createWindowFrame(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		};
		MyGUI::Button * createButton(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0); // создает кнопку
		MyGUI::Button * createButtonReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_BUTTON, EventCallback * pEventCallback = 0) { // создает кнопку
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createButton(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Edit * createEdit(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0); // создает окно редактирования
		MyGUI::Edit * createEditReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_EDIT, EventCallback * pEventCallback = 0) { // создает окно редактирования
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createEdit(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::VScroll * createVScroll(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0); // создает полосу прокрутки
		MyGUI::VScroll * createVScrollReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_VSCROLL, EventCallback * pEventCallback = 0) { // создает полосу прокрутки
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createVScroll(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Tab * createTab(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0); // создает панель вкладок
		MyGUI::Tab * createTabReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_TAB, EventCallback * pEventCallback = 0) { // создает панель вкладок
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createTab(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::StaticText * createStaticText(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0); // создает статический текст
		MyGUI::StaticText * createStaticTextReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, const DisplayString & strWindowText, uint8 uOverlay, uint8 uSkin = SKIN_STATIC_TEXT, EventCallback * pEventCallback = 0) { // создает статический текст
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createStaticText(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, strWindowText, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::List * createList(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0); // создает список
		MyGUI::List * createListReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_LIST_S, EventCallback * pEventCallback = 0) { // создает список
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createList(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::ComboBox * createComboBox(int16 iPosX, int16 iPosY, int16 iSizeX, int16 iSizeY, uint8 uOverlay, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0); // создает выпадающий список
		MyGUI::ComboBox * createComboBoxReal(Real fPosX, Real fPosY, Real fSizeX, Real fSizeY, uint8 uOverlay, uint8 uSkin = SKIN_COMBO_BOX, EventCallback * pEventCallback = 0) { // создает выпадающий список
			if (fSizeX != -1.0) fSizeX *= m_uWidth;
			if (fSizeY != -1.0) fSizeY *= m_uHeight;
			return createComboBox(fPosX*m_uWidth, fPosY*m_uHeight, fSizeX, fSizeY, uOverlay, uSkin, pEventCallback);
		}
		MyGUI::Message * createMessage(const DisplayString & strCaption, const DisplayString & strMessage, uint16 uID, bool bIsModal, const DisplayString & strButton1 = "Ok", const DisplayString & strButton2 = ""); // окно сообщения

		MyGUI::Window * getTopWindow(); // возвращает самое верхнее окно из перекрывающихся
		void destroyWindow(MyGUI::Window * pWindow); // уничтожает окно и удаляет из всех списков (на прямую не удалять)
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // уничтожает все окна из оверлея
		Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // создает оверлей

		void upZOrder(Window *pWindow); // поднять окно по слоям вверх
		void getLenghtText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // возвращает длинну текста
		void getCutText(__LP_MYGUI_FONT_INFO &font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAligin); // возвращает обрезанную строку равную размерам

		uint16 m_uMaxZOrder; // текущий максимальный номер слоя
		Overlay* m_overlayGUI[__OVERLAY_COUNT]; // оверлеи гуи
		OverlayContainer* m_overlayContainerMouse; // контейнер для курсора

		bool m_bIsActiveGUI; // активны ли GUI
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // текущее смещение для курсора
		unsigned char m_bShiftChars; // сдвиг в массиве символов для ввода (0 - 3)

		uint16 m_uWidth, m_uHeight; // размеры экрана

		vector <Window*> m_aWindowChild; // дети гуи
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // начало и конец в перекрывающихся окон в масиве
		Window * m_currentWindow; // окно над которым курсор, из личных дочек гуи
		Window * m_currentFocusWindow; // текущее окно которое может реагировать на мышь
		Window * m_currentEditWindow; // текущее окно которое может реагировать на клаву
		Window * m_currentFadeWindow; // окно полноэкранного затемнения
		EventCallback *m_pEventCallback; // главный указатель на класс для вызова функций и отсылки предупреждений
		bool m_bIsFocusWindowCapture; // захваченно ли окно
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // смещение нажатия внутри заголовка

		Timer m_time; // таймер для двойного щелчка
		uint8 m_bFadeState; // идет анимация затемнения
		Real m_fCurrentFadeAlpha; // текущая альфа затемнения
		uint8 m_uFadeID; // идентификация затемнения
		EventCallback *m_pEventCallbackFade; // указатель на класс для вызова после затенения

		vector <__LP_MYGUI_FONT_INFO> m_fontInfo; // вектор всех шрифтов
		vector <__LP_MYGUI_WINDOW_INFO> m_windowInfo; // вектор всех окон скинов
		vector <__LP_MYGUI_POINTER_INFO> m_pointerInfo; // вектор указателей на курсоры

		vector <String> m_strSkinNames; // имена скинов
		vector <String> m_strFontNames; // имена шрифтов

		map <String, uint16> m_mapFlagEvent; // имена флагов для парсинга
		map <String, uint16> m_mapFlagAligin; // имена флагов для парсинга
		map <String, uint16> m_mapFlagStyle; // имена флагов для парсинга

	};

}
//==================================================================================================
#endif
//==================================================================================================
