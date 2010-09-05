/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/

#include "CommandManager.h"

namespace demo
{

	CommandManager* CommandManager::msCommandManager = 0;

	CommandManager::CommandManager()
	{
		msCommandManager = this;
	}

	CommandManager::~CommandManager()
	{
		msCommandManager = 0;
	}

	void CommandManager::execiteCommand(const std::string& _name, MyGUI::Any _data)
	{
		MapDelegate::iterator item = mDelegates.find(_name);
		if (item != mDelegates.end())
			(*item).second(_name, _data);
	}

	void CommandManager::registerCommand(const std::string& _name, CommandDelegate::IDelegate* _delegate)
	{
		mDelegates[_name] = _delegate;
	}

	void CommandManager::unregisterCommand(const std::string& _name)
	{
		MapDelegate::iterator item = mDelegates.find(_name);
		if (item != mDelegates.end())
			mDelegates.erase(item);
	}


} // namespace demo
