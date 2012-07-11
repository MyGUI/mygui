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

	bool CommandManager::executeCommand(const MyGUI::UString& _command)
	{
		bool result = false;
		MyGUI::UString command = _command;
		size_t index = _command.find('.');
		if (index != MyGUI::UString::npos)
		{
			command = _command.substr(0, index);
			mData = _command.substr(index + 1);
		}

		MapDelegate::iterator iter = mDelegates.find(command);
		if (iter != mDelegates.end())
		{
			iter->second(command, result);
		}
		else
		{
			MYGUI_LOG(Warning, "Command '" << command << "' not found");
		}

		mData.clear();

		return result;
	}

	void CommandManager::registerCommand(const MyGUI::UString& _command, CommandDelegate::IDelegate* _delegate)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter == mDelegates.end())
			iter = mDelegates.insert(std::make_pair(_command, CommandDelegate())).first;
		(*iter).second += _delegate;
	}

	void CommandManager::unregisterCommand(const MyGUI::UString& _command, CommandDelegate::IDelegate* _delegate)
	{
		MapDelegate::iterator iter = mDelegates.find(_command);
		if (iter != mDelegates.end())
		{
			(*iter).second -= _delegate;
			if ((*iter).second.empty())
				mDelegates.erase(iter);
		}
	}

	void CommandManager::setCommandData(const MyGUI::UString& _data)
	{
		mData = _data;
	}

	const MyGUI::UString& CommandManager::getCommandData() const
	{
		return mData;
	}

} // namespace tools
