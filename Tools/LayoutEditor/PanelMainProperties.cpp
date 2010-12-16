/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "Localise.h"
#include "PanelMainProperties.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "CommandManager.h"
#include "PropertyFieldManager.h"

namespace tools
{
	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mButtonRelativePosition(nullptr),
		mCurrentWidget(nullptr),
		mPositionField(nullptr)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));

		assignWidget(mButtonRelativePosition, "buttonRelativePosition");
		mButtonRelativePosition->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);

		EditorWidgets::getInstance().eventChangeWidgetCoord += MyGUI::newDelegate(this, &PanelMainProperties::notifyPropertyChangeCoord);

		CommandManager::getInstance().registerCommand("Command_ToggleRelativeMode", MyGUI::newDelegate(this, &PanelMainProperties::commandToggleRelativeMode));
	}

	void PanelMainProperties::shutdown()
	{
		destroyPropertyFields();
		EditorWidgets::getInstance().eventChangeWidgetCoord -= MyGUI::newDelegate(this, &PanelMainProperties::notifyPropertyChangeCoord);
	}

	void PanelMainProperties::notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner)
	{
		if (_owner == "PropertiesPanelView" || _widget != mCurrentWidget)
			return;

		updatePositionCaption();
	}

	void PanelMainProperties::notifyToggleRelativeMode(MyGUI::Widget* _sender)
	{
		if (mCurrentWidget)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			if (widgetContainer->relative_mode)
				mButtonRelativePosition->setCaption(replaceTags("to_percents"));
			else
				mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			widgetContainer->relative_mode = !widgetContainer->relative_mode;
		}

		updatePositionCaption();
	}

	void PanelMainProperties::updatePositionCaption()
	{
		if (mCurrentWidget)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			if (mPositionField != nullptr)
				mPositionField->setValue(widgetContainer->position());
		}
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
		mFields.push_back(field);

		if (widgetType->resizeable)
		{
			// update caption of LayoutEditor_buttonRelativePosition
			mButtonRelativePosition->setVisible(true);
			if (widgetContainer->relative_mode)
				mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			else
				mButtonRelativePosition->setCaption(replaceTags("to_percents"));

			IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Position", _currentWidget);
			field->setName("Position");
			field->setValue(widgetContainer->position());
			mFields.push_back(field);

			mPositionField = field;
		}
		else
		{
			mButtonRelativePosition->setVisible(false);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Type", _currentWidget);
		field->setName("Type");
		field->setValue(widgetContainer->type);
		mFields.push_back(field);

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Align", _currentWidget);
		field->setName("Align");
		field->setValue(widgetContainer->align);
		mFields.push_back(field);

		if (mCurrentWidget->isRootWidget())
		{
			IPropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Layer", _currentWidget);
			field->setName("Layer");
			field->setValue(widgetContainer->getLayerName());
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Skin", _currentWidget);
		field->setName("Skin");
		field->setValue(widgetContainer->skin);
		mFields.push_back(field);

		mWidgetClient->_forcePeek(mButtonRelativePosition);

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

	void PanelMainProperties::commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result)
	{
		notifyToggleRelativeMode();

		_result = true;
	}

	void PanelMainProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();

		mPositionField = nullptr;
	}

} // namespace tools
