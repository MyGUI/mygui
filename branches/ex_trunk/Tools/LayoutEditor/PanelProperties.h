/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_PROPERTIES_H__
#define __PANEL_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "WidgetTypes.h"
#include "WidgetContainer.h"

extern const int PropertyItemHeight;

class PanelProperties : public wraps::BasePanelViewItem
{
public:

	PanelProperties();

	virtual void initialise();
	virtual void shutdown();

	enum PropertiesGroup { TYPE_PROPERTIES, WIDGET_PROPERTIES };
	void update(MyGUI::Widget* _current_widget, PropertiesGroup _group);

	typedef MyGUI::delegates::CDelegate5<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
	EventHandle_EventCreatePair eventCreatePair;

private:
	size_t AddParametrs(WidgetStyle * widgetType, WidgetContainer * widgetContainer, int& y);

};

#endif // __PANEL_PROPERTIES_H__
