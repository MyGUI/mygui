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

	class MYGUI_EXPORT_DLL StateManager :
		public MyGUI::Singleton<StateManager>
	{
	public:
		StateManager();
		virtual ~StateManager();

		void initialise();
		void shutdown();

		void pushState(StateController* _state);
		void pushState(const std::string& _stateName);
		void popState();

		bool getStateActivate(StateController* _state);
		StateController* getCurentState();
		void rollbackToState(StateController* _state);

		void stateEvent(StateController* _state, const std::string& _event);
		void stateEvent(const std::string& _stateName, const std::string& _event);

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

}

#endif
