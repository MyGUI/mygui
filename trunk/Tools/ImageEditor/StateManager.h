/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_MANAGER_H__
#define __STATE_MANAGER_H__

#include <MyGUI.h>
#include "StateController.h"

namespace tools
{
	class StateManager :
		public MyGUI::Singleton<StateManager>
	{
	public:
		StateManager();
		virtual ~StateManager();

		void initialise();
		void shutdown();

		void pushState(StateController* _state);
		void popState();

		bool getStateActivate(StateController* _state);
		StateController* getCurentState();
		void rollbackToState(StateController* _state);

		void stateEvent(StateController* _state, const std::string& _event);

		void registerState(StateController* _state, const std::string& _name);
		void registerEventState(const std::string& _stateName, const std::string& _eventName, const std::string& _toState);

	private:
		std::string getNameState(StateController* _state);
		std::string getEventToState(const std::string& _currentStateName, const std::string& _eventName);
		StateController* getStateByName(const std::string& _stateName);

	private:
		typedef std::vector<StateController*> VectorStateController;
		VectorStateController mStates;

		typedef std::map<std::string, StateController*> MapStateController;
		MapStateController mStateName;

		typedef std::pair< std::string, std::pair<std::string, std::string> > PairPairString;
		typedef std::vector<PairPairString> VectorPairPairString;
		VectorPairPairString mLinks;
	};

} // namespace tools

#endif // __STATE_MANAGER_H__
