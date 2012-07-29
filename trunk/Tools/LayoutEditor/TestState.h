/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEST_STATE_H__
#define __TEST_STATE_H__

#include "StateController.h"
#include "sigslot.h"

namespace tools
{
	class TestState :
		public StateController,
		public sigslot::has_slots<>
	{
	public:
		TestState();
		virtual ~TestState();

		virtual void initState();
		virtual void cleanupState();

		virtual void pauseState();
		virtual void resumeState();

	private:
		void commandQuit(const MyGUI::UString& _commandName, bool& _result);
		void command_Test(const MyGUI::UString& _commandName, bool& _result);
		void deleteTestLayout();

	private:
		MyGUI::xml::Document* mTestLayout;
	};

} // namespace tools

#endif // __TEST_STATE_H__
