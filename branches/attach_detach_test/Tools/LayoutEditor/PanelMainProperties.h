/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_MAIN_PROPERTIES_H__
#define __PANEL_MAIN_PROPERTIES_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
//#include <Ogre.h>

extern const int PropertyItemHeight;

class PanelMainProperties : public wraps::BasePanelViewItem
{
public:

	PanelMainProperties();

	virtual void initialise();
	virtual void shutdown();

	void notifyToggleRelativeMode(MyGUI::Widget* _sender = nullptr);
	void update(MyGUI::Widget* _current_widget);

	typedef MyGUI::delegates::CDelegate5<MyGUI::Widget*, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
	EventHandle_EventCreatePair eventCreatePair;
	typedef MyGUI::delegates::CDelegate1<const std::string&> EventHandle_EventSetPositionText;
	EventHandle_EventSetPositionText eventSetPositionText;

	MyGUI::Widget* getMainWidget() { return mWidgetClient; }
private:
	MyGUI::Button* mButtonRelativePosition;

	MyGUI::Widget* current_widget;
};

#endif // __PANEL_MAIN_PROPERTIES_H__
