/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _897c30fe_12de_4067_91d0_a2a336a18f83_
#define _897c30fe_12de_4067_91d0_a2a336a18f83_

#include <list>
#include "Action.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ActionManager
	{
	public:
		ActionManager();
		~ActionManager();

		static ActionManager& getInstance();
		static ActionManager* getInstancePtr();

		void initialise();
		void shutdown();

		void doAction(Action* _command);

		void undoAction();
		void redoAction();

		void saveChanges();
		bool getChanges();

		void setMaxActions(size_t _value);
		size_t getMaxActions() const;

		void reset();

		sigslot::signal0<> eventChanges;

	private:
		void clear();

		bool updateMaxActions();
		void removeRedo();

		void onChangeActions();

	private:
		static ActionManager* mInstance;
		typedef std::list<Action*> ListAction;
		ListAction mActions;
		ListAction::iterator mCurrentAction;
		ListAction::iterator mActionAsSave;
		size_t mMaxActions;
	};

}

#endif
