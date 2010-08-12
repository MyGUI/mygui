/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "SkinManager.h"
#include "ActionManager.h"
#include "MyGUI_FilterNoneSkin.h"

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
		MyGUI::FactoryManager::getInstance().registerFactory<MyGUI::FilterNone>("BasisSkin");

		MyGUI::ResourceManager::getInstance().load("SE_skins.xml");

		tools::SkinManager* skinManager = new tools::SkinManager();
		skinManager->initialise();

		tools::ActionManager* actionManager = new tools::ActionManager();
		actionManager->initialise();

		mMainPane = new tools::MainPane();
	}

	void DemoKeeper::destroyScene()
	{
		delete mMainPane;
		mMainPane = nullptr;

		tools::ActionManager* actionManager = tools::ActionManager::getInstancePtr();
		actionManager->shutdown();
		delete actionManager;

		tools::SkinManager* skinManager = tools::SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
