/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_VIEW_WINDOW_H__
#define __PANEL_VIEW_WINDOW_H__

#include <MyGUI.h>
#include "PanelView.h"

class PanelViewWindow : public wraps::BaseLayout
{
public:

	PanelViewWindow();

	virtual void initialise();

	//! Get number of items
	size_t getItemCount() { return mPanelView.getItemCount(); }

	//! Insert an item into a list at a specified position
	void insertItem(size_t _index, PanelBase * _item) { mPanelView.insertItem(_index, _item); }
	//! Add an item to the end of a list
	void addItem(PanelBase * _item) { mPanelView.addItem(_item); }
	//! Get item from specified position
	PanelBase * getItem(size_t _index) { return mPanelView.getItem(_index); }
	//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
	size_t findItem(PanelBase * _item) { return mPanelView.findItem(_item); }

	//! Remove item at a specified position
	void removeItemAt(size_t _index) { mPanelView.removeItemAt(_index); }
	//! Remove item at a specified position
	void removeItem(PanelBase * _item) { mPanelView.removeItem(_item); }
	//! Remove all items
	void removeAllItems() { return mPanelView.removeAllItems(); }

	bool getItemShow(PanelBase * _item) { return mPanelView.getItemShow(_item); }
	void setItemShow(PanelBase * _item, bool _show) { mPanelView.setItemShow(_item, _show); }


private:
	void notifyWindowChangeCoord(MyGUI::WidgetPtr _sender);

private:
	MyGUI::ScrollViewPtr mScrollView;
	PanelView mPanelView;
};

#endif // __PANEL_VIEW_WINDOW_H__
