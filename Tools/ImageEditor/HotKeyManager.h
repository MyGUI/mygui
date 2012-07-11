/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __HOT_KEY_MANAGER_H__
#define __HOT_KEY_MANAGER_H__

#include <MyGUI.h>
#include "HotKeyCommand.h"

namespace tools
{

	class HotKeyManager :
		public MyGUI::Singleton<HotKeyManager>
	{
	public:
		HotKeyManager();
		virtual ~HotKeyManager();

		void initialise();
		void shutdown();

		bool onKeyEvent(bool _pressed, bool _shift, bool _control, MyGUI::KeyCode _key);

	private:
		void loadXml(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

		void addCommand(HotKeyCommand& _command);

	private:
		typedef std::map<std::string, MyGUI::KeyCode> MapKeys;
		MapKeys mKeyNames;

		typedef std::vector<HotKeyCommand> VectorCommand;
		typedef std::map<MyGUI::KeyCode, VectorCommand> MapCommand;
		MapCommand mCommands;
	};

} // namespace tools

#endif // __HOT_KEY_MANAGER_H__
