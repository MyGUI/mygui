/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

    void DemoKeeper::createScene()
    {

		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("ButtonX", MyGUI::IntCoord(300, 300, 140, 140), MyGUI::Align::Default, "Main");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>(MyGUI::WidgetStyle::Overlapped, "ButtonX", MyGUI::IntCoord(70, 70, 140, 140), MyGUI::Align::Default, "Main");

		//widget2->detachFromWidget("Main");
		//widget2->attachToWidget(widget, MyGUI::WidgetStyle::Overlapped);
		widget2->setWidgetStyle(MyGUI::WidgetStyle::Child, "Main");

	}

    void DemoKeeper::destroyScene()
    {
    }

} // namespace demo
