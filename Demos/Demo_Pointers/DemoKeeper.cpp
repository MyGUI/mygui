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
		mControlPanel(nullptr),
		mPointerManager(nullptr)
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
		getGUI()->load("Wallpaper0.layout");
		MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().load("BackHelp.layout");
		root.at(0)->findWidget("Text")->setCaption("");

		mPointerManager = new PointerManager();
		mPointerManager->addContext("ptrx_Normal");

		mEnemyPanel = new EnemyPanel();
		mFriendPanel = new FriendPanel();
		mControlPanel = new ControlPanel(mPointerManager);

		MyGUI::Gui::getInstance().load("pointers.xml");

	}

	void DemoKeeper::destroyScene()
	{
		delete mEnemyPanel;
		mEnemyPanel = nullptr;
		delete mFriendPanel;
		mFriendPanel = nullptr;
		delete mControlPanel;
		mControlPanel = nullptr;

		delete mPointerManager;
		mPointerManager = nullptr;
	}

	void DemoKeeper::injectMouseMove(int _absx, int _absy, int _absz)
	{
		if (!getGUI())
			return;


		// ввод мыши находить вне гу€
		if (!getGUI()->injectMouseMove(_absx, _absy, _absz))
		{
			// пикаем сцену, в нашем случае это картинки,
			// которые по сути не пикаютс€ как гуевые
			if (mFriendPanel->isIntersect(_absx, _absy))
			{
				mPointerManager->setPointer("friend");
			}
			else if (mEnemyPanel->isIntersect(_absx, _absy))
			{
				mPointerManager->setPointer("enemy");
			}
			else
			{
				// курсор не во что не попал в сцене
				mPointerManager->setPointer("normal");
			}
		}
	}

	void DemoKeeper::injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (!getGUI()->injectMousePress(_absx, _absy, _id))
		{
		}
	}

	void DemoKeeper::injectMouseRelease(int _absx, int _absy, MyGUI::MouseButton _id)
	{
		if (!getGUI())
			return;

		if (!getGUI()->injectMouseRelease(_absx, _absy, _id))
		{
		}
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

