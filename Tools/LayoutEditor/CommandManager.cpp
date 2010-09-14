/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "CommandManager.h"

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

	void CommandManager::executeCommand(const MyGUI::UString& _command)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			iter->second(_command);
		}
		else
		{
			MYGUI_LOG(Warning, "Command '" << _command << "' not found");
		}

		mData.clear();
	}

	void CommandManager::registerCommand(const MyGUI::UString& _command, CommandDelegate::IDelegate* _delegate)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter == mDelegates.end())
			iter = mDelegates.insert(std::make_pair(_command, CommandDelegate())).first;
		(*iter).second += _delegate;
	}

	void CommandManager::setCommandData(const MyGUI::UString& _data)
	{
		mData = _data;
	}

	const MyGUI::UString& CommandManager::getCommandData()
	{
		return mData;
	}

} // namespace tools
