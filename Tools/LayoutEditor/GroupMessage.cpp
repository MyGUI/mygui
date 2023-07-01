#include "Precompiled.h"
#include "Localise.h"
#include "GroupMessage.h"
#include "MessageBoxManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(GroupMessage);

	void GroupMessage::initialise()
	{
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &GroupMessage::notifyFrameStarted);
	}

	void GroupMessage::shutdown()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &GroupMessage::notifyFrameStarted);
	}

	void GroupMessage::addMessage(const MyGUI::UString& _message, MyGUI::LogLevel _type)
	{
		if (_type == MyGUI::LogLevel::Error)
		{
			for (const auto& errorMessage : mErrorMessages)
			{
				if (errorMessage == _message)
					return;
			}
			mErrorMessages.push_back(_message);
		}
		else
		{
			for (const auto& warningMessage : mWarningMessages)
			{
				if (warningMessage == _message)
					return;
			}
			mWarningMessages.push_back(_message);
		}
	}

	void GroupMessage::showMessages()
	{
		if (!mWarningMessages.empty())
		{
			MyGUI::UString warningMess;
			for (const auto& warningMessage : mWarningMessages)
			{
				if (warningMess.empty())
					warningMess = warningMess + warningMessage;
				else
					warningMess = warningMess + "\n" + warningMessage;
			}
			if (!warningMess.empty())
			{
				/*MyGUI::Message* message = */ MessageBoxManager::getInstance().create(
					replaceTags("Warning"),
					warningMess,
					MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok);
			}

			mWarningMessages.clear();
		}

		if (!mErrorMessages.empty())
		{
			MyGUI::UString errorMessages;
			for (const auto& errorMessage : mErrorMessages)
			{
				if (errorMessages.empty())
					errorMessages = errorMessages + errorMessage;
				else
					errorMessages = errorMessages + "\n" + errorMessage;
			}
			if (!errorMessages.empty())
			{
				/*MyGUI::Message* message = */ MessageBoxManager::getInstance().create(
					replaceTags("Error"),
					errorMessages,
					MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok);
			}

			mErrorMessages.clear();
		}
	}

	void GroupMessage::notifyFrameStarted(float _time)
	{
		showMessages();
	}

}
