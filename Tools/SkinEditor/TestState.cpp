/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "TestState.h"
#include "FactoryManager.h"
#include "CommandManager.h"
#include "StateManager.h"
#include "DialogManager.h"
#include "MessageBoxManager.h"
#include "DataManager.h"
#include "DataUtility.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TestState)

	TestState::TestState() :
		mTestWindow(nullptr),
		mBack(nullptr)
	{
		CommandManager::getInstance().getEvent("Command_Test")->connect(this, &TestState::commandTest);
		CommandManager::getInstance().getEvent("Command_Quit")->connect(this, &TestState::commandQuit);
	}

	TestState::~TestState()
	{
		if (mTestWindow != nullptr)
		{
			delete mTestWindow;
			mTestWindow = nullptr;
		}

		if (mBack != nullptr)
		{
			delete mBack;
			mBack = nullptr;
		}
	}

	void TestState::initState()
	{
		if (mBack == nullptr)
		{
			mBack = new Control();
			mBack->Initialise(SettingsManager::getInstance().getValue("TestState/BackgroundLayout"));
		}

		mBack->getRoot()->setVisible(true);

		if (mTestWindow == nullptr)
		{
			mTestWindow = new TestWindow();
			mTestWindow->Initialise();
			mTestWindow->eventEndDialog.connect(this, &TestState::notifyEndDialogTest);
		}

		DataPtr data = DataUtility::getSelectedDataByType("Skin");
		if (data != nullptr)
		{
			mTestWindow->setSkinItem(data);
			mTestWindow->getRoot()->setVisible(true);
			mTestWindow->doModal();
		}
		else
		{
			StateManager::getInstance().stateEvent(this, "Exit");
		}
	}

	void TestState::cleanupState()
	{
		mTestWindow->getRoot()->setVisible(false);
		mBack->getRoot()->setVisible(false);
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
	}

	void TestState::commandTest(const MyGUI::UString& _commandName, bool& _result)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (MessageBoxManager::getInstance().hasAny())
			return;

		DataPtr data = DataUtility::getSelectedDataByType("Skin");
		if (data == nullptr)
			return;

		StateManager::getInstance().stateEvent("EditorState", "Test");

		_result = true;
	}

	void TestState::commandQuit(const MyGUI::UString& _commandName, bool& _result)
	{
		if (MessageBoxManager::getInstance().hasAny())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		StateManager::getInstance().stateEvent("TestState", "Exit");

		_result = true;
	}

	void TestState::notifyEndDialogTest(Dialog* _sender, bool _result)
	{
		_sender->endModal();

		StateManager::getInstance().stateEvent("TestState", "Exit");
	}

}
