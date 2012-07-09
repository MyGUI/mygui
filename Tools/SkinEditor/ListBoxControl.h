/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef __LIST_BOX_CONTROL_H__
#define __LIST_BOX_CONTROL_H__

#include <MyGUI.h>
#include "ItemBox/BaseItemBox.h"
#include "ListBoxItemControl.h"

namespace tools
{
	class ListBoxControl :
		public wraps::BaseItemBox<ListBoxItemControl>
	{
	public:
		ListBoxControl(MyGUI::Widget* _parent);
		virtual ~ListBoxControl();

		void setIndexSelected(size_t _value);

		void removeAllItems();

		void removeItem(DataType _data);

		void removeItemAt(size_t _index);

		size_t getItemCount() const;

		void redrawAllItems();

		size_t getIndexData(DataType _data) const;

		void setItemSelected(DataType _data);

		void setItemAt(size_t _index, DataType _data);

		MyGUI::delegates::CMultiDelegate2<ListBoxControl*, DataType>
			eventChangeItemPosition;

		MyGUI::delegates::CMultiDelegate3<ListBoxControl*, size_t, size_t>
			eventRelocationItem;

	private:
		void notifyChangeItemPosition(MyGUI::ItemBox* _sender, size_t _index);
		void notifyStartDrag(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result);
		void notifyRequestDrop(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool& _result);
		void notifyDropResult(MyGUI::DDContainer* _sender, const MyGUI::DDItemInfo& _info, bool _result);

	private:
		MyGUI::PopupMenu* mContextMenu;
	};
}

#endif
