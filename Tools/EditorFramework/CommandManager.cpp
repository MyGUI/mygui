/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "CommandManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(CommandManager);

	void CommandManager::initialise()
	{
	}

	void CommandManager::shutdown()
	{
		for (auto& event : mEvents)
			delete event.second;

		mEvents.clear();
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

		EventType* event = getEvent(command);
		if (event != nullptr)
		{
			event->operator()(command, result);
		}
		else
		{
			MYGUI_LOG(Warning, "Command '" << command << "' not found");
		}

		mData.clear();

		return result;
	}

	void CommandManager::setCommandData(const MyGUI::UString& _data)
	{
		mData = _data;
	}

	const MyGUI::UString& CommandManager::getCommandData() const
	{
		return mData;
	}

	CommandManager::EventType* CommandManager::getEvent(const MyGUI::UString& _command)
	{
		MapEvent::iterator event = mEvents.find(_command);
		if (event != mEvents.end())
			return (*event).second;

		EventType* type = new EventType();
		mEvents[_command] = type;
		return type;
	}

}
