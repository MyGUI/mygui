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
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Controllers");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		MyGUI::ResourceManager::getInstance().load("Resources.xml");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(128, 128);

		mMenu1.initialise("Menu1.layout");
		mMenu1.eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);
		mMenu2.initialise("Menu2.layout");
		mMenu2.eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);
		mMenu3.initialise("Menu3.layout");
		mMenu3.eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);
		mMenu4.initialise("Menu4.layout");
		mMenu4.eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu1.show();
		mIndex = 0;

		//mMenu1.mainWidget()->show();

		/*MyGUI::StaticImagePtr image = MyGUI::Gui::getInstance().createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), MyGUI::Align::Default, "Main");
		image->setItemResource("pic_Crystal_Clear_Butterfly");
		image->setItemGroup("States");
		image->setItemName("Normal");

		image->eventMouseSetFocus = MyGUI::newDelegate(notifyMouseSetFocus);
		image->eventMouseLostFocus = MyGUI::newDelegate(notifyMouseLostFocus);

		image->setMaskPeek("Crystal_Clear_Butterfly_Peek.png");*/

	}

	void DemoKeeper::notifyButtonPress(size_t _index)
	{
		if (mIndex == _index) return;

		if (mIndex == 0) mMenu1.hide();
		else if (mIndex == 1) mMenu2.hide();
		else if (mIndex == 2) mMenu3.hide();
		else if (mIndex == 3) mMenu4.hide();

		mIndex = _index;

		if (mIndex == 0) mMenu1.show();
		else if (mIndex == 1) mMenu2.show();
		else if (mIndex == 2) mMenu3.show();
		else if (mIndex == 3) mMenu4.show();
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo
