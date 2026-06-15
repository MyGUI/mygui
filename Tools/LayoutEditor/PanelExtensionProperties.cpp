/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PanelExtensionProperties.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "PropertyFieldManager.h"
#include "UndoManager.h"

namespace tools
{

	PanelExtensionProperties::PanelExtensionProperties() :
		BasePanelViewItem("PanelExtensionProperties.layout")
	{
	}

	void PanelExtensionProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("PanelExtensionPropertiesName"));
	}

	void PanelExtensionProperties::shutdown()
	{
		destroyPropertyFields();
	}

	void PanelExtensionProperties::AddParametrs(
		WidgetStyle* widgetType,
		WidgetContainer* widgetContainer,
		MyGUI::Widget* _currentWidget)
	{
		if (widgetType != nullptr)
		{
			for (auto& iter : widgetType->parameterData)
			{
				std::string_view value = widgetContainer->getUserData(iter.first);

				IPropertyField* field =
					PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, iter.second);
				field->setTarget(_currentWidget);
				field->setName(iter.first);
				field->setValue(value);
				field->eventAction = MyGUI::newDelegate(this, &PanelExtensionProperties::notifyAction);
				mFields.push_back(field);
			}
		}
	}

	void PanelExtensionProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(_currentWidget);
		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetContainer->getType());

		AddParametrs(widgetType, widgetContainer, mCurrentWidget);

		bool visible = !mFields.empty();
		setVisible(visible);

		updateSize();
	}

	void PanelExtensionProperties::updateSize()
	{
		int height = 0;

		for (auto& field : mFields)
		{
			MyGUI::IntSize size = field->getContentSize();
			field->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
			height += size.height;
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelExtensionProperties::destroyPropertyFields()
	{
		for (auto& field : mFields)
			delete field;
		mFields.clear();
	}

	void PanelExtensionProperties::notifyAction(std::string_view _name, std::string_view _value, bool _final)
	{
		if (_final)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			widgetContainer->setUserData(_name, _value);

			UndoManager::getInstance().addValue(PR_PROPERTIES);
		}
	}

}
