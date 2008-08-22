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

class PanelViewWindow : public BaseLayout
{
public:

	PanelViewWindow();

	virtual void initialise();

	//! Get number of items
	inline size_t getItemCount() { return mPanelView.getItemCount(); }

	//! Insert an item into a list at a specified position
	inline void insertItem(size_t _index, Panel * _item) { mPanelView.insertItem(_index, _item); }
	//! Add an item to the end of a list
	inline void addItem(Panel * _item) { mPanelView.addItem(_item); }
	//! Get item from specified position
	inline Panel * getItem(size_t _index) { return mPanelView.getItem(_index); }
	//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
	inline size_t findItem(Panel * _item) { return mPanelView.findItem(_item); }

	//! Remove item at a specified position
	inline void removeItemAt(size_t _index) { mPanelView.removeItemAt(_index); }
	//! Remove item at a specified position
	inline void removeItem(Panel * _item) { mPanelView.removeItem(_item); }
	//! Remove all items
	inline void removeAllItems() { return mPanelView.removeAllItems(); }


private:
	void notifyWindowChangeCoord(MyGUI::WidgetPtr _sender);

private:
	MyGUI::ScrollViewPtr mScrollView;
	PanelView mPanelView;
};

#endif // __PANEL_VIEW_WINDOW_H__
