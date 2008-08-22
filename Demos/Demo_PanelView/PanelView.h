/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include <MyGUI.h>
//#include "BaseLayout.h"
#include "Panel.h"

class PanelView// : public BaseLayout
{
public:
	typedef std::vector<Panel*> VectorPanel;

	//wrap
	void attach(MyGUI::ScrollViewPtr _widget);

	//! Get number of items
	inline size_t getItemCount() { return mItems.size(); }

	//! Insert an item into a list at a specified position
	void insertItem(size_t _index, Panel * _item);
	//! Add an item to the end of a list
	inline void addItem(Panel * _item) { insertItem(MyGUI::ITEM_NONE, _item); }
	//! Get item from specified position
	Panel * getItem(size_t _index);
	//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
	size_t findItem(Panel * _item);

	//! Remove item at a specified position
	void removeItemAt(size_t _index);
	//! Remove item at a specified position
	void removeItem(Panel * _item);
	//! Remove all items
	void removeAllItems();

	void updateView();

private:
	void notifyUpdatePanel(Panel * _panel);

private:
	MyGUI::ScrollViewPtr mScrollView;

	VectorPanel mItems;
};

#endif // __PANEL_VIEW_H__
