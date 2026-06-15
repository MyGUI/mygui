/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "StateManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(StateManager);

	void StateManager::initialise()
	{
	}

	void StateManager::shutdown()
	{
		rollbackToState(nullptr);

		for (auto& state : mStateName)
			delete state.second;
		mStateName.clear();
	}

	void StateManager::pushState(StateController* _state)
	{
		MYGUI_ASSERT(_state != nullptr, "State not found");
		MYGUI_ASSERT(std::find(mStates.begin(), mStates.end(), _state) == mStates.end(), "State already added");

		StateController* pauseState = getCurentState();

		mStates.push_back(_state);

		if (pauseState != nullptr)
			pauseState->pauseState();

		_state->initState();
	}

	void StateManager::pushState(std::string_view _stateName)
	{
		StateController* state = getStateByName(_stateName);

		MYGUI_ASSERT(state != nullptr, "State not found");
		MYGUI_ASSERT(std::find(mStates.begin(), mStates.end(), state) == mStates.end(), "State already added");

		pushState(state);
	}

	void StateManager::popState()
	{
		StateController* clenupState = getCurentState();

		if (!mStates.empty())
			mStates.pop_back();

		StateController* resumeState = getCurentState();

		if (clenupState != nullptr)
			clenupState->cleanupState();

		if (resumeState != nullptr)
			resumeState->resumeState();
	}

	bool StateManager::getStateActivate(StateController* _state)
	{
		return getCurentState() == _state;
	}

	StateController* StateManager::getCurentState()
	{
		if (mStates.empty())
			return nullptr;
		return mStates.back();
	}

	void StateManager::rollbackToState(StateController* _state)
	{
		while (getCurentState() != _state && getCurentState() != nullptr)
			popState();
	}

	void StateManager::stateEvent(std::string_view _stateName, std::string_view _event)
	{
		StateController* state = getStateByName(_stateName);

		MYGUI_ASSERT(state != nullptr, "State not found");

		stateEvent(state, _event);
	}

	void StateManager::stateEvent(StateController* _state, std::string_view _event)
	{
		std::string_view currentStateName = getNameState(_state);
		std::string_view toStateName = getEventToState(currentStateName, _event);

		StateController* state = getStateByName(toStateName);
		if (state == nullptr)
			return;

		if (std::find(mStates.begin(), mStates.end(), state) == mStates.end())
		{
			pushState(state);
		}
		else
		{
			rollbackToState(state);
		}
	}

	void StateManager::registerState(StateController* _state, std::string_view _name)
	{
		MyGUI::mapSet(mStateName, _name, _state);
	}

	void StateManager::registerEventState(
		std::string_view _stateName,
		std::string_view _eventName,
		std::string_view _toState)
	{
		mLinks.emplace_back(_stateName, std::make_pair(_eventName, _toState));
	}

	std::string_view StateManager::getNameState(StateController* _state)
	{
		for (MapStateController::const_iterator item = mStateName.begin(); item != mStateName.end(); ++item)
		{
			if ((*item).second == _state)
				return (*item).first;
		}
		return {};
	}

	std::string_view StateManager::getEventToState(std::string_view _currentStateName, std::string_view _eventName)
	{
		for (const auto& link : mLinks)
		{
			if (link.first == _currentStateName && link.second.first == _eventName)
				return link.second.second;
		}
		return {};
	}

	StateController* StateManager::getStateByName(std::string_view _stateName)
	{
		MapStateController::iterator item = mStateName.find(_stateName);
		if (item != mStateName.end())
			return (*item).second;
		return nullptr;
	}

}
