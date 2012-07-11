/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "CommandManager.h"
#include "pugixml.hpp"
#include "MyGUI_DataManager.h"
#include "DataInfoManager.h"

namespace tools
{
	CommandManager* CommandManager::mInstance = nullptr;

	CommandManager::CommandManager() :
		mMaxCommands(256)
	{
		mInstance = this;
	}

	CommandManager::~CommandManager()
	{
		mInstance = nullptr;
	}

	CommandManager& CommandManager::getInstance()
	{
		return *mInstance;
	}

	CommandManager* CommandManager::getInstancePtr()
	{
		return mInstance;
	}

	void CommandManager::initialise()
	{
		mCurrentCommand = mCommands.end();
		mCommandAsSave = mCommands.end();
	}

	void CommandManager::shutdown()
	{
		clear();
	}

	void CommandManager::clear()
	{
		for (ListCommand::iterator command = mCommands.begin(); command != mCommands.end(); command ++)
			delete *command;
		mCommands.clear();

		mCurrentCommand = mCommands.end();
		mCommandAsSave = mCommands.end();
	}

	void CommandManager::doCommand(Command* _command)
	{
		removeRedo();

		mCommands.push_back(_command);
		mCurrentCommand = mCommands.end();
		mCurrentCommand --;

		updateMaxCommands();

		_command->doCommand();

		onChangeCommands();
	}

	void CommandManager::undoCommand()
	{
		if (mCurrentCommand != mCommands.begin())
		{
			(*mCurrentCommand)->undoCommand();
			mCurrentCommand --;

			onChangeCommands();
		}
	}

	void CommandManager::redoCommand()
	{
		ListCommand::iterator next = mCurrentCommand;
		next ++;
		if (next != mCommands.end())
		{
			(*mCurrentCommand)->doCommand();
			mCurrentCommand = next;

			onChangeCommands();
		}
	}

	void CommandManager::setCurrentCommandAsSave()
	{
		mCommandAsSave = mCurrentCommand;

		onChangeCommands();
	}

	bool CommandManager::getCurrentCommandAsSave()
	{
		return mCurrentCommand == mCommandAsSave;
	}

	void CommandManager::setMaxCommands(size_t _value)
	{
		MYGUI_ASSERT(_value > 0, "Max commands wrong");
		mMaxCommands = _value;

		bool change = updateMaxCommands();
		if (change)
			onChangeCommands();
	}

	size_t CommandManager::getMaxCommands() const
	{
		return mMaxCommands;
	}

	bool CommandManager::updateMaxCommands()
	{
		bool change = false;

		while (mCommands.size() > mMaxCommands)
		{
			if (mCommands.begin() == mCommandAsSave)
			{
				mCommandAsSave = mCommands.end();
				change = true;
			}

			Command* command = mCommands.front();
			mCommands.pop_front();
			delete command;
		}

		return change;
	}

	void CommandManager::removeRedo()
	{
		ListCommand::iterator last = mCommands.end();
		last --;

		while (mCurrentCommand != last)
		{
			Command* command = *last;
			delete command;

			if (last == mCommandAsSave)
			{
				mCommandAsSave = mCommands.end();
			}

			last --;
		}
	}

	void CommandManager::onChangeCommands()
	{
		// event
	}
}
