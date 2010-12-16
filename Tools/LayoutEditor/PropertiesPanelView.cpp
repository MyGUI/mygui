/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PropertiesPanelView.h"
#include "EditorWidgets.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"

namespace tools
{

	PropertiesPanelView::PropertiesPanelView(MyGUI::Widget* _parent) :
		BaseLayout("PropertiesPanelView.layout", _parent),
		mPanelView(nullptr),
		mPanelMainProperties(nullptr),
		mPanelItems(nullptr),
		mPanelUserData(nullptr),
		mPanelControllers(nullptr),
		mCurrentWidget(nullptr),
		mPropertyItemHeight(0),
		mToolTip(nullptr)
	{
		assignBase(mPanelView, "scroll_View");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
		{
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &PropertiesPanelView::notifyWindowChangeCoord);
			mOldSize = window->getSize();
		}

		mToolTip = new EditorToolTip();

		mPanelMainProperties = new PanelMainProperties();
		mPanelView->addItem(mPanelMainProperties);
		mPanelMainProperties->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);

		mPanelItems = new PanelItems();
		mPanelView->addItem(mPanelItems);

		mPanelUserData = new PanelUserData();
		mPanelView->addItem(mPanelUserData);

		mPanelControllers = new PanelControllers();
		mPanelView->addItem(mPanelControllers);
		mPanelControllers->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
		mPanelControllers->eventHidePairs = MyGUI::newDelegate(this, &PropertiesPanelView::hideWidgetsPairs);

		mPanels.push_back(mPanelMainProperties);
		mPanels.push_back(mPanelItems);
		mPanels.push_back(mPanelUserData);
		mPanels.push_back(mPanelControllers);

		mPropertyItemHeight = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight");

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);

		notifyChangeSelectedWidget(nullptr);
	}

	PropertiesPanelView::~PropertiesPanelView()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);

		delete mToolTip;
		mToolTip = nullptr;

		mPanelView->removeAllItems();
		delete mPanelMainProperties;
		delete mPanelItems;
		delete mPanelUserData;
		delete mPanelControllers;

		for (MapPropertyWindow::iterator item = mMapPropertyWindow.begin(); item != mMapPropertyWindow.end(); ++ item)
			delete (*item).second;
		mMapPropertyWindow.clear();
	}

	void PropertiesPanelView::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		const MyGUI::IntSize& size = _sender->getSize();
		if (size != mOldSize)
		{
			mOldSize = size;
			mPanelView->setNeedUpdate();
		}
	}

	void PropertiesPanelView::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		if (nullptr != mCurrentWidget)
		{
			MyGUI::LayerManager::getInstance().upLayerItem(mCurrentWidget);
			MyGUI::IntCoord coord = mCurrentWidget->getCoord();
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (nullptr != parent)
			{
				// если выбрали виджет на табе, то поднять лист таба
				if (parent->isType<MyGUI::TabItem>())
				{
					MyGUI::TabControl* tab = parent->getParent()->castType<MyGUI::TabControl>();
					MyGUI::TabItem* sheet = parent->castType<MyGUI::TabItem>();
					tab->setItemSelected(sheet);
				}
				// если выбрали лист таба, то поднять лист таба
				if (mCurrentWidget->isType<MyGUI::TabItem>())
				{
					MyGUI::TabControl* tab = parent->castType<MyGUI::TabControl>();
					MyGUI::TabItem* sheet = mCurrentWidget->castType<MyGUI::TabItem>();
					tab->setItemSelected(sheet);
				}
				coord = mCurrentWidget->getAbsoluteCoord();
			}

			EditorWidgets::getInstance().onSetWidgetCoord(mCurrentWidget, mCurrentWidget->getAbsoluteCoord(), "PropertiesPanelView");
		}

		// delete(hide) all previous properties
		for (MapInfo::iterator item = mPropertyInfo.begin(); item != mPropertyInfo.end(); ++item)
		{
			hideWidgetsPairs(item->first);
		}

		if (nullptr == mCurrentWidget)
		{
			mPanelMainProperties->setVisible(false);
			mPanelItems->setVisible(false);
			mPanelUserData->setVisible(false);
			mPanelControllers->setVisible(false);
			for (MapPropertyWindow::iterator item = mMapPropertyWindow.begin(); item != mMapPropertyWindow.end(); ++ item)
				(*item).second->setVisible(false);
		}
		else
		{
			mPanelMainProperties->setVisible(true);
			mPanelItems->setVisible(true);
			mPanelUserData->setVisible(true);
			mPanelControllers->setVisible(true);

			mPanelMainProperties->update(mCurrentWidget);

			std::string widgetTypeName = mCurrentWidget->getTypeName();

			WidgetContainer* container = EditorWidgets::getInstance().find(mCurrentWidget);
			for (MyGUI::VectorStringPairs::iterator item = container->mUserString.begin(); item != container->mUserString.end(); ++item)
			{
				if ((*item).first == "TargetWidgetType")
				{
					widgetTypeName = (*item).second;
					break;
				}
			}

			for (MapPropertyWindow::iterator item = mMapPropertyWindow.begin(); item != mMapPropertyWindow.end(); ++ item)
			{
				(*item).second->setVisible(false);
				(*item).second->update(nullptr, nullptr);
			}

			WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetTypeName);

			while (widgetType != nullptr && !widgetType->base.empty())
			{
				PanelProperties* panel = getPropertyWindow(widgetType);
				panel->setVisible(true);
				panel->update(mCurrentWidget, widgetType);

				widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetType->base);
			}

			mPanelItems->update(mCurrentWidget);
			mPanelUserData->update(mCurrentWidget);
			mPanelControllers->update(mCurrentWidget);
		}
	}

	void PropertiesPanelView::hideWidgetsPairs(MyGUI::Widget* _window)
	{
		for (VectorPropertyField::iterator iter = mPropertyInfo[_window].begin(); iter != mPropertyInfo[_window].end(); ++iter)
			(*iter).destroy();
		mPropertyInfo[_window].clear();
	}

	void PropertiesPanelView::createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y, PropertyField& _field)
	{
		PropertyField entry;
		entry.createPropertiesWidgetsPair(_window, _property, _value, _type, y, mPropertyItemHeight, mCurrentWidget, mToolTip);
		mPropertyInfo[_window].push_back(entry);
		_field = entry;
	}

	PanelProperties* PropertiesPanelView::getPropertyWindow(WidgetStyle* _style)
	{
		MapPropertyWindow::iterator item = mMapPropertyWindow.find(_style);
		if (item == mMapPropertyWindow.end())
		{
			PanelProperties* result = new PanelProperties();
			mPanelView->addItem(result);
			result->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
			mMapPropertyWindow[_style] = result;
			mPanels.push_back(result);
			return result;
		}
		return (*item).second;
	}

} // namespace tools
