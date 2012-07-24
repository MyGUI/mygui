/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _41f706df_2a78_46f6_a92a_c9e12a910964_
#define _41f706df_2a78_46f6_a92a_c9e12a910964_

#include "StateController.h"
#include "sigslot.h"
#include "TestWindow.h"

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
		void commandTest(const MyGUI::UString& _commandName, bool& _result);
		void commandQuit(const MyGUI::UString& _commandName, bool& _result);
		void notifyEndDialogTest(Dialog* _sender, bool _result);

	private:
		TestWindow* mTestWindow;
		Control* mBack;
	};

}

#endif
