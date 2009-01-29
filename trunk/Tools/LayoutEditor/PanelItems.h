/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_ITEMS_H__
#define __PANEL_ITEMS_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

extern const int PropertyItemHeight;

class PanelItems : public wraps::BasePanelViewItem
{
public:

	PanelItems();

	virtual void initialise();
	virtual void shutdown();

	void update(MyGUI::WidgetPtr _current_widget);

	void notifyRectangleDoubleClick(MyGUI::WidgetPtr _sender);
private:
	void addSheetToTab(MyGUI::WidgetPtr _tab, Ogre::String _caption = "");
	void syncItems(bool _apply, bool _add = false, Ogre::String _value = "");
	void notifyAddItem(MyGUI::WidgetPtr _sender = 0);
	void notifyDeleteItem(MyGUI::WidgetPtr _sender);
	void notifySelectSheet(MyGUI::WidgetPtr _sender);
	void notifyUpdateItem(MyGUI::EditPtr _widget);
	void notifySelectItem(MyGUI::ListPtr _widget, size_t _position);

	MyGUI::EditPtr mEdit;
	MyGUI::ListPtr mList;
	MyGUI::ButtonPtr mButtonAdd;
	MyGUI::ButtonPtr mButtonDelete;
	MyGUI::ButtonPtr mButtonSelect;

	MyGUI::WidgetPtr current_widget;
};

#endif // __PANEL_ITEMS_H__
