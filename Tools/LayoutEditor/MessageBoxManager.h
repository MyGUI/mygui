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

		Message* create(const MyGUI::UString& _caption, const MyGUI::UString& _message, MessageBoxStyle _style);
		bool hasAny();
		void endTop(MessageBoxStyle _button);

	private:
		void registerMessageBox(Message* _message);
		void notifMessageBoxResultRegister(Message* _sender, MessageBoxStyle _result);

	private:
		typedef std::vector<Message*> VectorMessage;
		VectorMessage mMessages;
	};

} // namespace tools

#endif // __MESSAGE_BOX_MANAGER_H__
