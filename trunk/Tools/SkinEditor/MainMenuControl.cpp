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

	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mMainMenu(nullptr)
	{
		assignWidget(mMainMenu, "MainMenu");

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);

		updateRecentFilesMenu();
	}

	MainMenuControl::~MainMenuControl()
	{
		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);
	}

	void MainMenuControl::notifyMenuCtrlAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::UString* data = _item->getItemData<MyGUI::UString>(false);
		if (data != nullptr)
			CommandManager::getInstance().setCommandData(*data);

		const std::string& command = _item->getItemId();
		if (command.size() > 8 && command.substr(0, 8) == "Command_")
		{
			CommandManager::getInstance().executeCommand(command);
		}
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

} // namespace tools
