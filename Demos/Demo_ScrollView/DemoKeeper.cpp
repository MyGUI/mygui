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
	using namespace MyGUI;
	const IntSize & view = Gui::getInstance().getViewSize();
	const IntSize size(600, 300);

	WindowPtr window = Gui::getInstance().createWidget<Window>("WindowCS", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), Align::Default, "Main");
	window->setMinSize(200, 100);
	window->setCaption("ScrollView demo");
	ScrollViewPtr scroll_view = window->createWidget<ScrollView>("ScrollView", IntCoord(2, 2, window->getClientCoord().width-2, window->getClientCoord().height-2), Align::Stretch);

	scroll_view->setCanvasSize(512, 256);
	StaticImagePtr image = scroll_view->createWidget<StaticImage>("StaticImage", IntCoord(0, 0, 512, 256), Align::Default);
	image->setImageTexture("Ogre.png");
}

void DemoKeeper::end()
{
}
