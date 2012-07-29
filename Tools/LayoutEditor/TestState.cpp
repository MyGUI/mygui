/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "TestState.h"
#include "CommandManager.h"
#include "StateManager.h"
#include "DialogManager.h"
#include "MessageBoxManager.h"
#include "EditorWidgets.h"
#include "WidgetSelectorManager.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TestState)

	TestState::TestState() :
		mTestLayout(nullptr),
		mBackgroundControl(nullptr)
	{
		CommandManager::getInstance().getEvent("Command_Quit")->connect(this, &TestState::commandQuit);
		CommandManager::getInstance().getEvent("Command_Test")->connect(this, &TestState::command_Test);
	}

	TestState::~TestState()
	{
		deleteTestLayout();
	}

	void TestState::initState()
	{
		WidgetSelectorManager::getInstance().saveSelectedWidget();
		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		mTestLayout = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(mTestLayout, true);

		mBackgroundControl = new BackgroundControl();
	}

	void TestState::cleanupState()
	{
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(mTestLayout, false);

		deleteTestLayout();

		WidgetSelectorManager::getInstance().restoreSelectedWidget();

		delete mBackgroundControl;
		mBackgroundControl = nullptr;
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
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

	void TestState::deleteTestLayout()
	{
		if (mTestLayout != nullptr)
		{
			delete mTestLayout;
			mTestLayout = nullptr;
		}
	}

	void TestState::command_Test(const MyGUI::UString& _commandName, bool& _result)
	{
		if (DialogManager::getInstance().getAnyDialog())
			return;

		if (MessageBoxManager::getInstance().hasAny())
			return;

		StateManager::getInstance().stateEvent("EditorState", "Test");

		_result = true;
	}

}
