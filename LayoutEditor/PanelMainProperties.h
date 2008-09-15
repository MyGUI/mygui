/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

extern const int PropertyItemHeight;

class PanelMainProperties : public BaseLayout, public PanelBase
{
public:

	PanelMainProperties();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();

	void notifyToggleRelativeMode(MyGUI::WidgetPtr _sender = null);
	void update(MyGUI::WidgetPtr _current_widget);
private:
	MyGUI::ButtonPtr mButtonRelativePosition;
	
	MyGUI::WidgetPtr current_widget;
};

#endif // __PANEL_MAIN_PROPERTIES_H__
