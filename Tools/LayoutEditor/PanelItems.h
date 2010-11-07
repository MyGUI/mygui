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

	private:
		void updateList();
		void addItem(const std::string& _value);
		void removeItem(size_t _index);

		void notifyAddItem(MyGUI::Widget* _sender = 0);
		void notifyDeleteItem(MyGUI::Widget* _sender);
		void notifySelectSheet(MyGUI::Widget* _sender);
		void notifyUpdateItem(MyGUI::Edit* _widget);
		void notifySelectItem(MyGUI::List* _widget, size_t _position);
		virtual void notifyChangeWidth(int _width);

		void addSheetToTab(MyGUI::Widget* _tab, const std::string& _caption = "");
		//void addItemToMenu(MyGUI::Widget* _tab, const std::string& _caption = "");

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
