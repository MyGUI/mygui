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

		void OnRenameData();

		void setEnableChangePosition(bool _value);

		void setDataInfo(const std::string& _parentType, const std::string& _property);

		sigslot::signal2<Data*, Data*> eventChangePosition;
		sigslot::signal2<Data*, const std::string&> eventChangeName;

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyItem(MyGUI::ListBox* _sender, const MyGUI::IBNotifyItemData& _info);
		void notifyEndDialog(Dialog* _sender, bool _result);

		void invalidateList();
		void invalidateSelection();

	private:
		MyGUI::ListBox* mListBox;
		MyGUI::EditBox* mHelpPanel;
		Data* mParentData;
		size_t mLastIndex;
		std::string mPropertyForName;
		MyGUI::PopupMenu* mContextMenu;
		TextFieldControl* mTextFieldControl;
		bool mEnableChangePosition;
	};
}

#endif
