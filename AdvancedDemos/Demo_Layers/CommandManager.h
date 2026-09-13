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
		using CommandDelegate = MyGUI::delegates::Delegate<std::string_view, MyGUI::Any>;

	public:
		CommandManager();
		virtual ~CommandManager();

		static CommandManager& getInstance()
		{
			return *msCommandManager;
		}

		void executeCommand(std::string_view _name, MyGUI::Any _data);

		void registerCommand(std::string_view _name, CommandDelegate::IDelegate* _delegate);
		void unregisterCommand(std::string_view _name);

	private:
		static CommandManager* msCommandManager;
		using MapDelegate = std::map<std::string, CommandDelegate, std::less<>>;
		MapDelegate mDelegates;
	};

} // namespace demo

#endif // COMMAND_MANAGER_H_
