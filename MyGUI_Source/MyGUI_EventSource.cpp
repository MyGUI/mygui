#include "MyGUI_Defines.h"
#include "MyGUI_ScalarPair.h"
#include "MyGUI_OIS.h"
#include <OgreTimer.h>

namespace MyGUI
{
    //Prototypes
    class MouseEventSource;
    class MouseEventListener;
    
    class MouseEventListener
    {
        public:
        virtual void onLeftClick        (const ScalarPair &Pos) { }
        virtual void onLeftDown         (const ScalarPair &Pos) { }
        virtual void onLeftUp           (const ScalarPair &Pos) { }
        virtual void onLeftDoubleClick  (const ScalarPair &Pos) { }
        virtual void onLeftDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos) { }
        
        virtual void onRightClick        (const ScalarPair &Pos) { }
        virtual void onRightDown         (const ScalarPair &Pos) { }
        virtual void onRightUp           (const ScalarPair &Pos) { }
        virtual void onRightDoubleClick  (const ScalarPair &Pos) { }
        virtual void onRightDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos) { }
        
        virtual void onMiddleClick        (const ScalarPair &Pos) { }
        virtual void onMiddleDown         (const ScalarPair &Pos) { }
        virtual void onMiddleUp           (const ScalarPair &Pos) { }
        virtual void onMiddleDoubleClick  (const ScalarPair &Pos) { }
        virtual void onMiddleDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos) { }
        
        virtual void onHover              (const ScalarPair &Pos) { }
        
        //Implementation might get weird because we probably don't want to store the tooltip text in this class
        virtual void onTooltip            (const ScalarPair &Pos) { }
    };
    
    class MouseEventSource : public MouseEventListener
    {
        bool mClickIsWhenMouseFirstGoesDown; //should be made a global setting for the entire 
        std::list<MouseEventListener *> mListeners;
        typedef std::list<MouseEventListener *>::iterator ListenerIterator;
        
        ScalarPair Origin;
        Ogre::Timer LeftTimer, MiddleTimer, RightTimer; //Times how long since the last click
        
        #define FOR_EACH_LISTENER for(ListenerIterator Iter = mListeners.begin(); Iter != mListeners.end(); ++Iter) (*Iter)
        
        MouseEventSource(bool ClickIsWhenMouseFirstGoesDown = true)
        {
            mClickIsWhenMouseFirstGoesDown = ClickIsWhenMouseFirstGoesDown;
        }
        
        public:
        void addListener(MouseEventListener *listener)
        {
            mListeners.push_back(listener);        
        }
        
        /********************************************************************************
        Note: these functions assume that arg has been modified before they recieve it such that the
        mouse position is relative to the window.  For instance, if there's a window at 100,100 and
        it's 200 by 200 pixels, and the user clicks at 200,200, arg should return coordinates of 100,100
        and relative coordinates of .5, .5
        *********************************************************************************/
        
        virtual void onHover(const ScalarPair &Pos)
        {
            FOR_EACH_LISTENER->onHover(Pos);
            
            const unsigned int TOOLTIP_POPUP_DELAY = 1000; //1 second
            
            //if(HoverTime.getMilliseconds() >= TOOLTIP_POPUP_DELAY)
            //    onTooltip(Pos);
        }
        
        virtual void onTooltip(const ScalarPair &Pos)
        {
            FOR_EACH_LISTENER->onTooltip(Pos);
            //Tell GUI to display the tooltip for this object
        }       
        
        #define FuncName(Event) onLeft ## Event
        #define CurrTimer LeftTimer
        #include "EventSourceHelper.h"
        #undef FuncName
        #undef CurrTimer
        
        #define FuncName(Event) onRight ## Event
        #define CurrTimer RightTimer
        #include "EventSourceHelper.h"        
        #undef FuncName
        #undef CurrTimer
        
        #define FuncName(Event) onMiddle ## Event
        #define CurrTimer MiddleTimer
        #include "EventSourceHelper.h"
        #undef FuncName
        #undef CurrTimer
        
        #undef FOR_EACH_LISTENER        
    };
}