/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PropertiesPanelView.h"
#include "EditorWidgets.h"
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

		mPanelItems = new PanelItems();
		mPanelView->addItem(mPanelItems);

		mPanelUserData = new PanelUserData();
		mPanelView->addItem(mPanelUserData);

		mPanelControllers = new PanelControllers();
		mPanelView->addItem(mPanelControllers);

		mPanels.push_back(mPanelMainProperties);
		mPanels.push_back(mPanelItems);
		mPanels.push_back(mPanelUserData);
		mPanels.push_back(mPanelControllers);

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

		for (MapPropertyWindow::iterator item = mMapPropertyWindow.begin(); item != mMapPropertyWindow.end(); ++ item)
		{
			(*item).second->setVisible(false);
			(*item).second->update(nullptr, nullptr);
		}

		if (nullptr == mCurrentWidget)
		{
			mPanelMainProperties->setVisible(false);
			mPanelMainProperties->update(nullptr);

			mPanelItems->setVisible(false);
			mPanelItems->update(nullptr);

			mPanelUserData->setVisible(false);
			mPanelUserData->update(nullptr);

			mPanelControllers->setVisible(false);
			mPanelControllers->update(nullptr);
		}
		else
		{
			mPanelMainProperties->setVisible(true);
			mPanelMainProperties->update(mCurrentWidget);

			mPanelItems->setVisible(true);
			mPanelItems->update(mCurrentWidget);

			mPanelUserData->setVisible(true);
			mPanelUserData->update(mCurrentWidget);

			mPanelControllers->setVisible(true);
			mPanelControllers->update(mCurrentWidget);

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

			WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetTypeName);

			while (widgetType != nullptr && !widgetType->base.empty())
			{
				PanelProperties* panel = getPropertyWindow(widgetType);
				panel->setVisible(true);
				panel->update(mCurrentWidget, widgetType);

				widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetType->base);
			}
		}
	}

	PanelProperties* PropertiesPanelView::getPropertyWindow(WidgetStyle* _style)
	{
		MapPropertyWindow::iterator item = mMapPropertyWindow.find(_style);
		if (item == mMapPropertyWindow.end())
		{
			PanelProperties* result = new PanelProperties();
			mPanelView->addItem(result);
			mMapPropertyWindow[_style] = result;
			mPanels.push_back(result);
			return result;
		}
		return (*item).second;
	}

} // namespace tools
