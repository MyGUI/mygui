/******************************************************************************
This is a rather odd way of doing this (an abomination perhaps?), but I couldn't
figure out how to do it with just templates that wouldn't drive the end user insane.

Basically, there are three versions of functions that handle clicks: one for each mouse button type.
They should all behave identically, but be distinct, which means they need to be
defined using the same code.  I couldn't think of any other method except just
copy-past-and-modify, which is bad practice.

The goal here is maintainability.  See MyGUI_EventSource.cpp for implementation.

FuncName(Event) should be defined prior to inclusion of this file.
CurrTimer should too.
********************************************************************************/

virtual void FuncName(Click)(const ScalarPair &Pos)
{
   FOR_EACH_LISTENER->FuncName(Click)(Pos);
}
        
virtual void FuncName(Down)(const ScalarPair &Pos)
{
    static const unsigned int DOUBLE_CLICK_THRESHOLD = 250;
    
    Origin.x = Pos.x;
    Origin.y = Pos.y;
    
    FOR_EACH_LISTENER->FuncName(Down)(Pos);
    
    //Check for a double click
    if(CurrTimer.getMilliseconds() <= DOUBLE_CLICK_THRESHOLD)
    {
        FuncName(DoubleClick)(Pos);
        return;
    }
    else
        CurrTimer.reset();
    
    if(mClickIsWhenMouseFirstGoesDown)
        FuncName(Click)(Pos);
}
virtual void FuncName(Up)(const ScalarPair &Pos)
{
    Origin.x = -1;
    Origin.y = -1;
    
    FOR_EACH_LISTENER->FuncName(Up)(Pos);
    
    //Dragging logic
    
    //add check against doing this on the double click return stroke
    if(!mClickIsWhenMouseFirstGoesDown)
    {
        FuncName(Click)(Pos);
        FOR_EACH_LISTENER->FuncName(Click)(Pos);
    }
}

virtual void FuncName(DoubleClick)(const ScalarPair &Pos)
{
    FOR_EACH_LISTENER->FuncName(DoubleClick)(Pos);
}

virtual void FuncName(Drag)(const ScalarPair &Pos, const ScalarPair &ChangeInPos)
{
    FOR_EACH_LISTENER->FuncName(Drag)(Pos, ChangeInPos);
}