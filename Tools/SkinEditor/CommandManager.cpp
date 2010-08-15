/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "CommandManager.h"
#include "DemoKeeper.h"

template <> tools::CommandManager* MyGUI::Singleton<tools::CommandManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::CommandManager>::mClassTypeName("CommandManager");

namespace tools
{

	CommandManager::CommandManager()
	{
	}

	CommandManager::~CommandManager()
	{
	}

	void CommandManager::initialise()
	{
	}

	void CommandManager::shutdown()
	{
	}

	void CommandManager::ExecuteCommand(const MyGUI::UString& _command)
	{
	}

} // namespace tools
