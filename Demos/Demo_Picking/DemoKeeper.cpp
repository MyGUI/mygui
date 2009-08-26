/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"

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

	void DemoKeeper::setupResources()
	{
		addResourceLocation("../../Media/Demos/Demo_Picking");
		addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::setupResources();
	}

	void DemoKeeper::createScene()
	{
		setWallpaper("wallpaper0.jpg");
		setDescriptionText("Sample of picking mask for widget.");

		MyGUI::ResourceManager::getInstance().load("Resources.xml");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(128, 128);

		MyGUI::StaticImagePtr image = mGUI->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		image->setItemResource("pic_Crystal_Clear_Butterfly");
		image->setItemGroup("States");
		image->setItemName("Normal");

		image->eventMouseSetFocus = MyGUI::newDelegate(notifyMouseSetFocus);
		image->eventMouseLostFocus = MyGUI::newDelegate(notifyMouseLostFocus);

		image->setMaskPick("Crystal_Clear_Butterfly_Pick.png");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
