#ifndef _0b449527_0787_4563_b64a_2cb4729eb9ab_
#define _0b449527_0787_4563_b64a_2cb4729eb9ab_

#include <MyGUI.h>

namespace tools
{

	class GroupMessage
	{
		MYGUI_SINGLETON_DECLARATION(GroupMessage);

	public:
		GroupMessage() :
			mSingletonHolder(this)
		{
		}
		void initialise();
		void shutdown();

		void addMessage(const MyGUI::UString& _message, MyGUI::LogLevel _type);

	private:
		void showMessages();
		void notifyFrameStarted(float _time);

	private:
		using VectorUString = std::vector<MyGUI::UString>;
		VectorUString mErrorMessages;
		VectorUString mWarningMessages;
	};

}

#endif
