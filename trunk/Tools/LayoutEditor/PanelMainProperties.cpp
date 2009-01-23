/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "precompiled.h"
#include "PanelMainProperties.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"

extern const int PropertyItemHeight = 22;

inline const Ogre::UTFString localise(const Ogre::UTFString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PanelMainProperties::PanelMainProperties() : BasePanelViewItem("PanelMainProperties.layout"),
	current_widget(nullptr)
{
}

void PanelMainProperties::initialise()
{
	mPanelCell->setCaption(localise("Main_properties"));

	assignWidget(mButtonRelativePosition, "buttonRelativePosition");
	mButtonRelativePosition->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);
}

void PanelMainProperties::shutdown()
{
}

void PanelMainProperties::notifyToggleRelativeMode(MyGUI::WidgetPtr _sender)
{
	if (current_widget){
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
		if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(localise("to_percents"));
		else mButtonRelativePosition->setCaption(localise("to_pixels"));
		widgetContainer->relative_mode = !widgetContainer->relative_mode;
		eventSetPositionText(widgetContainer->position());
	}
}

void PanelMainProperties::update(MyGUI::WidgetPtr _current_widget)
{
	int y = 0;
	current_widget = _current_widget;

	WidgetStyle * widgetType = WidgetTypes::getInstance().find(current_widget->getTypeName());
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);

	eventCreatePair(mWidgetClient, "Name", widgetContainer->name, "Name", y);
	y += PropertyItemHeight;

	if (widgetType->resizeable)
	{
		// update caption of LayoutEditor_buttonRelativePosition
		mButtonRelativePosition->setVisible(true);
		if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(localise("to_pixels"));
		else mButtonRelativePosition->setCaption(localise("to_percents"));

		eventCreatePair(mWidgetClient, "Position", widgetContainer->position(), "Position", y);
		y += PropertyItemHeight;
	}
	else
	{
		mButtonRelativePosition->setVisible(false);
	}

	eventCreatePair(mWidgetClient, "Align", widgetContainer->align, "Align", y);
	y += PropertyItemHeight;

	if (nullptr == current_widget->getParent())
	{
		eventCreatePair(mWidgetClient, "Layer", widgetContainer->layer, "Layer", y);
		y += PropertyItemHeight;
	}

	if (widgetType->skin.size() > 1)
	{
		eventCreatePair(mWidgetClient, "Skin", widgetContainer->skin, "Skin", y);
		y += PropertyItemHeight;
	}

	mWidgetClient->_forcePeek(mButtonRelativePosition);
	mPanelCell->setClientHeight(y);
}
