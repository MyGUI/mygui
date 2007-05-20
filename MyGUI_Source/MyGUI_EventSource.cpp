#include "MyGUI_EventSource.h"

namespace MyGUI
{
    #define FOR_EACH_LISTENER for(ListenerIterator Iter = mListeners.begin(); Iter != mListeners.end(); ++Iter) (*Iter)
    
    MouseEventSource::MouseEventSource(bool ClickIsWhenMouseFirstGoesDown)
    {
        mClickIsWhenMouseFirstGoesDown = ClickIsWhenMouseFirstGoesDown;
    }
    
    void MouseEventSource::addListener(MouseEventListener *listener)
    {
        mListeners.push_back(listener);        
    }
    
    /********************************************************************************
    Note: these functions assume that arg has been modified before they recieve it such that the
    mouse position is relative to the window.  For instance, if there's a window at 100,100 and
    it's 200 by 200 pixels, and the user clicks at 200,200, arg should return coordinates of 100,100
    and relative coordinates of .5, .5
    *********************************************************************************/
    
    void MouseEventSource::onHover(const ScalarPair &Pos)
    {
        FOR_EACH_LISTENER->onHover(Pos);
        
        const unsigned int TOOLTIP_POPUP_DELAY = 1000; //1 second
        
        //if(HoverTime.getMilliseconds() >= TOOLTIP_POPUP_DELAY)
        //    onTooltip(Pos);
    }
    
    void MouseEventSource::onTooltip(const ScalarPair &Pos)
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
}