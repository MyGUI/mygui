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

	typedef MyGUI::delegates::CDelegate1<const MyGUI::UString &> CommandDelegate;

	class CommandManager :
		public MyGUI::Singleton<CommandManager>
	{
	public:
		CommandManager();
		virtual ~CommandManager();

		void initialise();
		void shutdown();

		void registerCommand(const MyGUI::UString & _command, CommandDelegate::IDelegate * _delegate);

		void executeCommand(const MyGUI::UString& _command);

	private:
		typedef std::map<MyGUI::UString, CommandDelegate> MapDelegate;
		MapDelegate mDelegates;
	};

} // namespace tools

#endif // __COMMAND_MANAGER_H__
