#include "Precompiled.h"
#include "Localise.h"
#include "GroupMessage.h"
#include "MessageBoxManager.h"

template <> tools::GroupMessage* MyGUI::Singleton<tools::GroupMessage>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::GroupMessage>::mClassTypeName = "GroupMessage";

namespace tools
{

	GroupMessage::GroupMessage()
	{
	}

	GroupMessage::~GroupMessage()
	{
	}

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
			for (VectorUString::const_iterator item = mErrorMessages.begin(); item != mErrorMessages.end(); ++item)
			{
				if ((*item) == _message)
					return;
			}
			mErrorMessages.push_back(_message);
		}
		else
		{
			for (VectorUString::const_iterator item = mWarningMessages.begin(); item != mWarningMessages.end(); ++item)
			{
				if ((*item) == _message)
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
			for (VectorUString::const_iterator iter = mWarningMessages.begin(); iter != mWarningMessages.end(); ++iter)
			{
				if (warningMess.empty())
					warningMess = warningMess + *iter;
				else
					warningMess = warningMess + "\n" + *iter;
			}
			if (!warningMess.empty())
			{
				/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
					replaceTags("Warning"),
					warningMess,
					MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok);
			}

			mWarningMessages.clear();
		}

		if (!mErrorMessages.empty())
		{
			MyGUI::UString errorMessages;
			for (VectorUString::const_iterator iter = mErrorMessages.begin(); iter != mErrorMessages.end(); ++iter)
			{
				if (errorMessages.empty())
					errorMessages = errorMessages + *iter;
				else
					errorMessages = errorMessages + "\n" + *iter;
			}
			if (!errorMessages.empty())
			{
				/*MyGUI::Message* message = */MessageBoxManager::getInstance().create(
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
