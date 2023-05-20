/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef COMMAND_MANAGER_H_
#define COMMAND_MANAGER_H_

#include <MyGUI.h>

namespace demo
{

	class CommandManager
	{
	public:
		typedef MyGUI::delegates::CDelegate2<std::string_view, MyGUI::Any> CommandDelegate;

	public:
		CommandManager();
		virtual ~CommandManager();

		static CommandManager& getInstance()
		{
			return *msCommandManager;
		}

		void execiteCommand(std::string_view _name, MyGUI::Any _data);

		void registerCommand(std::string_view _name, CommandDelegate::IDelegate* _delegate);
		void unregisterCommand(std::string_view _name);

	private:
		static CommandManager* msCommandManager;
		typedef std::map<std::string, CommandDelegate, std::less<>> MapDelegate;
		MapDelegate mDelegates;
	};

} // namespace demo

#endif // COMMAND_MANAGER_H_
