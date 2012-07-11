/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "StateManager.h"

template <> tools::StateManager* MyGUI::Singleton<tools::StateManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::StateManager>::mClassTypeName("StateManager");

namespace tools
{

	StateManager::StateManager()
	{
	}

	StateManager::~StateManager()
	{
	}

	void StateManager::initialise()
	{
	}

	void StateManager::shutdown()
	{
		rollbackToState(nullptr);
	}

	void StateManager::pushState(StateController* _state)
	{
		MYGUI_ASSERT(_state != nullptr, "State is nullptr");
		MYGUI_ASSERT(std::find(mStates.begin(), mStates.end(), _state) == mStates.end(), "State already added");

		StateController* pauseState = getCurentState();

		mStates.push_back(_state);

		if (pauseState != nullptr)
			pauseState->pauseState();

		_state->initState();
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

	void StateManager::stateEvent(StateController* _state, const std::string& _event)
	{
		std::string currentStateName = getNameState(_state);
		std::string toStateName = getEventToState(currentStateName, _event);

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

	void StateManager::registerState(StateController* _state, const std::string& _name)
	{
		mStateName[_name] = _state;
	}

	void StateManager::registerEventState(const std::string& _stateName, const std::string& _eventName, const std::string& _toState)
	{
		mLinks.push_back(std::make_pair(_stateName, std::make_pair(_eventName, _toState)));
	}

	std::string StateManager::getNameState(StateController* _state)
	{
		for (MapStateController::const_iterator item = mStateName.begin(); item != mStateName.end(); ++item)
		{
			if ((*item).second == _state)
				return (*item).first;
		}
		return "";
	}

	std::string StateManager::getEventToState(const std::string& _currentStateName, const std::string& _eventName)
	{
		for (VectorPairPairString::const_iterator item = mLinks.begin(); item != mLinks.end(); ++item)
		{
			if ((*item).first == _currentStateName && (*item).second.first == _eventName)
				return (*item).second.second;
		}
		return "";
	}

	StateController* StateManager::getStateByName(const std::string& _stateName)
	{
		MapStateController::iterator item = mStateName.find(_stateName);
		if (item != mStateName.end())
			return (*item).second;
		return nullptr;
	}

} // namespace tools
