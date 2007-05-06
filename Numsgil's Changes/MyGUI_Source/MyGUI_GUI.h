#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_Coord.h"
#include "MyGUI_OIS.h"
#include "MyGUI_EventCallBack.h"
#include <OgreTimer.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>

namespace MyGUI {

	class GUI : public OIS::MouseListener, public OIS::KeyListener
	{
        GUI ();
		~GUI();
	
	public:
	    static GUI *getSingleton()
	    {
	        static GUI gui;
	        return &gui;	    
	    }
		
		GUI *Initialize(uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0);
    
        //Animation
		uint8 m_bFadeState; // идет анимация затемнения
		    //Fade
		    Real m_fCurrentFadeAlpha; // текущая альфа затемнения
		    uint8 m_uFadeID; // идентификация затемнения
		    EventCallback *m_pEventCallbackFade; // указатель на класс для вызова после затенения
		void eventUpdateAnimation(Real fTimeLastFrame); // обновляет анимацию гуи
        Window * m_currentFadeWindow; // окно полноэкранного затемнения
        
		//Mouse
		private:
		bool m_bIsActiveGUI; // активны ли GUI
		
		public:
		//override OIS implementations
		bool mouseMoved( const OIS::MouseEvent &arg ); // вызывать при движении мыши
		bool mousePressed( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при нажатии клавиш мыши
		bool mouseReleased( const OIS::MouseEvent &arg , OIS::MouseButtonID id ); // вызывать при отпускании клавиш мыши
		
		void createMousePointer();
		void setMousePointer(const Ogre::String uTypePointer);
		void showMousePointer(bool bIsShow)
		{
		    if (bIsShow)
		        m_overlayGUI[OVERLAY_MOUSE]->show();
            else
                m_overlayGUI[OVERLAY_MOUSE]->hide();
        };
        
        Ogre::String uPointerType; //current pointer type
		
		//Keyboard
		private:
		unsigned char m_bShiftChars; // сдвиг в массиве символов для ввода (0 - 3)
		
		public:
		//override OIS implementations
		bool keyPressed( const OIS::KeyEvent &arg ); // вызывать при нажатии кнопок клавы
		bool keyReleased( const OIS::KeyEvent &arg ); // вызывать при отпускании кнопок клавы
		
		void setKeyFocus(Window * pWindow); // ставим фокус ввода
		void fadeScreen(bool bIsFade, uint8 uFadeID = 0, EventCallback *pEventCallback = 0); // затеняем экран
		
		void detectLangShift(int keyEvent, bool bIsKeyPressed); // проверка на переключение языков
		wchar_t getKeyChar(int keyEvent); // возвращает символ по его скан коду
		
        //GUI Builder
		template<typename Widget> Widget *spawn(const Coord &Pos, const Coord &Size,
            const uint8 uOverlay, const Ogre::String &uSkin = __SKIN_WIDGET_DEFAULT)
        {
            if(uSkin == __SKIN_WIDGET_DEFAULT)
                return Widget::create(Pos, Size, NULL, 0, uOverlay);
            else
                return Widget::create(Pos, Size, NULL, 0, uOverlay, uSkin);
        }
        
        template<typename Widget> Widget *spawnReal(const Ogre::Vector2 &Pos, const Ogre::Vector2 &Size,
            const uint8 uOverlay, const uint8 uSkin = __SKIN_WIDGET_DEFAULT)
        {
            Coord NewPos, NewSize;
            
            NewPos  = Coord(Pos *  Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight));
            NewSize = Coord(Size * Vector2(GUI::getSingleton()->m_uWidth, GUI::getSingleton()->m_uHeight));
            
            return spawn<Widget>(NewPos, NewSize, uOverlay, uSkin);
        }
		
		
		Message * createMessage(const Ogre::DisplayString & strCaption,
		                        const Ogre::DisplayString & strMessage,
		                        uint16 uID, bool bIsModal,
		                        const Ogre::DisplayString & strButton1 = "Ok",
		                        const Ogre::DisplayString & strButton2 = "");

		
		Window * m_currentWindow; // окно над которым курсор, из личных дочек гуи
		Window * m_currentFocusWindow; // текущее окно которое может реагировать на мышь
		Window * m_currentEditWindow; // текущее окно которое может реагировать на клаву
				
		Window * getTopWindow();
		void destroyWindow(Window * pWindow); //destroys a specific window
		
		void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); //destroys all windows on an overlay level (?)
		
		//Z Order
		void upZOrder(Window *pWindow); // поднять окно по слоям вверх
		uint16 m_uMaxZOrder; // текущий максимальный номер слоя
        
        //Overlay
		Ogre::Overlay * createOverlay(Ogre::String strName, uint16 zOrder, bool bIsShow = true);
		Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // оверлеи гуи
		Ogre::OverlayContainer* m_overlayContainerMouse; // контейнер для курсора
        
        public:
		void eventWindowResize(uint16 uWidth, uint16 uHeight); // изменился размер главного окна
		uint16 m_uWidth, m_uHeight; // размеры экрана

		std::vector <Window*> m_aWindowChild; // дети гуи
		uint16 m_uOverlappedStart, m_uOverlappedEnd; // начало и конец в перекрывающихся окон в масиве
		
		EventCallback *m_pEventCallback; // главный указатель на класс для вызова функций и отсылки предупреждений
		bool m_bIsFocusWindowCapture; // захваченно ли окно
		int16 m_iOffsetPressedX, m_iOffsetPressedY; // смещение нажатия внутри заголовка
	};

}