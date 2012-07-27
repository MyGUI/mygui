/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _76731bb0_9d85_4b74_a603_1b3f5f6a8067_
#define _76731bb0_9d85_4b74_a603_1b3f5f6a8067_

#include <MyGUI.h>
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL CommandManager :
		public MyGUI::Singleton<CommandManager>
	{
	public:
		CommandManager();
		virtual ~CommandManager();

		void initialise();
		void shutdown();

		typedef sigslot::signal2<const MyGUI::UString&, bool&> EventType;
		EventType* getEvent(const MyGUI::UString& _command);

		bool executeCommand(const MyGUI::UString& _command);

		void setCommandData(const MyGUI::UString& _data);
		const MyGUI::UString& getCommandData() const;

	private:
		MyGUI::UString mData;
		typedef std::map<MyGUI::UString, EventType*> MapEvent;
		MapEvent mEvents;
	};

}

#endif
