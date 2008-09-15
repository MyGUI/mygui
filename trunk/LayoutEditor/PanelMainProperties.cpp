/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/

#include "PanelMainProperties.h"
#include "WidgetContainer.h"
#include "WidgetTypes.h"
#include "EditorState.h" //FIXME_HOOK


extern const int PropertyItemHeight = 22;

inline const Ogre::UTFString localise(const Ogre::UTFString & _str) {
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

PanelMainProperties::PanelMainProperties() :
	BaseLayout("PanelMainProperties.layout"),
	PanelBase(),
	current_widget(null)
{
}

void PanelMainProperties::initialiseCell(PanelCell * _cell)
{
	PanelBase::initialiseCell(_cell);

	loadLayout(_cell->getClient());
	mMainWidget->setPosition(0, 0, _cell->getClient()->getWidth(), mMainWidget->getHeight());
	_cell->setCaption(localise("Main_properties"));

	assignWidget(mButtonRelativePosition, "buttonRelativePosition");
	mButtonRelativePosition->eventMouseButtonClick = MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);
}

void PanelMainProperties::shutdownCell()
{
	PanelBase::shutdownCell();

	BaseLayout::shutdown();
}

void PanelMainProperties::notifyToggleRelativeMode(MyGUI::WidgetPtr _sender)
{
	if (current_widget){
		WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);
		if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(localise("to_percents"));
		else mButtonRelativePosition->setCaption(localise("to_pixels"));
		widgetContainer->relative_mode = !widgetContainer->relative_mode;
		//propertiesElement[1]->setCaption(widgetContainer->position()); //FIXME_AGAIN
	}
}

void PanelMainProperties::update(MyGUI::WidgetPtr _current_widget)
{
	int y = 0;
	current_widget = _current_widget;

	WidgetType * widgetType = WidgetTypes::getInstance().find(current_widget->getTypeName());
	WidgetContainer * widgetContainer = EditorWidgets::getInstance().find(current_widget);

	mEditor.createPropertiesWidgetsPair(mWidgetClient, "Name", widgetContainer->name, "Name", y); //FIXME_HOOK
	y += PropertyItemHeight;

	if (widgetType->resizeable)
	{
		// update caption of LayoutEditor_buttonRelativePosition
		mButtonRelativePosition->show();
		if (widgetContainer->relative_mode) mButtonRelativePosition->setCaption(localise("to_pixels"));
		else mButtonRelativePosition->setCaption(localise("to_percents"));

		mEditor.createPropertiesWidgetsPair(mWidgetClient, "Position", widgetContainer->position(), "Position", y); //FIXME_HOOK
		y += PropertyItemHeight;
	}
	else
	{
		mButtonRelativePosition->hide();
	}

	mEditor.createPropertiesWidgetsPair(mWidgetClient, "Align", widgetContainer->align, "Align", y); //FIXME_HOOK
	y += PropertyItemHeight;

	if (null == current_widget->getParent())
	{
		mEditor.createPropertiesWidgetsPair(mWidgetClient, "Layer", widgetContainer->layer, "Layer", y); //FIXME_HOOK
		y += PropertyItemHeight;
	}

	if (widgetType->skin.size() > 1)
	{
		mEditor.createPropertiesWidgetsPair(mWidgetClient, "Skin", widgetContainer->skin, "Skin", y); //FIXME_HOOK
		y += PropertyItemHeight;
	}

	mWidgetClient->_forcePeek(mButtonRelativePosition);
	mPanelCell->setClientHeight(y);
}
