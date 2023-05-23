/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#include "CommandManager.h"

namespace demo
{

	CommandManager* CommandManager::msCommandManager = nullptr;

	CommandManager::CommandManager()
	{
		msCommandManager = this;
	}

	CommandManager::~CommandManager()
	{
		msCommandManager = nullptr;
	}

	void CommandManager::executeCommand(std::string_view _name, MyGUI::Any _data)
	{
		MapDelegate::iterator item = mDelegates.find(_name);
		if (item != mDelegates.end())
			(*item).second(_name, _data);
	}

	void CommandManager::registerCommand(std::string_view _name, CommandDelegate::IDelegate* _delegate)
	{
		auto it = mDelegates.find(_name);
		if (it == mDelegates.end())
			it = mDelegates.emplace(_name, CommandDelegate()).first;
		it->second = _delegate;
	}

	void CommandManager::unregisterCommand(std::string_view _name)
	{
		MapDelegate::iterator item = mDelegates.find(_name);
		if (item != mDelegates.end())
			mDelegates.erase(item);
	}


} // namespace demo
