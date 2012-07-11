/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __MESSAGE_BOX_MANAGER_H__
#define __MESSAGE_BOX_MANAGER_H__

#include <MyGUI.h>
#include "MessageBox/MessageBox.h"

namespace tools
{

	class MessageBoxManager :
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

} // namespace tools

#endif // __MESSAGE_BOX_MANAGER_H__
