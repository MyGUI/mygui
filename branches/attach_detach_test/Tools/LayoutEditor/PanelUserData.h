/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
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

	void update(MyGUI::Widget* _current_widget);

private:
	virtual void notifyChangeWidth(int _width);

	void notifyAddUserData(MyGUI::Widget* _sender = 0);
	void notifyDeleteUserData(MyGUI::Widget* _sender);
	void notifyUpdateUserData(MyGUI::Edit* _widget);
	void notifySelectUserDataItem(MyGUI::MultiList* _widget, size_t _index);

	MyGUI::Edit* mEditKey;
	MyGUI::Edit* mEditValue;
	MyGUI::Button* mButtonAdd;
	MyGUI::Button* mButtonDelete;
	MyGUI::MultiList* mMultilist;

	MyGUI::Widget* current_widget;

	int mEditLeft, mEditRight, mEditSpace;
	int mButtonLeft, mButtonRight, mButtonSpace;
};

#endif // __PANEL_USER_DATA_H__
