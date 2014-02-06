/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "MainMenuControl.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"
#include "WidgetContainer.h"
#include "EditorWidgets.h"
#include "Localise.h"
#include "RecentFilesManager.h"

namespace tools
{

	MainMenuControlLE::MainMenuControlLE(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mBar(nullptr),
		mPopupMenuWidgets(nullptr)
	{
		assignWidget(mBar, "Menu");

		createMainMenu();

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &MainMenuControlLE::notifySettingsChanged);
		EditorWidgets::getInstance().eventChangeWidgets += MyGUI::newDelegate(this, &MainMenuControlLE::notifyChangeWidgets);

		CommandManager::getInstance().getEvent("Command_OnChangeScale")->connect(this, &MainMenuControlLE::CommandOnChangeScale);

		updateMenuScale(100);
	}

	MainMenuControlLE::~MainMenuControlLE()
	{
		EditorWidgets::getInstance().eventChangeWidgets -= MyGUI::newDelegate(this, &MainMenuControlLE::notifyChangeWidgets);
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
	}

	void MainMenuControlLE::createMainMenu()
	{
		updateRecentFilesMenu();
		updateRecentProjectsMenu();

		// меню для виджетов
		mPopupMenuWidgets = mBar->findItemById("Widgets", true)->getItemChild();

		//FIXME
		mPopupMenuWidgets->setPopupAccept(true);
		mPopupMenuWidgets->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControlLE::notifyWidgetsSelect);

		mBar->eventMenuCtrlAccept += newDelegate(this, &MainMenuControlLE::notifyPopupMenuAccept);
	}

	void MainMenuControlLE::notifyPopupMenuAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
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

	void MainMenuControlLE::widgetsUpdate()
	{
		bool print_name = SettingsManager::getInstance().getValue<bool>("Settings/ShowName");
		bool print_type = SettingsManager::getInstance().getValue<bool>("Settings/ShowType");
		bool print_skin = SettingsManager::getInstance().getValue<bool>("Settings/ShowSkin");

		mPopupMenuWidgets->removeAllItems();

		EnumeratorWidgetContainer widget = EditorWidgets::getInstance().getWidgets();
		while (widget.next())
			createWidgetPopup(widget.current(), mPopupMenuWidgets, print_name, print_type, print_skin);
	}

	void MainMenuControlLE::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuControl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
	{
		bool submenu = !_container->childContainers.empty();

		_parentPopup->addItem(getDescriptionString(_container->getWidget(), _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
		_parentPopup->setItemDataAt(_parentPopup->getItemCount() - 1, _container->getWidget());

		if (submenu)
		{
			MyGUI::MenuControl* child = _parentPopup->createItemChildAt(_parentPopup->getItemCount() - 1);
			child->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControlLE::notifyWidgetsSelect);
			child->setPopupAccept(true);

			for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
			{
				createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
			}
		}
	}

	void MainMenuControlLE::notifyWidgetsSelect(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::Widget* widget = *_item->getItemData<MyGUI::Widget*>();
		WidgetSelectorManager::getInstance().setSelectedWidget(widget);
	}

	std::string MainMenuControlLE::getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);

		addUserTag("WidgetName", _print_name ? widgetContainer->getName() : "");
		addUserTag("WidgetType", _print_type ? _widget->getTypeName() : "");
		addUserTag("WidgetSkin", _print_skin ? widgetContainer->getSkin() : "");

		addUserTag("FormatWidgetName", (_print_name && !widgetContainer->getName().empty()) ? "#{PatternWidgetName}" : "");
		addUserTag("FormatWidgetType", _print_type ? "#{PatternWidgetType}" : "");
		addUserTag("FormatWidgetSkin", _print_skin ? "#{PatternWidgetSkin}" : "");

		return replaceTags("MenuItemWidgetInfo");
	}

	void MainMenuControlLE::notifyChangeWidgets()
	{
		widgetsUpdate();
	}

	void MainMenuControlLE::notifySettingsChanged(const std::string& _path)
	{
		if (_path == "Settings/ShowName" ||
			_path == "Settings/ShowType" ||
			_path == "Settings/ShowSkin")
			widgetsUpdate();
		else if (_path == "Files/RecentFile.List")
			updateRecentFilesMenu();
		else if (_path == "Files/RecentProject.List")
			updateRecentProjectsMenu();
	}

	void MainMenuControlLE::updateRecentFilesMenu()
	{
		MyGUI::MenuItem* recentFilesMenu = mBar->findItemById("RecentFiles", true);
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
					recentFilesMenu->getItemChild()->addItem(replaceTags("FormatRecentFile"), MyGUI::MenuItemType::Normal, "Command_OpenRecentFile", *iter);
				}
			}
		}
	}

	void MainMenuControlLE::updateRecentProjectsMenu()
	{
		MyGUI::MenuItem* recentProjectsMenu = mBar->findItemById("RecentProjects", true);
		if (recentProjectsMenu != nullptr)
		{
			recentProjectsMenu->getItemChild()->removeAllItems();
			// список последних открытых проектов
			const RecentFilesManager::VectorUString& recentProjects = RecentFilesManager::getInstance().getRecentProjects();
			if (!recentProjects.empty())
			{
				size_t index = 1;
				for (RecentFilesManager::VectorUString::const_iterator iter = recentProjects.begin(); iter != recentProjects.end(); ++iter, ++index)
				{
					addUserTag("IndexRecentProject", MyGUI::utility::toString(index));
					addUserTag("RecentProject", *iter);
					recentProjectsMenu->getItemChild()->addItem(replaceTags("FormatRecentProject"), MyGUI::MenuItemType::Normal, "Command_OpenRecentProject", *iter);
				}
			}
		}
	}

	void MainMenuControlLE::setVisible(bool _value)
	{
		mBar->setVisible(_value);
	}

	void MainMenuControlLE::CommandOnChangeScale(const MyGUI::UString& _commandName, bool& _result)
	{
		updateMenuScale(MyGUI::utility::parseValue<size_t>(CommandManager::getInstance().getCommandData()));

		_result = true;
	}

	void MainMenuControlLE::updateMenuScale(size_t _scale)
	{
		MyGUI::MenuItem* scaleMenu = mBar->findItemById("Scale", true);
		if (scaleMenu != nullptr)
		{
			MyGUI::MenuControl* popup = scaleMenu->getItemChild();
			if (popup != nullptr)
			{
				std::string id = MyGUI::utility::toString("Command_ChangeScale.", (int)_scale);
				for (size_t index = 0; index < popup->getItemCount(); index ++)
				{
					MyGUI::MenuItem* item = popup->getItemAt(index);
					item->setItemChecked(item->getItemId() == id);
				}
			}
		}
	}

}
