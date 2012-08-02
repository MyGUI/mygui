/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "SettingsWindow.h"
#include "CommandManager.h"

namespace tools
{

	SettingsWindow::SettingsWindow() :
		mListSettings(nullptr),
		mTabSettings(nullptr)
	{
	}

	SettingsWindow::~SettingsWindow()
	{
	}

	void SettingsWindow::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		setDialogRoot(mMainWidget);

		assignWidget(mListSettings, "ListSettings", false, false);
		assignWidget(mTabSettings, "TabSettings", false, false);

		CommandManager::getInstance().getEvent("Command_SettingsAccept")->connect(this, &SettingsWindow::commandSettingsAccept);
		CommandManager::getInstance().getEvent("Command_SettingsCancel")->connect(this, &SettingsWindow::commandSettingsCancel);

		mMainWidget->setVisible(false);

		if (mListSettings != nullptr && mTabSettings != nullptr)
			InitialiseListTab();
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

	void SettingsWindow::InitialiseListTab()
	{
		for (size_t index = 0; index < mTabSettings->getItemCount(); index ++)
			mListSettings->addItem(mTabSettings->getItemNameAt(index));

		if (mListSettings->getItemCount() != 0)
			mListSettings->setIndexSelected(0);

		mListSettings->eventListChangePosition += MyGUI::newDelegate(this, &SettingsWindow::notifyListChangePosition);
	}

	void SettingsWindow::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
		{
			_index = mListSettings->getItemCount() - 1;
			mListSettings->setIndexSelected(_index);
		}

		mTabSettings->setIndexSelected(_index);
	}

}
