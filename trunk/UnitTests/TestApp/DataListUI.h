/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __EDIT_PANEL_H__
#define __EDIT_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "sigslot.h"

namespace demo
{
	ATTRIBUTE_CLASS_LAYOUT(DataListUI, "DataListUI.layout");
	class DataListUI :
		public wraps::BaseLayout,
		public sigslot::has_slots<>
	{
	public:
		DataListUI();

	private:
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void bindEvents(MyGUI::Widget* _widget);

		void updateActions();
		void updateListData();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(DataListUI, mListBox, "ListBox");
		MyGUI::ListBox* mListBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(DataListUI, mTextBox, "SaveTextBox");
		MyGUI::TextBox* mTextBox;

		size_t mIndex;
	};
}

#endif
