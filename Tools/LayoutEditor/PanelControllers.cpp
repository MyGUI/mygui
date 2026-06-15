/*!
	@file
	@author		Georgiy Evmenov
	@date		12/2009
*/

#include "Precompiled.h"
#include "PanelControllers.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "PropertyFieldManager.h"
#include "Localise.h"

namespace tools
{

	PanelControllers::PanelControllers() :
		BasePanelViewItem("PanelControllers.layout")
	{
	}

	void PanelControllers::initialise()
	{
		mPanelCell->setCaption(replaceTags("PanelControllersName"));

		assignWidget(mControllerName, "controllerName");
		assignWidget(mButtonAdd, "buttonAdd");
		assignWidget(mButtonDelete, "buttonDelete");
		assignWidget(mList, "list");

		mButtonAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelControllers::notifyAdd);
		mButtonDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelControllers::notifyDelete);
		mList->eventListChangePosition += MyGUI::newDelegate(this, &PanelControllers::notifySelectItem);

		mButtonLeft = mButtonAdd->getLeft();
		mButtonRight = mMainWidget->getWidth() - mButtonDelete->getRight();
		mButtonSpace = mButtonDelete->getLeft() - mButtonAdd->getRight();

		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("ControllerTypes") =
			MyGUI::newDelegate(this, &PanelControllers::loadControllerTypes);
		MyGUI::ResourceManager::getInstance().load("Controllers.xml");
	}

	void PanelControllers::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelControllers::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);

		mList->removeAllItems();
		for (auto& iter : widgetContainer->mController)
		{
			mList->addItem(iter->mType, iter);
		}

		mPanelCell->setClientHeight(mButtonDelete->getCoord().bottom());
	}

	void PanelControllers::notifyChangeWidth(int _width)
	{
		int width = mMainWidget->getClientCoord().width;

		int half_width = (width - (mButtonLeft + mButtonRight + mButtonSpace)) / 2;
		mButtonAdd->setSize(half_width, mButtonAdd->getHeight());
		mButtonDelete->setCoord(
			mButtonAdd->getRight() + mButtonSpace,
			mButtonDelete->getTop(),
			width - (mButtonAdd->getRight() + mButtonSpace + mButtonRight),
			mButtonDelete->getHeight());
	}

	void PanelControllers::notifyAdd(MyGUI::Widget* _sender)
	{
		auto key = mControllerName->getOnlyText();
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		ControllerInfo* controllerInfo = new ControllerInfo();
		controllerInfo->mType = key;
		widgetContainer->mController.push_back(controllerInfo);

		mList->addItem(key, controllerInfo);
		UndoManager::getInstance().addValue();
	}

	void PanelControllers::notifyDelete(MyGUI::Widget* _sender)
	{
		size_t index = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == index)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		std::vector<ControllerInfo*>::iterator iter = std::find(
			widgetContainer->mController.begin(),
			widgetContainer->mController.end(),
			*mList->getItemDataAt<ControllerInfo*>(index));
		if (iter != widgetContainer->mController.end())
		{
			delete *iter;
			widgetContainer->mController.erase(iter);
		}

		mList->removeItemAt(index);
		UndoManager::getInstance().addValue();

		// обновляем текущий индекс
		notifySelectItem(mList, mList->getIndexSelected());
	}

	void PanelControllers::notifySelectItem(MyGUI::ListBox* _sender, size_t _index)
	{
		destroyPropertyFields();

		mIndexSelected = mList->getIndexSelected();
		if (MyGUI::ITEM_NONE == mIndexSelected)
			return;

		const MyGUI::UString& key = mList->getItemNameAt(mIndexSelected);
		mControllerName->setOnlyText(key);
		auto property = mControllersProperties.find(key.asUTF8());

		if (property != mControllersProperties.end())
		{
			ControllerInfo* controllerInfo = *mList->getItemDataAt<ControllerInfo*>(mIndexSelected);

			for (const auto& [pKey, pValue] : property->second)
			{
				std::string_view value;
				if (controllerInfo->mProperty.find(pKey) != controllerInfo->mProperty.end())
					value = controllerInfo->mProperty[pKey];

				IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, pValue);
				field->setTarget(mCurrentWidget);
				field->setName(pKey);
				field->setValue(value);
				field->eventAction = MyGUI::newDelegate(this, &PanelControllers::notifyAction);
				mFields.push_back(field);
			}
		}

		updateSize();
	}

	void PanelControllers::updateSize()
	{
		int height = mButtonDelete->getCoord().bottom() + 3;

		for (auto& field : mFields)
		{
			MyGUI::IntSize size = field->getContentSize();
			field->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
			height += size.height;
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelControllers::loadControllerTypes(
		MyGUI::xml::ElementPtr _node,
		std::string_view /*_file*/,
		MyGUI::Version /*_version*/)
	{
		MyGUI::xml::ElementEnumerator controller = _node->getElementEnumerator();
		while (controller.next("Controller"))
		{
			MyGUI::MapString controllerProperties;
			std::string_view name = controller->findAttribute("name");
			mControllerName->addItem(MyGUI::UString(name));
			MyGUI::xml::ElementEnumerator prop = controller->getElementEnumerator();
			while (prop.next("Property"))
			{
				std::string_view key = prop->findAttribute("key");
				std::string_view value = prop->findAttribute("type");
				MyGUI::mapSet(controllerProperties, key, value);
			}
			MyGUI::mapSet(mControllersProperties, name, controllerProperties);
		}
	}

	void PanelControllers::destroyPropertyFields()
	{
		for (auto& field : mFields)
			delete field;
		mFields.clear();
	}

	void PanelControllers::notifyAction(std::string_view _name, std::string_view _value, bool _final)
	{
		if (_final)
		{
			if (mIndexSelected != MyGUI::ITEM_NONE)
			{
				WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
				auto& prop = widgetContainer->mController[mIndexSelected]->mProperty;
				MyGUI::mapSet(prop, _name, _value);

				UndoManager::getInstance().addValue(PR_PROPERTIES);
			}
		}
	}

}
