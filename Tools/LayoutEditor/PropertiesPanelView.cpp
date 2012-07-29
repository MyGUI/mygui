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
		mPanelTemplateProperties(nullptr),
		mPanelExtensionProperties(nullptr),
		mCurrentWidget(nullptr)
	{
		assignBase(mPanelView, "scroll_View");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
		{
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &PropertiesPanelView::notifyWindowChangeCoord);
			mOldSize = window->getSize();
		}

		mPanelMainProperties = new PanelMainProperties();
		mPanelView->addItem(mPanelMainProperties);

		mPanelTemplateProperties = new PanelTemplateProperties();
		mPanelView->addItem(mPanelTemplateProperties);

		mPanelExtensionProperties = new PanelExtensionProperties();
		mPanelView->addItem(mPanelExtensionProperties);

		mPanelItems = new PanelItems();
		mPanelView->addItem(mPanelItems);

		mPanelUserData = new PanelUserData();
		mPanelView->addItem(mPanelUserData);

		mPanelControllers = new PanelControllers();
		mPanelView->addItem(mPanelControllers);

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);

		notifyChangeSelectedWidget(nullptr);
	}

	PropertiesPanelView::~PropertiesPanelView()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);

		mPanelView->removeAllItems();
		delete mPanelMainProperties;
		delete mPanelItems;
		delete mPanelUserData;
		delete mPanelControllers;
		delete mPanelTemplateProperties;
		delete mPanelExtensionProperties;

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
			MyGUI::Widget* parent = mCurrentWidget;

			// if we select TabItem, it's chuld or subchild then show TabItem
			while (nullptr != parent && !parent->isType<MyGUI::TabItem>())
				parent = parent->getParent();
			if (nullptr != parent)
			{
				MyGUI::TabControl* tab = parent->getParent()->castType<MyGUI::TabControl>();
				MyGUI::TabItem* sheet = parent->castType<MyGUI::TabItem>();
				tab->setItemSelected(sheet);
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

			mPanelTemplateProperties->setVisible(false);
			mPanelTemplateProperties->update(nullptr, nullptr);

			mPanelExtensionProperties->setVisible(false);
			mPanelExtensionProperties->update(nullptr);
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

			mPanelExtensionProperties->setVisible(true);
			mPanelExtensionProperties->update(mCurrentWidget);

			std::string widgetTypeName = mCurrentWidget->getTypeName();

			bool templateName = false;
			WidgetContainer* container = EditorWidgets::getInstance().find(mCurrentWidget);
			if (container->existUserData("LE_TargetWidgetType"))
			{
				widgetTypeName = container->getUserData("LE_TargetWidgetType");
				templateName = true;
			}

			WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetTypeName);

			mPanelTemplateProperties->setVisible(true);
			mPanelTemplateProperties->update(mCurrentWidget, templateName ? widgetType : nullptr);

			while (widgetType != nullptr)
			{
				PanelProperties* panel = getPropertyWindow(widgetType, widgetType->deep);
				panel->setVisible(true);
				panel->update(mCurrentWidget, widgetType);

				if (widgetType->name == "Widget")
					break;

				widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetType->base);
			}
		}
	}

	PanelProperties* PropertiesPanelView::getPropertyWindow(WidgetStyle* _style, size_t _deep)
	{
		MapPropertyWindow::iterator item = mMapPropertyWindow.find(_style);
		if (item == mMapPropertyWindow.end())
		{
			PanelProperties* result = new PanelProperties();
			result->setDeep(_deep);

			mPanelView->insertItem(getIndexByDeep(_deep), result);

			mMapPropertyWindow[_style] = result;
			return result;
		}

		return (*item).second;
	}

	size_t PropertiesPanelView::getIndexByDeep(size_t _deep)
	{
		size_t result = 1;
		for (MapPropertyWindow::iterator item = mMapPropertyWindow.begin(); item != mMapPropertyWindow.end(); ++ item)
		{
			if ((*item).second->getDeep() < _deep)
			{
				size_t index = getIndexPanel((*item).second);
				if (index >= result)
					result = index + 1;
			}
		}
		return result;
	}

	size_t PropertiesPanelView::getIndexPanel(PanelProperties* _panel)
	{
		for (size_t index = 0; index < mPanelView->getItemCount(); ++ index)
		{
			if (mPanelView->getItem(index) == _panel)
				return index;
		}
		return MyGUI::ITEM_NONE;
	}

}
