/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void notifyMouseSetFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _old)
	{
		MyGUI::StaticImagePtr image = _sender->castType<MyGUI::StaticImage>();
		image->setItemName("Active");
	}

	void notifyMouseLostFocus(MyGUI::WidgetPtr _sender, MyGUI::WidgetPtr _new)
	{
		MyGUI::StaticImagePtr image = _sender->castType<MyGUI::StaticImage>();
		image->setItemName("Normal");
	}

	void DemoKeeper::createScene()
	{
        base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Peeking");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper1.jpg");

		MyGUI::ResourceManager::getInstance().load("Resources.xml");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(128, 128);

		MyGUI::StaticImagePtr image = MyGUI::Gui::getInstance().createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		image->setItemResource("pic_Crystal_Clear_Butterfly");
		image->setItemGroup("States");
		image->setItemName("Normal");

		image->eventMouseSetFocus = MyGUI::newDelegate(notifyMouseSetFocus);
		image->eventMouseLostFocus = MyGUI::newDelegate(notifyMouseLostFocus);

	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo
