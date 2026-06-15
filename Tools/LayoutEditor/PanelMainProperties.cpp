/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PanelMainProperties.h"
#include "Localise.h"
#include "PropertyFieldManager.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"
#include "GroupMessage.h"

namespace tools
{

	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mUserDataTargetType("LE_TargetWidgetType")
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
		hidePropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
		bool existTargetType = widgetContainer->existUserData(mUserDataTargetType);

		IPropertyField* field = getPropertyField(mWidgetClient, "Name", "Name");
		field->setTarget(_currentWidget);
		field->setValue(widgetContainer->getName());

		if (widgetType->resizeable)
		{
			field = getPropertyField(mWidgetClient, "Position", "Position");
			field->setTarget(_currentWidget);
			field->setValue(widgetContainer->position());
		}

		field = getPropertyField(mWidgetClient, "Type", "Type");
		field->setTarget(_currentWidget);
		if (existTargetType)
		{
			std::string_view targetType = widgetContainer->getUserData(mUserDataTargetType);
			field->setValue(targetType);
		}
		else
		{
			field->setValue(widgetContainer->getType());
		}

		field = getPropertyField(mWidgetClient, "Align", "Align");
		field->setTarget(_currentWidget);
		field->setValue(widgetContainer->getAlign());

		if (mCurrentWidget->isRootWidget())
		{
			field = getPropertyField(mWidgetClient, "Layer", "Layer");
			field->setTarget(_currentWidget);
			field->setValue(widgetContainer->getLayerName());

			field = getPropertyField(mWidgetClient, "Template", "Bool");
			field->setTarget(_currentWidget);
			field->setValue(existTargetType ? "true" : "false");
		}

		field = getPropertyField(mWidgetClient, "Skin", "Skin");
		field->setTarget(_currentWidget);
		field->setValue(widgetContainer->getSkin());

		updateSize();
	}

	void PanelMainProperties::updateSize()
	{
		int height = 0;

		for (auto& field : mFields)
		{
			if (field.second->getVisible())
			{
				MyGUI::IntSize size = field.second->getContentSize();
				field.second->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
				height += size.height;
			}
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelMainProperties::destroyPropertyFields()
	{
		for (auto& field : mFields)
			delete field.second;
		mFields.clear();
	}

	void PanelMainProperties::notifyActionSkin(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			widgetContainer->setSkin(_value);
			if (isSkinExist(widgetContainer->getSkin()) || widgetContainer->getSkin().empty())
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
				std::string mess = MyGUI::utility::toString(
					"Skin '",
					widgetContainer->getSkin(),
					"' not found. This value will be saved.");
				GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
			}

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	bool PanelMainProperties::isSkinExist(std::string_view _skinName)
	{
		return _skinName == "Default" || MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool PanelMainProperties::checkTemplate(std::string_view _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (const auto& container : data)
			{
				if (container.name == "Root")
					return true;
			}
		}

		return false;
	}

	void PanelMainProperties::notifyActionLayer(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setLayerName(_value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelMainProperties::notifyActionName(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setName(_value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelMainProperties::notifyActionType(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			if (!widgetContainer->existUserData(mUserDataTargetType))
			{
				widgetContainer->setType(_value);

				WidgetSelectorManager::getInstance().saveSelectedWidget();

				MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
				EditorWidgets::getInstance().clear();
				EditorWidgets::getInstance().loadxmlDocument(savedDoc);
				delete savedDoc;

				WidgetSelectorManager::getInstance().restoreSelectedWidget();

				UndoManager::getInstance().addValue(PR_PROPERTIES);
			}
			else
			{
				widgetContainer->setUserData(mUserDataTargetType, _value);

				WidgetSelectorManager::getInstance().saveSelectedWidget();

				MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
				EditorWidgets::getInstance().clear();
				EditorWidgets::getInstance().loadxmlDocument(savedDoc);
				delete savedDoc;

				WidgetSelectorManager::getInstance().restoreSelectedWidget();

				UndoManager::getInstance().addValue(PR_PROPERTIES);
			}
		}
	}

	void PanelMainProperties::notifyActionAlign(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			widgetContainer->setAlign(_value);
			widgetContainer->getWidget()->setAlign(MyGUI::Align::parse(_value));

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

	void PanelMainProperties::notifyActionTemplate(std::string_view /*_type*/, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			if (_value == "true")
			{
				if (!widgetContainer->existUserData(mUserDataTargetType))
				{
					widgetContainer->setUserData(mUserDataTargetType, widgetContainer->getType());
					widgetContainer->setType(MyGUI::Widget::getClassTypeName());

					WidgetSelectorManager::getInstance().saveSelectedWidget();

					MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
					EditorWidgets::getInstance().clear();
					EditorWidgets::getInstance().loadxmlDocument(savedDoc);
					delete savedDoc;

					WidgetSelectorManager::getInstance().restoreSelectedWidget();

					UndoManager::getInstance().addValue(PR_PROPERTIES);
				}
			}
			else
			{
				if (widgetContainer->existUserData(mUserDataTargetType))
				{
					std::string_view targetType = widgetContainer->getUserData(mUserDataTargetType);
					widgetContainer->clearUserData(mUserDataTargetType);
					widgetContainer->setType(targetType);

					WidgetSelectorManager::getInstance().saveSelectedWidget();

					MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
					EditorWidgets::getInstance().clear();
					EditorWidgets::getInstance().loadxmlDocument(savedDoc);
					delete savedDoc;

					WidgetSelectorManager::getInstance().restoreSelectedWidget();

					UndoManager::getInstance().addValue(PR_PROPERTIES);
				}
			}
		}
	}

	IPropertyField* PanelMainProperties::getPropertyField(
		MyGUI::Widget* _client,
		std::string_view _name,
		std::string_view _type)
	{
		MapPropertyField::iterator item = mFields.find(_name);
		if (item != mFields.end())
		{
			(*item).second->setVisible(true);
			return (*item).second;
		}

		IPropertyField* result = PropertyFieldManager::getInstance().createPropertyField(_client, _type);
		result->setName(_name);
		result->eventAction = MyGUI::newDelegate(this, &PanelMainProperties::notifyAction);

		mFields.emplace(_name, result);

		return result;
	}

	void PanelMainProperties::hidePropertyFields()
	{
		for (auto& field : mFields)
			field.second->setVisible(false);
	}

	void PanelMainProperties::notifyAction(std::string_view _name, std::string_view _value, bool _final)
	{
		if (_name == "Skin")
			notifyActionSkin(_name, _value, _final);
		else if (_name == "Layer")
			notifyActionLayer(_name, _value, _final);
		else if (_name == "Name")
			notifyActionName(_name, _value, _final);
		else if (_name == "Type")
			notifyActionType(_name, _value, _final);
		else if (_name == "Align")
			notifyActionAlign(_name, _value, _final);
		else if (_name == "Template")
			notifyActionTemplate(_name, _value, _final);
	}

}
