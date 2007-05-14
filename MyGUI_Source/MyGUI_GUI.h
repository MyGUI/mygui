#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_OIS.h"
#include "MyGUI_Skin.h"
#include <OgreTimer.h>
#include <map>
#include <OgreOverlayElement.h>
#include <OgreOverlay.h>

namespace MyGUI {

    using Ogre::DisplayString;

	class GUI {

	protected:

		GUI () { }// обязательная регистрация размеров
		~GUI() { }
		
	public:
	    static GUI *getSingleton()
	    {
	        static GUI Instance;
	        
	        return &Instance;
        }
        
        GUI *Initialize (uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0); // обязательная регистрация размеров
        void Shutdown();

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
		void setMousePointer(const String &TypePointer); // изменить указатель
		void showMousePointer(bool bIsShow) {
		    if (bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
            else
                m_overlayGUI[OVERLAY_MOUSE]->hide();
        } // скрыть показать курсор
		void setKeyFocus(Window * pWindow); // ставим фокус ввода
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // затеняем экран
		
		template<typename Widget> Widget *spawn(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
		    uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
		{
	        if(Skin == __SKIN_WIDGET_DEFAULT)
	            return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay);
	        else
	            return Widget::create(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay, Skin);
	    }
		    
		template<typename Widget> Widget *spawnReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
		    uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
		{
		    return spawn<Widget>(PosX *  GUI::getSingleton()->m_uWidth, PosY *  GUI::getSingleton()->m_uHeight,
                             SizeX * GUI::getSingleton()->m_uWidth, SIzeY * GUI::getSingleton()->m_uHeight,
                             uOverlay, uSkin);		
		}

		MyGUI::Message * createMessage(const DisplayString & strCaption,
		    const DisplayString & strMessage, uint16 uID, bool bIsModal,
		    const DisplayString & strButton1 = "Ok", const DisplayString & strButton2 = ""); // окно сообщения

		MyGUI::Window * getTopWindow(); // возвращает самое верхнее окно из перекрывающихся
		void destroyWindow(MyGUI::Window * pWindow); // уничтожает окно и удаляет из всех списков (на прямую не удалять)
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // уничтожает все окна из оверлея
		Ogre::Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // создает оверлей

		void upZOrder(Window *pWindow); // поднять окно по слоям вверх
		void getLenghtText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // возвращает длинну текста
		void getCutText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign); // возвращает обрезанную строку равную размерам

		uint16 m_uMaxZOrder; // текущий максимальный номер слоя
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // оверлеи гуи
		Ogre::OverlayContainer* m_overlayContainerMouse; // контейнер для курсора

		bool m_bIsActiveGUI; // активны ли GUI
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // текущее смещение для курсора
		unsigned char m_bShiftChars; // сдвиг в массиве символов для ввода (0 - 3)

		uint16 m_uWidth, m_uHeight; // размеры экрана

		std::vector <Window*> m_aWindowChild; // дети гуи
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // начало и конец в перекрывающихся окон в масиве
		Window * m_currentWindow; // окно над которым курсор, из личных дочек гуи
		Window * m_currentFocusWindow; // текущее окно которое может реагировать на мышь
		Window * m_currentEditWindow; // текущее окно которое может реагировать на клаву
		Window * m_currentFadeWindow; // окно полноэкранного затемнения
		EventCallback *m_pEventCallback; // главный указатель на класс для вызова функций и отсылки предупреждений
		bool m_bIsFocusWindowCapture; // захваченно ли окно
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // смещение нажатия внутри заголовка

		Ogre::Timer m_time; // таймер для двойного щелчка
		uint8 m_bFadeState; // идет анимация затемнения
		Real m_fCurrentFadeAlpha; // текущая альфа затемнения
		uint8 m_uFadeID; // идентификация затемнения
		EventCallback *m_pEventCallbackFade; // указатель на класс для вызова после затенения

		std::map <String, uint16> m_mapFlagEvent; // имена флагов для парсинга
		std::map <String, uint16> m_mapFlagAlign; // имена флагов для парсинга
		std::map <String, uint16> m_mapFlagStyle; // имена флагов для парсинга

	};

}