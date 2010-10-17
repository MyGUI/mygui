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
	MainMenuControl::MainMenuControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("MainMenuControl.layout", _parent),
		mBar(nullptr),
		mPopupMenuWidgets(nullptr)
	{
		assignWidget(mBar, "Menu");

		createMainMenu();

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
		EditorWidgets::getInstance().eventChangeWidgets += MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);
	}

	MainMenuControl::~MainMenuControl()
	{
		EditorWidgets::getInstance().eventChangeWidgets -= MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
	}

	void MainMenuControl::createMainMenu()
	{
		updateRecentFilesMenu();
		updateRecentProjectsMenu();

		// меню для виджетов
		mPopupMenuWidgets = mBar->findItemById("Widgets", true)->getItemChild();

		//FIXME
		mPopupMenuWidgets->setPopupAccept(true);
		mPopupMenuWidgets->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyWidgetsSelect);

		mBar->eventMenuCtrlAccept += newDelegate(this, &MainMenuControl::notifyPopupMenuAccept);
	}

	void MainMenuControl::notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
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

	void MainMenuControl::widgetsUpdate()
	{
		bool print_name = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowName");
		bool print_type = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowType");
		bool print_skin = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<bool>("ShowSkin");

		mPopupMenuWidgets->removeAllItems();

		EnumeratorWidgetContainer widget = EditorWidgets::getInstance().getWidgets();
		while (widget.next())
			createWidgetPopup(widget.current(), mPopupMenuWidgets, print_name, print_type, print_skin);
	}

	void MainMenuControl::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
	{
		bool submenu = !_container->childContainers.empty();

		_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
		_parentPopup->setItemDataAt(_parentPopup->getItemCount() - 1, _container->widget);

		if (submenu)
		{
			MyGUI::MenuCtrl* child = _parentPopup->createItemChildAt(_parentPopup->getItemCount() - 1);
			child->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenuControl::notifyWidgetsSelect);
			child->setPopupAccept(true);

			for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter )
			{
				createWidgetPopup(*iter, child, _print_name, _print_type, _print_skin);
			}
		}
	}

	void MainMenuControl::notifyWidgetsSelect(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::Widget* widget = *_item->getItemData<MyGUI::Widget*>();
		WidgetSelectorManager::getInstance().setSelectedWidget(widget);
	}

	std::string MainMenuControl::getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);

		addUserTag("WidgetName", _print_name ? widgetContainer->name : "");
		addUserTag("WidgetType", _print_type ? _widget->getTypeName() : "");
		addUserTag("WidgetSkin", _print_skin ? widgetContainer->skin : "");

		addUserTag("FormatWidgetName", (_print_name && !widgetContainer->name.empty()) ? "#{PatternWidgetName}" : "");
		addUserTag("FormatWidgetType", _print_type ? "#{PatternWidgetType}" : "");
		addUserTag("FormatWidgetSkin", _print_skin ? "#{PatternWidgetSkin}" : "");

		return replaceTags("MenuItemWidgetInfo");
	}

	void MainMenuControl::notifyChangeWidgets()
	{
		widgetsUpdate();
	}

	void MainMenuControl::notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
	{
		if (_sectionName == "Settings")
		{
			widgetsUpdate();
		}
		else if (_sectionName == "Main")
		{
			if (_propertyName == "RecentFiles")
			{
				// если удалить изменить меню когда оно активно то оно не открывается
				//updateRecentFilesMenu();
			}
		}
	}

	void MainMenuControl::updateRecentFilesMenu()
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

	void MainMenuControl::updateRecentProjectsMenu()
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

	void MainMenuControl::setVisible(bool _value)
	{
		mBar->setVisible(_value);
	}

} // namespace tools
