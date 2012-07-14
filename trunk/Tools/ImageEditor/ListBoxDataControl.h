/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _b0713d87_24ee_44cf_bee9_c96494be178e_
#define _b0713d87_24ee_44cf_bee9_c96494be178e_

#include "Control.h"
#include "sigslot.h"
#include "Data.h"
#include "TextFieldControl.h"

namespace tools
{
	class ListBoxDataControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		ListBoxDataControl();
		virtual ~ListBoxDataControl();

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

		void OnRenameData();
		void setEnableChangePosition(bool _value);

	private:
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyItem(MyGUI::ListBox* _sender, const MyGUI::IBNotifyItemData& _info);
		void notifyEndDialog(Dialog* _sender, bool _result);

		void invalidateList();
		void invalidateSelection();

	private:
		MyGUI::ListBox* mListBox;
		Data* mParentData;
		size_t mLastIndex;
		std::string mPropertyForName;
		MyGUI::PopupMenu* mContextMenu;
		TextFieldControl* mTextFieldControl;
		bool mEnableChangePosition;
	};
}

#endif
