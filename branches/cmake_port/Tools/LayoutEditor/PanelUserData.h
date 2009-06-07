/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_USER_DATA_H__
#define __PANEL_USER_DATA_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

extern const int PropertyItemHeight;

class PanelUserData : public wraps::BasePanelViewItem
{
public:

	PanelUserData();

	virtual void initialise();
	virtual void shutdown();

	void update(MyGUI::WidgetPtr _current_widget);

private:
	virtual void notifyChangeWidth(int _width);

	void notifyAddUserData(MyGUI::WidgetPtr _sender = 0);
	void notifyDeleteUserData(MyGUI::WidgetPtr _sender);
	void notifyUpdateUserData(MyGUI::EditPtr _widget);
	void notifySelectUserDataItem(MyGUI::MultiListPtr _widget, size_t _index);

	MyGUI::EditPtr mEditKey;
	MyGUI::EditPtr mEditValue;
	MyGUI::ButtonPtr mButtonAdd;
	MyGUI::ButtonPtr mButtonDelete;
	MyGUI::MultiListPtr mMultilist;

	MyGUI::WidgetPtr current_widget;

	int mEditLeft, mEditRight, mEditSpace;
	int mButtonLeft, mButtonRight, mButtonSpace;
};

#endif // __PANEL_USER_DATA_H__
