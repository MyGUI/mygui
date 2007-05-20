#include "MyGUI_ScalarPair.h"
#include "MyGUI_EventSource.h"

/*
    The "Widget" class defines simple and rough "container" behavior for
    GUI objects (called "widgets").  Event handling, parent and child relations,
    sizing and position are controlled here.  Things like skins, graphics,
    representation, etc. are not.  Those are handled in the child class
    "Window".
*/

namespace MyGUI
{
    class Widget : public MouseEventSource
    {
        ScalarPair mPos, mSize;
        std::list<Widget *> mChildren;
        typedef std::list<Widget *>::iterator ChildrenIterator;
        Widget *mParent;
        
        Widget(ScalarPair Bounds, Widget *parent) : MouseEventSource()            
        {
            mParent = parent;
            //set bounds
        }
        
        void addChild(Widget *child)
        {
            assert(child != this);
            child->mParent = this;
            mChildren.push_back(child);
            mChildren.sort(); //sort so we can call unique
            mChildren.unique();
        }
        
        void removeChild(Widget *child)
        {
            assert(child != this);
            mChildren.remove(child);
        }
        
        const ScalarPair &pos()
        {
            return mPos;
        }
        
        const ScalarPair &size()
        {
            return mSize;
        }
        
        void resize(const ScalarPair &NewSize)
        {
            //1st, check to see if we're going to be exceeding our parent's bounds
            mSize = NewSize;
            
            for(ChildrenIterator Iter = mChildren.begin(); Iter != mChildren.end(); ++Iter)
                (*Iter)->redefineBounds(mSize);
        }
        
        void move(const ScalarPair &NewPos)
        {
        
        }
        
        void redefineBounds(const ScalarPair &Bounds)
        {
            //Check to see if we need to move ourselves
            //towards our parent's origin
            if(mPos.x > mParent->size().x ||
               mPos.y > mParent->size().y)
            {
            
            }
            
            if(mParent->size().x < mPos.x + mSize.x)
            {
                //Size + Pos = Parent
                //Size = Parent - Pos;
                //mSize.x = __max(0, (mParent->size().x - mPos.x).);
            }
            if(mParent->size().y < mPos.y + mSize.y)
            {
                //mSize.y = __max(0, mParent->size().y - mPos.y);
            }
            
            //if(mParent->size()
        }
    };
}