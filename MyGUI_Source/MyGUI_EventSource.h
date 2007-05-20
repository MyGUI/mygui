#include "MyGUI_ScalarPair.h"
#include <OgreTimer.h>

namespace MyGUI
{
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
        public:
        /********************
        Inherited from MouseEventListener
        *********************/
            virtual void onLeftClick        (const ScalarPair &Pos);
            virtual void onLeftDown         (const ScalarPair &Pos);
            virtual void onLeftUp           (const ScalarPair &Pos);
            virtual void onLeftDoubleClick  (const ScalarPair &Pos);
            virtual void onLeftDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos);
            
            virtual void onRightClick        (const ScalarPair &Pos);
            virtual void onRightDown         (const ScalarPair &Pos);
            virtual void onRightUp           (const ScalarPair &Pos);
            virtual void onRightDoubleClick  (const ScalarPair &Pos);
            virtual void onRightDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos);
            
            virtual void onMiddleClick        (const ScalarPair &Pos);
            virtual void onMiddleDown         (const ScalarPair &Pos);
            virtual void onMiddleUp           (const ScalarPair &Pos);
            virtual void onMiddleDoubleClick  (const ScalarPair &Pos);
            virtual void onMiddleDrag         (const ScalarPair &Pos, const ScalarPair &ChangeInPos);
            
            virtual void onHover              (const ScalarPair &Pos);
        
            //Implementation might get weird because we probably don't want to store the tooltip text in this class
            virtual void onTooltip            (const ScalarPair &Pos);
    
        bool mClickIsWhenMouseFirstGoesDown; //should be made a global setting for the entire 
        std::list<MouseEventListener *> mListeners;
        typedef std::list<MouseEventListener *>::iterator ListenerIterator;
        
        ScalarPair Origin;
        Ogre::Timer LeftTimer, MiddleTimer, RightTimer; //Times how long since the last click
        
        public:
        MouseEventSource(bool ClickIsWhenMouseFirstGoesDown = true);
        
        void addListener(MouseEventListener *listener);
    };
}