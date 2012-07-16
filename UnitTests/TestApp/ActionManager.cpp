/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "ActionManager.h"
#include "pugixml.hpp"
#include "MyGUI_DataManager.h"
#include "DataInfoManager.h"

namespace tools
{
	ActionManager* ActionManager::mInstance = nullptr;

	ActionManager::ActionManager() :
		mMaxActions(5)
	{
		mInstance = this;
	}

	ActionManager::~ActionManager()
	{
		mInstance = nullptr;
	}

	ActionManager& ActionManager::getInstance()
	{
		return *mInstance;
	}

	ActionManager* ActionManager::getInstancePtr()
	{
		return mInstance;
	}

	void ActionManager::initialise()
	{
		reset();
	}

	void ActionManager::shutdown()
	{
		clear();
	}

	void ActionManager::clear()
	{
		for (ListAction::iterator command = mActions.begin(); command != mActions.end(); command ++)
			delete *command;
		mActions.clear();

		mCurrentAction = mActions.end();
		mActionAsSave = mActions.end();
	}

	void ActionManager::doAction(Action* _command)
	{
		removeRedo();

		mActions.push_back(_command);
		mCurrentAction ++;

		updateMaxActions();

		_command->doAction();

		onChangeActions();
	}

	void ActionManager::undoAction()
	{
		if (mCurrentAction == mActions.begin())
			return;

		Action* command = (*mCurrentAction);
		mCurrentAction --;

		command->undoAction();
		onChangeActions();
	}

	void ActionManager::redoAction()
	{
		ListAction::iterator next = mCurrentAction;
		next ++;

		if (next == mActions.end())
			return;

		mCurrentAction = next;
		Action* command = *mCurrentAction;

		command->doAction();
		onChangeActions();
	}

	void ActionManager::resetChanges()
	{
		mActionAsSave = mCurrentAction;

		onChangeActions();
	}

	bool ActionManager::getChanges()
	{
		return mCurrentAction != mActionAsSave;
	}

	void ActionManager::setMaxActions(size_t _value)
	{
		MYGUI_ASSERT(_value > 0, "Max commands wrong");
		mMaxActions = _value;

		bool change = updateMaxActions();
		if (change)
			onChangeActions();
	}

	size_t ActionManager::getMaxActions() const
	{
		return mMaxActions;
	}

	bool ActionManager::updateMaxActions()
	{
		bool change = false;

		if (mActions.size() < 2)
			return change;

		while (mActions.size() > (mMaxActions + 1))
		{
			ListAction::iterator second = mActions.begin();
			second ++;

			if (second == mActionAsSave || mActionAsSave == mActions.begin())
			{
				mActionAsSave = mActions.end();
				change = true;
			}

			Action* command = *second;
			mActions.erase(second);
			delete command;
		}

		return change;
	}

	void ActionManager::removeRedo()
	{
		ListAction::iterator last = mActions.end();
		last --;

		while (mCurrentAction != last)
		{
			Action* command = *last;

			if (last == mActionAsSave)
				mActionAsSave = mActions.end();

			last--;
			mActions.pop_back();

			delete command;
		}
	}

	void ActionManager::onChangeActions()
	{
		eventChanges();
	}

	void ActionManager::reset()
	{
		clear();

		Action* command = new Action();
		mActions.push_back(command);

		mCurrentAction = mActions.begin();
		mActionAsSave = mActions.begin();
	}
}
