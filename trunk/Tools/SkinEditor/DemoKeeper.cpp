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
#include "CommandManager.h"
#include "MyGUI_FilterNoneSkin.h"

template <> demo::DemoKeeper* MyGUI::Singleton<demo::DemoKeeper>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<demo::DemoKeeper>::mClassTypeName("DemoKeeper");

namespace demo
{

	DemoKeeper::DemoKeeper() :
		mMainPane(nullptr),
		mChanges(false)
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

		tools::CommandManager* commandManager = new tools::CommandManager();
		commandManager->initialise();

		mMainPane = new tools::MainPane();

		updateCaption();
	}

	void DemoKeeper::destroyScene()
	{
		delete mMainPane;
		mMainPane = nullptr;

		tools::CommandManager* commandManager = tools::CommandManager::getInstancePtr();
		commandManager->shutdown();
		delete commandManager;

		tools::ActionManager* actionManager = tools::ActionManager::getInstancePtr();
		actionManager->shutdown();
		delete actionManager;

		tools::SkinManager* skinManager = tools::SkinManager::getInstancePtr();
		skinManager->shutdown();
		delete skinManager;

		MyGUI::FactoryManager::getInstance().unregisterFactory<MyGUI::FilterNone>("BasisSkin");
	}

	void DemoKeeper::onFileDrop(const std::wstring& _filename)
	{
	}

	bool DemoKeeper::onWinodwClose(size_t _handle)
	{
		return true;
	}

	void DemoKeeper::setChanges(bool _value)
	{
		mChanges = _value;
		updateCaption();
	}

	void DemoKeeper::setFileName(const MyGUI::UString& _value)
	{
		mFileName = _value;
		updateCaption();
	}

	void DemoKeeper::updateCaption()
	{
		std::wstring result = L"Skin editor - '";
		result += mFileName.empty() ? L"unnamed" : mFileName;
		result += L"' ";
		result += mChanges ? L"*" : L"";

		setWindowCaption(result);
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)
