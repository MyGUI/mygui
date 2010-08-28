/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEST_STATE_H__
#define __TEST_STATE_H__

#include "StateController.h"
#include "TestWindow.h"

namespace tools
{
	class TestState :
		public StateController
	{
	public:
		TestState();
		virtual ~TestState();

		virtual void initState();
		virtual void cleanupState();

		virtual void pauseState();
		virtual void resumeState();

	private:
		void notifyEndDialogTest(Dialog* _sender, bool _result);

	private:
		TestWindow* mTestWindow;
	};

} // namespace tools

#endif // __TEST_STATE_H__
