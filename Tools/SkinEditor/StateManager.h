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

	private:
		typedef std::vector<StateController*> VectorStateController;
		VectorStateController mStates;
	};

} // namespace tools

#endif // __STATE_MANAGER_H__
