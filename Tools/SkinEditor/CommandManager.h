/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	class CommandManager :
		public MyGUI::Singleton<CommandManager>
	{
	public:
		CommandManager();
		virtual ~CommandManager();

		void initialise();
		void shutdown();

		void ExecuteCommand(const MyGUI::UString& _command);
	};

} // namespace tools

#endif // __COMMAND_MANAGER_H__
