/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _89360b4d_f172_4c02_9008_673f376bdee7_
#define _89360b4d_f172_4c02_9008_673f376bdee7_

#include <MyGUI.h>
#include "StateController.h"

namespace tools
{

	class MYGUI_EXPORT_DLL StateManager
	{
		MYGUI_SINGLETON_DECLARATION(StateManager);

	public:
		StateManager() :
			mSingletonHolder(this)
		{
		}
		void initialise();
		void shutdown();

		void pushState(StateController* _state);
		void pushState(std::string_view _stateName);
		void popState();

		bool getStateActivate(StateController* _state);
		StateController* getCurentState();
		void rollbackToState(StateController* _state);

		void stateEvent(StateController* _state, std::string_view _event);
		void stateEvent(std::string_view _stateName, std::string_view _event);

		void registerState(StateController* _state, std::string_view _name);
		void registerEventState(std::string_view _stateName, std::string_view _eventName, std::string_view _toState);

	private:
		std::string_view getNameState(StateController* _state);
		std::string_view getEventToState(std::string_view _currentStateName, std::string_view _eventName);
		StateController* getStateByName(std::string_view _stateName);

	private:
		using VectorStateController = std::vector<StateController*>;
		VectorStateController mStates;

		using MapStateController = std::map<std::string, StateController*, std::less<>>;
		MapStateController mStateName;

		using PairPairString = std::pair<std::string, std::pair<std::string, std::string>>;
		using VectorPairPairString = std::vector<PairPairString>;
		VectorPairPairString mLinks;
	};

}

#endif
