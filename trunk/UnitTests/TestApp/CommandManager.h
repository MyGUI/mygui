/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _897c30fe_12de_4067_91d0_a2a336a18f83_
#define _897c30fe_12de_4067_91d0_a2a336a18f83_

#include <list>
#include "Command.h"
#include "sigslot.h"

namespace tools
{
	class CommandManager
	{
	public:
		CommandManager();
		~CommandManager();

		static CommandManager& getInstance();
		static CommandManager* getInstancePtr();

		void initialise();
		void shutdown();

		void doCommand(Command* _command);

		void undoCommand();
		void redoCommand();

		void setCurrentCommandAsSave();
		bool getCurrentCommandAsSave();

		void setMaxCommands(size_t _value);
		size_t getMaxCommands() const;

		void reset();

		sigslot::signal0<> eventChangeCommands;

	private:
		void clear();

		bool updateMaxCommands();
		void removeRedo();

		void onChangeCommands();

	private:
		static CommandManager* mInstance;
		typedef std::list<Command*> ListCommand;
		ListCommand mCommands;
		ListCommand::iterator mCurrentCommand;
		ListCommand::iterator mCommandAsSave;
		size_t mMaxCommands;
	};
}

#endif
