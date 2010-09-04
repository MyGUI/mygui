#ifndef __GROUP_MESSAGE__
#define __GROUP_MESSAGE__

#include <MyGUI.h>

namespace tools
{
	class GroupMessage :
		public MyGUI::Singleton<GroupMessage>
	{
	public:
		GroupMessage();
		virtual ~GroupMessage();

		void initialise();
		void shutdown();

		void addMessage(const MyGUI::UString & _message, MyGUI::LogLevel _type);

	private:
		void showMessages();
		void notifyFrameStarted(float _time);

	private:
		typedef std::vector<MyGUI::UString> VectorUString;
		VectorUString mErrorMessages;
		VectorUString mWarningMessages;
	};

} // namespace tools

#endif // __GROUP_MESSAGE__
