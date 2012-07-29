/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _e7d61788_3d54_4fbc_90c9_77878d1e73c1_
#define _e7d61788_3d54_4fbc_90c9_77878d1e73c1_

#include "StateController.h"
#include "BackgroundControl.h"
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
		BackgroundControl* mBackgroundControl;
	};

}

#endif
