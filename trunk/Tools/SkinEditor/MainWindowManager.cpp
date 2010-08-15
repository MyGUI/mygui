/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainWindowManager.h"
#include "DemoKeeper.h"

template <> tools::MainWindowManager* MyGUI::Singleton<tools::MainWindowManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::MainWindowManager>::mClassTypeName("MainWindowManager");

namespace tools
{

	MainWindowManager::MainWindowManager() :
		mChanges(false)
	{
	}

	MainWindowManager::~MainWindowManager()
	{
	}

	void MainWindowManager::initialise()
	{
		updateCaption();
	}

	void MainWindowManager::shutdown()
	{
	}

	void MainWindowManager::setChanges(bool _value)
	{
		mChanges = _value;
		updateCaption();
	}

	void MainWindowManager::setFileName(const MyGUI::UString& _value)
	{
		mFileName = _value;
		updateCaption();
	}

	void MainWindowManager::updateCaption()
	{
		demo::DemoKeeper::getInstance().setWindowCaption(MyGUI::utility::toString("Skin editor - '", mFileName.empty() ? "unnamed" : mFileName, "' ", mChanges ? "*" : ""));
	}

} // namespace tools
