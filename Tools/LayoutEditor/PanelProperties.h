/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

extern const int PropertyItemHeight;

class PanelProperties : public wraps::BasePanelViewItem
{
public:

	PanelProperties();

	virtual void initialise();
	virtual void shutdown();

	enum PropertiesGroup {TYPE_PROPERTIES, WIDGET_PROPERTIES, EVENTS};
	void update(MyGUI::WidgetPtr _current_widget, PropertiesGroup _group);

	typedef MyGUI::delegates::CDelegate5<MyGUI::WidgetPtr, Ogre::String, Ogre::String, std::string, int> EventHandle_EventCreatePair;
	EventHandle_EventCreatePair eventCreatePair;
};

#endif // __PANEL_PROPERTIES_H__
