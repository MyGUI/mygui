#ifndef __GROUP_MESSAGE__
#define __GROUP_MESSAGE__

#include <MyGUI.h>

namespace tools
{
	class GroupMessage :
		public MyGUI::Singleton<GroupMessage>
	{
	public:
		void addMessage(const MyGUI::UString & _message, MyGUI::LogLevel _type);
		void showMessages();

	private:
		typedef std::vector<MyGUI::UString> VectorUString;
		VectorUString mErrorMessages;
		VectorUString mWarningMessages;
	};

} // namespace tools

#endif // __GROUP_MESSAGE__
