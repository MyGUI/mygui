/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "BasisManager.h"

void DemoKeeper::start()
{

	MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(100, 100, 500, 500), MyGUI::ALIGN_DEFAULT, "Main");
	window->setCaption("ScrollView demo");
	MyGUI::ScrollViewPtr view = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(0, 0, window->getClientCoord().width, window->getClientCoord().height), MyGUI::ALIGN_STRETCH);

	view->setCanvasSize(168, 300);
	MyGUI::StaticImagePtr image = view->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 168, 300), MyGUI::ALIGN_DEFAULT);
	image->setImageTexture("child.png");
	image->addItem(MyGUI::IntCoord(0, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*2, 0, 42, 75));
	image->addItemFrame(0, MyGUI::IntCoord(42*3, 0, 42, 75));
	image->addItemFrameDublicate(0, 0);
	image->addItemFrameDublicate(0, 3);
	image->setItemFrameRate(0, 0.15);
	image->setItemSelect(0);
}

void DemoKeeper::end()
{
}
