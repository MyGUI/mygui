/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include <MyGUI.h>
#include "PanelCell.h"
#include "PanelBase.h"

class PanelView
{
public:
	typedef std::vector<PanelBase*> VectorPanel;

	//wrap
	void attach(MyGUI::ScrollViewPtr _widget);

	//! Get number of items
	inline size_t getItemCount() { return mItems.size(); }

	//! Insert an item into a list at a specified position
	void insertItem(size_t _index, PanelBase * _item);
	//! Add an item to the end of a list
	inline void addItem(PanelBase * _item) { insertItem(MyGUI::ITEM_NONE, _item); }
	//! Get item from specified position
	PanelBase * getItem(size_t _index);
	//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
	size_t findItem(PanelBase * _item);

	inline bool getItemShow(PanelBase * _item) { return _item->getPanelCell()->mainWidget()->isShow(); }
	inline void setItemShow(PanelBase * _item, bool _show)
	{
		_show ? _item->getPanelCell()->mainWidget()->show() : _item->getPanelCell()->mainWidget()->hide();
		setNeedUpdate();
	}

	//! Remove item at a specified position
	void removeItemAt(size_t _index);
	//! Remove item at a specified position
	void removeItem(PanelBase * _item);
	//! Remove all items
	void removeAllItems();

	void updateView();
	void setNeedUpdate();

private:
	void notifyUpdatePanel(PanelCell * _panel);
	void frameEntered(float _time){updateView();};

private:
	MyGUI::ScrollViewPtr mScrollView;
	VectorPanel mItems;

	bool mNeedUpdate;
	//std::string mPanelCellLayout;
};

#endif // __PANEL_VIEW_H__
