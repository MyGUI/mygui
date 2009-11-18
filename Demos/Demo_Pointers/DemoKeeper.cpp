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
		mEnemyPanel(nullptr),
		mFriendPanel(nullptr),
		mControlPanel(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Demos/Demo_Pointers");
		addResourceLocation(getRootMedia() + "/Common/Wallpapers");
	}

	void DemoKeeper::createScene()
	{
		//getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("");

		mEnemyPanel = new EnemyPanel();
		mFriendPanel = new FriendPanel();
		mControlPanel = new ControlPanel();

		MyGUI::Gui::getInstance().load("pointers.xml");
		MyGUI::PointerManager::getInstance().setPointer("ptr_Repare");
	}

	void DemoKeeper::destroyScene()
	{
		delete mEnemyPanel;
		mEnemyPanel = nullptr;
		delete mFriendPanel;
		mFriendPanel = nullptr;
		delete mControlPanel;
		mControlPanel = nullptr;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

