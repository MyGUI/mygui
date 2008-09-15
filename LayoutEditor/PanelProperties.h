/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

extern const int PropertyItemHeight;

class PanelProperties : public BaseLayout, public PanelBase
{
public:

	PanelProperties();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();
	enum PropertiesGroup {TYPE_PROPERTIES, WIDGET_PROPERTIES, EVENTS};
	void update(MyGUI::WidgetPtr _current_widget, PropertiesGroup _group);
};

#endif // __PANEL_PROPERTIES_H__
