#include "precompiled.h"
#include "Common.h"
#include "GroupMessage.h"


#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DEBUG_NEW
#	endif
#endif

MYGUI_INSTANCE_IMPLEMENT( GroupMessage )

void GroupMessage::addMessage(const MyGUI::UString & _message, MyGUI::LogManager::LogLevel _type)
{
	if (_type == MyGUI::LogManager::Error)
		mErrorMessages.push_back(_message);
	else /*if (_type == MyGUI::LogManager::Warning)*/
		mWarningMessages.push_back(_message);
}

void GroupMessage::showMessages()
{
	if (mWarningMessages.size())
	{
		MyGUI::UString warningMess;
		for (MyGUI::VectorString::iterator iter = mWarningMessages.begin(); iter != mWarningMessages.end(); ++iter)
		{
			if (warningMess.empty()) warningMess = warningMess + *iter;
			else warningMess = warningMess + "\n" + *iter;
		}
		if (!warningMess.empty())
			MyGUI::Message::createMessageBox("Message", localise("Warning"), warningMess, MyGUI::MessageBoxStyle::IconWarning | MyGUI::MessageBoxStyle::Ok);

		mWarningMessages.clear();
	}

	if (mErrorMessages.size())
	{
		MyGUI::UString errorMessages;
		for (MyGUI::VectorString::iterator iter = mErrorMessages.begin(); iter != mErrorMessages.end(); ++iter)
		{
			if (errorMessages.empty()) errorMessages = errorMessages + *iter;
			else errorMessages = errorMessages + "\n" + *iter;
		}
		if (!errorMessages.empty())
			MyGUI::Message::createMessageBox("Message", localise("Error"), errorMessages , MyGUI::MessageBoxStyle::IconError | MyGUI::MessageBoxStyle::Ok);

		mErrorMessages.clear();
	}
}