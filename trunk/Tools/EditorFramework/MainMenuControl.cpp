/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "MainMenuControl.h"
#include "CommandManager.h"
#include "RecentFilesManager.h"
#include "Localise.h"

namespace tools
{

	MainMenuControl::MainMenuControl() :
		mMainMenu(nullptr),
		mScaleMenu(nullptr)
	{
	}

	MainMenuControl::~MainMenuControl()
	{
		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);
	}

	void MainMenuControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mMainMenu, "MainMenu");

		mScaleMenu = mMainMenu->findItemById("Scale");

		CommandManager::getInstance().getEvent("Command_UpdateAppCaption")->connect(this, &MainMenuControl::command_UpdateAppCaption);

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);

		updateRecentFilesMenu();
	}

	void MainMenuControl::notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::UString* data = _item->getItemData<MyGUI::UString>(false);
		if (data != nullptr)
			CommandManager::getInstance().setCommandData(*data);

		const std::string& command = _item->getItemId();
		if (MyGUI::utility::startWith(command, "Command_"))
			CommandManager::getInstance().executeCommand(command);
	}

	void MainMenuControl::updateRecentFilesMenu()
	{
		MyGUI::MenuItem* recentFilesMenu = mMainMenu->findItemById("RecentFiles", true);
		if (recentFilesMenu != nullptr)
		{
			recentFilesMenu->getItemChild()->removeAllItems();
			// список последних открытых файлов
			const RecentFilesManager::VectorUString& recentFiles = RecentFilesManager::getInstance().getRecentFiles();
			if (!recentFiles.empty())
			{
				size_t index = 1;
				for (RecentFilesManager::VectorUString::const_iterator iter = recentFiles.begin(); iter != recentFiles.end(); ++iter, ++index)
				{
					addUserTag("IndexRecentFile", MyGUI::utility::toString(index));
					addUserTag("RecentFile", *iter);
					recentFilesMenu->getItemChild()->addItem(replaceTags("FormatRecentFile"), MyGUI::MenuItemType::Normal, "Command_RecentFiles", *iter);
				}
			}
		}
	}

	void MainMenuControl::command_UpdateAppCaption(const MyGUI::UString& _commandName, bool& _result)
	{
		if (mScaleMenu != nullptr)
		{
			MyGUI::MenuControl* menu = mScaleMenu->getItemChild();
			if (menu != nullptr)
			{
				int value = MyGUI::utility::parseValue<int>(replaceTags("CurrentScale"));
				std::string id = MyGUI::utility::toString("Command_ChangeScale", ".", value);
				for (size_t index = 0; index < menu->getItemCount(); index ++)
				{
					MyGUI::MenuItem* item = menu->getItemAt(index);
					item->setItemChecked(item->getItemId() == id);
				}
			}
		}
	}

}
