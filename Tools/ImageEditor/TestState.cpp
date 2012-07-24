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

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TestState)

	TestState::TestState()
	{
		CommandManager::getInstance().getEvent("Command_Test")->connect(this, &TestState::commandTest);
		CommandManager::getInstance().getEvent("Command_Quit")->connect(this, &TestState::commandQuit);
	}

	TestState::~TestState()
	{
	}

	void TestState::initState()
	{
	}

	void TestState::cleanupState()
	{
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
	}

	void TestState::commandTest(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		Data* data = DataUtility::getSelectedDataByType("Skin");
		if (data == nullptr)
			return;

		StateManager::getInstance().stateEvent("EditorState", "Test");

		_result = true;
	}

	void TestState::commandQuit(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		if (!StateManager::getInstance().getStateActivate(this))
			return;

		StateManager::getInstance().stateEvent("TestState", "Exit");

		_result = true;
	}

	bool TestState::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		return true;
	}

}
