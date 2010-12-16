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
#include "SettingsManager.h"
#include "CommandManager.h"

namespace tools
{
	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mButtonRelativePosition(nullptr),
		mCurrentWidget(nullptr),
		mPropertyItemHeight(0),
		mPositionEdit(nullptr)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));

		assignWidget(mButtonRelativePosition, "buttonRelativePosition");
		mButtonRelativePosition->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);

		mPropertyItemHeight = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight");

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

			if (mPositionEdit != nullptr)
				mPositionEdit->setCaption(widgetContainer->position());
		}
	}

	void PanelMainProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		PropertyField field;
		int y = 0;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		eventCreatePair(mWidgetClient, "Name", widgetContainer->name, "Name", y, field);
		mFields.push_back(field);

		y += mPropertyItemHeight;

		if (widgetType->resizeable)
		{
			// update caption of LayoutEditor_buttonRelativePosition
			mButtonRelativePosition->setVisible(true);
			if (widgetContainer->relative_mode)
				mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			else
				mButtonRelativePosition->setCaption(replaceTags("to_percents"));

			eventCreatePair(mWidgetClient, "Position", widgetContainer->position(), "Position", y, field);
			mFields.push_back(field);

			mPositionEdit = field.getField();

			y += mPropertyItemHeight;
		}
		else
		{
			mButtonRelativePosition->setVisible(false);
		}

		eventCreatePair(mWidgetClient, "Type", widgetContainer->type, "Type", y, field);
		mFields.push_back(field);

		y += mPropertyItemHeight;

		eventCreatePair(mWidgetClient, "Align", widgetContainer->align, "Align", y, field);
		mFields.push_back(field);

		y += mPropertyItemHeight;

		if (mCurrentWidget->isRootWidget())
		{
			eventCreatePair(mWidgetClient, "Layer", widgetContainer->getLayerName(), "Layer", y, field);
			mFields.push_back(field);

			y += mPropertyItemHeight;
		}

		eventCreatePair(mWidgetClient, "Skin", widgetContainer->skin, "Skin", y, field);
		mFields.push_back(field);

		y += mPropertyItemHeight;

		mWidgetClient->_forcePeek(mButtonRelativePosition);
		mPanelCell->setClientHeight(y);
	}

	void PanelMainProperties::commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result)
	{
		notifyToggleRelativeMode();

		_result = true;
	}

	void PanelMainProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			(*item).destroy();
		mFields.clear();
	}

} // namespace tools
