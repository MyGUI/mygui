/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "SkinManager.h"

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr)
	{
	}

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/Tools/SkinEditor");
	}

	void DemoKeeper::createScene()
	{
		MyGUI::ResourceManager::getInstance().load("BackgroundTile.xml");

		tools::SkinManager* skinManager = new tools::SkinManager();
		skinManager->initialise();

		mMainPane = new tools::MainPane();
	}

	void DemoKeeper::destroyScene()
	{
		delete mMainPane;
		mMainPane = nullptr;

		tools::SkinManager* skinManager = tools::SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
