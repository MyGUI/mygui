#ifndef __GROUP_MESSAGE__
#define __GROUP_MESSAGE__

class GroupMessage : public MyGUI::Singleton<GroupMessage>
{
public:
	void addMessage(const MyGUI::UString & _message, MyGUI::LogLevel _type);
	void showMessages();
private:
	MyGUI::VectorString mErrorMessages;
	MyGUI::VectorString mWarningMessages;
};

#endif // __GROUP_MESSAGE__
