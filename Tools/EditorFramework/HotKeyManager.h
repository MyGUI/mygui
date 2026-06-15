/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _a64ca567_5ede_4f68_bc3d_2cc6a37a3863_
#define _a64ca567_5ede_4f68_bc3d_2cc6a37a3863_

#include <MyGUI.h>
#include "HotKeyCommand.h"

namespace tools
{

	class MYGUI_EXPORT_DLL HotKeyManager
	{
		MYGUI_SINGLETON_DECLARATION(HotKeyManager);

	public:
		HotKeyManager();

		void initialise();
		void shutdown();

		bool onKeyEvent(bool _pressed, bool _shift, bool _control, MyGUI::KeyCode _key);

	private:
		void loadXml(MyGUI::xml::ElementPtr _node, std::string_view _file, MyGUI::Version _version);

		void addCommand(HotKeyCommand& _command);

	private:
		using MapKeys = std::map<std::string, MyGUI::KeyCode>;
		MapKeys mKeyNames;

		using VectorCommand = std::vector<HotKeyCommand>;
		using MapCommand = std::map<MyGUI::KeyCode, VectorCommand>;
		MapCommand mCommands;
	};

}

#endif
