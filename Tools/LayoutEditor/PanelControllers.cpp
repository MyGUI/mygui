/*!
	@file
	@author		Georgiy Evmenov
	@date		12/2009
	@module
*/

#include "precompiled.h"
#include "PanelControllers.h"
#include "EditorWidgets.h"
#include "UndoManager.h"

PanelControllers::PanelControllers() : BasePanelViewItem("PanelControllers.layout")
{
}

void PanelControllers::initialise()
{
	mPanelCell->setCaption("Controllers");

	assignWidget(mControllerName, "controllerName");
	assignWidget(mButtonAdd, "buttonAdd");
	assignWidget(mButtonDelete, "buttonDelete");
	assignWidget(mList, "list");

	mButtonAdd->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelControllers::notifyAdd);
	mButtonDelete->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelControllers::notifyDelete);
	mList->eventListChangePosition = MyGUI::newDelegate(this, &PanelControllers::notifySelectItem);

	mButtonLeft = mButtonAdd->getLeft();
	mButtonRight = mMainWidget->getWidth() - mButtonDelete->getRight();
	mButtonSpace = mButtonDelete->getLeft() - mButtonAdd->getRight();

	
	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("ControllerTypes") = MyGUI::newDelegate(this, &PanelControllers::loadControllerTypes);
	MyGUI::ResourceManager::getInstance().load("controllers.xml");
}

void PanelControllers::shutdown()
{
}

void PanelControllers::update(MyGUI::Widget* _current_widget)
{
	//int y = 0;
	current_widget = _current_widget;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

	mList->removeAllItems();
	for (std::vector<ControllerInfo*>::iterator iter = widgetContainer->mController.begin(); iter != widgetContainer->mController.end(); ++iter)
	{
		mList->addItem((*iter)->mType, *iter);
	}

	mPanelCell->setClientHeight(mButtonDelete->getCoord().bottom());
}

void PanelControllers::notifyChangeWidth(int _width)
{
	int width = mMainWidget->getClientCoord().width;

	int half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
	mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
	mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight), mButtonDelete->getHeight());
}

void PanelControllers::notifyAdd(MyGUI::Widget* _sender)
{
	std::string key = mControllerName->getOnlyText();
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);

	ControllerInfo* controllerInfo = new ControllerInfo();
	controllerInfo->mType = key;
	widgetContainer->mController.push_back(controllerInfo);

	mList->addItem(key, controllerInfo);
	UndoManager::getInstance().addValue();
}

void PanelControllers::notifyDelete(MyGUI::Widget* _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	std::vector<ControllerInfo*>::iterator iter = std::find(widgetContainer->mController.begin(), widgetContainer->mController.end(), *mList->getItemDataAt<ControllerInfo*>(item));
	if (iter != widgetContainer->mController.end())
	{
		delete *iter;
		widgetContainer->mController.erase(iter);
	}

	mList->removeItemAt(item);
	UndoManager::getInstance().addValue();
}

void PanelControllers::notifySelectItem(MyGUI::List* _sender, size_t _index)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	std::string key = mList->getItemNameAt(item);
	mControllerName->setOnlyText(key);

	int y = mButtonDelete->getCoord().bottom();
	eventHidePairs(mWidgetClient);
	if (mControllersProperties.find(key) != mControllersProperties.end())
	{
		ControllerInfo* controllerInfo = *mList->getItemDataAt<ControllerInfo*>(item);

		for (MyGUI::MapString::iterator iter = mControllersProperties[key].begin(); iter != mControllersProperties[key].end(); ++iter)
		{
			std::string val = "";
			if (controllerInfo->mProperty.find(iter->first) != controllerInfo->mProperty.end())
				val = controllerInfo->mProperty[iter->first];
			eventCreatePair(mWidgetClient, "Controller_" + iter->first, val, iter->second, y);

			y += PropertyItemHeight;
		}
	}
	mPanelCell->setClientHeight(y);
}

void PanelControllers::loadControllerTypes(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator controller = _node->getElementEnumerator();
	while (controller.next("Controller"))
	{
		MyGUI::MapString controllerProperties;
		std::string name = controller->findAttribute("name");
		mControllerName->addItem(name);
		MyGUI::xml::ElementEnumerator prop = controller->getElementEnumerator();
		while (prop.next("Property"))
		{
			controllerProperties[prop->findAttribute("key")] = prop->findAttribute("type");
		}
		mControllersProperties[name] = controllerProperties;
	}
}
