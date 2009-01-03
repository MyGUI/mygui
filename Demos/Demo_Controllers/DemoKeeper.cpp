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
		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_Controllers");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		MyGUI::ResourceManager::getInstance().load("Resources.xml");

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(128, 128);

		mMenu1 = new Menu("Menu1.layout", ControllerType::Inertional);
		mMenu1->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu2 = new Menu("Menu2.layout", ControllerType::Accelerated);
		mMenu2->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu3 = new Menu("Menu3.layout", ControllerType::Slowed);
		mMenu3->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu4 = new Menu("Menu4.layout", ControllerType::Jump);
		mMenu4->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu1->show();
		mType = ControllerType::Inertional;

	}

	void DemoKeeper::destroyScene()
	{
		delete mMenu1;
		delete mMenu2;
		delete mMenu3;
		delete mMenu4;
	}

	void DemoKeeper::notifyButtonPress(ControllerType _type)
	{
		if (mType == _type) return;

		if (mType == ControllerType::Inertional) mMenu1->hide();
		else if (mType == ControllerType::Accelerated) mMenu2->hide();
		else if (mType == ControllerType::Slowed) mMenu3->hide();
		else if (mType == ControllerType::Jump) mMenu4->hide();

		mType = _type;

		if (mType == ControllerType::Inertional) mMenu1->show();
		else if (mType == ControllerType::Accelerated) mMenu2->show();
		else if (mType == ControllerType::Slowed) mMenu3->show();
		else if (mType == ControllerType::Jump) mMenu4->show();
	}

} // namespace demo
