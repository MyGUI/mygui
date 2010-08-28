/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
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
		while (getCurentState() != _state || getCurentState() != nullptr)
			popState();
	}

} // namespace tools
