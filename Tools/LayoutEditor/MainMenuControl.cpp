/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "MainMenuControl.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"
#include "WidgetContainer.h"
#include "EditorWidgets.h"
#include "Localise.h"

namespace tools
{
	MainMenuControl::MainMenuControl() :
		mBar(nullptr),
		mPopupMenuWidgets(nullptr)
	{
		createMainMenu();

		tools::SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
		EditorWidgets::getInstance().eventChangeWidgets += MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);

		setEdgeHideController();
	}

	MainMenuControl::~MainMenuControl()
	{
		EditorWidgets::getInstance().eventChangeWidgets -= MyGUI::newDelegate(this, &MainMenuControl::notifyChangeWidgets);
		tools::SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &MainMenuControl::notifySettingsChanged);
	}

	void MainMenuControl::createMainMenu()
	{
		MyGUI::VectorWidgetPtr menu_items = MyGUI::LayoutManager::getInstance().loadLayout("MainMenuControl.layout");
		MYGUI_ASSERT(menu_items.size() == 1, "Error load main menu");
		mBar = menu_items[0]->castType<MyGUI::MenuBar>();
		mBar->setCoord(0, 0, mBar->getParentSize().width, mBar->getHeight());

		MyGUI::MenuItem* recentFilesMenu = mBar->findItemById("RecentFiles", true);
		if (recentFilesMenu != nullptr)
		{
			// список последних открытых файлов
			const tools::VectorUString& recentFiles = tools::SettingsManager::getInstance().getRecentFiles();
			if (!recentFiles.empty())
			{
				size_t index = 1;
				for (tools::VectorUString::const_reverse_iterator iter = recentFiles.rbegin(); iter != recentFiles.rend(); ++iter, ++index)
				{
					addUserTag("LE_IndexRecentFile", MyGUI::utility::toString(index));
					addUserTag("LE_RecentFile", *iter);
					recentFilesMenu->getItemChild()->addItem(replaceTags("LE_FormatRecentFile"), MyGUI::MenuItemType::Normal, "Command_RecentFiles", *iter);
				}
			}
		}

		// меню для виджетов
		mPopupMenuWidgets = mBar->findItemById("Widgets", true)->getItemChild();

		mBar->eventMenuCtrlAccept += newDelegate(this, &MainMenuControl::notifyPopupMenuAccept);
	}

	void MainMenuControl::notifyPopupMenuAccept(MyGUI::MenuCtrl* _sender, MyGUI::MenuItem* _item)
	{
		MyGUI::UString* data = _item->getItemData<MyGUI::UString>(false);
		if (data != nullptr)
			tools::CommandManager::getInstance().setCommandData(*data);

		const std::string& command = _item->getItemId();
		if (command.size() > 8 && command.substr(0, 8) == "Command_")
		{
			tools::CommandManager::getInstance().executeCommand(command);
		}
	}

	void MainMenuControl::widgetsUpdate()
	{
		bool print_name = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowName");
		bool print_type = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowType");
		bool print_skin = tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "ShowSkin");

		mPopupMenuWidgets->removeAllItems();

		for (std::vector<WidgetContainer*>::iterator iter = EditorWidgets::getInstance().widgets.begin(); iter != EditorWidgets::getInstance().widgets.end(); ++iter )
		{
			createWidgetPopup(*iter, mPopupMenuWidgets, print_name, print_type, print_skin);
		}
	}

	void MainMenuControl::createWidgetPopup(WidgetContainer* _container, MyGUI::MenuCtrl* _parentPopup, bool _print_name, bool _print_type, bool _print_skin)
	{
		bool submenu = !_container->childContainers.empty();

		_parentPopup->addItem(getDescriptionString(_container->widget, _print_name, _print_type, _print_skin), submenu ? MyGUI::MenuItemType::Popup : MyGUI::MenuItemType::Normal);
		_parentPopup->setItemDataAt(_parentPopup->getItemCount()-1, _container->widget);

		if (submenu)
		{
			MyGUI::MenuCtrl* child = _parentPopup->createItemChildAt(_parentPopup->getItemCount()-1);
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
		tools::WidgetSelectorManager::getInstance().setSelectedWidget(widget);
	}

	std::string MainMenuControl::getDescriptionString(MyGUI::Widget* _widget, bool _print_name, bool _print_type, bool _print_skin)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_widget);

		addUserTag("LE_WidgetName", _print_name ? widgetContainer->name : "");
		addUserTag("LE_WidgetType", _print_type ? _widget->getTypeName() : "");
		addUserTag("LE_WidgetSkin", _print_skin ? widgetContainer->skin : "");

		addUserTag("LE_FormatWidgetName", (_print_name && !widgetContainer->name.empty()) ? "#{LE_PatternWidgetName}" : "");
		addUserTag("LE_FormatWidgetType", _print_type ? "#{LE_PatternWidgetType}" : "");
		addUserTag("LE_FormatWidgetSkin", _print_skin ? "#{LE_PatternWidgetSkin}" : "");

		return replaceTags("LE_MenuItemWidgetInfo");
	}

	void MainMenuControl::notifyChangeWidgets()
	{
		widgetsUpdate();
	}

	void MainMenuControl::notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
	{
		if (_sectionName == "SettingsWindow")
		{
			widgetsUpdate();
		}
	}

	void MainMenuControl::setEdgeHideController()
	{
		if (tools::SettingsManager::getInstance().getPropertyValue<bool>("SettingsWindow", "EdgeHide"))
		{
			MyGUI::ControllerItem* item = MyGUI::ControllerManager::getInstance().createItem(MyGUI::ControllerEdgeHide::getClassTypeName());
			MyGUI::ControllerEdgeHide* controller = item->castType<MyGUI::ControllerEdgeHide>();

			controller->setTime(tools::SettingsManager::getInstance().getPropertyValue<float>("Settings", "EdgeHideTime"));
			controller->setRemainPixels(tools::SettingsManager::getInstance().getPropertyValue<int>("Settings", "EdgeHideRemainPixels"));
			controller->setShadowSize(tools::SettingsManager::getInstance().getPropertyValue<int>("Settings", "EdgeHideShadowSize"));

			MyGUI::ControllerManager::getInstance().addItem(mBar, controller);
		}
	}

} // namespace tools
