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

inline const MyGUI::UString localise(const MyGUI::UString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

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
	mControllerName->eventComboAccept = MyGUI::newDelegate(this, &PanelControllers::notifyUpdate);
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

void PanelControllers::update(MyGUI::WidgetPtr _current_widget)
{
	//int y = 0;
	current_widget = _current_widget;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(_current_widget);

	mList->removeAllItems();
	for (MyGUI::VectorString::iterator iter = widgetContainer->mController.begin(); iter != widgetContainer->mController.end(); ++iter)
	{
		mList->addItem(*iter);
	}
}

void PanelControllers::notifyChangeWidth(int _width)
{
	int width = mMainWidget->getClientCoord().width;

	int half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
	mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
	mButtonDelete->setCoord(mButtonAdd->getRight() + mButtonSpace, mButtonDelete->getTop(), width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight), mButtonDelete->getHeight());
}

void PanelControllers::notifyAdd(MyGUI::WidgetPtr _sender)
{
	std::string key = mControllerName->getOnlyText();
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	//if (std::find(widgetContainer->mController, key) == widgetContainer->mController.end())
	{
		mList->addItem(key);
	}
	//mList->setItemNameAt(1, mMultilist->findSubItemWith(0, key), value);
	widgetContainer->mController.push_back(key);
	UndoManager::getInstance().addValue();
}

void PanelControllers::notifyDelete(MyGUI::WidgetPtr _sender)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	widgetContainer->mController.erase(widgetContainer->mController.begin() + item);

	mList->removeItemAt(item);
	UndoManager::getInstance().addValue();
}

void PanelControllers::notifyUpdate(MyGUI::ComboBoxPtr _sender, size_t _index)
{
	/*size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item)
	{
		notifyAdd();
		return;
	}
	std::string key = mControllerName->getOnlyText();
	std::string lastkey = mList->getItemNameAt(item);

	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
	mList->removeItemAt(mList->findItemIndexWith(lastkey));
	widgetContainer->mController.erase(widgetContainer->mController.begin() + item);
	if (std::find(widgetContainer->mController.begin(), widgetContainer->mController.end(), key) == widgetContainer->mController.end())
	{
		mList->addItem(key);
	}
	mList->setItemNameAt(item, key);

	mList->setIndexSelected(item);
	MapSet(widgetContainer->mUserString, key, value);
	UndoManager::getInstance().addValue();*/
}

void PanelControllers::notifySelectItem(MyGUI::ListPtr _sender, size_t _index)
{
	size_t item = mList->getIndexSelected();
	if (MyGUI::ITEM_NONE == item) return;
	std::string key = mList->getItemNameAt(item);
	mControllerName->setOnlyText(key);
}

void PanelControllers::loadControllerTypes(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	MyGUI::xml::ElementEnumerator parameter = _node->getElementEnumerator();
	while (parameter.next("Controller"))
	{
		std::string name = parameter->findAttribute("name");
		mControllerName->addItem(name);
	}
}
