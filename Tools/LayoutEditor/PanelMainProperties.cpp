/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PanelMainProperties.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "PropertyFieldManager.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"
#include "GroupMessage.h"

namespace tools
{

	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mCurrentWidget(nullptr)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));
	}

	void PanelMainProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelMainProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Name", _currentWidget);
		field->setName("Name");
		field->setValue(widgetContainer->name);
		field->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyActionName);
		mFields.push_back(field);

		if (widgetType->resizeable)
		{
			field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Position", _currentWidget);
			field->setName("Position");
			field->setValue(widgetContainer->position());
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Type", _currentWidget);
		field->setName("Type");
		field->setValue(widgetContainer->type);
		field->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyActionType);
		mFields.push_back(field);

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Align", _currentWidget);
		field->setName("Align");
		field->setValue(widgetContainer->align);
		field->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyActionAlign);
		mFields.push_back(field);

		if (mCurrentWidget->isRootWidget())
		{
			IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Layer", _currentWidget);
			field->setName("Layer");
			field->setValue(widgetContainer->getLayerName());
			field->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyActionLayer);
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Skin", _currentWidget);
		field->setName("Skin");
		field->setValue(widgetContainer->skin);
		field->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyActionSkin);
		mFields.push_back(field);

		updateSize();
	}

	void PanelMainProperties::updateSize()
	{
		int height = 0;

		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
		{
			MyGUI::IntSize size = (*item)->getContentSize();
			(*item)->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
			height += size.height;
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelMainProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

	void PanelMainProperties::notifyActionSkin(const std::string& _type, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			widgetContainer->skin = _value;
			if (isSkinExist(widgetContainer->skin) || widgetContainer->skin.empty())
			{
				WidgetSelectorManager::getInstance().saveSelectedWidget();

				MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
				EditorWidgets::getInstance().clear();
				EditorWidgets::getInstance().loadxmlDocument(savedDoc);
				delete savedDoc;

				WidgetSelectorManager::getInstance().restoreSelectedWidget();
			}
			else
			{
				std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
				GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
			}

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	bool PanelMainProperties::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" ||
			MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool PanelMainProperties::checkTemplate(const std::string& _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
					return true;
			}
		}

		return false;
	}

	void PanelMainProperties::notifyActionLayer(const std::string& _type, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setLayerName(_value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelMainProperties::notifyActionName(const std::string& _type, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->name = _value;
			EditorWidgets::getInstance().invalidateWidgets();
		}
	}

	void PanelMainProperties::notifyActionType(const std::string& _type, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->type = _value;

			WidgetSelectorManager::getInstance().saveSelectedWidget();

			MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
			EditorWidgets::getInstance().clear();
			EditorWidgets::getInstance().loadxmlDocument(savedDoc);
			delete savedDoc;

			WidgetSelectorManager::getInstance().restoreSelectedWidget();

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelMainProperties::notifyActionAlign(const std::string& _type, const std::string& _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			widgetContainer->align = _value;
			widgetContainer->widget->setAlign(MyGUI::Align::parse(_value));

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

} // namespace tools
