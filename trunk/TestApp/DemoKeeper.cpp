/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
namespace demo
{
 
    void DemoKeeper::createScene()
    {

        using namespace MyGUI;
        const IntSize & view = Gui::getInstance().getViewSize();
        const IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
        base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		DDContainerPtr container = Gui::getInstance().createWidget<DDContainer>("EditStretch", IntCoord(10, 10, 200, 200), Align::Default, "Overlapped");

 
    }
 
    void DemoKeeper::destroyScene()
    {
    }
 
} // namespace demo 
 


  