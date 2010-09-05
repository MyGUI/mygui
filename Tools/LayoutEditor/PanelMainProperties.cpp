/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "precompiled.h"
#include "Localise.h"
#include "PanelMainProperties.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "SettingsManager.h"

namespace tools
{
	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mButtonRelativePosition(nullptr),
		mCurrentWidget(nullptr),
		mPropertyItemHeight(0)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));

		assignWidget(mButtonRelativePosition, "buttonRelativePosition");
		mButtonRelativePosition->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);

		mPropertyItemHeight = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight");
	}

	void PanelMainProperties::shutdown()
	{
	}

	void PanelMainProperties::notifyToggleRelativeMode(MyGUI::Widget* _sender)
	{
		if (mCurrentWidget)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(replaceTags("to_percents"));
			else mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			widgetContainer->relative_mode = !widgetContainer->relative_mode;
			eventSetPositionText(widgetContainer->position());
		}
	}

	void PanelMainProperties::update(MyGUI::Widget* _currentWidget)
	{
		int y = 0;
		mCurrentWidget = _currentWidget;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		eventCreatePair(mWidgetClient, "Name", widgetContainer->name, "Name", y);
		y += mPropertyItemHeight;

		if (widgetType->resizeable)
		{
			// update caption of LayoutEditor_buttonRelativePosition
			mButtonRelativePosition->setVisible(true);
			if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			else mButtonRelativePosition->setCaption(replaceTags("to_percents"));

			eventCreatePair(mWidgetClient, "Position", widgetContainer->position(), "Position", y);
			y += mPropertyItemHeight;
		}
		else
		{
			mButtonRelativePosition->setVisible(false);
		}

		eventCreatePair(mWidgetClient, "Align", widgetContainer->align, "Align", y);
		y += mPropertyItemHeight;

		if (nullptr == mCurrentWidget->getParent())
		{
			eventCreatePair(mWidgetClient, "Layer", widgetContainer->layer, "Layer", y);
			y += mPropertyItemHeight;
		}

		if (widgetType->skin.size() > 1)
		{
			eventCreatePair(mWidgetClient, "Skin", widgetContainer->skin, "Skin", y);
			y += mPropertyItemHeight;
		}

		mWidgetClient->_forcePeek(mButtonRelativePosition);
		mPanelCell->setClientHeight(y);
	}

} // namespace tools
