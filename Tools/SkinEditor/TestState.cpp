/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TestState.h"
#include "CommandManager.h"
#include "StateManager.h"
#include "SkinManager.h"

namespace tools
{

	TestState::TestState() :
		mTestWindow(nullptr)
	{
	}

	TestState::~TestState()
	{
	}

	void TestState::initState()
	{
		mTestWindow = new TestWindow();
		mTestWindow->eventEndDialog = MyGUI::newDelegate(this, &TestState::notifyEndDialogTest);

		SkinItem* item = SkinManager::getInstance().getItemSelected();
		if (item != nullptr)
		{
			mTestWindow->setSkinItem(item);
			mTestWindow->doModal();
		}
		else
		{
			StateManager::getInstance().stateEvent(this, "Exit");
		}
	}

	void TestState::cleanupState()
	{
		delete mTestWindow;
		mTestWindow = nullptr;
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
	}

	void TestState::notifyEndDialogTest(Dialog* _sender, bool _result)
	{
		_sender->endModal();

		StateManager::getInstance().stateEvent(this, "Exit");
	}

} // namespace tools
