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
		std::wstring result = L"Skin editor - '";
		result += mFileName.empty() ? L"unnamed" : mFileName;
		result += L"' ";
		result += mChanges ? L"*" : L"";

		demo::DemoKeeper::getInstance().setWindowCaption(result);
	}

} // namespace tools
