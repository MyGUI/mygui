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
#include "FactoryManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(MainMenuControl);

	MainMenuControl::MainMenuControl() :
		mMainMenu(nullptr)
	{
	}

	MainMenuControl::~MainMenuControl()
	{
		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenuControl::notifyMenuCtrlAccept);
	}

	void MainMenuControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		assignWidget(mMainMenu, "MainMenu");

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

} // namespace tools
