/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
	@module
*/
#ifndef __PANEL_USER_DATA_H__
#define __PANEL_USER_DATA_H__

#include "BaseLayout.h"
#include "PanelView/PanelBase.h"

extern const int PropertyItemHeight;

class PanelUserData : public wraps::BaseLayout, public PanelBase
{
public:

	PanelUserData();

	virtual void initialiseCell(PanelCell * _cell);
	virtual void shutdownCell();
	void update(MyGUI::WidgetPtr _current_widget);
private:
	void notifyAddUserData(MyGUI::WidgetPtr _sender = 0);
	void notifyDeleteUserData(MyGUI::WidgetPtr _sender);
	void notifyUpdateUserData(MyGUI::WidgetPtr _widget);
	void notifySelectUserDataItem(MyGUI::WidgetPtr _widget, size_t _index);

	MyGUI::EditPtr mEditKey;
	MyGUI::EditPtr mEditValue;
	MyGUI::ButtonPtr mButtonAdd;
	MyGUI::ButtonPtr mButtonDelete;
	MyGUI::MultiListPtr mMultilist;

	MyGUI::WidgetPtr current_widget;
};

#endif // __PANEL_USER_DATA_H__
