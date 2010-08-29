#ifndef __GROUP_MESSAGE__
#define __GROUP_MESSAGE__

namespace tools
{
	class GroupMessage :
		public MyGUI::Singleton<GroupMessage>
	{
	public:
		void addMessage(const MyGUI::UString & _message, MyGUI::LogLevel _type);
		void showMessages();
	private:
		MyGUI::VectorString mErrorMessages;
		MyGUI::VectorString mWarningMessages;
	};

} // namespace tools

#endif // __GROUP_MESSAGE__
