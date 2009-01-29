/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

extern const int PropertyItemHeight;

class PanelMainProperties : public wraps::BasePanelViewItem
{
public:

	PanelMainProperties();

	virtual void initialise();
	virtual void shutdown();

	void notifyToggleRelativeMode(MyGUI::WidgetPtr _sender = nullptr);
	void update(MyGUI::WidgetPtr _current_widget);

	typedef MyGUI::delegates::CDelegate5<MyGUI::WidgetPtr, Ogre::String, Ogre::String, std::string, int> EventHandle_EventCreatePair;
	EventHandle_EventCreatePair eventCreatePair;
	typedef MyGUI::delegates::CDelegate1<const std::string &> EventHandle_EventSetPositionText;
	EventHandle_EventSetPositionText eventSetPositionText;
private:
	MyGUI::ButtonPtr mButtonRelativePosition;

	MyGUI::WidgetPtr current_widget;
};

#endif // __PANEL_MAIN_PROPERTIES_H__
