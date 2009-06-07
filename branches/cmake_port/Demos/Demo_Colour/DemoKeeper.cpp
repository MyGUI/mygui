/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

namespace demo
{

	void foo(MyGUI::WidgetPtr _sender)
	{
	}

	void DemoKeeper::createScene()
	{
		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("Button", MyGUI::IntCoord(10, 10, 100, 100), MyGUI::Align::Default, "Popup");

		widget->eventMouseButtonDoubleClick = MyGUI::newDelegate(foo);
		widget->eventMouseButtonDoubleClick = nullptr;

		MyGUI::WidgetManager::getInstance().destroyWidget(widget);

		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Colour");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		base::BaseManager::getInstance().setDescriptionText("Sample colour picker implementation. Select text in Edit and then select colour to colour part of text.");
		MyGUI::Gui::getInstance().load("colour_slider_skin.xml");

		mColourPanel = new ColourPanel();
		mEditPanel = new EditPanel();

		mColourPanel->eventColourAccept = MyGUI::newDelegate(this, &DemoKeeper::notifyColourAccept);
	}

	void DemoKeeper::destroyScene()
	{
		delete mEditPanel;
		delete mColourPanel;
	}

	void DemoKeeper::notifyColourAccept(ColourPanel* _sender)
	{
		mEditPanel->setColourSelected(_sender->getColour());
	}

} // namespace demo

int startApp<demo::DemoKeeper>(int argc, char **argv);

int main(int argc, char **argv)
{
  return startApp<demo::DemoKeeper>(argc, argv);
}
