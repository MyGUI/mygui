/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __PANEL_ITEMS_H__
#define __PANEL_ITEMS_H__

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"

namespace tools
{
	class PanelItems :
		public wraps::BasePanelViewItem
	{
	public:
		PanelItems();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

		//void notifyRectangleDoubleClick(MyGUI::Widget* _sender);

	private:
		void addSheetToTab(MyGUI::Widget* _tab, std::string _caption = "");
		void syncItems(bool _apply, bool _add = false, std::string _value = "");
		void notifyAddItem(MyGUI::Widget* _sender = 0);
		void notifyDeleteItem(MyGUI::Widget* _sender);
		void notifySelectSheet(MyGUI::Widget* _sender);
		void notifyUpdateItem(MyGUI::Edit* _widget);
		void notifySelectItem(MyGUI::List* _widget, size_t _position);
		virtual void notifyChangeWidth(int _width);

	private:
		MyGUI::Edit* mEdit;
		MyGUI::List* mList;
		MyGUI::Button* mButtonAdd;
		MyGUI::Button* mButtonDelete;
		MyGUI::Button* mButtonSelect;

		MyGUI::Widget* mCurrentWidget;

		int mButtonLeft;
		int mButtonRight;
		int mButtonSpace;
	};

} // namespace tools

#endif // __PANEL_ITEMS_H__
