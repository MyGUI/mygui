/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _c0b91375_3314_4f80_9d74_10ee635a2d6f_
#define _c0b91375_3314_4f80_9d74_10ee635a2d6f_

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "ListBoxItemControl.h"

/*namespace tools
{

	class ListBoxControl :
		public wraps::BaseItemBox<ListBoxItemControl>
	{
	public:
		ListBoxControl(MyGUI::Widget* _parent);
		virtual ~ListBoxControl();

		void removeAllItems();

		void removeItem(DataType _data);

		void removeItemAt(size_t _index);

		size_t getItemCount() const;

		void setIndexSelected(size_t _value);

		void setItemSelected(DataType _data);

		size_t getIndexData(DataType _data) const;

		void setItemAt(size_t _index, DataType _data);

		void redrawAllItems();

		MyGUI::IntCoord getItemCoord(DataType _data);

		void ensureItemVisible(DataType _data);

		MyGUI::delegates::CMultiDelegate2<ListBoxControl*, DataType>
			eventChangeItemPosition;

		MyGUI::delegates::CMultiDelegate3<ListBoxControl*, size_t, size_t>
			eventRelocationItem;

		MyGUI::delegates::CMultiDelegate2<ListBoxControl*, const std::string&>
			eventCommand;

		MyGUI::delegates::CMultiDelegate2<ListBoxControl*, const std::string&>
			eventItemRename;

	private:
		void notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index);
		void notifyStartDrag(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result);
		void notifyRequestDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result);
		void notifyDropResult(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result);
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);
		void notifyNotifyItem(MyGUI::ItemBox* _sender, const MyGUI::IBNotifyItemData& _info);

		void onIndexSelected(size_t _index, bool _ensureVisible = true);

	private:
		MyGUI::PopupMenu* mContextMenu;
	};

}*/

#endif
