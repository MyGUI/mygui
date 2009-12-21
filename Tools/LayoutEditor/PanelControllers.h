/*!
	@file
	@author		Georgiy Evmenov
	@date		12/2009
	@module
*/
#ifndef __PANEL_CONTROLLERS_H__
#define __PANEL_CONTROLLERS_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

extern const int PropertyItemHeight;

class PanelControllers : public wraps::BasePanelViewItem
{
public:

	PanelControllers();

	virtual void initialise();
	virtual void shutdown();

	void update(MyGUI::WidgetPtr _current_widget);

	typedef MyGUI::delegates::CDelegate5<MyGUI::WidgetPtr, const std::string&, const std::string&, const std::string&, int> EventHandle_EventCreatePair;
	EventHandle_EventCreatePair eventCreatePair;
private:
	virtual void notifyChangeWidth(int _width);

	void notifyAdd(MyGUI::WidgetPtr _sender = 0);
	void notifyDelete(MyGUI::WidgetPtr _sender);
	void notifySelectItem(MyGUI::ListPtr _sender, size_t _index);

	void loadControllerTypes(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

	MyGUI::ComboBoxPtr mControllerName;
	MyGUI::ButtonPtr mButtonAdd;
	MyGUI::ButtonPtr mButtonDelete;
	MyGUI::ListPtr mList;

	MyGUI::WidgetPtr current_widget;

	int mButtonLeft, mButtonRight, mButtonSpace;

	std::map<std::string, MyGUI::MapString> mControllersProperties;
};

#endif // __PANEL_CONTROLLERS_H__
