#ifndef __GROUP_MESSAGE__
#define __GROUP_MESSAGE__

class GroupMessage
{
public:
	MYGUI_INSTANCE_HEADER(GroupMessage)

	void addMessage(const MyGUI::UString & _message, MyGUI::LogManager::LogLevel _type);
	void showMessages();
private:
	MyGUI::VectorString mErrorMessages;
	MyGUI::VectorString mWarningMessages;
};

#endif // __GROUP_MESSAGE__
