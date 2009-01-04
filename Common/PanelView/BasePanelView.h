/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __BASE_PANEL_VIEW_H__
#define __BASE_PANEL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

namespace wraps
{

	class BasePanelView : public BaseLayout2
	{
	public:
		typedef std::vector<BasePanelViewItem*> VectorCell;

	public:
		BasePanelView(const std::string & _layout, MyGUI::WidgetPtr _parent);
		virtual ~BasePanelView();

		//! Get number of items
		size_t getItemCount() { return mItems.size(); }

		//! Insert an item into a list at a specified position
		void insertItem(size_t _index, BasePanelViewItem * _item);
		//! Add an item to the end of a list
		void addItem(BasePanelViewItem * _item) { insertItem(MyGUI::ITEM_NONE, _item); }
		//! Get item from specified position
		BasePanelViewItem * getItem(size_t _index);
		//! Search item, returns the position of the first occurrence in list or ITEM_NONE if item not found
		size_t findItem(BasePanelViewItem * _item);

		bool getItemShow(BasePanelViewItem * _item) { return _item->getPanelCell()->isShow(); }
		void setItemShow(BasePanelViewItem * _item, bool _show)
		{
			_show ? _item->getPanelCell()->show() : _item->getPanelCell()->hide();
			setNeedUpdate();
		}

		//! Remove item at a specified position
		void removeItemAt(size_t _index);
		//! Remove item at a specified position
		void removeItem(BasePanelViewItem * _item);
		//! Remove all items
		void removeAllItems();

		void updateView();

		// изменились размеры
		// необходимо обновить все панели
		void setNeedUpdate();

	protected:
		virtual BasePanelViewCell * createCell() = 0;

	private:
		void notifyUpdatePanel(BasePanelViewCell * _panel);
		void frameEntered(float _time) { updateView(); }

	protected:
		MyGUI::ScrollViewPtr mScrollView;

	private:
		VectorCell mItems;
		bool mNeedUpdate;
		int mOldClientWidth;
	};

} // namespace wraps

#endif // __BASE_PANEL_VIEW_H__
