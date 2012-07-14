/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "SettingsWindow.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(SettingsWindow);

	SettingsWindow::SettingsWindow()
	{
	}

	SettingsWindow::~SettingsWindow()
	{
	}

	void SettingsWindow::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		setDialogRoot(mMainWidget);

		CommandManager::getInstance().registerCommand("Command_SettingsAccept", MyGUI::newDelegate(this, &SettingsWindow::commandSettingsAccept));
		CommandManager::getInstance().registerCommand("Command_SettingsCancel", MyGUI::newDelegate(this, &SettingsWindow::commandSettingsCancel));

		mMainWidget->setVisible(false);
	}

	bool SettingsWindow::checkCommand()
	{
		return isDialogModal();
	}

	void SettingsWindow::commandSettingsAccept(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		eventEndDialog(this, true);

		_result = true;
	}

	void SettingsWindow::commandSettingsCancel(const MyGUI::UString& _commandName, bool& _result)
	{
		if (!checkCommand())
			return;

		eventEndDialog(this, false);

		_result = true;
	}
}
