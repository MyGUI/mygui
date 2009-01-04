/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

extern const int PropertyItemHeight;

class PanelProperties : public wraps::BaseLayout, public wraps::PanelBase
{
public:

	PanelProperties();

	virtual void initialiseCell(wraps::PanelCell * _cell);
	virtual void shutdownCell();
	enum PropertiesGroup {TYPE_PROPERTIES, WIDGET_PROPERTIES, EVENTS};
	void update(MyGUI::WidgetPtr _current_widget, PropertiesGroup _group);

	typedef MyGUI::delegates::CDelegate5<MyGUI::WidgetPtr, Ogre::String, Ogre::String, std::string, int> EventInfo_EventCreatePair;
	EventInfo_EventCreatePair eventCreatePair;
};

#endif // __PANEL_PROPERTIES_H__
