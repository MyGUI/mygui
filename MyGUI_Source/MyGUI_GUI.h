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

	class GUI : public OIS::KeyListener, public OIS::MouseListener {

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

        //Mouse
		    bool mouseMoved( const OIS::MouseEvent &arg ); // вызывать при движении мыши
		    bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при нажатии клавиш мыши
		    bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при отпускании клавиш мыши
		        //Somehow related to input (Mouse)
		        bool m_bIsActiveGUI; // активны ли GUI

		//Keyboard
		    bool keyPressed( const OIS::KeyEvent &arg ); // вызывать при нажатии кнопок клавы
		    bool keyReleased( const OIS::KeyEvent &arg ); // вызывать при отпускании кнопок клавы
		    /*
		        Called by the message and combobox widgets.  A predominantly internal function
		        that sets the current focus window (that active window that receives key input)
		    */
		    void setKeyFocus(Window * pWindow); // ставим фокус ввода
		    //Helper functions:
		        void detectLangShift(int keyEvent, bool bIsKeyPressed); // проверка на переключение языков
		        /*
		            Non public.  Called by the keyboard functions.  Converts a key code into a character
		        */
		        wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду

		//Sizing
		    unsigned int m_uWidth, m_uHeight; // размеры экрана
		        /*
		            Public interface, must be called by user when they resize their window.
		        */
		        void eventWindowResize(const unsigned int uWidth, const unsigned int uHeight); // изменился размер главного окна
		
		//Animation
		
		    /*
		        Public interface: Must be called by the user every frame if they want to properly have animation working.
		        (Ie: at the moment this is just teh fade screen.)
		    */    
		    void eventUpdateAnimation(Real fTimeLastFrame); // обновляет анимацию гуи

		/*
		    Unknown scope, most likely private.  Called when the GUI is initialized.
		*/
		void createMousePointer(); // создает оверлей для мыши и курсоры
		
		/*
		    Public interface: allows user and widgets to change the current cursor
		    graphic.
		*/
		void setMousePointer(const String &TypePointer); // изменить указатель
		
		/*
		    Public interface: allows users to show or hide the mouse graphic
		    by turning on/off the associated overlay layer.
		*/
		void showMousePointer(bool bIsShow = true) { // скрыть показать курсор
		     if(bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
             else
		        m_overlayGUI[OVERLAY_MOUSE]->hide();
        }
        
        /*
            Public interface.  Sets the fade state.
                bIsFade turns on/off the fade screen.
                uFadeID appears to have something to do with callback events (?)
                pEventCallback is the EventCallback to call when the fade state changes (?).
                    Only one is allowed.  Newer callbacks replace older ones                
        */
		void fadeScreen(bool bIsFade, FADE_STATES uFadeID = NONE, EventCallback *pEventCallback = 0); // затеняем экран
		
		//GUI Builder
		    /*
		        GUI Builder functions.  Very public: these are the heart and soul of the GUI library.
		    */
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
        /*
            Never called, so I'm not sure what the intended use is.
        */
		Window * getTopWindow(); // возвращает самое верхнее окно из перекрывающихся
		
		/*
		    Destroys a specific window and handles any cleanup associated with it.
		    Question: why can we destroy a window in two ways?  (Ie: destroyWindow(Window) and delete Window)
		*/
		void destroyWindow(MyGUI::Window * pWindow); // уничтожает окно и удаляет из всех списков (на прямую не удалять)
		
		/*
		    Destroys an entire layer of Windows.
		*/		
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // уничтожает все окна из оверлея		
		
		/*
		    Called to set up the default overlays, as well as to set up new overlay layers as needed to handle
		    overlapping windows.  Does not need to be a public interface if the Window class's use can be abstracted
		*/
		Ogre::Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // создает оверлей

		/*
		    Moves a window up in its ZOrder (To the top of the stack?)
		*/
		void upZOrder(Window *pWindow);
		
		/*
		    These are both only referenced by Window class, so should be moved as private members of window class
		*/
		void getLengthText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, const DisplayString & strSource); // возвращает длинну текста
		void getCutText(const __tag_MYGUI_FONT_INFO *font, int16 &sizeX, int16 &sizeY, DisplayString & strDest, const DisplayString & strSource, uint16 uAlign); // возвращает обрезанную строку равную размерам

		uint16 m_uMaxZOrder; // текущий максимальный номер слоя
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // оверлеи гуи
		Ogre::OverlayContainer* m_overlayContainerMouse; // контейнер для курсора
        
        
		int16 m_iCurrentOffsetCursorX, m_iCurrentOffsetCursorY; // текущее смещение для курсора
		unsigned char m_bShiftChars; // сдвиг в массиве символов для ввода (0 - 3)

		std::vector <Window*> mChildWindows; // дети гуи
		size_t m_uOverlappedStart, m_uOverlappedEnd; // начало и конец в перекрывающихся окон в масиве
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
		FADE_STATES m_uFadeID; // идентификация затемнения
		EventCallback *m_pEventCallbackFade; // указатель на класс для вызова после затенения

		std::map <String, uint16> m_mapFlagEvent; // имена флагов для парсинга
		std::map <String, uint16> m_mapFlagAlign; // имена флагов для парсинга
		std::map <String, uint16> m_mapFlagStyle; // имена флагов для парсинга

	};

}