/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
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
	}

	void TestState::cleanupState()
	{
		MyGUI::xml::Document* mTestLayout = EditorWidgets::getInstance().savexmlDocument();
		EditorWidgets::getInstance().clear();
		EditorWidgets::getInstance().loadxmlDocument(mTestLayout, false);

		WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
	}

	void TestState::pauseState()
	{
	}

	void TestState::resumeState()
	{
	}

	void TestState::commandQuit(const MyGUI::UString& _commandName)
	{
		if (!checkCommand())
			return;

		StateManager::getInstance().stateEvent(this, "Exit");
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
