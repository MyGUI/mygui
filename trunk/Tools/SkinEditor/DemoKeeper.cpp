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
#include "MainWindowManager.h"
#include "MyGUI_FilterNoneSkin.h"

template <> demo::DemoKeeper* MyGUI::Singleton<demo::DemoKeeper>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<demo::DemoKeeper>::mClassTypeName("DemoKeeper");

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

		tools::MainWindowManager* mainWindowManager = new tools::MainWindowManager();
		mainWindowManager->initialise();

		mMainPane = new tools::MainPane();
	}

	void DemoKeeper::destroyScene()
	{
		delete mMainPane;
		mMainPane = nullptr;

		tools::MainWindowManager* mainWindowManager = tools::MainWindowManager::getInstancePtr();
		mainWindowManager->shutdown();
		delete mainWindowManager;

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
