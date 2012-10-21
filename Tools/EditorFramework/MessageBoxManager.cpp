/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "MessageBoxManager.h"

template <> tools::MessageBoxManager* MyGUI::Singleton<tools::MessageBoxManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::MessageBoxManager>::mClassTypeName = "MessageBoxManager";

namespace tools
{

	MessageBoxManager::MessageBoxManager()
	{
	}

	MessageBoxManager::~MessageBoxManager()
	{
	}

	void MessageBoxManager::initialise()
	{
	}

	void MessageBoxManager::shutdown()
	{
	}

	MyGUI::Message* MessageBoxManager::create(const MyGUI::UString& _caption, const MyGUI::UString& _message, MyGUI::MessageBoxStyle _style)
	{
		MyGUI::Message* message = new MyGUI::Message("MessageBox2.layout");

		message->setCaption(_caption);
		message->setMessageText(_message);
		message->setSmoothShow(true);
		message->setMessageStyle(_style);

		message->setMessageModal(true);

		registerMessageBox(message);

		return message;
	}

	bool MessageBoxManager::hasAny()
	{
		return !mMessages.empty();
	}

	void MessageBoxManager::endTop(MyGUI::MessageBoxStyle _button)
	{
		if (!mMessages.empty())
			mMessages.back()->endMessage(_button);
	}

	void MessageBoxManager::registerMessageBox(MyGUI::Message* _message)
	{
		mMessages.push_back(_message);
		_message->eventMessageBoxResult += MyGUI::newDelegate(this, &MessageBoxManager::notifMessageBoxResultRegister);
	}

	void MessageBoxManager::notifMessageBoxResultRegister(MyGUI::Message* _sender, MyGUI::MessageBoxStyle _result)
	{
		VectorMessage::iterator item = std::find(mMessages.begin(), mMessages.end(), _sender);
		if (item != mMessages.end())
			mMessages.erase(item);
	}

}
