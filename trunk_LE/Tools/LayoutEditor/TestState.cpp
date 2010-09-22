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

namespace tools
{

	TestState::TestState()
	{
		CommandManager::getInstance().registerCommand("Command_Quit", MyGUI::newDelegate(this, &TestState::commandQuit));
	}

	TestState::~TestState()
	{
	}

	void TestState::initState()
	{
		MyGUI::xml::Document* mTestLayout = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(mTestLayout, true);

		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		delete mTestLayout;
		mTestLayout = nullptr;
	}

	void TestState::cleanupState()
	{
		MyGUI::xml::Document* mTestLayout = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(mTestLayout, false);

		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);

		delete mTestLayout;
		mTestLayout = nullptr;
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
	}

	void TestState::commandQuit(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		StateManager::getInstance().stateEvent(this, "Exit");

		_result = true;
	}

	bool TestState::checkCommand()
	{
		if (DialogManager::getInstance().getAnyDialog())
			return false;

		if (MessageBoxManager::getInstance().hasAny())
			return false;

		if (!StateManager::getInstance().getStateActivate(this))
			return false;

		return true;
	}

} // namespace tools
