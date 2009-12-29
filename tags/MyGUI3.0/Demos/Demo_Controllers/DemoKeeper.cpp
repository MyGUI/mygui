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

	DemoKeeper::DemoKeeper() :
		mMenu1(nullptr),
		mMenu2(nullptr),
		mMenu3(nullptr),
		mMenu4(nullptr),
		mIsAnim(false)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Controllers");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("ControllerFadeAlpha and ControllerPosition examples.");

		mIsAnim = false;

		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(128, 128);

		mMenu1 = new State("Menu1.layout", ControllerType::Inertional);
		mMenu1->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu2 = new State("Menu2.layout", ControllerType::Accelerated);
		mMenu2->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu3 = new State("Menu3.layout", ControllerType::Slowed);
		mMenu3->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu4 = new State("Menu4.layout", ControllerType::Jump);
		mMenu4->eventButtonPress = MyGUI::newDelegate(this, &DemoKeeper::notifyButtonPress);

		mMenu1->setVisible(true);
		mType = ControllerType::Inertional;
	}

	void DemoKeeper::destroyScene()
	{
		delete mMenu1;
		mMenu1 = nullptr;
		delete mMenu2;
		mMenu2 = nullptr;
		delete mMenu3;
		mMenu3 = nullptr;
		delete mMenu4;
		mMenu4 = nullptr;
	}

	void DemoKeeper::notifyButtonPress(ControllerType _type, bool _complite)
	{
		if (_complite)
		{
			mIsAnim = false;
			return;
		}

		if (mType == _type || mIsAnim) return;
		mIsAnim = true;

		if (mType == ControllerType::Inertional) mMenu1->setVisible(false);
		else if (mType == ControllerType::Accelerated) mMenu2->setVisible(false);
		else if (mType == ControllerType::Slowed) mMenu3->setVisible(false);
		else if (mType == ControllerType::Jump) mMenu4->setVisible(false);

		mType = _type;

		if (mType == ControllerType::Inertional) mMenu1->setVisible(true);
		else if (mType == ControllerType::Accelerated) mMenu2->setVisible(true);
		else if (mType == ControllerType::Slowed) mMenu3->setVisible(true);
		else if (mType == ControllerType::Jump) mMenu4->setVisible(true);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
