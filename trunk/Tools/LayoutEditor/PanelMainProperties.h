/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

extern const int PropertyItemHeight;

class PanelMainProperties : public wraps::BaseLayout, public wraps::PanelBase
{
public:

	PanelMainProperties();

	virtual void initialiseCell(wraps::PanelCell * _cell);
	virtual void shutdownCell();

	void notifyToggleRelativeMode(MyGUI::WidgetPtr _sender = null);
	void update(MyGUI::WidgetPtr _current_widget);
	
	typedef MyGUI::delegates::CDelegate5<MyGUI::WidgetPtr, Ogre::String, Ogre::String, std::string, int> EventInfo_EventCreatePair;
	EventInfo_EventCreatePair eventCreatePair;
	typedef MyGUI::delegates::CDelegate1<const std::string &> EventInfo_EventSetPositionText;
	EventInfo_EventSetPositionText eventSetPositionText;
private:
	MyGUI::ButtonPtr mButtonRelativePosition;
	
	MyGUI::WidgetPtr current_widget;
};

#endif // __PANEL_MAIN_PROPERTIES_H__
