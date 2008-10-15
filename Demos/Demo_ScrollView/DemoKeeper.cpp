/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{
        base::BaseManager::getInstance().addResourceLocation("../../Media/Shared");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper1.jpg");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(500, 300);

		MyGUI::WindowPtr window = MyGUI::Gui::getInstance().createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		window->setMinSize(200, 100);
		window->setCaption("ScrollView demo");
		MyGUI::ScrollViewPtr scroll_view = window->createWidget<MyGUI::ScrollView>("ScrollView", MyGUI::IntCoord(2, 2, window->getClientCoord().width-2, window->getClientCoord().height-2), MyGUI::Align::Stretch);

		scroll_view->setCanvasSize(512, 256);
		MyGUI::StaticImagePtr image = scroll_view->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 512, 256), MyGUI::Align::Default);
		image->setImageTexture("Ogre.png");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo
