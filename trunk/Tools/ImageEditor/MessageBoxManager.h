/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _5faf9ff7_560b_4d0a_b812_8923f8da7805_
#define _5faf9ff7_560b_4d0a_b812_8923f8da7805_

#include <MyGUI.h>
#include "MessageBox/MessageBox.h"

namespace tools
{

	class MYGUI_EXPORT_DLL MessageBoxManager :
		public MyGUI::Singleton<MessageBoxManager>
	{
	public:
		MessageBoxManager();
		virtual ~MessageBoxManager();

		void initialise();
		void shutdown();

		MyGUI::Message* create(const MyGUI::UString& _caption, const MyGUI::UString& _message, MyGUI::MessageBoxStyle _style);
		bool hasAny();
		void endTop(MyGUI::MessageBoxStyle _button);

	private:
		void registerMessageBox(MyGUI::Message* _message);
		void notifMessageBoxResultRegister(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result);

	private:
		typedef std::vector<MyGUI::Message*> VectorMessage;
		VectorMessage mMessages;
	};

}

#endif
