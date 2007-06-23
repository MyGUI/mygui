#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_OIS.h"
#include "MyGUI_Skin.h"
#include "MyGUI_InputHandlers.h"
#include <map>
#include <OgreOverlayElement.h>
#include <OgreOverlay.h>

namespace MyGUI {

    using Ogre::DisplayString;

	class GUI : public MouseHandler, public KeyboardHandler {

	protected:

		GUI () : MouseHandler(), KeyboardHandler() { }
		~GUI() { }
		
	public:
	    static GUI *getSingleton()
	    {
	        static GUI Instance;
	        
	        return &Instance;
        }
        
        GUI *Initialize (uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback = 0); // обязательная регистрация размеров
        void Shutdown();
        
        template <typename Widget> Widget *create(int16 PosX, int16 PosY, int16 SizeX, int16 SizeY,
	        uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
	    {
            if(Skin == __SKIN_WIDGET_DEFAULT)
                return Widget::createWindow(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay);
            else
                return Widget::createWindow(PosX, PosY, SizeX, SizeY, NULL, 0, uOverlay, Skin);
        }
        
        template<typename Widget> Widget *createReal(Real PosX, Real PosY, Real SizeX, Real SizeY,
	        uint8 uOverlay, const String &Skin = __SKIN_WIDGET_DEFAULT)
	    {
	        return create<Widget>(PosX *  GUI::getSingleton()->getWidth(), PosY *  GUI::getSingleton()->getHeight(),
                             SizeX * GUI::getSingleton()->getWidth(), SIzeY * GUI::getSingleton()->getHeight(),
                             uOverlay, uSkin);		
	    }

	    MyGUI::Message * createMessage(const DisplayString & strCaption,
	        const DisplayString & strMessage, uint16 uID, bool bIsModal,
	        const DisplayString & strButton1 = "Ok",
	        const DisplayString & strButton2 = ""); // окно сообщения        
        
        EventCallback *m_pEventCallback; // главный указатель на класс для вызова функций и отсылки предупреждений
        
        //Sizing
		private:
		    unsigned int m_uWidth, m_uHeight; // размеры экрана
		public:
		    unsigned int getWidth() { return m_uWidth; }
		    unsigned int getHeight() { return m_uHeight; }
		    Real getAspectRatio()
		    {
		        assert(getWidth());
		        assert(getHeight());
		        
		        return Real(getWidth()) / Real(getHeight());
		    }
	        /*
	            Public interface, must be called by user when they resize their window.
	        */
	        void eventWindowResize(const unsigned int uWidth, const unsigned int uHeight); // изменился размер главного окна
		
		//Animation
		
		    /*
		        Public interface: Must be called by the user every frame if they want to properly have animation working.
		        (at the moment this is just the fade screen.)
		    */    
		    void eventUpdateAnimation(Real fTimeLastFrame); // обновляет анимацию гуи
		    
		    /*
                Public interface.  Sets the fade state.
                    bIsFade turns on/off the fade screen.
                    uFadeID appears to have something to do with callback events (?)
                    pEventCallback is the EventCallback to call when the fade state changes (?).
                        Only one is allowed.  Newer callbacks replace older ones                
            */
		    void fadeScreen(bool bIsFade, FADE_STATES uFadeID = NONE, EventCallback *pEventCallback = 0); // затеняем экран
		    
		    private:
		    Window * m_currentFadeWindow; // окно полноэкранного затемнения
		    uint8 m_bFadeState; // идет анимация затемнения
		    Real m_fCurrentFadeAlpha; // текущая альфа затемнения
		    FADE_STATES m_uFadeID; // идентификация затемнения
		    EventCallback *m_pEventCallbackFade; // указатель на класс для вызова после затенения
		    public:		
		
		/*
		    Called when the GUI is initialized.
		*/
		private:
		void createMousePointer(); // создает оверлей для мыши и курсоры
		public:
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
        
        //Window Management(?)
        
    		
		    /*
		        Destroys a specific window and handles any cleanup associated with it.
		        Question: why can we destroy a window in two ways?  (Ie: destroyWindow(Window) and delete Window)

				The answer: 
				It has been made after long searches of problems
				if there is time and desire you can correct on removal through delete.
				But all problems with removal you will incur.
		    */
		    void destroyWindow(MyGUI::Window * pWindow); // уничтожает окно и удаляет из всех списков (на прямую не удалять)
    		
		    /*
		        Destroys an entire layer of Windows.
		    */		
		    void destroyWindow(__MYGUI_OVERLAYS overlay = OVERLAY_DESTROY_ALL); // уничтожает все окна из оверлея		
		
		//Z Order related
            
            uint16 m_uMaxZOrder; // текущий максимальный номер слоя
            
            /*
		        Called to set up the default overlays, as well as to set up new overlay layers as needed to handle
		        overlapping windows.  Does not need to be a public interface if the Window class's use can be abstracted
		    */
		    Ogre::Overlay * createOverlay(String strName, uint16 zOrder, bool bIsShow = true); // создает оверлей

		    /*
		        Moves a window up in its ZOrder (To the top of the stack?)
		    */
		    void upZOrder(Window *pWindow);

		//Overlays
		    Ogre::Overlay* m_overlayGUI[__OVERLAY_COUNT]; // оверлеи гуи
		    Ogre::OverlayContainer* m_overlayContainerMouse; // контейнер для курсора
        
        
		std::vector <Window*> mRootWindows; // Root windows
		typedef std::vector <Window*>::reverse_iterator RootWindowReverseIterator;
		
		//Only referenced outside of GUI class by Window class
		size_t m_uOverlappedStart, m_uOverlappedEnd; // начало и конец в перекрывающихся окон в масиве
		
	};

}